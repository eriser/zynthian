#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "midi_rencoder.h"

int main()
{
  printf("HOLA ZYNCODERS!");

  init_rencoder();

  setup_gpio_switch(0,3);
  setup_gpio_switch(0,4);

  setup_zyncoder(0,1,90,127);
  setup_zyncoder(1,10,90,127);
  setup_zyncoder(2,71,90,127);
  setup_zyncoder(3,74,90,127);

  while(1) {
    /*
    int i;
    for (i=0;i<100;i++) {
      update_midi_rencoder(0);
      update_midi_rencoder(1);
      update_midi_rencoder(2);
      update_midi_rencoder(3);
      usleep(5000);
    }
    */
    printf("SW0 = %d\n", get_gpio_switch(0));
    printf("SW1 = %d\n", get_gpio_switch(1));
    printf("ZC0 = %d\n", get_value_zyncoder(0));
    printf("ZC1 = %d\n", get_value_zyncoder(1));
    printf("ZC2 = %d\n", get_value_zyncoder(2));
    printf("ZC3 = %d\n", get_value_zyncoder(3));
    usleep(500000);
  }

  return 0;
}

