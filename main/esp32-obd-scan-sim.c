#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <hal/gpio_types.h>
#include <nvs.h>
#include <nvs_flash.h>

#define BT_TAG "BLUETOOTH_INIT"

const gpio_num_t INTERNAL_LED = 2;

void initialize_bluetooth(void);
void initialize_gpio(void);

void app_main(void)
{
  initialize_bluetooth();

  initialize_gpio();
}

void initialize_bluetooth(void)
{
  esp_err_t ret = nvs_flash_init();

  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  ret = esp_bt_controller_init(&bt_cfg);
  if (ret != ESP_OK)
  {
    ESP_LOGE(BT_TAG, "Bluetooth controller initialization failed, error code %s", esp_err_to_name(ret));
    return;
  }

  ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
  if (ret != ESP_OK)
  {
    ESP_LOGE(BT_TAG, "Bluetooth controller enable failed, error code %s", esp_err_to_name(ret));
    return;
  }
}

void initialize_gpio(void)
{
  gpio_reset_pin(INTERNAL_LED);
  gpio_set_direction(INTERNAL_LED, GPIO_MODE_OUTPUT);

  gpio_set_level(INTERNAL_LED, 0);
}
