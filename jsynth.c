#include <stdio.h>
#define USE_MATH
#ifdef USE_MATH
#include <math.h>
#endif
#include <SDL.h>
#include <jack/jack.h>

int sampleFrequency = 44100;

int __volume = 100;	// %
int tempo = 140;		// bpm
int steps = 1;
int tune = 50;		// %
int note = 9;		// 9=A
int octave = 1;		// 1=NORM (0=DOWN, 2=UP)

int __sine_not_square = 1;
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
	int _volume = __volume;
	int _freq = __freq;
	int _period = __period;
	int _width = __width;
	int _square_not_tri = __square_not_tri;
	int _sine_not_square = __sine_not_square;
	int _delay = __delay;						// %
	int _attack = __attack;						// %
	int _hold = __hold;						// %
	int _decay = __decay;						// %
	int _sustain = __sustain;						// %
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
				int release_dur = (_width * sampleFrequency / 1000) * _release / 100;
				int sustain_dur = (_width * sampleFrequency / 1000) - (decay_end + release_dur);
				int sustain_start = decay_end;
				int sustain_end = sustain_start + sustain_dur;
				int release_start = (_width * sampleFrequency / 1000) - release_dur;
				int release_end = release_start + release_dur;
				if ((pos >= delay_start) && (pos < delay_end))
					amp *= (double)0;
				else if ((pos >= attack_start) && (pos < attack_end))
					amp *= ((double)pos - attack_start) / attack_dur;
				else if ((pos >= hold_start) && (pos < hold_end))
					amp *= (double)1;
				else if ((pos >= decay_start) && (pos < decay_end))
					amp = (AMAX - AMAX * (double)_sustain / 100) * ((double)decay_dur - (pos - decay_start)) / decay_dur + AMAX * (double)_sustain / 100;
				else if ((pos >= sustain_start) && (pos < sustain_end))
					amp *= (double)_sustain / 100;
				else
				if ((pos >= release_start) && (pos < release_end))
					amp *= ((double)_sustain / 100) * ((double)release_dur - (pos - release_start)) / release_dur;
				if (_square_not_tri) {
					if (_sine_not_square)
						s = sin( pos * 2 * M_PI / bytesPerPeriod);
					else {
						s = ((pos % bytesPerPeriod) >= (bytesPerPeriod / 2)) - 1;
						if (s >= 0)
							s = 1;
						else
							s = -1;
					}
				} else {
					s = 2 * ((double)(pos % bytesPerPeriod) / bytesPerPeriod) - 1;
				}
				s *= amp * _volume / 100;
#if 0
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
						case SDLK_w:
							__square_not_tri = 1 - __square_not_tri;
							break;
						case SDLK_x:
							__sine_not_square = 1 - __sine_not_square;
							break;
						case SDLK_q:
							__period--;
//	__width = __period / 2;	// ms
							break;
						case SDLK_a:
							__period++;
//	__width = __period / 2;	// ms
							break;
						case SDLK_s:
							__freq--;
							break;
						case SDLK_z:
							__freq++;
							break;
						case SDLK_d:
							__delay--;
							break;
						case SDLK_e:
							__delay++;
							break;
						case SDLK_f:
							__attack--;
							break;
						case SDLK_r:
							__attack++;
							break;
						case SDLK_g:
							__hold--;
							break;
						case SDLK_t:
							__hold++;
							break;
						case SDLK_h:
							__decay--;
							break;
						case SDLK_y:
							__decay++;
							break;
						case SDLK_j:
							__sustain--;
							break;
						case SDLK_u:
							__sustain++;
							break;
						case SDLK_k:
							__release--;
							break;
						case SDLK_i:
							__release++;
							break;
						case SDLK_l:
							__width--;
							break;
						case SDLK_o:
							__width++;
							break;
						case SDLK_m:
							__volume--;
							break;
						case SDLK_p:
							__volume++;
							break;
						default:
							break;
					}
					break;
			}
		}
		if (dirty) {
			printf( "Wsqu=%d Xsin=%d Aperiod=%d Zfreq=%d Edelay=%d Rattack=%d Thold=%d Ydecay=%d Usustain=%d Irelease=%d Owidth=%d Pvol=%d\n",
				__square_not_tri, __sine_not_square, __period, __freq, __delay, __attack, __hold, __decay, __sustain, __release, __width, __volume);
			dirty = 0;
		}
		SDL_Delay( 100);
	}
	SDL_FreeSurface( screen);
	return 0;
}

