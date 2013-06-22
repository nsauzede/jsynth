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
	uint32_t size, data;

	rbs_t rbs;
	fread( &rbs, sizeof( rbs), 1, in);
	if (strncmp( rbs.chunk_id, "CAT ", 4))
	{
		printf( "not IFF\n");
		goto bye;
	}
	size = ntohl( rbs.chunk_data_size);
	printf( "IFF Group ID %4s, size=%" PRIu32 "\n", rbs.chunk_id, size);
	if (strncmp( rbs.iff_type, "RB40", 4))
	{
		printf( "not RB40\n");
		goto bye;
	}

	head_t head;
	fread( &head, sizeof( head), 1, in);
	if (strncmp( head.chunk_id, "HEAD", 4))
	{
		printf( "no HEAD\n");
		goto bye;
	}
	size = ntohl( head.chunk_data_size);
	printf( "chunk ID HEAD, size=%" PRIu32 "\n", size);
	printf( "version=%s\n", head.version);
	printf( "copyright=%s\n", head.copyright);

	glob_t glob;
	fread( &glob, sizeof( glob), 1, in);
	if (strncmp( glob.chunk_id, "GLOB", 4))
	{
		printf( "no GLOB\n");
		goto bye;
	}
	size = ntohl( glob.chunk_data_size);
	printf( "chunk ID GLOB, size=%" PRIu32 "\n", size);
	printf( "play_mode=%" PRIx8 "\n", glob.play_mode);
	printf( "loop=%" PRIx8 "\n", glob.loop);
	data = ntohl( glob.tempo);
	printf( "tempo=%" PRIu32 "\n", data);
	data = ntohl( glob.loop_start);
	printf( "loop_start=%" PRIu32 "\n", data);
	data = ntohl( glob.loop_end);
	printf( "loop_end=%" PRIu32 "\n", data);
	printf( "shuffle=%" PRIx8 "\n", glob.shuffle_amount);
	printf( "mod_name=%s\n", glob.mod_name);
	printf( "mod_ftp=%s\n", glob.mod_ftp);
	printf( "mod_www=%s\n", glob.mod_www);
	printf( "vintage_mode=%" PRIx8 "\n", glob.vintage_mode);

	usri_t usri;
	fread( &usri, sizeof( usri), 1, in);
	if (strncmp( usri.chunk_id, "USRI", 4))
	{
		printf( "no USRI\n");
		goto bye;
	}
	size = ntohl( usri.chunk_data_size);
	printf( "chunk ID %4s, size=%" PRIu32 "\n", usri.chunk_id, size);
	printf( "window_title=%s\n", usri.window_title);
	printf( "song_information=%s\n", usri.song_information);
	printf( "web_page=%s\n", usri.web_page);
	printf( "show_song_info=%" PRIx8 "\n", usri.show_song_info);

	fread( &rbs, sizeof( rbs), 1, in);
	if (strncmp( rbs.chunk_id, "CAT ", 4))
	{
		printf( "no CAT subgroup for DEVL\n");
		goto bye;
	}
	size = ntohl( rbs.chunk_data_size);
	printf( "IFF Group ID %4s, size=%" PRIu32 "\n", rbs.chunk_id, size);
	if (strncmp( rbs.iff_type, "DEVL", 4))
	{
		printf( "no DEVL\n");
		goto bye;
	}

	uint32_t chunk_size;
	mixr_t mixr;
	fread( &mixr, sizeof( mixr), 1, in);
	if (strncmp( mixr.chunk_id, "MIXR", 4))
	{
		printf( "no MIXR\n");
		goto bye;
	}
	chunk_size = ntohl( mixr.chunk_data_size);
	printf( "chunk ID %4s, size=%" PRIu32 "\n", mixr.chunk_id, chunk_size);
	printf( "master_fader_level=%" PRIx8 "\n", mixr.master_fader_level);
	printf( "compressor_id=%" PRIx8 "\n", mixr.compressor_id);
	printf( "pcf_id=%" PRIx8 "\n", mixr.pcf_id);
	printf( "mix_enabled_3a=%" PRIx8 "\n", mixr.mix_enabled_3a);
	printf( "mix_level_3a=%" PRIx8 "\n", mixr.mix_level_3a);
	printf( "mix_pan_3a=%" PRIx8 "\n", mixr.mix_pan_3a);
	printf( "delay_send_amt_3a=%" PRIx8 "\n", mixr.delay_send_amt_3a);
	printf( "disto_enabled_3a=%" PRIx8 "\n", mixr.disto_enabled_3a);
	printf( "mix_enabled_3b=%" PRIx8 "\n", mixr.mix_enabled_3b);
	printf( "mix_level_3b=%" PRIx8 "\n", mixr.mix_level_3b);
	printf( "mix_pan_3b=%" PRIx8 "\n", mixr.mix_pan_3b);
	printf( "delay_send_amt_3b=%" PRIx8 "\n", mixr.delay_send_amt_3b);
	printf( "disto_enabled_3b=%" PRIx8 "\n", mixr.disto_enabled_3b);
	printf( "mix_enabled_8=%" PRIx8 "\n", mixr.mix_enabled_8);
	printf( "mix_level_8=%" PRIx8 "\n", mixr.mix_level_8);
	printf( "mix_pan_8=%" PRIx8 "\n", mixr.mix_pan_8);
	printf( "delay_send_amt_8=%" PRIx8 "\n", mixr.delay_send_amt_8);
	printf( "disto_enabled_8=%" PRIx8 "\n", mixr.disto_enabled_8);
	printf( "mix_enabled_9=%" PRIx8 "\n", mixr.mix_enabled_9);
	printf( "mix_level_9=%" PRIx8 "\n", mixr.mix_level_9);
	printf( "mix_pan_9=%" PRIx8 "\n", mixr.mix_pan_9);
	printf( "delay_send_amt_9=%" PRIx8 "\n", mixr.delay_send_amt_9);
	printf( "disto_enabled_9=%" PRIx8 "\n", mixr.disto_enabled_9);

	size -= chunk_size;

	while (size > 0)
	{
		void *buf;
		chunk_t chunk;
		fread( &chunk, sizeof( chunk), 1, in);
		chunk_size = ntohl( chunk.chunk_data_size);
		printf( "chunk ID %4s, size=%" PRIu32 "\n", chunk.chunk_id, chunk_size);
		buf = malloc( size);
		fread( buf, size, 1, in);
		free( buf);

		size -= chunk_size;
	}

bye:
	fclose( in);

	return 0;
}
