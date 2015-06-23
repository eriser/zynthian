#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "midi_rencoder.h"

int main()
{
  printf("HOLA ZYNCODERS!");

  init_rencoder();

  setup_zyncoder(0,1,90);
  setup_zyncoder(1,10,90);
  setup_zyncoder(2,71,90);
  setup_zyncoder(3,74,90);

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
    printf("ZC0 = %d\n", get_value_zyncoder(0));
    printf("ZC1 = %d\n", get_value_zyncoder(1));
    printf("ZC2 = %d\n", get_value_zyncoder(2));
    printf("ZC3 = %d\n", get_value_zyncoder(3));
    usleep(500000);
  }

  return 0;
}

