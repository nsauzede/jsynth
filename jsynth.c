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

int square_not_tri = 1;

//int period = 60 * sampleFrequency / tempo / steps;	// ms
//int period = 251;	// ms
int __period = 0;	// ms
//int width = period / 2;	// ms
//int __width = 132;	// ms
int __width = 0;	// ms
//int freq = 15.7 + note * 2.5 * 2 * octave;		// Hz
//int freq = 440;		// Hz
int __freq = 0;		// Hz

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
				double amp = 0.5;
				int decay = 10;						// %
				int decay_dur = (_width * sampleFrequency / 1000) * decay / 100;
				int decay_start = (_width * sampleFrequency / 1000) - decay_dur;
				int decay_end = (_width * sampleFrequency / 1000);
				if ((pos >= decay_start) && (pos < decay_end))
					amp *= ((double)decay_dur - (pos - decay_start)) / decay_dur;
//				printf( "pos=%d width=%d decay=%d decay_dur=%d decay_start=%d decay_end=%d amp=%f\n", pos, width, decay, decay_dur, decay_start, decay_end, amp);
//				s = sin( pos * 2 * M_PI / bytesPerPeriod);
				s = ((pos % bytesPerPeriod) >= (bytesPerPeriod / 2)) - 1;
				if (square_not_tri) {
					if (s >= 0)
						s = 1;
					else
						s = -1;
				} else {
					s = 2 * ((double)(pos % bytesPerPeriod) / bytesPerPeriod) - 1;
				}
				s *= amp * volume / 100;
				printf( "pos=%d bytesPerPeriod=%d width=%d decay=%d decay_dur=%d decay_start=%d decay_end=%d amp=%.1f s=%.1f\n",
					pos, bytesPerPeriod, _width, decay, decay_dur, decay_start, decay_end, amp, s);
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
		sscanf( argv[arg++], "%d", &square_not_tri);
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
	printf( "period=%d freq=%d\n", __period, __freq);
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
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							done = 1;
						default:
							break;
					}
					break;
			}
		}
		SDL_Delay( 100);
	}
	SDL_FreeSurface( screen);
	return 0;
}

