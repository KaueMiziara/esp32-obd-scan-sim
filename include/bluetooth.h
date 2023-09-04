#pragma once

#include <esp_spp_api.h>

void initialize_bluetooth(void);

void spp_server_task(void* param);
void spp_server_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t* param);
