#include <stdio.h>
#define USE_MATH
#ifdef USE_MATH
#include <math.h>
#endif
#include <SDL.h>
#include <jack/jack.h>

typedef struct {
	int note;
	int octave;
	int play_not_silence;	// 0=silence 1=play
	int accent;
	int slide;
} step_t;

#define MAX_STEPS 16
#define AMAX ((double)1.0)
#define FILTER_MIN ((double)50)
#define FILTER_MAX ((double)200)
step_t pattern[MAX_STEPS] = {
//    n  o  p  a  s
// this pattern by kurt kurasaki - Peff.com
	{ 0, 0, 1, 0, 0},		// step1
	{ 0, 1, 1, 0, 0},		// step2
	{ 0, 1, 1, 0, 0},		// step3
	{ 0, 1, 0, 0, 0},		// step4
	{ 3, 1, 1, 1, 0},		// step5
	{ 0, 1, 1, 0, 0},		// step6
	{ 0, 1, 0, 0, 0},		// step7
	{10, 1, 1, 0, 0},		// step8
	{ 0, 2, 1, 0, 1},		// step9
	{ 0, 1, 0, 0, 0},		// step10
	{ 0, 1, 1, 0, 0},		// step11
	{ 0, 1, 0, 0, 0},		// step12
	{ 0, 1, 1, 1, 0},		// step13
	{ 0, 1, 1, 0, 0},		// step14
	{ 0, 1, 1, 0, 0},		// step15
	{ 0, 1, 0, 0, 0},		// step16
};

int sampleFrequency = 44100;
typedef jack_default_audio_sample_t sample_t;

int __volume = 100;			// %
int __tempo = 140;			// bpm
int __steps = 16;			// n
int __tune = 50;			// %
int __cutoff = 100;			// %
int __reso = 0;				// %
int __envmod = 0;			// %
//int __decay = 100;		// %
int __accent = 100;			// %
int __square_not_tri = 0;	// 0=tri, 1=square
int __sine_not_square = 0;	// 0=square, 1=sine

int __delay = 0;			// % of width
int __attack = 0;			// % of width
int __hold = 0;				// % of width
int __decay = 0;			// % of width
int __sustain = 50;			// % of amplitude max
int __release = 5;			// % of width

int next_t = 0;				// sample
int t = 0;					// sample
int pos = -1;				// sample
int step = -1;
int done = 0;

double filter( double s, double fc)
{
	double result = 0;
	static double last = 0;
	double rc = (double)1 / 2 / M_PI / fc;
	double dt = (double)1 / sampleFrequency;
	double a = dt / (rc + dt);
	result = a * s + ((double)1 - a) * last;
	last = result;
	return result;
}

int process_audio( jack_nframes_t nframes, void *arg) {
	jack_port_t *port = arg;
	sample_t *stream = jack_port_get_buffer( port, nframes);
	int i;
	int _tune = __tune;
	int _volume = __volume;
	int note = 0;
	int octave = 0;
	int n = 0;
	int _freq = 1;
	if (!pattern[step].play_not_silence || step == -1) {
		_volume = 0;
	} else {
		note = pattern[step].note;
		octave = pattern[step].octave;
		n = note + (12 * octave) + ((double)24 * _tune / 100);
		_freq = (double)16.3516 * pow( (double)1.0594630943592952645618252949463, n);		// Hz
	}
	int _tempo = __tempo;
	int _steps = __steps;
	int _cutoff = __cutoff;						// %
	int _period = 750 * 20 / _tempo;			// ms
	int _width = _period / 2;					// ms
	int _square_not_tri = __square_not_tri;
	int _sine_not_square = __sine_not_square;
	int _delay = __delay;						// %
	int _attack = __attack;						// %
	int _hold = __hold;							// %
	int _decay = __decay;						// %
	int _sustain = __sustain;					// %
	int _release = __release;					// %
	if (pattern[step].slide) {
		_width = _period - 1;
		_release = 5;
	}
	if (pattern[step].accent) {
		_attack = 10;
		_hold = 10;
		_decay = 80;
	}
	unsigned int bytesPerPeriod = sampleFrequency / _freq;
	for (i = 0; i < nframes; i++) {
		sample_t s = 0;
		if (t >= next_t) {
			next_t = t + _period * sampleFrequency / 1000;
			pos = 0;
//			printf( "step#%d: tune=%d note=%d octave=%d n=%d\n", step, _tune, note, octave, n);
		}
		if (pos >= 0) {
			if (pos >= (_width * sampleFrequency / 1000)) {
				next_t = t + (_period - _width) * sampleFrequency / 1000;
				pos = -1;
				step = (step + 1) % _steps;
			}
			else {
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

				int tri = pos % bytesPerPeriod;		// base triangle

				if (_square_not_tri) {
					if (_sine_not_square)								// sine
						s = sin( pos * 2 * M_PI / bytesPerPeriod);
					else {												// square
						s = (tri >= (bytesPerPeriod / 2)) - 1;
						if (s >= 0)
							s = 1;
						else
							s = -1;
					}
				} else {												// triangle
					s = 2 * ((double)(tri) / bytesPerPeriod) - 1;
				}
				s *= amp * _volume / 100;

				double fc = (double)FILTER_MIN + (double)_cutoff * (FILTER_MAX - FILTER_MIN) / (double)100;
				s = filter( s, fc);
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
void fillbox( SDL_Surface *screen, SDL_Rect *_rect, Uint32 col) {
	SDL_Rect rect;
	rect.w = 1;
	rect.h = 1;
	int x, y;
	for (y = 0; y < _rect->h; y++) {
		rect.y = y + _rect->y;
		for (x = 0; x < _rect->w; x++) {
			rect.x = x + _rect->x;
			SDL_FillRect( screen, &rect, col);
		}
	}
}
int main( int argc, char *argv[]) {
	int arg = 1;
	if (argc > arg) {
		sscanf( argv[arg++], "%d", &__tune);
	if (argc > arg) {
		sscanf( argv[arg++], "%d", &__square_not_tri);
	}
	}
	SDL_Init( SDL_INIT_VIDEO);
	atexit( SDL_Quit);
	int ww = 100;
	int hh = 100;
	SDL_Surface *screen = SDL_SetVideoMode( ww, hh, 32, 0);
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
							__tempo--;
							break;
						case SDLK_a:
							__tempo++;
							break;
						case SDLK_s:
							__steps--;
							break;
						case SDLK_z:
							__steps++;
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
							__cutoff--;
							break;
						case SDLK_o:
							__cutoff++;
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
			printf( "Wsqu=%d Xsin=%d Atempo=%d Zsteps=%d Edelay=%d Rattack=%d Thold=%d Ydecay=%d Usustain=%d Irelease=%d Ocutoff=%d Pvol=%d\n",
				__square_not_tri, __sine_not_square, __tempo, __steps, __delay, __attack, __hold, __decay, __sustain, __release, __cutoff, __volume);
			dirty = 0;
		}
		int _steps = __steps;
		int _step = step;
		static int _old_step = -1;
		int w, h;
		w = ww / _steps;
		h = w;
		SDL_Rect rect;
		Uint32 black, red, blue;
		black = SDL_MapRGB( screen->format, 0, 0, 0);
		red = SDL_MapRGB( screen->format, 255, 0, 0);
		blue = SDL_MapRGB( screen->format, 0, 0, 255);
		rect.x = 0;
		rect.y = 0;
		rect.w = ww;
		rect.h = hh;
		SDL_FillRect( screen, &rect, black);
		rect.x = _step * w;
		rect.y = 0;
		rect.w = w;
		rect.h = h;
		fillbox( screen, &rect, red);
		rect.y = (double)hh - (pattern[_step].note + 13 * pattern[_step].octave + 1) * (hh - h) / (13 * 3);
		fillbox( screen, &rect, blue);
		SDL_UpdateRect( screen, 0, 0, 0, 0);
		if (_old_step == _step)
			SDL_Delay( 1);
		_old_step = _step;
	}
	SDL_FreeSurface( screen);
	return 0;
}

