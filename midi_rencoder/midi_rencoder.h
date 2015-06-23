// 17 pins / 2 pins per encoder = 8 maximum encoders
#define max_midi_rencoders 8

struct midi_rencoder
{
    unsigned int pin_a;
    unsigned int pin_b;
    unsigned int midi_ctrl;
    volatile unsigned int value;
    volatile unsigned int last_encoded;
};

int init_rencoder();
int init_seq_midi_rencoder();
void send_seq_midi_reconder(unsigned int i);

//Pre-allocate encoder objects on the stack so we don't have to 
//worry about freeing them
struct midi_rencoder midi_rencoders[max_midi_rencoders];

void update_midi_rencoder(unsigned int i);

/*
  Should be run for every rotary encoder you want to control
  Returns a pointer to the new rotary encoder structer
  The pointer will be NULL is the function failed for any reason
*/
struct midi_rencoder *setup_midi_rencoder(unsigned int pin_a, unsigned int pin_b, unsigned int midi_ctrl, unsigned int value); 

/*
  Returns the value for a specific controller
*/
unsigned int get_value_midi_rencoder(unsigned int i);

/*
 Zynthian Encoders
*/

// Zynthian uses some pins for HiFiBerry and PiTFT => left only 5
#define max_zyncoders 5

struct midi_rencoder *setup_zyncoder(unsigned int i, unsigned int midi_ctrl, unsigned int value);

/*
  Returns the value for a specific controller
*/
unsigned int get_value_zyncoder(unsigned int i);
