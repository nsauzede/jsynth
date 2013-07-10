#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>
#include <stdlib.h>
#include "x0x.h"
int main( int argc, char *argv[]) {
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
	char *xname = 0;
	int arg = 1;
	if (arg < argc)
		xname = argv[arg++];
	if (!xname)
	{
		printf( "usage: catx0x <file.x0x>\n");
		exit( 1);
	}
	FILE *in = fopen( xname, "rb");
	if (!in)
	{
		perror( "fopen");
		exit( 1);
	}
	x0x_t *x0x = x0x_load( xname);
	printf( "%s: song_info=[%s]\n", __func__, x0x->song_info);
	printf( "%s: volume=%" PRIu8 "\n", __func__, x0x->volume);
	printf( "%s: tempo=%" PRIu32 "\n", __func__, x0x->tempo);
	printf( "%s: pat=%" PRIu8 "\n", __func__, x0x->pat);
	printf( "%s: tune=%" PRIu8 "\n", __func__, x0x->tune);
	printf( "%s: cutoff=%" PRIu8 "\n", __func__, x0x->cutoff);
	printf( "%s: reso=%" PRIu8 "\n", __func__, x0x->reso);
	printf( "%s: envmod=%" PRIu8 "\n", __func__, x0x->envmod);
	printf( "%s: decay=%" PRIu8 "\n", __func__, x0x->decay);
	printf( "%s: accent=%" PRIu8 "\n", __func__, x0x->accent);
	printf( "%s: wave_form=%" PRIu8 "\n", __func__, x0x->wave_form);
	printf( "%s: nbars=%" PRIu8 "\n", __func__, x0x->nbars);
	int j;
	for (j = 0; j < x0x->nbars; j++)
	{
		printf( "%s: bar%d=%" PRIu8 "\n", __func__, j, x0x->song[j]);
		printf( "%s: nsteps=%" PRIu8 "\n", __func__, x0x->nsteps[j]);
		int i;
		for (i = 0; i < x0x->nsteps[j]; i++)
		{
			printf( "%s: step%d : %" PRIu8 " %" PRIu8 "\n", __func__, i, x0x->steps[j][i][0], x0x->steps[j][i][1]);
		}
	}
	free( x0x);
//bye:
	fclose( in);
	return 0;
}

