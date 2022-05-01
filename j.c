#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <jack/jack.h>
#include <jack/midiport.h>

typedef struct {
	jack_port_t *input;
	jack_port_t *output;
} j_t;

void process_midi_input(jack_port_t *port, jack_nframes_t nframes) {
	void *port_buffer = jack_port_get_buffer(port, nframes);
	if (!port_buffer) {
		printf("null midi input buffer?\n");
		return;
	}
	int events = jack_midi_get_event_count(port_buffer);
	for (int i = 0; i < events; i++) {
		jack_midi_event_t event;
		if (jack_midi_event_get(&event, port_buffer, i)) {
			printf("failed to get midi input event?\n");
			continue;
		}
		static int count = 0;
		printf("Received midi event #%d with %d bytes\n", count, (int)event.size);
		count++;
	}
}

void process_midi_output(jack_port_t *port, jack_nframes_t nframes) {
	void *port_buffer = jack_port_get_buffer(port, nframes);
	if (!port_buffer) {
		printf("null midi output buffer?\n");
		return;
	}
	jack_midi_clear_buffer(port_buffer);
}

int process_callback(jack_nframes_t nframes, void *arg) {
	j_t *j = (j_t *)arg;
	process_midi_input(j->input, nframes);
	process_midi_output(j->output, nframes);
	return 0;
}

int main() {
	jack_client_t *client = jack_client_open("j", JackNoStartServer, 0);
	if (!client) {
		printf("Can't open Jack client\n");
		exit(1);
	}
	j_t j;
	j.input = jack_port_register(client, "input", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	if (!j.input) {
		printf("Can't register Jack midi input port\n");
		exit(1);
	}
	j.output = jack_port_register(client, "output", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
	if (!j.input) {
		printf("Can't register Jack midi output port\n");
		exit(1);
	}
	if (jack_set_process_callback(client, process_callback, &j)) {
		printf("Can't set Jack process callback\n");
		exit(1);
	}
	if (jack_activate(client)) {
		printf("Can't activate Jack client\n");
		exit(1);
	}
	printf("Running..\n");
	while (1) {
		usleep(1000);
	}
	return 0;
}
