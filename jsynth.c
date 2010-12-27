#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <jack/jack.h>

int sampleFrequency = 44100;

int tempo = 60;		// bpm
int steps = 1;
int tune = 50;		// %
int volume = 30;	// %
int note = 9;		// 9=A
int octave = 1;		// 1=NORM (0=DOWN, 2=UP)
int square_not_tri = 1;

int period = 251*1;	// ms
int width = 132;	// ms
//int freq = 15.7 + note * 2.5 * 2 * octave;		// Hz
int freq = 440;		// Hz
int next_t = 0;		// sample
int t = 0;			// sample
int pos = -1;		// sample

typedef jack_default_audio_sample_t sample_t;
int process_audio( jack_nframes_t nframes, void *arg) {
	jack_port_t *port = arg;
	sample_t *stream = jack_port_get_buffer( port, nframes);
	int i;
	unsigned int bytesPerPeriod = sampleFrequency / freq;
	for (i = 0; i < nframes; i++) {
		sample_t s = 0;
		if (t >= next_t) {
			next_t = t + period * sampleFrequency / 1000;
			pos = 0;
		}
		if (pos >= 0) {
			if (pos >= (width * sampleFrequency / 1000)) {
				next_t = t + (period - width) * sampleFrequency / 1000;
				pos = -1;
			}
			else
				pos++;
		}
		if (pos >= 0)
			s = ((double)0.5 * volume * sin( pos * 2 * M_PI / bytesPerPeriod)) / 100;
		t++;
		stream[i] = s;
	}
	return 0;
}
int main( int argc, char *argv[]) {
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
		jack_activate( client);
	} else {
		printf( "jack server not running ?\n");
	}
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

