#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

typedef struct {
  uint8_t chunk_id[4];
  uint32_t chunk_data_size;
} chunk_t;

int main( int argc, char *argv[])
{
	char *chunkname = 0;
	char *fname = 0;
	int arg = 1;

	if (arg < argc)
	{
		fname = argv[arg++];
		if (arg < argc)
			chunkname = argv[arg];
	}
	if (!fname)
	{
		printf( "usage: %s <mid_file>\n", argv[0]);
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
		uint8_t *buf;
		char str[5];
		int disp = 0;
		memset( str, 0, sizeof( str));
		chunk_t chunk;
		fread( &chunk, sizeof( chunk), 1, in);
		if (feof( in))
			break;
		chunk_size = ntohl( chunk.chunk_data_size);
		strncpy( str, (char *)chunk.chunk_id, 4);
		if (!chunkname || !strcmp( chunkname, str))
			disp = 1;
		if (disp)
			printf( "chunk ID %s, size=%" PRIu32 "\n", str, chunk_size);
		if (!strncmp( (char *)chunk.chunk_id, "CAT ", 4))
		{
			fread( str, 4, 1, in);
			printf( "IFF Subgroup ID %s\n", str);
		}
		else
		{
			buf = malloc( chunk_size);
			fread( buf, chunk_size, 1, in);
			if (disp)
			{
				int i;
				for (i = 0; i < chunk_size; i++)
				{
					uint8_t b = buf[i];
					printf( " %02" PRIx8, b);
					if ((i >= 8) && !chunkname)
					{
						printf( " ..");
						break;
					}
				}
				printf( "\n");
			}
			free( buf);
		}
	}
	fclose( in);

	return 0;
}
