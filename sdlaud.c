#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>

typedef struct {
	SDL_AudioSpec *aspec;
	float t;
} actxt_t;

void MyAudioCallback(void *userdata, Uint8 *stream, int len) {
	actxt_t *ctx = userdata;
	int samplef = ctx->aspec->freq;
	float t = ctx->t;
	int nsamples = len / sizeof(float);
	int f = 440;
	float *buf = (float *)stream;
	for (int i = 0; i < nsamples; i++) {
		buf[i] = sin(2 * M_PI * f * (t + i) / samplef);
	}
	t += nsamples;
	ctx->t = t;
}

int main(int argc, char *argv[]) {
	setbuf(stdout, 0);
	int delayms = 3000;
	int samplef = 48000;
	printf("init SDL..\n");
	if (0 != SDL_Init(SDL_INIT_AUDIO)) {
		SDL_Log("Unable to init SDL");
		exit(1);
	}
	atexit(SDL_Quit);
	SDL_AudioSpec want, have;
	actxt_t actx;
	SDL_memset(&actx, 0, sizeof(actx));
	actx.aspec = &have;
	actx.t = 0;
	SDL_memset(&want, 0, sizeof(want));
	want.freq = samplef;
	want.format = AUDIO_F32;
	want.channels = 1;
	want.samples = 4096;
	want.callback = MyAudioCallback;
	want.userdata = &actx;
	SDL_AudioDeviceID adev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
	printf("have freq=%d format=%d channels=%d samples=%d size=%d\n",
		have.freq, (int)have.format, (int)have.channels, (int)have.samples, (int)have.size);
	SDL_PauseAudioDevice(adev, 0);
	printf("delay..\n");
	SDL_Delay(delayms);
	SDL_CloseAudioDevice(adev);
	printf("bye!\n");
	return 0;
}
