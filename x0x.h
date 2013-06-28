#include <inttypes.h>

#pragma pack(1)

typedef struct {
	char song_info[65];
	uint8_t volume;
	uint32_t tempo;
	uint8_t pat, tune, cutoff, reso, envmod, decay, accent, wave_form;
#define MAXPAT 32
	uint8_t nsteps[MAXPAT];
// .[0] : note
// .[1] : flags : 4, 3, 2, 1 : pause/note, norm/down, norm/up, accent, slide
	uint8_t steps[MAXPAT][16][2];
	uint8_t nbars;
#define MAXBAR 128
	uint8_t song[MAXBAR];
} x0x_t;

#pragma pack()

int x0x_save( x0x_t *x0x, char *name);
x0x_t *x0x_load( char *name);

