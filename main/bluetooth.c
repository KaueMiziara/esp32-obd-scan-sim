#include "../include/bluetooth.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_bt.h>
#include <esp_bt_device.h>
#include <esp_bt_main.h>
#include <esp_err.h>
#include <esp_gap_bt_api.h>
#include <esp_log.h>
#include <esp_spp_api.h>
#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

#define BT_TAG "BLUETOOTH_INIT"

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

void spp_server_task(void* param)
{
  while (true)
    vTaskDelay(1000/ portTICK_PERIOD_MS);
}

void spp_server_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t* param)
{
  switch (event)
  {
    case ESP_SPP_INIT_EVT:
      ESP_LOGI(BT_TAG, "SPP server initialized");
      esp_bt_dev_set_device_name("ESP32-SPP");
      esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    break;

    case ESP_SPP_OPEN_EVT:
      ESP_LOGI(BT_TAG, "Client connected");
      // TODO: turn on LED when connected
    break;

    case ESP_SPP_CLOSE_EVT:
      ESP_LOGI(BT_TAG, "Client disconnected");
      // TODO: turn off LED when disconencted
    break;

    case ESP_SPP_DATA_IND_EVT:
      ESP_LOGI(BT_TAG, "Received data");
      // TODO: handle received data
      // PLACEHOLDER (echo):
      esp_spp_write(param->srv_open.handle, param->data_ind.len, param->data_ind.data);
    break;

    case ESP_SPP_CONG_EVT:
      if (param->cong.cong == 0)
      {
        ESP_LOGI(BT_TAG, "SPP flow control released, ready to send more data");
      }
      else
      {
        ESP_LOGI(BT_TAG, "SPP flow control enabled");
      }
    break;
    
    default:
      break;
  }
}
