#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>

#include "rbs.h"

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

typedef struct {
	rbs_t rb40;
	head_t head;
	glob_t glob;
	usri_t usri;

	rbs_t devl;
	mixr_t mixr;
	dely_t dely;
	pcf_t pcf;
	dist_t dist;
	comp_t comp;
	tb303_t tb303a;
	tb303_t tb303b;
	tb808_t tb808;
	tb909_t tb909;

	rbs_t trkl;
	trak1_t trak1;
} x0x_t;

#define htonl ntohl
uint32_t ntohl( uint32_t val)
{
	uint32_t result;

	result = ((val & 0x000000ff) << 24) +
		((val & 0x0000ff00) << 8) +
		((val & 0x00ff0000) >> 8) +
		((val & 0xff000000) >> 24);
	return result;
}

int x0x_write_rbs( x0x_t *x0x, char *name)
{
	FILE *out = fopen( name, "wb");
	int rb40_size = 0, devl_size = 0, trkl_size = 0, size;
 	
	rb40_size += size = sizeof( head_t) - 8;
	x0x->head.chunk_data_size = htonl( size);
	rb40_size += size = sizeof( glob_t) - 8;
	x0x->glob.chunk_data_size = htonl( size);
	rb40_size += size = sizeof( usri_t) - 8;
	x0x->usri.chunk_data_size = htonl( size);

	rb40_size += devl_size += size = sizeof( mixr_t) - 8;
	x0x->mixr.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( dely_t) - 8;
	x0x->dely.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( pcf_t) - 8;
	x0x->pcf.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( dist_t) - 8;
	x0x->dist.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( comp_t) - 8;
	x0x->comp.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( tb303_t) - 8 - 1;
	x0x->tb303a.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( tb303_t) - 8 - 1;
	x0x->tb303b.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( tb808_t) - 8;
	x0x->tb808.chunk_data_size = htonl( size);
	rb40_size += devl_size += size = sizeof( tb909_t) - 8 - 1;
	x0x->tb909.chunk_data_size = htonl( size);

	rb40_size += trkl_size += size = sizeof( trak1_t) - 8;
	x0x->trak1.chunk_data_size = htonl( size);

	x0x->trkl.chunk_data_size = htonl( trkl_size);
	x0x->devl.chunk_data_size = htonl( devl_size);
	x0x->rb40.chunk_data_size = htonl( rb40_size);
	
	strncpy( x0x->rb40.chunk_id, "CAT ", 4);
	strncpy( x0x->rb40.iff_type, "RB40", 4);

	strncpy( x0x->head.chunk_id, "HEAD", 4);
	strncpy( x0x->glob.chunk_id, "GLOB", 4);
	strncpy( x0x->usri.chunk_id, "USRI", 4);

	strncpy( x0x->devl.chunk_id, "CAT ", 4);
	strncpy( x0x->devl.iff_type, "DEVL", 4);

	strncpy( x0x->mixr.chunk_id, "MIXR", 4);
	strncpy( x0x->dely.chunk_id, "DELY", 4);
	strncpy( x0x->pcf.chunk_id, "PCF ", 4);
	strncpy( x0x->dist.chunk_id, "DIST", 4);
	strncpy( x0x->comp.chunk_id, "COMP", 4);
	strncpy( x0x->tb303a.chunk_id, "303 ", 4);
	strncpy( x0x->tb303b.chunk_id, "303 ", 4);
	strncpy( x0x->tb808.chunk_id, "808 ", 4);
	strncpy( x0x->tb909.chunk_id, "909 ", 4);

	strncpy( x0x->trkl.chunk_id, "CAT ", 4);
	strncpy( x0x->trkl.iff_type, "TRKL", 4);
	
	strncpy( x0x->trak1.chunk_id, "TRAK", 4);

	fwrite( x0x, sizeof( *x0x), 1, out);
	fclose( out);

	return 0;
}

int main( int argc, char *argv[]) {
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
	char *fname = 0;
	int arg = 1;
	if (arg < argc)
		fname = argv[arg++];
	
	uint8_t version[9] = { 0x5b, 0x54, 0x5b, 0x54, 0xbc, 0x04, 0x02, 0x00, 0x00 };
//	char copyright[129] = "(c)2013 Nicolas Sauzede, all rights reserved";
	char copyright[129] = "(c)1997 Propellerhead Software, all rights reserved";
#if 0
	int tempo = __tempo * 1000;
	int master = __volume * 0x7f / 100;
	int pattern = 0;
	int tune = __tune * 0x7f / 100;
	int cutoff = __cutoff * 0x7f / 100;
	int reso = __reso * 0x7f / 100;
	int env_mod = __envmod * 0x7f / 100;
	int decay = __decay * 0x7f / 100;
	int accent = __accent * 0x7f / 100;
	int wave = __square_not_tri;
	int nbanks = sizeof( banks) / sizeof( banks[0]);
	printf( "HEAD:\n");
	printf( "version=");
	int i;
	for (i = 0; i < sizeof( version); i++)
		printf( " %02" PRIx8, version[i]);
	printf( "\n");
	printf( "copyright=%s\n", copyright);
	printf( "GLOB:\n");
	printf( "tempo=%d\n", tempo);
	printf( "USRI:\n");
	printf( "song_info=%s\n", song_info);
	printf( "MIXR:\n");
	printf( "master=%d\n", master);
	printf( "303:\n");
	printf( "pattern=%d\n", pattern);
	printf( "tune=%d\n", tune);
	printf( "cutoff=%d\n", cutoff);
	printf( "reso=%d\n", reso);
	printf( "env_mod=%d\n", env_mod);
	printf( "decay=%d\n", decay);
	printf( "accent=%d\n", accent);
	printf( "wave_form=%d (%s)\n", wave, wave == 0 ? "saw" : "square");
	printf( "steps=%d\n", __steps);
	printf( "TRAK:\n");
	printf( "nbars=%d\n", nbars);
	printf( "nbanks=%d\n", nbanks);
#endif

#if 1
	if (fname)
	{
		x0x_t *x0x = malloc( sizeof( x0x_t));
		memset( x0x, 0, sizeof( *x0x));
		memcpy( x0x->head.version, version, sizeof( x0x->head.version));
		strncpy( x0x->head.copyright, copyright, sizeof( x0x->head.copyright));
		x0x_write_rbs( x0x, fname);
	}
#else	
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
#endif

	return 0;
}

