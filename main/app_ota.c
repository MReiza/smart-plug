#include "global.h"
#include "app_driver.h"
#include <esp_http_client.h>
#include <esp_https_ota.h>

char *output_buffer;

extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_pem_end");

static const char *TAG = "OTA";

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        // ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        // ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
        // printf("%.*s", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ON_DATA:
        // ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            output_buffer = (char *)malloc(esp_http_client_get_content_length(evt->client));
            strncpy(output_buffer, (char *)evt->data, evt->data_len);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

esp_err_t do_firmware_update(char *ota_url)
{
    esp_http_client_config_t config = {
        .url = ota_url,
        .cert_pem = (char *)server_cert_pem_start,
    };
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "OTA finished, rebooting from new image...");
        esp_restart();
    }
    else
    {
        ESP_LOGE(TAG, "OTA failed: 0x%x", ret);
        return ESP_FAIL;
    }
    return ESP_OK;
}

void check_firmware_update(void)
{
    esp_http_client_config_t config = {
        .url = "https://irb-ota.web.app/ota/wifioutlet/dv/ota.json",
        .event_handler = _http_event_handle,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Status = %d, content_length = %d", esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
        cJSON *otaJson = cJSON_Parse(output_buffer);
        if (otaJson != NULL)
        {
            cJSON *version = cJSON_GetObjectItemCaseSensitive(otaJson, "versionCode");
            cJSON *url = cJSON_GetObjectItemCaseSensitive(otaJson, "otaUrl");

            if (version->valuedouble > FIRMWARE_VERSION)
            {
                ESP_LOGI(TAG, "New version available, updating...");
                led_quick_blink();
                do_firmware_update(url->valuestring);
            }
            else
            {
                ESP_LOGI(TAG, "Firmware is up to date");
            }
        }
        else
        {
            ESP_LOGE(TAG, "Invalid JSON");
        }
    }
    esp_http_client_cleanup(client);
    free(output_buffer);
}
