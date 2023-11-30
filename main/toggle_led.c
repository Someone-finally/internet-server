#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"

#define LED 21    //has to define which pin 

void init_led(void)
{

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED,GPIO_MODE_OUTPUT);
}


void toggle_bool(bool is_on)
{
  gpio_set_level(LED,is_on);
}