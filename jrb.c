#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include <jack/jack.h>
#include <jack/ringbuffer.h>

//#define USEJRB

int rbsize;	// max capacity
#ifdef USEJRB
jack_ringbuffer_t *rb;
#else
int rdspace;	// available to read
int wrspace;	// available to write
#endif

typedef struct {
#if 1
    int val;
//    struct timeval t;
    uint64_t t;
#else
    int a, b, c, d;
#endif
} elem_t;

static uint64_t g_us0;
void initt() {
    struct timeval g_tv0;
    gettimeofday(&g_tv0, 0);
    g_us0 = g_tv0.tv_sec * 1000000 + g_tv0.tv_usec;
//    printf("%s: g_us0=%" PRIu64 "\n", __func__, g_us0);
}
void sleept_us(uint64_t us) {
    struct timespec req;
    req.tv_sec = us / 1000000;
    req.tv_nsec = (us - req.tv_sec * 1000000) * 1000;
//    printf("%s: sleeping for %" PRIu64 " sec and %" PRIu64 " nsec\n", __func__, (uint64_t)req.tv_sec, (uint64_t)req.tv_nsec);
    printf("%s: sleeping for %" PRIu64 " usec\n", __func__, us);
    nanosleep(&req, 0);
}

uint64_t gett_us() {
    struct timeval tv;
    gettimeofday(&tv, 0);
//    printf("%s: sec=%" PRIu64 " usec=%" PRIu64 "\n", __func__, (uint64_t)tv.tv_sec, (uint64_t)tv.tv_usec);
    uint64_t us = tv.tv_sec * 1000000 + tv.tv_usec;
//    printf("%s: g_us0=%" PRIu64 " us=%" PRIu64 "\n", __func__, g_us0, us);
    us -= g_us0;
    printf("%s: us=%" PRIu64 "\n", __func__, us);
    return us;
}

int initrb(int n) {
    rbsize = n;
#ifdef USEJRB
    rb = jack_ringbuffer_create(rbsize * sizeof(elem_t));
    printf("rb=%p\n", rb);
    printf("%s: rdspace=%lu wrspace=%lu\n", __func__, jack_ringbuffer_read_space(rb), jack_ringbuffer_write_space(rb));
#else
    rdspace = 0;
    wrspace = rbsize - 1;
#endif
    return 0;
}

int jack_samplerate_cb(jack_nframes_t nframes, void *arg) {
    double sr = nframes;
    printf("%s: samplerate=%f\n", __func__, sr);
    return 0;
}

static int process_callback(jack_nframes_t nframes, void *arg) {
#if 0
    static uint64_t us0 = 0;
    uint64_t us = gett_us();
    printf("%s: dus=%" PRIu64 "\n", __func__, us - us0);
    us0 = us;
    printf("%s: nframes=%d\n", __func__, (int)nframes);
#endif
    return 0;
}

int initj() {
    jack_status_t status;
    char *client_name = "jrb";
    jack_client_t *jc = jack_client_open (client_name, JackNullOption, &status);
    double sr = jack_get_sample_rate(jc);
    printf("%s: samplerate=%f\n", __func__, sr);
    jack_set_sample_rate_callback(jc, jack_samplerate_cb, 0);
    jack_set_process_callback(jc, process_callback, 0);
    if (jack_activate(jc)) {
        printf("cannot activate jack client\n");
        exit(1);;
    }
    return 0;
}

int produce(int n) {
    int res;
#ifdef USEJRB
    int space = res = jack_ringbuffer_write_space(rb) / sizeof(elem_t);
    if (res > n) res = n;
    for (int i = 0; i < res; i++) {
        elem_t e;
        jack_ringbuffer_write(rb, (const char *)&e, sizeof(elem_t));
    }
    printf("%s: n=%d wrspace=%d return %d\n", __func__, n, space, res);
#else
    res = n;
    if (res > wrspace - 1) res = wrspace - 1;
    printf("%s: n=%d wrspace=%d return %d\n", __func__, n, wrspace, res);
    wrspace -= res;
    rdspace += res;
#endif
    return res;
}

int consume(int n) {
    int res;
#ifdef USEJRB
    int space = res = jack_ringbuffer_read_space(rb) / sizeof(elem_t);
    if (res > n) res = n;
    for (int i = 0; i < res; i++) {
        elem_t e;
        jack_ringbuffer_read(rb, (char *)&e, sizeof(elem_t));
    }
    printf("%s: n=%d rdspace=%d return %d\n", __func__, n, space, res);
#else
    res = n;
    if (res > rdspace) res = rdspace;
    printf("%s: n=%d rdspace=%d return %d\n", __func__, n, rdspace, res);
    rdspace -= res;
    wrspace += res;
#endif
    return res;
}

void cleanup() {
#ifdef USEJRB
    if (rb) {
        jack_ringbuffer_free(rb);
    }
#endif
}

int main() {
    initt();
    gett_us();
    initj();
    initrb(1024);
//    int count = 0;
    printf("sleeping 1s\n");
    sleep(1);
    gett_us();
    sleept_us(1500000);
    gett_us();

    produce(3);
    consume(1);
    produce(2);
    consume(3);
    produce(1);
    consume(5);

//    while (1);

    cleanup();
    return 0;
}
