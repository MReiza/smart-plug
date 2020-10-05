#include "global.h"
#include "esp_smartconfig.h"
#include "app_driver.h"

#define CONNECTED_BIT BIT0
#define ESPTOUCH_DONE_BIT BIT1

static EventGroupHandle_t wifi_event_group;

static const char *TAG = "WIFI";

static int retry_num = 0;
static bool provisioned;

static void smartconfig_task(void *arg);

esp_err_t esp_wifi_is_provisioned(bool *provisioned)
{
    *provisioned = false;
    wifi_config_t wifi_config;
    if (esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config) != ESP_OK)
        return ESP_FAIL;

    if (strlen((const char *)wifi_config.sta.ssid))
    {
        *provisioned = true;
        ESP_LOGI(TAG, "Found ssid: %s", (const char *)wifi_config.sta.ssid);
        ESP_LOGI(TAG, "Found password: %s", (const char *)wifi_config.sta.password);
    }
    return ESP_OK;
}

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        if (provisioned)
        {
            esp_wifi_connect();
            led_medium_blink();
        }
        else
        {
            xTaskCreate(smartconfig_task, "smartconfig_task", 4096, NULL, 2, NULL);
            led_slow_blink();
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (retry_num < 10)
        {
            ESP_LOGI(TAG, "Disconnected, reconnecting to AP...");
            esp_wifi_connect();
            led_medium_blink();
            retry_num++;
        }
        else
        {
            ESP_LOGE(TAG, "Connecting failed, restarting...");
            esp_restart();
        }
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "IP: " IPSTR, IP2STR(&event->ip_info.ip));
        led_quick_blink();
        retry_num = 0;
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = {0};
        uint8_t password[65] = {0};

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true)
        {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "Found ssid: %s", ssid);
        ESP_LOGI(TAG, "Found password: %s", password);

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_connect());
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}

static void smartconfig_task(void *arg)
{
    EventBits_t bits;
    ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
    while (1)
    {
        bits = xEventGroupWaitBits(wifi_event_group, ESPTOUCH_DONE_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

        if (bits & ESPTOUCH_DONE_BIT)
        {
            ESP_LOGI(TAG, "Smartconfing done");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}

void wifi_init(void)
{
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_is_provisioned(&provisioned));
    ESP_ERROR_CHECK(esp_wifi_start());
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    google_iot_init();
}