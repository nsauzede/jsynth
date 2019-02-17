#include <stdio.h>
#define USE_MATH
#ifdef USE_MATH
#include <math.h>
#endif

#include <SDL.h>
#include <jack/jack.h>

typedef struct {
	uint8_t note;
	int octave;
	int play_not_silence;	// 0=silence 1=play
	int accent;
	int slide;
} step_t;

#define MAX_STEPS 16
#define MAX_PATTERNS 8

typedef step_t pattern_t[MAX_STEPS];

#define AMAX ((double)1.0)
#define FILTER_MIN ((double)500)
#define FILTER_MAX ((double)3000)
#if 0
#define TEMPO 140
#define TUNE 50
#define STEPS 16
#define CUTOFF 100
#define ACCENT 100
#define SQUARE 0
#define SINE 0
char *song_info = "this song inspired by kurt kurasaki - Peff.com";
int steplen[] = { STEPS };
pattern_t banks[] = {
{
//    n  o  p  a  s
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
}
};
int song[] = {
	0,
};
#elif 0
#define TEMPO 140
#define TUNE 50
#define STEPS 8
#define CUTOFF 100
#define ACCENT 100
#define SQUARE 0
#define SINE 0
char *song_info = "this pattern sounds like pink floyd - on the run";
int steplen[] = { STEPS };
pattern_t banks[] = {
{
//    n  o  p  a  s
	{ 7, 0, 1, 0, 0},		// step1
	{11, 0, 1, 0, 0},		// step2
	{ 0, 0, 1, 0, 0},		// step3
	{11, 0, 1, 0, 0},		// step4
	{ 5, 1, 1, 0, 0},		// step5
	{ 4, 1, 1, 0, 0},		// step6
	{ 5, 1, 1, 0, 0},		// step7
	{ 7, 1, 1, 0, 0},		// step8
}
};
uint8_t song[] = {
	0,
};
#elif 0
#define TEMPO 290
#define TUNE 70
#define STEPS 8
#define CUTOFF 70
#define ACCENT 60
#define SQUARE 1
#define SINE 0
char *song_info = "this song inspired by legend b - lost in love";
uint8_t steplen[] = {
	STEPS,
	STEPS,
	STEPS,
	STEPS,
	STEPS,
	STEPS,
};
pattern_t banks[] = {
{
//    n  o  p  a  s
	{ 9, 2, 1, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{ 9, 2, 1, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{ 9, 2, 1, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{ 9, 2, 1, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{ 9, 2, 1, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{ 9, 2, 1, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
},
{
//    n  o  p  a  s
	{11, 2, 1, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{11, 2, 1, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{11, 2, 1, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{11, 2, 1, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{11, 2, 1, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{11, 2, 1, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
},
{
//    n  o  p  a  s
	{ 8, 2, 1, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{ 8, 2, 1, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{ 8, 2, 1, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{ 8, 2, 1, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{ 8, 2, 1, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{ 8, 2, 1, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
},
{
//    n  o  p  a  s
	{ 6, 2, 1, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{ 6, 2, 1, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{ 6, 2, 1, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{ 6, 2, 1, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{ 6, 2, 1, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{ 6, 2, 1, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
},
{
//    n  o  p  a  s
	{ 4, 2, 1, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{ 4, 2, 1, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{ 4, 2, 1, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{ 4, 2, 1, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{ 4, 2, 1, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{ 4, 2, 1, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
},
{
//    n  o  p  a  s
	{ 4, 2, 0, 1, 0},		// step1
	{ 0, 2, 0, 0, 0},		// step2
	{ 4, 2, 0, 1, 0},		// step3
	{ 0, 2, 0, 0, 0},		// step4
	{ 4, 2, 0, 1, 1},		// step5
	{ 0, 2, 0, 0, 0},		// step6
	{ 0, 2, 0, 0, 0},		// step7
	{ 0, 2, 0, 0, 0},		// step8
	{ 4, 2, 0, 1, 0},		// step9
	{ 0, 2, 0, 0, 0},		// step10
	{ 4, 2, 0, 1, 0},		// step11
	{ 0, 2, 0, 0, 0},		// step12
	{ 4, 2, 0, 1, 1},		// step13
	{ 0, 2, 0, 0, 0},		// step14
	{ 0, 2, 0, 0, 0},		// step15
	{ 0, 2, 0, 0, 0},		// step16
}
};
uint8_t song[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	3, 3, 3, 3,	3, 3, 3, 3,
	2, 2, 2, 2,
	4, 4, 4, 4,
	3, 3, 3, 3,	3, 3, 3, 3,
	2, 2, 2, 2,
	0, 0, 0,
//	1,
	5, 5,
};
#else
#define USE_X0X
#define SINE 0
#include "x0x.h"
uint32_t TEMPO;
uint8_t TUNE, CUTOFF, ACCENT, SQUARE, NBARS;
uint8_t *song;
char *song_info;
pattern_t banks[MAXPAT];
uint8_t steplen[MAXPAT];
int load( char *fname)
{
	x0x_t *x0x = x0x_load( fname);
	TUNE = x0x->tune;
	TEMPO = x0x->tempo;
	printf( "tempo=%" PRIu32 "\n", TEMPO);
	CUTOFF = x0x->cutoff;
	SQUARE = x0x->wave_form;
	NBARS = x0x->nbars;
	song_info = strdup( x0x->song_info);
	song = malloc( sizeof( song[0]) * x0x->nbars);
	memcpy( song, x0x->song, sizeof( song[0]) * x0x->nbars);
	int j, i;
	uint8_t  npat = MAXPAT;
	for (j = 0; j < npat; j++)
	{
		steplen[j] = x0x->nsteps[j];
		for (i = 0; i < x0x->nsteps[j]; i++)
		{
			int n, o, p, a, s;
			uint8_t flags = x0x->steps[j][i][1];
			n = x0x->steps[j][i][0];
			int up, down;
			p = !!(flags & 0x10);
			down = !!(flags & 0x08);
			up = !!(flags & 0x04);
			a = !!(flags & 0x02);
			s = !!(flags & 0x01);
			o = up && down ? 1 : up && !down ? 2 : !up && down ? 0 : 1;
			banks[j][i].note = n;
			banks[j][i].octave = o;
			banks[j][i].play_not_silence = p;
			banks[j][i].accent = a;
			banks[j][i].slide = s;
		}
	}
	free( x0x);
	return 0;
}
#define __tune TUNE
#define __tempo TEMPO
#define __cutoff CUTOFF
#define __square_not_tri SQUARE
#define nbars NBARS
#endif

int sampleFrequency = 44100;
typedef jack_default_audio_sample_t sample_t;

int __play_not_pause = 1;
int __volume = 100;			// %
#ifndef USE_X0X
int __tempo = TEMPO;		// bpm
int __steps = STEPS;		// n
int __tune = TUNE;			// %
int __cutoff = CUTOFF;		// %
int __accent = ACCENT;		// %
int __square_not_tri = SQUARE;	// 0=tri, 1=square
#endif
int __reso = 0;				// %
int __envmod = 0;			// %
int __sine_not_square = SINE;	// 0=square, 1=sine

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
int pattern = 0;
int bank = 0;
int done = 0;
int bar = 0;
#ifndef USE_X0X
int nbars = sizeof( song) / sizeof( song[0]);
#endif

#if 0
// this is some kind of half-working passive LPF.. (no resonance)
// double filter( double s, double fc)
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
#else
// this is an attempt of SVF LPF with resonnance (see http://www.fpga.synth.net/pmwiki/pmwiki.php?n=FPGASynth.SVF)
double filter( double input, double fc)
{
	static double output = 0.0;

	static double fb1 = 0.0, fb2 = 0.0;
	static double sum1, sum2, sum3;
	double f;
	double q, Q;
	double mult1, mult2, multq;
	f = 2 * sin( M_PI * fc / sampleFrequency);
	Q = 0.5 + __reso * 1.8 / 100;
	q = 1.0 / Q;

	multq = fb1 * q;
	
	sum1 = input + (-multq) + (-output);
	
	mult1 = f * sum1;
	sum2 = mult1 + fb1;

	mult2 = f * fb1;
	sum3 = mult2 + fb2;

	fb1 = sum2;
	fb2 = sum3;

	output = sum3;

	return output;
}
#endif

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
	int _tempo = __tempo;
	int _steps = steplen[pattern];
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
	if (step == -1 || !banks[pattern][step].play_not_silence || !__play_not_pause) {
		_volume = 0;
	} else {
		note = banks[pattern][step].note;
		octave = banks[pattern][step].octave;
		n = note + (12 * octave) + ((double)24 * _tune / 100);
		_freq = (double)16.3516 * pow( (double)1.0594630943592952645618252949463, n);		// Hz
		if (banks[pattern][step].slide) {
			_width = _period - 1;
			_release = 5;
		}
		if (banks[pattern][step].accent) {
			_attack = 10;
			_hold = 10;
			_decay = 80;
		}
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
				if ((step + 1) >= _steps)
				{
					bar = (bar + 1) % nbars;
					pattern = song[bar];
				}
				if (__play_not_pause)
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
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
#ifdef USE_X0X
	int arg = 1;
	char *fname = 0;
	if (argc > arg) {
		fname = argv[arg++];
	}

	if (fname)
		load( fname);
	else
	{
		printf( "usage: jsynth <file.x0x>\n");
		exit( 1);
	}
#endif

	printf( "playing [%s]\n", song_info);
	jack_status_t status;
	jack_client_t *client = jack_client_open( "jsynth", JackNoStartServer, &status);
	jack_port_t *port;
	if (client) {
		port = jack_port_register( client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		jack_set_process_callback( client, process_audio, port);
		sampleFrequency = jack_get_sample_rate( client);
		printf( "sampleFrequency=%d\n", sampleFrequency);
		jack_activate( client);
	} else {
		printf( "jack server not running ?\n");
		exit( 1);
	}
	SDL_Init( SDL_INIT_VIDEO);
	atexit( SDL_Quit);
//	freopen( "CON", "w", stdout );
//	freopen( "CON", "w", stderr );
	int ww = 100;
	int hh = 100;
#ifdef SDL1
	SDL_Surface *screen = SDL_SetVideoMode( ww, hh, 32, 0);
	if (!screen) {
		fprintf(stderr, "Unable to open video: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#else
	SDL_Window *sdlWindow = 0;
	SDL_Renderer *sdlRenderer = 0;
	SDL_Texture *sdlTexture = 0;
	SDL_CreateWindowAndRenderer(ww, hh, 0
//	|| SDL_WINDOW_FULLSCREEN_DESKTOP
	, &sdlWindow, &sdlRenderer);
	SDL_Surface *screen = SDL_CreateRGBSurface( 0, ww, hh, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
	if (screen) {
		sdlTexture = SDL_CreateTexture( sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, ww, hh);
	} else {
		fprintf(stderr, "Unable to open video: %s\n", SDL_GetError());
		exit(1);
	}
#endif
	int dirty = 1;
	int done = 0;
	int shift = 0;
	while (!done) {
		SDL_Event event;
		while (!done && SDL_PollEvent( &event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_LSHIFT:
						case SDLK_RSHIFT:
							shift = 0;
							dirty = 0;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYDOWN:
					dirty = 1;
					switch (event.key.keysym.sym) {
						case SDLK_LSHIFT:
						case SDLK_RSHIFT:
							shift = 1;
							dirty = 0;
							break;
						case SDLK_ESCAPE:
							done = 1;
							break;
						case SDLK_SPACE:
							__play_not_pause = 1 - __play_not_pause;
							if (__play_not_pause)
								step = 0;
							break;
						case SDLK_w:
							__square_not_tri = 1 - __square_not_tri;
							break;
						case SDLK_x:
							__sine_not_square = 1 - __sine_not_square;
							break;
						case SDLK_a:
							if (shift)
								__tempo++;
							else
								__tempo--;
							break;
						case SDLK_z:
							if (shift)
							{
								if (steplen[pattern] < MAX_STEPS)
									steplen[pattern]++;
							}
							else
							{
								if (steplen[pattern] > 1)
									steplen[pattern]--;
							}
							break;
						case SDLK_e:
							if (shift)
								__delay++;
							else
								__delay--;
							break;
						case SDLK_r:
							if (shift)
								__attack++;
							else
								__attack--;
							break;
						case SDLK_t:
							if (shift)
								__hold++;
							else
								__hold--;
							break;
						case SDLK_y:
							if (shift)
								__decay++;
							else
								__decay--;
							break;
						case SDLK_u:
							if (shift)
								__sustain++;
							else
								__sustain--;
							break;
						case SDLK_i:
							if (shift)
								__release++;
							else
								__release--;
							break;
						case SDLK_o:
							if (shift)
								__cutoff++;
							else
								__cutoff--;
							break;
						case SDLK_p:
							if (shift)
								__volume++;
							else
								__volume--;
							break;
						case SDLK_v:
							if (shift)
								__tune++;
							else
								__tune--;
							break;
						case SDLK_b:
							if (shift)
								__reso++;
							else
								__reso--;
							break;
						default:
							break;
					}
					break;
			}
		}
		int _steps = steplen[pattern];
		if (dirty) {
			printf( "%cWsqu=%d Xsin=%d Atempo=%d Zsteps=%d Edelay=%d Rattack=%d Thold=%d Ydecay=%d Usustain=%d Irelease=%d Ocutoff=%d Breso=%d Pvol=%d Vtune=%d\n",
				__play_not_pause?'=':'-', __square_not_tri, __sine_not_square, __tempo, _steps, __delay, __attack, __hold, __decay, __sustain, __release, __cutoff, __reso, __volume, __tune);
			fflush( stdout);
			dirty = 0;
		}
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
		rect.y = (double)hh - (banks[pattern][_step].note + 13 * banks[pattern][_step].octave + 1) * (hh - h) / (13 * 3);
		fillbox( screen, &rect, blue);
#ifdef SDL1
		SDL_UpdateRect(screen, 0, 0, 0, 0);
#else
		SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(sdlRenderer);
#endif
		if (_old_step == _step)
			SDL_Delay( 1);
		_old_step = _step;
	}
	SDL_FreeSurface( screen);
	return 0;
}

