typedef struct {
	char song_info[65];
	int volume, tempo, tune, cutoff, reso, envmod, accent, wave_form;
	int nsteps;
	int npat;
	int nbank;
	int nbars;
	int steps[4][8][16][5];
	int song[128];
} x0x_t;

int x0x_save( x0x_t *x0x, char *name);
x0x_t *x0x_load( char *name);

