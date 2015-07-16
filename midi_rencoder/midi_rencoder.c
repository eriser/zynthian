//-----------------------------------------------------------------------------
// Zynthian Project: C Library for RPi GPIO Switches and Rotary Encoders
//-----------------------------------------------------------------------------
// by José Fernando Moyano Domínguez (2015)
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <alsa/asoundlib.h>

#include "midi_rencoder.h"

snd_seq_t *rencoder_seq_handle=NULL;
int rencoder_seq_portid;

//-----------------------------------------------------------------------------
// Library Initialization
//-----------------------------------------------------------------------------

int init_rencoder()
{
    int i;
    for (i=0;i<max_gpio_switches;i++) gpio_switches[i].enabled=0;
    for (i=0;i<max_midi_rencoders;i++) midi_rencoders[i].enabled=0;
    wiringPiSetup();
    return init_seq_midi_rencoder();
}

int init_seq_midi_rencoder()
{
    if (snd_seq_open(&rencoder_seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        fprintf(stderr, "Error creating ALSA client.\n");
        return -1;
    }
    snd_seq_set_client_name(rencoder_seq_handle, "Zynthian_rencoder" );

    if (( rencoder_seq_portid = snd_seq_create_simple_port(rencoder_seq_handle, "Output port",
        SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
        //SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
        fprintf(stderr, "Error creating output port.\n" );
        return -2;
    }
    return rencoder_seq_portid;
}

//-----------------------------------------------------------------------------
// GPIO Switches
//-----------------------------------------------------------------------------

void update_gpio_switch(unsigned int i)
{
    if (i>=max_gpio_switches) return;
    struct gpio_switch *gpio_switch = gpio_switches + i;
    if (gpio_switch->enabled==0) return;

    gpio_switch->status= digitalRead(gpio_switch->pin);
}

void update_gpio_switch_0() { update_gpio_switch(0); }
void update_gpio_switch_1() { update_gpio_switch(1); }
void update_gpio_switch_2() { update_gpio_switch(2); }
void update_gpio_switch_3() { update_gpio_switch(3); }
void update_gpio_switch_4() { update_gpio_switch(4); }
void update_gpio_switch_5() { update_gpio_switch(5); }
void update_gpio_switch_6() { update_gpio_switch(6); }
void update_gpio_switch_7() { update_gpio_switch(7); }
void (*update_gpio_switch_funcs[8])={
    update_gpio_switch_0,
    update_gpio_switch_1,
    update_gpio_switch_2,
    update_gpio_switch_3,
    update_gpio_switch_4,
    update_gpio_switch_5,
    update_gpio_switch_6,
    update_gpio_switch_7
};

//-----------------------------------------------------------------------------

struct gpio_switch *setup_gpio_switch(unsigned int i, unsigned int pin)
{
    if (i >= max_gpio_switches)
    {
        printf("Maximum number of gpio switches exceded: %i\n", max_gpio_switches);
        return NULL;
    }
    
    struct gpio_switch *gpio_switch = gpio_switches + i;
    gpio_switch->enabled = 1;
    gpio_switch->pin = pin;
    gpio_switch->status = 0;

    pinMode(pin, INPUT);
    pullUpDnControl(pin, PUD_UP);
    wiringPiISR(pin,INT_EDGE_BOTH, update_gpio_switch_funcs[i]);

    return gpio_switch;
}

unsigned int get_gpio_switch(unsigned int i) {
    if (i >= max_gpio_switches) return 0;
    unsigned int status=gpio_switches[i].status;
    gpio_switches[i].status=0;
    return status;
}

//-----------------------------------------------------------------------------
// Generic Rotary Encoders
//-----------------------------------------------------------------------------

void send_seq_midi_rencoder(unsigned int i)
{
    if (i>=max_midi_rencoders || rencoder_seq_handle==NULL) return;
    struct midi_rencoder *midi_rencoder = midi_rencoders + i;
    if (midi_rencoder->enabled==0 || midi_rencoder->midi_ctrl==0) return;

    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_direct(&ev);
    //snd_seq_ev_set_dest(&ev, 64, 0); /* send to 64:0 */
    snd_seq_ev_set_subs(&ev);        /* send to subscribers of source port */
    snd_seq_ev_set_controller(&ev, 0, midi_rencoder->midi_ctrl, midi_rencoder->value);
    snd_seq_event_output_direct( rencoder_seq_handle, &ev );
    //snd_seq_drain_output( rencoder_seq_handle);
    //printf("SEND MIDI CTRL: %d => %d\n",midi_rencoder->midi_ctrl,midi_rencoder->value);
}

void update_midi_rencoder(unsigned int i)
{
    if (i>=max_midi_rencoders) return;
    struct midi_rencoder *midi_rencoder = midi_rencoders + i;
    if (midi_rencoder->enabled==0) return;

    unsigned int MSB = digitalRead(midi_rencoder->pin_a);
    unsigned int LSB = digitalRead(midi_rencoder->pin_b);
    unsigned int encoded = (MSB << 1) | LSB;
    unsigned int sum = (midi_rencoder->last_encoded << 2) | encoded;
    unsigned int last_value=midi_rencoder->value;

    if (midi_rencoder->value>midi_rencoder->max_value) midi_rencoder->value=midi_rencoder->max_value;

    if (midi_rencoder->value<midi_rencoder->max_value && (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)) {
        midi_rencoder->value++;
    }
    else if (midi_rencoder->value>0 && (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)) {
        midi_rencoder->value--;
    }
    midi_rencoder->last_encoded = encoded;

    if (last_value!=midi_rencoder->value) send_seq_midi_rencoder(i);
}

void update_midi_rencoder_0() { update_midi_rencoder(0); }
void update_midi_rencoder_1() { update_midi_rencoder(1); }
void update_midi_rencoder_2() { update_midi_rencoder(2); }
void update_midi_rencoder_3() { update_midi_rencoder(3); }
void update_midi_rencoder_4() { update_midi_rencoder(4); }
void update_midi_rencoder_5() { update_midi_rencoder(5); }
void update_midi_rencoder_6() { update_midi_rencoder(6); }
void update_midi_rencoder_7() { update_midi_rencoder(7); }
void (*update_midi_rencoder_funcs[8])={
    update_midi_rencoder_0,
    update_midi_rencoder_1,
    update_midi_rencoder_2,
    update_midi_rencoder_3,
    update_midi_rencoder_4,
    update_midi_rencoder_5,
    update_midi_rencoder_6,
    update_midi_rencoder_7
};

//-----------------------------------------------------------------------------

struct midi_rencoder *setup_midi_rencoder(unsigned int i, unsigned int pin_a, unsigned int pin_b, unsigned int midi_ctrl, unsigned int value, unsigned int max_value)
{
    if (i > max_midi_rencoders)
    {
        printf("Maximum number of midi rotary encoders exceded: %i\n", max_midi_rencoders);
        return NULL;
    }
    

    struct midi_rencoder *rencoder = midi_rencoders + i;
    if (value>max_value) value=max_value;
    rencoder->midi_ctrl = midi_ctrl;
    rencoder->max_value = max_value;
    rencoder->value = value;

    if (rencoder->enabled==0 || rencoder->pin_a!=pin_a || rencoder->pin_b!=pin_b) {
        rencoder->enabled = 1;
        rencoder->pin_a = pin_a;
        rencoder->pin_b = pin_b;
        rencoder->last_encoded = 0;

        pinMode(pin_a, INPUT);
        pinMode(pin_b, INPUT);
        pullUpDnControl(pin_a, PUD_UP);
        pullUpDnControl(pin_b, PUD_UP);
        wiringPiISR(pin_a,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);
        wiringPiISR(pin_b,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);
    }

    return rencoder;
}

unsigned int get_value_midi_rencoder(unsigned int i) {
    if (i >= max_midi_rencoders) return 0;
    return midi_rencoders[i].value;
}

void set_value_midi_rencoder(unsigned int i, unsigned int v) {
    if (i >= max_midi_rencoders) return;
    if (v>127) v=127;
    midi_rencoders[i].value=v;
}

//-----------------------------------------------------------------------------
// Zynthian Rotary Encoders
//-----------------------------------------------------------------------------

struct midi_rencoder *setup_zyncoder(unsigned int i, unsigned int midi_ctrl, unsigned int value, unsigned int max_value)
{
    if (i >= max_zyncoders)
    {
        printf("Maximum number of Zynthian midi rotary encoders exceded: %i\n", max_zyncoders);
        return NULL;
    }

    struct midi_rencoder *zyncoder = midi_rencoders + i;
    if (value>max_value) value=max_value;
    zyncoder->midi_ctrl = midi_ctrl;
    zyncoder->max_value = max_value;
    zyncoder->value = value;

    // Pin Assignment
    static unsigned int zyncoder_pin_a[max_zyncoders]={26,25,21,7,3};
    static unsigned int zyncoder_pin_b[max_zyncoders]={23,27,2,0,4};

    if (zyncoder->enabled==0 || zyncoder->pin_a!=zyncoder_pin_a[i] || zyncoder->pin_b!=zyncoder_pin_b[i]) {
        zyncoder->enabled = 1;
        zyncoder->pin_a = zyncoder_pin_a[i];
        zyncoder->pin_b = zyncoder_pin_b[i];
        zyncoder->last_encoded = 0;

        pinMode(zyncoder->pin_a, INPUT);
        pinMode(zyncoder->pin_b, INPUT);
        pullUpDnControl(zyncoder->pin_a, PUD_UP);
        pullUpDnControl(zyncoder->pin_b, PUD_UP);
        wiringPiISR(zyncoder->pin_a,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);
        wiringPiISR(zyncoder->pin_b,INT_EDGE_BOTH, update_midi_rencoder_funcs[i]);
    }

    return zyncoder;
}

unsigned int get_value_zyncoder(unsigned int i) {
    if (i >= max_zyncoders) return 0;
    return midi_rencoders[i].value;
}

void set_value_zyncoder(unsigned int i, unsigned int v) {
    if (i >= max_zyncoders) return;
    if (v>127) v=127;
    midi_rencoders[i].value=v;
}
