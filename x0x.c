#include <stdio.h>
#include <malloc.h>

#include "x0x.h"

int x0x_save( x0x_t *x0x, char *name)
{
	FILE *out = fopen( name, "wb");
	printf( "%s: song_info=[%s]\n", __func__, x0x->song_info);
	printf( "%s: nsteps=%d\n", __func__, x0x->nsteps);
	printf( "%s: npat=%d\n", __func__, x0x->npat);
	printf( "%s: nbank=%d\n", __func__, x0x->nbank);
	printf( "%s: nbars=%d\n", __func__, x0x->nbars);
	fwrite( x0x, sizeof( *x0x), 1, out);
	fclose( out);

	return 0;
}
x0x_t *x0x_load( char *fname)
{
	x0x_t *x0x = 0;
	FILE *in = fopen( fname, "rb");
	x0x = malloc( sizeof( *x0x));
	fread( x0x, sizeof( *x0x), 1, in);
	fclose( in);
	printf( "%s: song_info=[%s]\n", __func__, x0x->song_info);
	printf( "%s: nsteps=%d\n", __func__, x0x->nsteps);
	printf( "%s: npat=%d\n", __func__, x0x->npat);
	printf( "%s: nbank=%d\n", __func__, x0x->nbank);
	printf( "%s: nbars=%d\n", __func__, x0x->nbars);
	return x0x;
}

