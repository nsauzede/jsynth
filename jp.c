#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <jack/jack.h>
#include <jack/midiport.h>

typedef enum {
LEARN,
RUN_STOP,
TEMPOPLUS,
TEMPOMINUS,
//
MAX_PREFS
} prefs_e;
typedef struct {
	int note[MAX_PREFS];
} prefs_t;

typedef struct {
	jack_client_t *client;
	jack_port_t *input;
	jack_port_t *output;
    jack_nframes_t samplerate;
    jack_nframes_t tempo;	// in BPM

	int learn;
	int run_stop;
	prefs_t prefs;
	int learn_idx;
	char *prefs_f;
} j_t;
static j_t j;

static void load_prefs() {
	printf("Loading prefs from '%s'\n", j.prefs_f);
	FILE *f = fopen(j.prefs_f, "rt");
	if (f) {
		for (int i = 0; i < MAX_PREFS; i++) {
			fscanf(f, "%d", &j.prefs.note[i]);
		}
		fclose(f);
	}
}

static void save_prefs() {
	printf("Saving prefs to '%s'\n", j.prefs_f);
	FILE *f = fopen(j.prefs_f, "wt");
	if (f) {
		for (int i = 0; i < MAX_PREFS; i++) {
			fprintf(f, " %d", j.prefs.note[i]);
		}
		fclose(f);
	}
}

static void info() {
	printf("learn=%d(%d) idx=%d run_stop=%d(%d) tempo+=%d(%d) tempo-=%d(%d)", j.learn, j.prefs.note[LEARN], j.learn_idx, j.run_stop, j.prefs.note[RUN_STOP], j.tempo, j.prefs.note[TEMPOPLUS], j.tempo, j.prefs.note[TEMPOMINUS]);
	printf("\n");
}

static void process_midi_input(jack_port_t *port, jack_nframes_t nframes) {
	void *port_buffer = jack_port_get_buffer(port, nframes);
	if (!port_buffer) {
		printf("null midi input buffer?\n");
		return;
	}
	static int bank = 0;
	static int program = 0;
	int nbank = bank;
	int nprogram = program;
	int events = jack_midi_get_event_count(port_buffer);
	for (int i = 0; i < events; i++) {
		jack_midi_event_t event;
		if (jack_midi_event_get(&event, port_buffer, i)) {
			printf("failed to get midi input event?\n");
			continue;
		}
		static int count = 0;
		int size = event.size;
#if 1
		jack_time_t jtime = jack_get_time();
		jack_time_t ftime = jack_frames_to_time(j.client, event.time);
		static jack_time_t _jtime = 0;
		static jack_time_t _ftime = 0;
		jack_time_t djtime = jtime - _jtime;
		jack_time_t dftime = ftime - _ftime;
		_jtime = jtime;
		_ftime = ftime;
		printf("#%d bytes=%d jtime=%" PRIu64 "(%6" PRIu64 ") ftime=%" PRIu64 "(%6" PRIu64 "): ", count, size, (uint64_t)jtime, (uint64_t)djtime, (uint64_t)ftime, (uint64_t)dftime);
#else
		printf("#%4d bytes=%d : ", count, size);
#endif
#if 0
1000----=note off
1001----=note on
1010----=polyphonic aftertouch
1011----=control/mode change
1100----=program change
1101----=channel aftertouch
1110----=pitch bend change
11110000=system exclusive
11110001=MIDI Time code Qtr. Frame
11110010=Song Position Pointer
11110011=Song select

System Exclusive
MIDI Time Code Qtr. Frame
Song Position Pointer
Song Select (Song #)
Undefined F4 (Reserved)
Undefined F5 (Reserved)
Tune request
End of SysEx (EOX)
Timing clock
Undefined F9 (Reserved)
Start
Continue
Stop
Undefined FD (Reserved)
Active Sensing
System Reset
#endif
			int status = event.buffer[0];
			static int lsb_control[32] = {0};
			switch (status & 0xf0) {
			case 0xf0:
				switch (status & 0xf) {
				case 0x0:printf("System Exclusive");break;
				case 0x1:printf("MIDI Time Code Qtr. Frame");break;
				case 0x2:printf("Song Position Pointer");break;
				case 0x3:printf("Song Select (Song #)");break;
				case 0x4:printf("Undefined F4 (Reserved)");break;
				case 0x5:printf("Undefined F5 (Reserved)");break;
				case 0x6:printf("Tune request");break;
				case 0x7:printf("End of SysEx (EOX)");break;
				case 0x8:printf("Timing clock");break;
				case 0x9:printf("Undefined F9 (Reserved)");break;
				case 0xa:printf("Start");break;
				case 0xb:printf("Continue");break;
				case 0xc:printf("Stop");break;
				case 0xd:printf("Undefined FD (Reserved)");break;
				case 0xe:printf("Active Sensing");break;
				case 0xf:printf("System Reset");break;
				}
				break;
			case 0x80 ... 0x9f: {
				int chan = status & 0xf;
				int freq = event.buffer[1];
				int vel = event.buffer[2];
				if (status & 0x10) {
					printf("NOTE ON ");
				} else {
					printf("NOTE OFF");
					if (j.learn) {
						if (freq == j.prefs.note[LEARN]) {
							j.learn_idx++;
							if (j.learn_idx >= MAX_PREFS) {
								j.learn_idx = 0;
								j.learn = 0;
								save_prefs();
								info();
							}
							info();
						} else {
							j.prefs.note[j.learn_idx] = freq;
							info();
						}
					} else {
						if (freq == j.prefs.note[LEARN]) {
							j.learn = 1;
							info();
						} else if (freq == j.prefs.note[RUN_STOP]) {
							j.run_stop = 1 - j.run_stop;
							info();
						} else if (freq == j.prefs.note[TEMPOPLUS]) {
							j.tempo++;
							info();
						} else if (freq == j.prefs.note[TEMPOMINUS]) {
							j.tempo--;
							info();
						}
					}
				}
				printf(": chan=%d freq=%d velo=%d", chan, freq, vel);
				break;
			}
			case 0xb0 ... 0xbf: {
				printf("Control Change");
				int chan = status & 0xf;
				printf(": chan=%d", chan);
				switch (event.buffer[1]) {
				case 0x00: {
					int val = (event.buffer[2] << 7) + lsb_control[0];
					printf(" Bank Select value=%d", val);
					nbank = val;
					break;
				}
				case 0x01: {
					int val = event.buffer[2];
					printf(" Modulation Wheel or Lever value=%d", val);
					break;
				}
				case 0x20 ... 0x3f: {
					int control = event.buffer[1] - 0x20;
					int val = event.buffer[2];
					printf(" LSB for Control %d value=%d", control, val);
					lsb_control[control] = val;
					break;
				}
				}
				break;
			}
			case 0xc0 ... 0xcf: {
				printf("Program Change");
				int chan = status & 0xf;
				int val = event.buffer[1];
				printf(": chan=%d program=%d", chan, val);
				nprogram = val;
				break;
			}
			case 0xe0 ... 0xef: {
				printf("Pitch Bend Change");
				int chan = status & 0xf;
				int val = ((event.buffer[2] & 0x7f) << 7) + (event.buffer[1] & 0x7f);
				val = (double)val / 16383 * 200 - 100;
				printf(": chan=%d value=%d", chan, val);
				break;
			}
			default:
				printf("UNKNOWN");
				break;
			}
		for (int j = 0; j < size; j++) {
			unsigned char d = event.buffer[j];
			printf(" %02x", d);
		}
		printf("\n");
		count++;
	}
	if (nbank != bank || nprogram != program) {
	    program = nprogram;
	    bank = nbank;
	    printf("Bank %d Program %d\n", bank, program);
	}
}

typedef struct {
    jack_nframes_t start;
    jack_nframes_t len;
    int channel;
    int freq;
    int velocity;
} note_t;

int jack_samplerate_cb(jack_nframes_t nframes, void *arg) {
    j_t *j = (j_t *)arg;
    j->samplerate = nframes;
    printf("%s: samplerate=%d\n", __func__, (int)j->samplerate);
    return 0;
}

// in percentage of samplerate
#define L 100
note_t notes[] = {
    {0*L, L, 0, 40, 44},
    {0*L, L, 0, 52, 36},

    {1*L, L, 0, 55, 36},
    {1*L, L, 0, 59, 40},
    {1*L, L, 0, 64, 40},

    {2*L, L, 0, 40, 44},
    {2*L, L, 0, 52, 36},

    {3*L, L, 0, 57, 40},
    {3*L, L, 0, 60, 40},
    {3*L, L, 0, 66, 44},

    {4*L, L, 0, 40, 44},
    {4*L, L, 0, 52, 36},

    {5*L, L, 0, 58, 53},
    {5*L, L, 0, 61, 36},
    {5*L, L, 0, 67, 44},

    {6*L, L, 0, 40, 44},
    {6*L, L, 0, 52, 36},

    {7*L, L, 0, 57, 36},
    {7*L, L, 0, 61, 32},
    {7*L, L, 0, 63, 40},
};
int nnotes = sizeof(notes)/sizeof(notes[0]);

#define MAXFRAME 999999999
jack_nframes_t g_frame = 0;
jack_nframes_t g_max = MAXFRAME;
static void process_midi_output(jack_port_t *port, jack_nframes_t nframes) {
	void *port_buffer = jack_port_get_buffer(port, nframes);
	if (!port_buffer) {
		printf("null midi output buffer?\n");
		return;
	}
	jack_midi_clear_buffer(port_buffer);
	if (nframes <= 0)return;
	if (!j.run_stop)return;
	static int count = 0;
	jack_nframes_t nextframe = g_frame + nframes;
/*
       nframes  nframes
g_framev       vv       v
       [-------][-------]
A  S E				=> nothing
B  S      E			=> NOTEOFF
C        S         E		=> NOTEON
D         S   E			=> NOTEON, NOTEOFF
E                 S   E		=> nothing
*/
	jack_nframes_t max = 0;
	jack_nframes_t offset = 0;
	for (int i = 0; i < nnotes; i++) {
		jack_nframes_t start = (double)notes[i].start * j.samplerate * 60 / 100 / j.tempo;
		jack_nframes_t len = (double)notes[i].len * j.samplerate * 60 / 100 / j.tempo;
		int channel = notes[i].channel;
		int freq = notes[i].freq;
		int velocity = notes[i].velocity;
#if 0
		if (g_max >= g_frame && g_max < nextframe) {	// end of notes -- loop
	printf("#%3d g_f=%d i=%d ", count, (int)g_frame, i);
			printf("Looping!\n");
			nextframe = 0;	// g_frame will be set to this
			break;
		}
#endif
		if (start >= g_frame) {
			if (start < nextframe) {
				unsigned char *buf = jack_midi_event_reserve(port_buffer, start - g_frame + offset, 3);
				if (!buf){
					printf("Null buf NOTE ON\n");
					return;
				}
#if 0
	printf("#%3d g_f=%d i=%d ", count, (int)g_frame, i);
				printf("Sending NOTE ON f=%d v=%d at %d\n", freq, velocity, (int)(start - g_frame + offset));
#endif
				buf[0] = 0x90 + channel;
				buf[1] = freq;
				buf[2] = velocity;
			}
		}
		jack_nframes_t end = start + len;
		if (end > max)
		    max = end;
		if (end >= g_frame) {
			if (end < nextframe) {
				unsigned char *buf = jack_midi_event_reserve(port_buffer, end - g_frame + offset, 3);
				if (!buf){
					printf("Null buf NOTE OFF\n");
					return;
				}
#if 0
	printf("#%3d g_f=%d i=%d ", count, (int)g_frame, i);
				printf("Sending NOTE OFF f=%d v=%d at %d\n", freq, velocity, (int)(end - g_frame + offset));
#endif
				buf[0] = 0x80 + channel;
				buf[1] = freq;
				buf[2] = velocity;
if (i == nnotes - 1) {
#if 0
	printf("#%3d g_f=%d i=%d ", count, (int)g_frame, i);
			printf("Looping!\n");
#endif
			offset = end - g_frame + offset;
			g_frame = 0;	// g_frame will be set to this
			nextframe = g_frame + nframes;
			i = -1;	// continue for will postincrement
			continue;
}
			}
		}
	}
	g_frame = nextframe;
	if (max > 0)
		g_max = max;
	count++;
}

static int process_callback(jack_nframes_t nframes, void *arg) {
	j_t *j = (j_t *)arg;
	process_midi_input(j->input, nframes);
	process_midi_output(j->output, nframes);
	return 0;
}

int main(int argc, char *argv[]) {
	char *prefs="j.pref";
	int arg = 1;
	if (arg < argc) {
		prefs = argv[arg++];
	}
	j.tempo = 120;
	j.prefs_f = prefs;
	// default prefs are empty
	// to start, just put the note freq of the learn key (eg: "108")
	// then press it to learn the other ones
	// they will all be stored in indicated prefs file above
	load_prefs();
	jack_client_t *client = jack_client_open("j", JackNoStartServer, 0);
	if (!client) {
		printf("Can't open Jack client\n");
		exit(1);
	}
	j.client = client;
	j.input = jack_port_register(client, "input", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	if (!j.input) {
		printf("Can't register Jack midi input port\n");
		exit(1);
	}
	j.output = jack_port_register(client, "output", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
	if (!j.input) {
		printf("Can't register Jack midi output port\n");
		exit(1);
	}
    j.samplerate = jack_get_sample_rate(client);
    printf("%s: samplerate=%d\n", __func__, (int)j.samplerate);
    jack_set_sample_rate_callback(client, jack_samplerate_cb, &j);
//	sleep(1);
	if (jack_set_process_callback(client, process_callback, &j)) {
		printf("Can't set Jack process callback\n");
		exit(1);
	}
	sleep(2);
	if (jack_activate(client)) {
		printf("Can't activate Jack client\n");
		exit(1);
	}
	info();
	printf("Running..\n");
	while (1) {
		usleep(1000);
	}
	return 0;
}
