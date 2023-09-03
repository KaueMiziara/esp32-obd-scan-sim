#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <hal/gpio_types.h>

#include "../include/bluetooth.h"

const gpio_num_t INTERNAL_LED = 2;

void initialize_gpio(void);

void app_main(void)
{
  initialize_bluetooth();

  initialize_gpio();
}

void initialize_gpio(void)
{
  gpio_reset_pin(INTERNAL_LED);
  gpio_set_direction(INTERNAL_LED, GPIO_MODE_OUTPUT);

  gpio_set_level(INTERNAL_LED, 0);
}
