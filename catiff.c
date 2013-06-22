#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "rbs.h"

uint32_t ntohl( uint32_t val)
{
	uint32_t result;

	result = ((val & 0x000000ff) << 24) +
		((val & 0x0000ff00) << 8) +
		((val & 0x00ff0000) >> 8) +
		((val & 0xff000000) >> 24);
	return result;
}

int main( int argc, char *argv[])
{
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif

	printf( "hello rbs\n");

	char *fname = 0;
	int arg = 1;

	if (arg < argc)
	{
		fname = argv[arg++];
	}
	if (!fname)
	{
		printf( "usage: rbs <file.rbs>\n");
		exit( 0);
	}
	FILE *in;
	in = fopen( fname, "rb");
	if (!in)
	{
		perror( "fopen");
		exit( 1);
	}
	uint32_t chunk_size;
	while (!feof( in))
	{
		void *buf;
		char str[5];
		memset( str, 0, sizeof( str));
		chunk_t chunk;
		fread( &chunk, sizeof( chunk), 1, in);
		chunk_size = ntohl( chunk.chunk_data_size);
		strncpy( str, chunk.chunk_id, 4);
		printf( "chunk ID %s, size=%" PRIu32 "\n", str, chunk_size);
		if (!strncmp( chunk.chunk_id, "CAT ", 4))
		{
			fread( str, 4, 1, in);
			printf( "IFF Subgroup ID %s\n", str);
		}
		else
		{
			buf = malloc( chunk_size);
			fread( buf, chunk_size, 1, in);
			free( buf);
		}
	}
	fclose( in);

	return 0;
}

