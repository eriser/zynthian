//-----------------------------------------------------------------------------
// Zynthian Project: C Library for RPi GPIO Switches and Rotary Encoders
//-----------------------------------------------------------------------------
// by José Fernando Moyano Domínguez (2015)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Library Initialization
//-----------------------------------------------------------------------------

int init_rencoder();
int init_seq_midi_rencoder();

//-----------------------------------------------------------------------------
// GPIO Switches
//-----------------------------------------------------------------------------

// The real limit in RPi2 is 17
#define max_gpio_switches 8

struct gpio_switch
{
	unsigned int enabled;
	unsigned int pin;
	volatile double tsus;
	volatile unsigned int dtus;
	volatile unsigned int status;
};

//Pre-allocate switch objects on the stack so we don't have to 
//worry about freeing them
struct gpio_switch gpio_switches[max_gpio_switches];

void update_gpio_switch(unsigned int i);
struct gpio_switch *setup_gpio_switch(unsigned int i, unsigned int pin); 
unsigned int get_gpio_switch(unsigned int i);
unsigned int get_gpio_switch_dtus(unsigned int i);

//-----------------------------------------------------------------------------
// Generic Rotary Encoders
//-----------------------------------------------------------------------------

// 17 pins / 2 pins per encoder = 8 maximum encoders
#define max_midi_rencoders 8

struct midi_rencoder
{
	unsigned int enabled;
	unsigned int pin_a;
	unsigned int pin_b;
	unsigned int midi_chan;
	unsigned int midi_ctrl;
	unsigned int max_value;
	volatile unsigned int value;
	volatile unsigned int last_encoded;
};

//Pre-allocate encoder objects on the stack so we don't have to 
//worry about freeing them
struct midi_rencoder midi_rencoders[max_midi_rencoders];

void send_seq_midi_reconder(unsigned int i);
void update_midi_rencoder(unsigned int i);

/*
  Should be run for every rotary encoder you want to control
  Returns a pointer to the new rotary encoder structer
  The pointer will be NULL is the function failed for any reason
*/
struct midi_rencoder *setup_midi_rencoder(unsigned int i, unsigned int pin_a, unsigned int pin_b, unsigned int midi_chan, unsigned int midi_ctrl, unsigned int value, unsigned int max_value); 
unsigned int get_value_midi_rencoder(unsigned int i);
void set_value_midi_rencoder(unsigned int i, unsigned int v);


//-----------------------------------------------------------------------------
// Zynthian Rotary Encoders
//-----------------------------------------------------------------------------

// Zynthian uses some pins for HiFiBerry and PiTFT => left only 5
#define max_zyncoders 5

struct midi_rencoder *setup_zyncoder(unsigned int i, unsigned int midi_chan, unsigned int midi_ctrl, unsigned int value, unsigned int max_value);
unsigned int get_value_zyncoder(unsigned int i);
void set_value_zyncoder(unsigned int i, unsigned int v);
