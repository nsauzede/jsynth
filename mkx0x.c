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
#define MAX_PATTERNS 8
#define MAX_BANKS 4

typedef step_t pattern_t[MAX_STEPS];
typedef pattern_t bank_t[MAX_PATTERNS];

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
bank_t banks[MAX_BANKS] = {
{
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
}
};
int song[] = {
	0,
};
#elif 1
#define TEMPO 140
#define TUNE 50
#define STEPS 8
#define CUTOFF 100
#define ACCENT 100
#define SQUARE 0
#define SINE 0
// this pattern sounds like "pink floyd - on the run"
char song_info[] = "this pattern sounds like pink floyd - on the run";
bank_t banks[MAX_BANKS] = {
{
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
bank_t banks[MAX_BANKS] = {
{
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

int main( int argc, char *argv[]) {
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
	
	char version[9] = { 0x5b, 0x54, 0x5b, 0x54, 0xbc, 0x04, 0x02, 0x00, 0x00 };
	char copyright[129] = "(c)2013 Nicolas Sauzede, all rights reserved";
	printf( "HEAD:\n");
	printf( "version=%s\n", version);
	printf( "copyright=%s\n", copyright);
	printf( "GLOB:\n");
	int tempo = __tempo * 1000;
	printf( "tempo=%d\n", tempo);
	printf( "USRI:\n");
	printf( "song_info=%s\n", song_info);
	printf( "MIXR:\n");
	int master = __volume * 0x7f / 100;
	printf( "master=%d\n", master);
	printf( "303:\n");
	int pattern = 0;
	printf( "pattern=%d\n", pattern);
	int tune = __tune * 0x7f / 100;
	printf( "tune=%d\n", tune);
	int cutoff = __cutoff * 0x7f / 100;
	printf( "cutoff=%d\n", cutoff);
	int reso = __reso * 0x7f / 100;
	printf( "reso=%d\n", reso);
	int env_mod = __envmod * 0x7f / 100;
	printf( "env_mod=%d\n", env_mod);
	int decay = __decay * 0x7f / 100;
	printf( "decay=%d\n", decay);
	int accent = __accent * 0x7f / 100;
	printf( "accent=%d\n", accent);
	int wave = __square_not_tri;
	printf( "wave_form=%d (%s)\n", wave, wave == 0 ? "saw" : "square");
	printf( "steps=%d\n", __steps);
	printf( "TRAK:\n");
	printf( "nbars=%d\n", nbars);
	int nbanks = sizeof( banks) / sizeof( banks[0]);
	printf( "nbanks=%d\n", nbanks);
	
// nbytes=2 per step
// nsteps=16 per pattern
// npatterns=8 per bank
// nbanks=4

#define NSTEPS 16
#define NPATTERNS 8
#define NBANKS 4
#define NNOTES (NBANKS*NPATTERNS*NSTEPS)
#define NBYTES (2*NNOTES)
	uint8_t *notes = malloc( NBYTES);
	memset( notes, 0, NBYTES);
	FILE *out = fopen( "out.x0x", "wb");
	fwrite( notes, NBYTES, 1, out);
	fclose( out);
	free( notes);

	return 0;
}

