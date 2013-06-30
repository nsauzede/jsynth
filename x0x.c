#include <stdio.h>
#include <malloc.h>

#include "x0x.h"

int x0x_save( x0x_t *x0x, char *name)
{
	FILE *out = fopen( name, "wb");
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
	return x0x;
}

