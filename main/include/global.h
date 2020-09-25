#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_ota_ops.h>
#include <esp_event.h>
#include <esp_timer.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <lwip/apps/sntp.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <jsmn.h>
#include <time.h>
#include <cJSON.h>

#define FIRMWARE_VERSION 3

#define FACTORY_PARTITION_LABEL     "fact_cfg"
#define FACTORY_PARTITION_NAMESPACE "irbfactory"

void wifi_init(void);
void check_firmware_update(void);
void google_iot_init(void);