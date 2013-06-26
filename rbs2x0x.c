#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>

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
		int nbank = sizeof( banks) / sizeof( banks[0]);
		int npat = sizeof( banks) / sizeof( banks[0][0]);
//		int nbank = 1;
//		int npat = 1;

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
		x0x->nsteps = __steps;
		x0x->npat = npat;
		x0x->nbank = nbank;
		x0x->nbars = nbars;
		int i;
		int b = 0;
		int p = 0;
		printf( "song_info=%s\n", song_info);
		printf( "nbank=%d\n", nbank);
		printf( "npat=%d\n", npat);
		printf( "nsteps=%d\n", __steps);
		for (b = 0; b < nbank; b++)
		for (p = 0; p < npat; p++)
		{
			for (i = 0; i < __steps; i++)
			{
			x0x->steps[b][p][i][0] = banks[b][p][i].note;
			x0x->steps[b][p][i][1] = banks[b][p][i].octave;
			x0x->steps[b][p][i][2] = banks[b][p][i].play_not_silence;
			x0x->steps[b][p][i][3] = banks[b][p][i].accent;
			x0x->steps[b][p][i][4] = banks[b][p][i].slide;
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

