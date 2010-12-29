#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <jack/jack.h>

int sampleFrequency = 44100;

int volume = 100;	// %
int tempo = 140;		// bpm
int steps = 1;
int tune = 50;		// %
int note = 9;		// 9=A
int octave = 1;		// 1=NORM (0=DOWN, 2=UP)

int __square_not_tri = 1;

//int period = 60 * sampleFrequency / tempo / steps;	// ms
//int period = 251;	// ms
int __period = 0;	// ms
//int width = period / 2;	// ms
//int __width = 132;	// ms
int __width = 0;	// ms
//int freq = 15.7 + note * 2.5 * 2 * octave;		// Hz
//int freq = 440;		// Hz
int __freq = 0;		// Hz

int __delay = 0;						// % of width
int __attack = 10;						// % of width
int __hold = 10;						// % of width
int __decay = 10;						// % of width
int __sustain = 50;						// % of amplitude max
int __release = 10;						// % of width

int next_t = 0;		// sample
int t = 0;			// sample
int pos = -1;		// sample
int done = 0;

typedef jack_default_audio_sample_t sample_t;
int process_audio( jack_nframes_t nframes, void *arg) {
	jack_port_t *port = arg;
	sample_t *stream = jack_port_get_buffer( port, nframes);
	int i;
	int _freq = __freq;
	int _period = __period;
	int _width = __width;
	int _square_not_tri = __square_not_tri;
#define USE_DAHDSR
#ifdef USE_DAHDSR
	int _delay = __delay;						// %
	int _attack = __attack;						// %
	int _hold = __hold;						// %
	int _decay = __decay;						// %
	int _sustain = __sustain;						// %
#endif
	int _release = __release;						// %
	unsigned int bytesPerPeriod = sampleFrequency / _freq;
	for (i = 0; i < nframes; i++) {
		sample_t s = 0;
		if (t >= next_t) {
			next_t = t + _period * sampleFrequency / 1000;
			pos = 0;
		}
		if (pos >= 0) {
			if (pos >= (_width * sampleFrequency / 1000)) {
				next_t = t + (_period - _width) * sampleFrequency / 1000;
				pos = -1;
			}
			else {
				#define AMAX ((double)0.5)
				double amp = AMAX;
#ifdef USE_DAHDSR
				int delay_dur = (_width * sampleFrequency / 1000) * _delay / 100;
				int delay_start = 0;
				int delay_end = delay_start + delay_dur;
				int attack_dur = (_width * sampleFrequency / 1000) * _attack / 100;
				int attack_start = delay_end;
				int attack_end = attack_start + attack_dur;
				int hold_dur = (_width * sampleFrequency / 1000) * _hold / 100;
				int hold_start = attack_end;
				int hold_end = hold_start + hold_dur;
				int decay_dur = (_width * sampleFrequency / 1000) * _decay / 100;
				int decay_start = hold_end;
				int decay_end = decay_start + decay_dur;
#endif
				int release_dur = (_width * sampleFrequency / 1000) * _release / 100;
#ifdef USE_DAHDSR
				int sustain_dur = (_width * sampleFrequency / 1000) - (decay_end + release_dur);
				int sustain_start = decay_end;
				int sustain_end = sustain_start + sustain_dur;
#endif
				int release_start = (_width * sampleFrequency / 1000) - release_dur;
				int release_end = release_start + release_dur;
#ifdef USE_DAHDSR
				if ((pos >= delay_start) && (pos < delay_end))
					amp *= (double)0;
				else if ((pos >= attack_start) && (pos < attack_end))
					amp *= ((double)pos - attack_start) / attack_dur;
				else if ((pos >= hold_start) && (pos < hold_end))
					amp *= (double)1;
				else if ((pos >= decay_start) && (pos < decay_end))
					amp *= ((double)decay_dur - (pos - decay_start)) / decay_dur;
				else if ((pos >= sustain_start) && (pos < sustain_end))
					amp *= (double)_sustain / 100;
				else
#endif
				if ((pos >= release_start) && (pos < release_end))
					amp *= ((double)release_dur - (pos - release_start)) / release_dur;
//				printf( "pos=%d width=%d release=%d release_dur=%d release_start=%d release_end=%d amp=%f\n",
//					pos, _width, _release, release_dur, release_start, release_end, amp);
#ifdef SQUARE_NOT_TRI
				s = ((pos % bytesPerPeriod) >= (bytesPerPeriod / 2)) - 1;
#else
				s = sin( pos * 2 * M_PI / bytesPerPeriod);
#endif
				if (_square_not_tri) {
#ifdef SQUARE_NOT_SIN
					if (s >= 0)
						s = 1;
					else
						s = -1;
#endif
				} else {
					s = 2 * ((double)(pos % bytesPerPeriod) / bytesPerPeriod) - 1;
				}
				s *= amp * volume / 100;
#if 1
				printf( "pos=%d bPerPer=%d w=%d attack=%d/%d@%d:%d release=%d/%d@%d:%d amp=%.1f s=%.1f\n",
					pos, bytesPerPeriod, _width, _attack, attack_dur, attack_start, attack_end, _release, release_dur, release_start, release_end, amp, s);
#endif
				pos++;
			}
		}
		t++;
		stream[i] = s;
	}
	return 0;
}
int main( int argc, char *argv[]) {
	int arg = 1;
	if (argc > arg) {
		sscanf( argv[arg++], "%d", &tune);
	if (argc > arg) {
		sscanf( argv[arg++], "%d", &__square_not_tri);
	}
	}
	printf( "tune=%d\n", tune);
	SDL_Init( SDL_INIT_VIDEO);
	atexit( SDL_Quit);
	SDL_Surface *screen = SDL_SetVideoMode( 100, 100, 32, 0);
	if (!screen) {
		fprintf(stderr, "Unable to open video: %s\n", SDL_GetError());
		exit(1);
	}
	jack_status_t status;
	jack_client_t *client = jack_client_open( "metro", JackNoStartServer, &status);
	jack_port_t *port;
	if (client) {
		port = jack_port_register( client, "140_bpm", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		jack_set_process_callback( client, process_audio, port);
		sampleFrequency = jack_get_sample_rate( client);
		printf( "sampleFrequency=%d\n", sampleFrequency);
		jack_activate( client);
	} else {
		printf( "jack server not running ?\n");
	}
	__period = 1000 * 60 / tempo / steps;	// ms
	__width = __period / 2;	// ms
	__freq = ((double)15.7 + 363.6 * tune / 100 + note * 2.5) * 2 * octave;		// Hz
	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	int dirty = 1;
	int done = 0;
	while (!done) {
		SDL_Event event;
		while (!done && SDL_PollEvent( &event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
				default:
					break;
				case SDL_KEYDOWN:
					dirty = 1;
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							done = 1;
							break;
						case SDLK_m:
							__square_not_tri = 1 - __square_not_tri;
							break;
						case SDLK_a:
							__freq--;
							break;
						case SDLK_z:
							__freq++;
							break;
						case SDLK_q:
							__release--;
							break;
						case SDLK_s:
							__release++;
							break;
						case SDLK_w:
							__period--;
	__width = __period / 2;	// ms
							break;
						case SDLK_x:
							__period++;
	__width = __period / 2;	// ms
							break;
						default:
							break;
					}
					break;
			}
		}
		if (dirty) {
			printf( "square_not_tri=%d period=%d freq=%d width=%d release=%d\n",
				__square_not_tri, __period, __freq, __width, __release);
			dirty = 0;
		}
		SDL_Delay( 100);
	}
	SDL_FreeSurface( screen);
	return 0;
}

