#include "../include/leds.h"

#include <driver/gpio.h>

static const gpio_num_t INTERNAL_LED = 2;

void initialize_gpio(void)
{
  gpio_reset_pin(INTERNAL_LED);
  gpio_set_direction(INTERNAL_LED, GPIO_MODE_OUTPUT);

  gpio_set_level(INTERNAL_LED, 0);
}
