#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <malloc.h>
#include <stdlib.h>

#include "rbs.h"
#include "x0x.h"

uint32_t ntohl( uint32_t val)
{
    uint32_t result;

    result = ((val & 0x000000ff) << 24) +
        ((val & 0x0000ff00) << 8) +
        ((val & 0x00ff0000) >> 8) +
        ((val & 0xff000000) >> 24);
    return result;
}

int main( int argc, char *argv[]) {
#ifdef WIN32
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
#endif
	char *rname = 0;
	char *xname = 0;
	int arg = 1;
	if (arg < argc)
	{
		rname = argv[arg++];
		if (arg < argc)
		{
			xname = argv[arg++];
		}
	}
	
	if (!rname)
	{
		printf( "usage: mkx0x <file.rbs> [file.x0x]\n");
		exit( 1);
	}

	FILE *in = fopen( rname, "rb");
	if (!in)
	{
		perror( "fopen");
		exit( 1);
	}

	x0x_t *x0x = malloc( sizeof( x0x_t));
	memset( x0x, 0, sizeof( *x0x));

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
    
    while (!feof( in) && (size > 0))
    {
    	uint32_t chunk_size;
    	chunk_t chunk;
        fread( &chunk, sizeof( chunk), 1, in);
        chunk_size = ntohl( chunk.chunk_data_size);
        if (!strncmp( chunk.chunk_id, "CAT ", 4))
        {
            char str[5];
            memset( str, 0, 5);
            fread( str, 4, 1, in);
            printf( "Subchunk ID %4s\n", chunk.chunk_id);
            size -= sizeof( chunk);
        }
        else
        {
        	printf( "chunk ID %4s, size=%" PRIu32 "\n", chunk.chunk_id, chunk_size);
            if (!strncmp( chunk.chunk_id, "GLOB", 4))
            {
            	glob_t glob;
                fread( (char *)&glob + sizeof( chunk), sizeof( glob) - sizeof( chunk), 1, in);
				static int count = 0;
				if (count++ == 0)
				{
	                printf( "tempo=%d\n", glob.tempo);
					x0x->tempo = glob.tempo;
				}
            }
            else if (!strncmp( chunk.chunk_id, "USRI", 4))
            {
            	usri_t usri;
                fread( (char *)&usri + sizeof( chunk), sizeof( usri) - sizeof( chunk), 1, in);
				static int count = 0;
				if (count++ == 0)
				{
	                printf( "song_information=%s\n", usri.song_information);
					strncpy( x0x->song_info, usri.song_information, sizeof( x0x->song_info));
				}
            }
            else if (!strncmp( chunk.chunk_id, "MIXR", 4))
            {
                mixr_t mixr;
                fread( (char *)&mixr + sizeof( chunk), sizeof( mixr) - sizeof( chunk), 1, in);
				static int count = 0;
				if (count++ == 0)
				{
	                printf( "master_fader_level=%d\n", mixr.master_fader_level);
					x0x->volume = mixr.master_fader_level;
	            }
	        }
            else if (!strncmp( chunk.chunk_id, "303 ", 4))
            {
                tb303_t tb303;
                fread( (char *)&tb303 + sizeof( chunk), sizeof( tb303) - sizeof( chunk), 1, in);
				static int count = 0;
				if (count++ == 0)
				{
	                printf( "enabled=%d\n", tb303.tb303_enabled);
	                printf( "pattern=%d\n", tb303.tb303_pattern);
	                printf( "tune=%d\n", tb303.tb303_tune);
	                printf( "cutoff=%d\n", tb303.tb303_cutoff);
	                printf( "reso=%d\n", tb303.tb303_reso);
	                printf( "env_mod=%d\n", tb303.tb303_env_mod);
	                printf( "decay=%d\n", tb303.tb303_decay);
	                printf( "accent=%d\n", tb303.tb303_accent);
	                printf( "waveform=%d\n", tb303.tb303_wave);
	                printf( "shuffle=%d\n", tb303.tb303_shuffle);
	                printf( "steps=%d\n", tb303.tb303_steps);
					x0x->tune = tb303.tb303_tune;
					x0x->cutoff = tb303.tb303_cutoff;
					x0x->reso = tb303.tb303_reso;
					x0x->envmod = tb303.tb303_env_mod;
					x0x->accent = tb303.tb303_accent;
					x0x->wave_form = tb303.tb303_wave;
					x0x->nsteps = tb303.tb303_steps;
#if 1
	int npat = 8;
	int nbank = 1;
	int nbars = 1;
	x0x->npat = npat;
	x0x->nbank = nbank;
	x0x->nbars = nbars;
	int i;
	int b = 0;
	int p = 0;
	for (b = 0; b < nbank; b++)
	for (p = 0; p < npat; p++)
	{
//		x0x->steps[b][p][i][0] = tb303.reserved[p * 34 + 0];//shuffle
//		x0x->steps[b][p][i][0] = tb303.reserved[p * 34 + 1];//len
		for (i = 0; i < x0x->nsteps; i++)
		{
			x0x->steps[b][p][i][0] = tb303.reserved[p * 34 + 2 + i * 2 + 0];				//note
			x0x->steps[b][p][i][1] = (tb303.reserved[p * 34 + 2 + i * 2 + 1] & 0xc) >> 2;	//octave;
			x0x->steps[b][p][i][2] = !!(tb303.reserved[p * 34 + 2 + i * 2 + 1] & 0x10);		//play_not_silence;
			x0x->steps[b][p][i][3] = !!(tb303.reserved[p * 34 + 2 + i * 2 + 1] & 0x2);		//accent;
			x0x->steps[b][p][i][4] = !!(tb303.reserved[p * 34 + 2 + i * 2 + 1] & 0x1);		//slide;
			printf( "%d: n=%d o=%d pns=%d a=%d s=%d\n", i
				,x0x->steps[b][p][i][0]
				,x0x->steps[b][p][i][1]
				,x0x->steps[b][p][i][2]
				,x0x->steps[b][p][i][3]
				,x0x->steps[b][p][i][4]
			);
		}
	}
					x0x->nbars = 0;
					for (i = 0; i < x0x->nbars; i++)
					{
						x0x->song[i] = 0;
					}
#endif
				}
            }
#if 0
            else if (!strncmp( chunk.chunk_id, "TRAK", 4))
            {
                trak_t trak;
                fread( (char *)&trak + sizeof( chunk), sizeof( trak) - sizeof( chunk), 1, in);
				static int count = 0;
				if (count++ == 0)
				{
	            }
	        }
#endif
            else
            {
            	uint8_t *buf;
            	if (chunk_size & 1)
    	            chunk_size++;               // IFF specification mandates padding for odd lengths
	            buf = malloc( chunk_size);
	            fread( buf, chunk_size, 1, in);
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
			}
			size -= chunk_size;
		}
    }

	if (xname)
		x0x_save( x0x, xname);
	free( x0x);

bye:
	fclose( in);

	return 0;
}

