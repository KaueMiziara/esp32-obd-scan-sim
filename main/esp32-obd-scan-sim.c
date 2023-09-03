#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../include/bluetooth.h"
#include "../include/leds.h"

void app_main(void)
{
  initialize_bluetooth();

  initialize_gpio();
}
