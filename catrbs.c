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
	uint32_t size;

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
	printf( "version=");
	int i;
	for (i = 0; i < sizeof( head.version); i++)
		printf( " %02" PRIx8, head.version[i]);
	printf( "\n");
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
#if 1
	printf( "glob chunk !!\n");
#else
	uint32_t data;
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
#endif

	usri_t usri;
	fread( &usri, sizeof( usri), 1, in);
	if (strncmp( usri.chunk_id, "USRI", 4))
	{
		printf( "no USRI\n");
		goto bye;
	}
	size = ntohl( usri.chunk_data_size);
	printf( "chunk ID %4s, size=%" PRIu32 "\n", usri.chunk_id, size);
#if 1
	printf( "usri chunk !!\n");
#else
	printf( "window_title=%s\n", usri.window_title);
	printf( "song_information=%s\n", usri.song_information);
	printf( "web_page=%s\n", usri.web_page);
	printf( "show_song_info=%" PRIx8 "\n", usri.show_song_info);
#endif

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
#if 1
	printf( "mixr chunk !!\n");
#else
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
#endif

	size -= chunk_size;

	uint8_t pad = 0;
	printf( "size0=%d\n", size);
	while (!feof( in) && (size > 0))
	{
//		printf( "size=%d\n", size);
		uint8_t *buf;
		chunk_t chunk;
		fread( &chunk, sizeof( chunk), 1, in);
		if (feof( in))
      break;
		chunk_size = ntohl( chunk.chunk_data_size);
		if (!strncmp( chunk.chunk_id, "CAT ", 4))
		{
			char str[5];
			memset( str, 0, 5);
			fread( str, 4, 1, in);
			printf( "Subchunk ID %4s : %s\n", chunk.chunk_id, str);
		}
		else
		{
			printf( "chunk ID %4s, size=%" PRIu32 " (%" PRIx32 ")\n", chunk.chunk_id, chunk_size, chunk_size);
			if (!strncmp( chunk.chunk_id, "303 ", 4))
			{
				tb303_t tb303;
				fread( (char *)&tb303 + sizeof( chunk), sizeof( tb303) - sizeof( chunk), 1, in);
#if 1
				printf( "303 chunk !!\n");
#else
				printf( "enabled=%d\n", tb303.tb303_enabled);
				printf( "pattern=%d\n", tb303.tb303_pattern);
				printf( "tune=%d\n", tb303.tb303_tune);
				printf( "cutoff=%d\n", tb303.tb303_cutoff);
				printf( "reso=%d\n", tb303.tb303_reso);
				printf( "env_mod=%d\n", tb303.tb303_env_mod);
				printf( "decay=%d\n", tb303.tb303_decay);
				printf( "accent=%d\n", tb303.tb303_accent);
				printf( "waveform=%d\n", tb303.tb303_wave);
#endif
				size -= chunk_size;
			}
			else
			if (!strncmp( chunk.chunk_id, "TRAK", 4))
			{
        char *traks[] = {
          "mixer", "tb303a", "tb303b", "tr808", "tr909", "delay", "dist", "pcf", "compr"
        };
        static int count = 0;
				uint32_t trak_nevents;
				fread( &trak_nevents, 4, 1, in);
				trak_nevents = ntohl( trak_nevents);
				printf( "%s trak_nevents=%" PRIu32 " (%" PRIx32 ")\n", traks[count], trak_nevents, trak_nevents);
				int i;
				uint32_t abspos = 0;
				for (i = 0; i < trak_nevents; i++)
				{
          printf( "%s event#%d :", traks[count], i);
					fread( &pad, 1, 1, in);
//          printf( " pad=%" PRIu8, pad);
					uint32_t deltapos;
          deltapos = pad;
					if (pad & 0x80)
					{
            deltapos &= 0x7f;
            do {
              fread( &pad, 1, 1, in);
//              printf( " pad=%" PRIu8, pad);
              deltapos = (deltapos << 7) | (pad & 0x7f);
            } while (pad & 0x80);
					}
					abspos += deltapos;
//          printf( " deltapos=%" PRIu32, deltapos);
          printf( " abspos=%" PRIu32, abspos);
          fread( &pad, 1, 1, in);
          char *traktype( int trak, int ID)
          {
            char *ret = "??";
            switch (trak)
            {
              case 0://mixer
                switch (ID)
                {
                  case 0x01: ret = "Compressor device id";break;
                  case 0x02: ret = "PCF device id";break;
                  case 0x06: ret = "TB303 1 mix level";break;
                  case 0x07: ret = "TB303 1 pan";break;
                  case 0x08: ret = "TB303 1 delay send amt";break;
                  case 0x09: ret = "TB303 1 dist enabled";break;
                  case 0x0c: ret = "TB303 2 mix level";break;
                  case 0x0d: ret = "TB303 2 pan";break;
                  case 0x0e: ret = "TB303 2 delay send amt";break;
                  case 0x0f: ret = "TB303 2 dist enabled";break;
                  case 0x12: ret = "TR808 mix level";break;
                  case 0x13: ret = "TR808 pan";break;
                  case 0x14: ret = "TR808 delay send amt";break;
                  case 0x15: ret = "TR808 dist enabled";break;
                  case 0x18: ret = "TR909 mix level";break;
                  case 0x19: ret = "TR909 pan";break;
                  case 0x1a: ret = "TR909 delay send amt";break;
                  case 0x1b: ret = "TR909 dist enabled";break;
                }
                break;
              case 1: case 2://tb303
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Selected pattern";break;
                  case 0x02: ret = "Tune";break;
                  case 0x03: ret = "Cutoff";break;
                  case 0x04: ret = "Resonance";break;
                  case 0x05: ret = "EnvMod";break;
                  case 0x06: ret = "Decay";break;
                  case 0x07: ret = "Accent";break;
                  case 0x08: ret = "Waveform";break;
                }
                break;
              case 3://tb808
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Selected pattern";break;
                  case 0x02: ret = "AccentLevel";break;
                  case 0x03: ret = "BassLevel";break;
                  case 0x04: ret = "BassTone";break;
                  case 0x05: ret = "BassDecay";break;
                  case 0x06: ret = "SnareLevel";break;
                  case 0x07: ret = "SnareTone";break;
                  case 0x08: ret = "SnareSnappy";break;
                  case 0x09: ret = "LowTomLevel";break;
                  case 0x0a: ret = "LowTomTuning";break;
                  case 0x0b: ret = "LowTomSelector";break;
                  case 0x0c: ret = "MidTomLevel";break;
                  case 0x0d: ret = "MidTomTuning";break;
                  case 0x0e: ret = "MidTomSelector";break;
                  case 0x0f: ret = "HiTomLevel";break;
                  case 0x10: ret = "HiTomTuning";break;
                  case 0x11: ret = "HiTomSelector";break;
                  case 0x12: ret = "RimShotLevel";break;
                  case 0x13: ret = "RimShotSelector";break;
                  case 0x14: ret = "ClapLevel";break;
                  case 0x15: ret = "ClapSelector";break;
                  case 0x16: ret = "CowBellLevel";break;
                  case 0x17: ret = "CymbalLevel";break;
                  case 0x18: ret = "CymbalTone";break;
                  case 0x19: ret = "CymbalDecay";break;
                  case 0x1a: ret = "OpenHiHatLevel";break;
                  case 0x1b: ret = "OpenHiHatDecay";break;
                  case 0x1c: ret = "ClosedHiHatLevel";break;
                }
                break;
              case 4://tb909
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Selected pattern";break;
                  case 0x02: ret = "AccentLevel";break;
                  case 0x03: ret = "BassLevel";break;
                  case 0x04: ret = "BassTune";break;
                  case 0x05: ret = "BassAttack";break;
                  case 0x06: ret = "BassDecay";break;
                  case 0x07: ret = "SnareLevel";break;
                  case 0x08: ret = "SnareTune";break;
                  case 0x09: ret = "SnareTone";break;
                  case 0x0a: ret = "SnareSnappy";break;
                  case 0x0b: ret = "LowTomLevel";break;
                  case 0x0c: ret = "LowTomTune";break;
                  case 0x0d: ret = "LowTomDecay";break;
                  case 0x0e: ret = "MidTomLevel";break;
                  case 0x0f: ret = "MidTomTune";break;
                  case 0x10: ret = "MidTomDecay";break;
                  case 0x11: ret = "HiTomLevel";break;
                  case 0x12: ret = "HiTomTune";break;
                  case 0x13: ret = "HiTomDecay";break;
                  case 0x14: ret = "RimShotLevel";break;
                  case 0x15: ret = "ClapLevel";break;
                  case 0x16: ret = "HiHatLevel";break;
                  case 0x17: ret = "ClosedHiHatDecay";break;
                  case 0x18: ret = "OpenHiHatDecay";break;
                  case 0x19: ret = "CrashCymbalLevel";break;
                  case 0x1a: ret = "CrashCymbalTune";break;
                  case 0x1b: ret = "RideCymbalLevel";break;
                  case 0x1c: ret = "RideCymbalTune";break;
                  case 0x1d: ret = "FlamInterval";break;
                }
                break;
              case 5://delay
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Steps";break;
                  case 0x02: ret = "Step mode";break;
                  case 0x03: ret = "Feedback amount";break;
                  case 0x04: ret = "Pan";break;
                }
              case 6://dist
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Amount";break;
                  case 0x02: ret = "Shape (0x00 = 1.5 mode)";break;
                }
              case 7://pcf
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Frequency";break;
                  case 0x02: ret = "Resonance";break;
                  case 0x03: ret = "Amount";break;
                  case 0x04: ret = "Wave";break;
                  case 0x05: ret = "Decay";break;
                  case 0x06: ret = "Mode";break;
                }
              case 8://compressor
                switch (ID)
                {
                  case 0x00: ret = "Enabled";break;
                  case 0x01: ret = "Amount";break;
                  case 0x02: ret = "Threshold";break;
                }
                break;
            }
            return ret;
          }
          
          printf( " ID=%" PRIu8 " %s", pad, traktype( count, pad));
          fread( &pad, 1, 1, in);
          printf( " val=%" PRIu8, pad);
          printf( "\n");
				}
				if (chunk_size & 1)
					fread( &pad, 1, 1, in);
				count++;
			}
			else
			{
			buf = malloc( chunk_size);
			fread( buf, chunk_size, 1, in);
			size -= chunk_size;
			int i;
			for (i = 0; i < chunk_size; i++)
			{
				uint8_t b = buf[i];
				printf( " %02" PRIx8, b);
				if (i >= 10)
				{
					printf( "..");
					break;
				}
			}
			printf( "\n");
			free( buf);
			if (chunk_size & 1)
				fread( &pad, 1, 1, in);				// IFF specification mandates padding for odd lengths
			}
		}
	}

bye:
	fclose( in);

	return 0;
}
