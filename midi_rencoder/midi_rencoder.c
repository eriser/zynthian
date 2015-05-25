#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <alsa/asoundlib.h>
#include "wiringPi.h"

#include "midi_rencoder.h"

snd_seq_t *rencoder_seq_handle=NULL;
int rencoder_seq_portid;

unsigned int n_midi_rencoders = 0;

int init_seq_midi_rencoder()
{
    if (snd_seq_open(&rencoder_seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        fprintf(stderr, "Error creating ALSA client.\n");
        return 0;
    }
    snd_seq_set_client_name(rencoder_seq_handle, "rencoder" );

    if (( rencoder_seq_portid = snd_seq_create_simple_port(rencoder_seq_handle, "Output port",
        SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
        SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
        fprintf(stderr, "Error creating output port.\n" );
        return 0;
    }
    return rencoder_seq_portid;
}

void send_seq_midi_reconder(unsigned int i)
{
    if (i >= n_midi_rencoders || rencoder_seq_handle==NULL) return;
    struct midi_rencoder *midi_rencoder = midi_rencoders + i;
    snd_seq_event_t ev;
    ev.type=SND_SEQ_EVENT_CONTROLLER;
    ev.flags=0;
    ev.tag=0;
    ev.queue=SND_SEQ_QUEUE_DIRECT;
    ev.time.time.tv_sec=0;
    ev.time.time.tv_nsec=0;
    ev.source.client=0;
    ev.source.port=rencoder_seq_portid;
    ev.dest.client=0;
    ev.dest.port=0;
    ev.data.control.channel=0;
    ev.data.control.unused[0]=0;
    ev.data.control.unused[1]=0;
    ev.data.control.unused[2]=0;
    ev.data.control.param=midi_rencoder->midi_ctrl;
    ev.data.control.value=midi_rencoder->value;
    snd_seq_event_output_direct( rencoder_seq_handle, &ev );
}

void update_midi_rencoder(unsigned int i)
{
    if (i >= n_midi_rencoders) return;

    struct midi_rencoder *midi_rencoder = midi_rencoders + i;
    unsigned int MSB = digitalRead(midi_rencoder->pin_a);
    unsigned int LSB = digitalRead(midi_rencoder->pin_b);

    unsigned int encoded = (MSB << 1) | LSB;
    unsigned int sum = (midi_rencoder->last_encoded << 2) | encoded;

    if (midi_rencoder->value<127 && (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)) {
        midi_rencoder->value++;
    }
    else if (midi_rencoder->value>0 && (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)) {
        midi_rencoder->value--;
    }
    midi_rencoder->last_encoded = encoded;

    send_seq_midi_reconder(i);
}

void update_midi_rencoder_1() { update_midi_rencoder(1); }
void update_midi_rencoder_2() { update_midi_rencoder(2); }
void update_midi_rencoder_3() { update_midi_rencoder(3); }
void update_midi_rencoder_4() { update_midi_rencoder(4); }
void update_midi_rencoder_5() { update_midi_rencoder(5); }
void update_midi_rencoder_6() { update_midi_rencoder(6); }
void update_midi_rencoder_7() { update_midi_rencoder(7); }
void update_midi_rencoder_8() { update_midi_rencoder(8); }
void *update_midi_rencoder_funcs[8]={
    update_midi_rencoder_1,
    update_midi_rencoder_2,
    update_midi_rencoder_3,
    update_midi_rencoder_4,
    update_midi_rencoder_5,
    update_midi_rencoder_6,
    update_midi_rencoder_7,
    update_midi_rencoder_8
};

struct midi_rencoder *setup_midi_rencoder(unsigned int pin_a, unsigned int pin_b, unsigned int midi_ctrl, unsigned int value)
{
    if (n_midi_rencoders > max_midi_rencoders)
    {
        printf("Maximum number of midi rotary encoders exceded: %i\n", max_midi_rencoders);
        return NULL;
    }
    
    if (value>127) value=127;

    struct midi_rencoder *newencoder = midi_rencoders + n_midi_rencoders;
    newencoder->pin_a = pin_a;
    newencoder->pin_b = pin_b;
    newencoder->midi_ctrl = midi_ctrl;
    newencoder->value = value;
    newencoder->last_encoded = 0;

    pinMode(pin_a, INPUT);
    pinMode(pin_b, INPUT);
    pullUpDnControl(pin_a, PUD_UP);
    pullUpDnControl(pin_b, PUD_UP);
    wiringPiISR(pin_a,INT_EDGE_BOTH, update_midi_rencoder_funcs[n_midi_rencoders]);
    wiringPiISR(pin_b,INT_EDGE_BOTH, update_midi_rencoder_funcs[n_midi_rencoders]);

    n_midi_rencoders++;

    return newencoder;
}

unsigned int get_value_midi_rencoder(unsigned int i) {
    if (i >= max_midi_rencoders) return 0;
    return midi_rencoders[i].value;
}

struct midi_rencoder *setup_zyncoder(unsigned int i, unsigned int midi_ctrl, unsigned int value)
{
    if (i >= max_zyncoders)
    {
        printf("Maximum number of Zynthian midi rotary encoders exceded: %i\n", max_zyncoders);
        return NULL;
    }

    if (value>127) value=127;

    struct midi_rencoder *zyncoder = midi_rencoders + i;
    zyncoder->pin_a = zyncoder_pin_a[i];
    zyncoder->pin_b = zyncoder_pin_b[i];
    zyncoder->midi_ctrl = midi_ctrl;
    zyncoder->value = value;
    zyncoder->last_encoded = 0;

    pinMode(zyncoder->pin_a, INPUT);
    pinMode(zyncoder->pin_b, INPUT);
    pullUpDnControl(zyncoder->pin_a, PUD_UP);
    pullUpDnControl(zyncoder->pin_b, PUD_UP);
    wiringPiISR(zyncoder->pin_a,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);
    wiringPiISR(zyncoder->pin_b,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);

    return zyncoder;
}

unsigned int get_value_zyncoder(unsigned int i) {
    if (i >= max_zyncoders) return 0;
    return midi_rencoders[i].value;
}
