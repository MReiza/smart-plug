#include "global.h"
#include "app_driver.h"
#include "esp_http_server.h"

static const char *TAG = "HTTP_SERVER";

static esp_err_t relay_handler(httpd_req_t *req)
{
#define STR "<html><head><title>Wi-Fi Outlet</title><style>body{background-color:%s;text-align:center;color:white;font-family:Arial,Helvetica,sans-serif;}</style></head><body><h1>Hello There!</h1></body></html>"
    char outbuf[300];
    bool relay_state = relay_get_state();
    relay_set_state(!relay_state);
    snprintf(outbuf, sizeof(outbuf), STR, relay_get_state() ? "#52ff52" : "#ff5252");
    httpd_resp_send(req, outbuf, strlen(outbuf));
    return ESP_OK;
#undef STR
}

static const httpd_uri_t relay = {
    .uri = "/relay",
    .method = HTTP_GET,
    .handler = relay_handler,
};

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &relay);
        return server;
    }
    ESP_LOGE(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

static void disconnect_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server)
    {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server == NULL)
    {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

void webserver_init(void)
{
    static httpd_handle_t server = NULL;
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
    server = start_webserver();
}