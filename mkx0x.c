#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>

typedef struct {
	int note;
	int octave;
	int play_not_silence;	// 0=silence 1=play
	int accent;
	int slide;
} step_t;

#define MAX_STEPS 16
//#define MAX_STEPS
#define MAX_PATTERNS 8
//#define MAX_PATTERNS

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
// this song inspired by kurt kurasaki - Peff.com
char song_info[] = "this song inspired by kurt kurasaki - Peff.com";
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
// this pattern sounds like "pink floyd - on the run"
char song_info[] = "this pattern sounds like pink floyd - on the run";
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
int song[] = {
	0,
};
#else
#define TEMPO 290
#define TUNE 70
#define STEPS 8
#define CUTOFF 70
#define ACCENT 60
#define SQUARE 1
#define SINE 0
// this song inspired by legend b - lost in love
char song_info[] = "this song inspired by legend b - lost in love";
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
int song[] = {
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
#endif

int sampleFrequency = 44100;

int __play_not_pause = 1;
int __volume = 100;			// %
int __tempo = TEMPO;		// bpm
int __steps = STEPS;		// n
int __tune = TUNE;			// %
int __cutoff = CUTOFF;		// %
int __reso = 0;				// %
int __envmod = 0;			// %
int __accent = ACCENT;		// %
int __square_not_tri = SQUARE;	// 0=tri, 1=square
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
int nbars = sizeof( song) / sizeof( song[0]);

#include "x0x.h"

int main( int argc, char *argv[]) {
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
	char *fname = 0;
	int arg = 1;
	if (arg < argc)
		fname = argv[arg++];
	
	if (!fname)
	{
		printf( "usage: mkx0x <file.x0x>\n");
//		exit( 1);
	}
//	else
	{
		int npat = MAXPAT;

		x0x_t *x0x = malloc( sizeof( x0x_t));
		memset( x0x, 0, sizeof( *x0x));
		strncpy( x0x->song_info, song_info, strlen( song_info));
		x0x->volume = __volume;
		x0x->tempo = __tempo;
		x0x->tune = __tune;
		x0x->cutoff = __cutoff;
		x0x->reso = __reso;
		x0x->envmod = __envmod;
		x0x->accent = __accent;
		x0x->wave_form = __square_not_tri;
		x0x->nbars = __square_not_tri;
		x0x->nbars = nbars;
		int i;
		int p = 0;
		printf( "song_info=%s\n", song_info);
		printf( "npat=%d\n", npat);
		printf( "tempo=%d\n", __tempo);
		printf( "nsteps=%d\n", __steps);
		printf( "nbars=%d\n", nbars);
		for (p = 0; p < npat; p++)
		{
			x0x->nsteps[p] = __steps;
			for (i = 0; i < x0x->nsteps[p]; i++)
			{
				int o, pns, a, s, up, down;
				x0x->steps[p][i][0] = banks[p][i].note;
				o = banks[p][i].octave;
				pns = banks[p][i].play_not_silence;
				a = banks[p][i].accent;
				s = banks[p][i].slide;
				up = o == 2;
				down = o == 0;
				uint8_t flags =
						((pns & 1) << 4) +
						((down & 1) << 3) +
						((up & 1) << 2) +
						((a & 1) << 1) +
						((s & 1) << 0);
				x0x->steps[p][i][1] = flags;
			}
		}
		for (i = 0; i < nbars; i++)
		{
			x0x->song[i] = song[i];
		}

		if (fname)
			x0x_save( x0x, fname);
		free( x0x);
	}

	return 0;
}

