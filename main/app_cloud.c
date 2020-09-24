#include "global.h"
#include "app_cloud.h"
#include "app_driver.h"
#include "app_cmd.h"
#include "app_uart.h"
#include <iotc.h>
#include <iotc_error.h>
#include <iotc_jwt.h>

#define IOTC_UNUSED(x) (void)(x)

#define GIOT_PROJECT_ID "irb100"
#define GIOT_REGION "asia-east1"
#define GIOT_REGISTRY_ID "IRB100"
#define DEVICE_PATH "projects/%s/locations/%s/registries/%s/devices/%s"
#define SUBSCRIBE_TOPIC_COMMAND "/devices/%s/commands/#"
#define SUBSCRIBE_TOPIC_CONFIG "/devices/%s/config"
#define PUBLISH_TOPIC_EVENT "/devices/%s/events"
#define PUBLISH_TOPIC_STATE "/devices/%s/state"
#define STATE_DATA "{\"DeviceID\":\"%s\",\"FwVersion\":\"%s\",\"RelayState\":\"%s\"}"
#define POWER_DATA "{\"content_type\":\"power_data\",\"content_value\":{\"active_power\":\"%s\",\"reactive_power\":\"0.00\",\"apparent_power\":\"0.00\"}}"

char *subscribe_topic_command, *subscribe_topic_config;
char *private_key;
char device_id[12];
char version_name[32];

iotc_mqtt_qos_t iotc_example_qos = IOTC_MQTT_QOS_AT_LEAST_ONCE;
static iotc_timed_task_handle_t telemetry_publish_task = IOTC_INVALID_TIMED_TASK_HANDLE;
iotc_context_handle_t iotc_context = IOTC_INVALID_CONTEXT_HANDLE;

static const char *TAG = "IOTC";

void parse_cmd(char *payload);

esp_err_t get_private_key()
{
    esp_err_t err = nvs_flash_init_partition(FACTORY_PARTITION_LABEL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) initializing factory partition", esp_err_to_name(err));
        return err;
    }

    nvs_handle_t nvs_handle;
    err = nvs_open_from_partition(FACTORY_PARTITION_LABEL, FACTORY_PARTITION_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening factory partition", esp_err_to_name(err));
        return err;
    }
    size_t required_size;
    nvs_get_str(nvs_handle, "iot_cert_pem", NULL, &required_size);
    private_key = malloc(required_size);
    nvs_get_str(nvs_handle, "iot_cert_pem", private_key, &required_size);

    nvs_close(nvs_handle);
    return err;
}

static void get_device_id(char *device_id, size_t n)
{
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    snprintf(device_id, n, "SJIO_%02X%02X%02X", mac[3], mac[4], mac[5]);
    ESP_LOGI(TAG, "Device id: %s", device_id);
}

static void get_firmware_version(char *firmware_version)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK)
    {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
        strcpy(firmware_version, running_app_info.version);
    }
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "time.google.com");
    sntp_init();
}

static void obtain_time(void)
{
    initialize_sntp();
    time_t now = 0;
    struct tm timeinfo = {0};
    char strftime_buf[64];

    while (timeinfo.tm_year < (2016 - 1900))
    {
        ESP_LOGI(TAG, "Waiting for system time to be set...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        setenv("TZ", "WIB-7", 1);
        tzset();
        localtime_r(&now, &timeinfo);
    }

    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "Time is set: %s", strftime_buf);
}

void publish_state_cb(iotc_context_handle_t context_handle, void *data, iotc_state_t state)
{
    ESP_LOGI(TAG, "Response received %d", state);
}

void publish_state(iotc_context_handle_t context_handle, iotc_timed_task_handle_t timed_task, void *user_data)
{
    IOTC_UNUSED(timed_task);
    IOTC_UNUSED(user_data);

    char *publish_topic = NULL;
    char *publish_message = NULL;

    asprintf(&publish_topic, PUBLISH_TOPIC_STATE, device_id);
    asprintf(&publish_message, STATE_DATA, device_id, version_name, relay_get_state() ? "ON" : "OFF");

    ESP_LOGI(TAG, "Publishing device state \"%s\"", publish_message);

    iotc_publish(context_handle, publish_topic, publish_message, iotc_example_qos, publish_state_cb, NULL);

    free(publish_topic);
    free(publish_message);
}

void publish_telemetry(iotc_context_handle_t context_handle, iotc_timed_task_handle_t timed_task, void *user_data)
{
    IOTC_UNUSED(timed_task);
    IOTC_UNUSED(user_data);

    bool is_relay_closed = relay_get_state();
    if (is_relay_closed)
    {
        char *current_power = NULL;
        char *publish_topic = NULL;
        char *publish_message = NULL;

        uart_read_data(PAAVG, 5);

        asprintf(&current_power, "%.2f", power_read);
        asprintf(&publish_topic, PUBLISH_TOPIC_EVENT, device_id);
        asprintf(&publish_message, POWER_DATA, current_power);

        iotc_publish(context_handle, publish_topic, publish_message, iotc_example_qos, NULL, NULL);
        ESP_LOGI(TAG, "Publishing telemetry event \"%s\"", publish_message);

        free(current_power);
        free(publish_topic);
        free(publish_message);
    }
}

void cloud_publish(void)
{
    if (iotc_context == IOTC_INVALID_CONTEXT_HANDLE)
        return;

    publish_telemetry(iotc_context, IOTC_INVALID_TIMED_TASK_HANDLE, NULL);
    publish_state(iotc_context, IOTC_INVALID_TIMED_TASK_HANDLE, NULL);
}

void iotc_mqttlogic_subscribe_callback(
    iotc_context_handle_t in_context_handle, iotc_sub_call_type_t call_type,
    const iotc_sub_call_params_t *const params, iotc_state_t state,
    void *user_data)
{
    IOTC_UNUSED(in_context_handle);
    IOTC_UNUSED(call_type);
    IOTC_UNUSED(state);
    IOTC_UNUSED(user_data);
    if (params != NULL && params->message.topic != NULL)
    {
        // ESP_LOGI(TAG, "Subscription topic: %s", params->message.topic);
        char *sub_message = (char *)malloc(params->message.temporary_payload_data_length + 1);
        if (sub_message == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory");
            return;
        }
        memcpy(sub_message, params->message.temporary_payload_data, params->message.temporary_payload_data_length);
        sub_message[params->message.temporary_payload_data_length] = '\0';
        ESP_LOGI(TAG, "Message payload: %s", sub_message);
        if (params->message.temporary_payload_data_length > 0)
        {
            parse_cmd(sub_message);
        }
        free(sub_message);
    }
}

void on_connection_state_changed(iotc_context_handle_t in_context_handle, void *data, iotc_state_t state)
{
    iotc_connection_data_t *conn_data = (iotc_connection_data_t *)data;

    switch (conn_data->connection_state)
    {
    /* IOTC_CONNECTION_STATE_OPENED means that the connection has been
       established and the IoTC Client is ready to send/recv messages */
    case IOTC_CONNECTION_STATE_OPENED:
        ESP_LOGI(TAG, "Connected to IOTC");

        /* Publish immediately upon connect. 'publish_function' is defined
           in this example file and invokes the IoTC API to publish a
           message. */
        asprintf(&subscribe_topic_command, SUBSCRIBE_TOPIC_COMMAND, device_id);
        ESP_LOGI(TAG, "Subscribe to topic:\"%s\"", subscribe_topic_command);
        iotc_subscribe(in_context_handle, subscribe_topic_command, IOTC_MQTT_QOS_AT_LEAST_ONCE,
                       &iotc_mqttlogic_subscribe_callback, /*user_data=*/NULL);

        // asprintf(&subscribe_topic_config, SUBSCRIBE_TOPIC_CONFIG, device_id);
        // ESP_LOGI(TAG, "Subscribe to topic:\"%s\"", subscribe_topic_config);
        // iotc_subscribe(in_context_handle, subscribe_topic_config, IOTC_MQTT_QOS_AT_LEAST_ONCE,
        //                &iotc_mqttlogic_subscribe_callback, /*user_data=*/NULL);

        /* Create a timed task to publish every 10 seconds. */
        telemetry_publish_task = iotc_schedule_timed_task(in_context_handle, publish_telemetry, 3, 15, /*user_data=*/NULL);

        publish_state(in_context_handle, IOTC_INVALID_TIMED_TASK_HANDLE, NULL);
        publish_telemetry(in_context_handle, IOTC_INVALID_TIMED_TASK_HANDLE, NULL);

        if (relay_get_state())
            led_on();
        else
            led_off();
        break;

    /* IOTC_CONNECTION_STATE_OPEN_FAILED is set when there was a problem
       when establishing a connection to the server. The reason for the error
       is contained in the 'state' variable. Here we log the error state and
       exit out of the application. */

    /* Publish immediately upon connect. 'publish_function' is defined
       in this example file and invokes the IoTC API to publish a
       message. */
    case IOTC_CONNECTION_STATE_OPEN_FAILED:
        ESP_LOGE(TAG, "Failed to establish connection, error %d", state);

        iotc_connect(in_context_handle, conn_data->username, conn_data->password,
                     conn_data->client_id, conn_data->connection_timeout,
                     conn_data->keepalive_timeout, &on_connection_state_changed);
        break;

    /* IOTC_CONNECTION_STATE_CLOSED is set when the IoTC Client has been
       disconnected. The disconnection may have been caused by some external
       issue, or user may have requested a disconnection. In order to
       distinguish between those two situation it is advised to check the state
       variable value. If the state == IOTC_STATE_OK then the application has
       requested a disconnection via 'iotc_shutdown_connection'. If the state !=
       IOTC_STATE_OK then the connection has been closed from one side. */
    case IOTC_CONNECTION_STATE_CLOSED:
        free(subscribe_topic_command);
        free(subscribe_topic_config);
        /* When the connection is closed it's better to cancel some of previously
           registered activities. Using cancel function on handler will remove the
           handler from the timed queue which prevents the registered handle to be
           called when there is no connection. */
        if (IOTC_INVALID_TIMED_TASK_HANDLE != telemetry_publish_task)
        {
            iotc_cancel_timed_task(telemetry_publish_task);
            telemetry_publish_task = IOTC_INVALID_TIMED_TASK_HANDLE;
        }

        if (state == IOTC_STATE_OK)
        {
            /* The connection has been closed intentionally. Therefore, stop
               the event processing loop as there's nothing left to do
               in this example. */
            iotc_events_stop();
        }
        else
        {
            ESP_LOGE(TAG, "Connection closed, error %d", state);
            /* The disconnection was unforeseen.  Try reconnect to the server
            with previously set configuration, which has been provided
            to this callback in the conn_data structure. */

            iotc_crypto_key_data_t iotc_connect_private_key_data;
            iotc_connect_private_key_data.crypto_key_signature_algorithm = IOTC_CRYPTO_KEY_SIGNATURE_ALGORITHM_ES256;
            iotc_connect_private_key_data.crypto_key_union_type = IOTC_CRYPTO_KEY_UNION_TYPE_PEM;
            iotc_connect_private_key_data.crypto_key_union.key_pem.key = private_key;

            char jwt[IOTC_JWT_SIZE] = {0};
            size_t bytes_written = 0;
            state = iotc_create_iotcore_jwt(GIOT_PROJECT_ID,
                                            /*jwt_expiration_period_sec=*/3600,
                                            &iotc_connect_private_key_data, jwt,
                                            IOTC_JWT_SIZE, &bytes_written);
            if (IOTC_STATE_OK != state)
            {
                ESP_LOGE(TAG, "Failed to create JWT, error %d", state);
            }
            else
            {
                iotc_connect(in_context_handle, conn_data->username, jwt,
                             conn_data->client_id, conn_data->connection_timeout,
                             conn_data->keepalive_timeout, &on_connection_state_changed);
            }
        }
        break;

    default:
        printf("wrong value\n");
        break;
    }
}

static void mqtt_task(void *pvParameters)
{
    /* Format the key type descriptors so the client understands
     which type of key is being represented. In this case, a PEM encoded
     byte array of a ES256 key. */
    iotc_crypto_key_data_t iotc_connect_private_key_data;
    iotc_connect_private_key_data.crypto_key_signature_algorithm = IOTC_CRYPTO_KEY_SIGNATURE_ALGORITHM_ES256;
    iotc_connect_private_key_data.crypto_key_union_type = IOTC_CRYPTO_KEY_UNION_TYPE_PEM;
    iotc_connect_private_key_data.crypto_key_union.key_pem.key = private_key;

    /* initialize iotc library and create a context to use to connect to the
    * GCP IoT Core Service. */
    const iotc_state_t error_init = iotc_initialize();

    if (IOTC_STATE_OK != error_init)
    {
        printf(" iotc failed to initialize, error: %d\n", error_init);
        vTaskDelete(NULL);
    }

    /*  Create a connection context. A context represents a Connection
        on a single socket, and can be used to publish and subscribe
        to numerous topics. */
    iotc_context = iotc_create_context();
    if (IOTC_INVALID_CONTEXT_HANDLE >= iotc_context)
    {
        printf(" iotc failed to create context, error: %d\n", -iotc_context);
        vTaskDelete(NULL);
    }

    /*  Queue a connection request to be completed asynchronously.
        The 'on_connection_state_changed' parameter is the name of the
        callback function after the connection request completes, and its
        implementation should handle both successful connections and
        unsuccessful connections as well as disconnections. */
    const uint16_t connection_timeout = 0;
    const uint16_t keepalive_timeout = 60;

    /* Generate the client authentication JWT, which will serve as the MQTT
     * password. */
    char jwt[IOTC_JWT_SIZE] = {0};
    size_t bytes_written = 0;
    iotc_state_t state = iotc_create_iotcore_jwt(GIOT_PROJECT_ID,
                                                 /*jwt_expiration_period_sec=*/3600,
                                                 &iotc_connect_private_key_data, jwt,
                                                 IOTC_JWT_SIZE, &bytes_written);

    if (IOTC_STATE_OK != state)
    {
        printf("iotc_create_iotcore_jwt returned with error: %ul", state);
        vTaskDelete(NULL);
    }

    char *device_path = NULL;
    asprintf(&device_path, DEVICE_PATH, GIOT_PROJECT_ID, GIOT_REGION, GIOT_REGISTRY_ID, device_id);
    iotc_connect(iotc_context, NULL, jwt, device_path, connection_timeout,
                 keepalive_timeout, &on_connection_state_changed);
    free(device_path);
    /* The IoTC Client was designed to be able to run on single threaded devices.
        As such it does not have its own event loop thread. Instead you must
        regularly call the function iotc_events_process_blocking() to process
        connection requests, and for the client to regularly check the sockets for
        incoming data. This implementation has the loop operate endlessly. The loop
        will stop after closing the connection, using iotc_shutdown_connection() as
        defined in on_connection_state_change logic, and exit the event handler
        handler by calling iotc_events_stop(); */
    iotc_events_process_blocking();

    iotc_delete_context(iotc_context);

    iotc_shutdown();

    vTaskDelete(NULL);
}

void parse_cmd(char *payload)
{
    cJSON *command_type = NULL;
    cJSON *request_value = NULL;
    cJSON *message = cJSON_Parse(payload);
    command_type = cJSON_GetObjectItem(message, "command_type");
    request_value = cJSON_GetObjectItem(message, "request_value");

    if (strcmp(command_type->valuestring, "set_relay") == 0)
        cmd_relay();
    else if (strcmp(command_type->valuestring, "set_timer") == 0)
        cmd_scene(request_value);
    else if (strcmp(command_type->valuestring, "set_countdown") == 0)
        cmd_timer(request_value);
    else if (strcmp(command_type->valuestring, "ir_transmit") == 0)
        cmd_rmt(request_value);
    else if (strcmp(command_type->valuestring, "get_ota") == 0)
        cmd_ota();

    cJSON_Delete(message);
}

void google_iot_init(void)
{
    esp_err_t err = get_private_key();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get private key, restarting...");
        esp_restart();
    }
    get_firmware_version(version_name);
    get_device_id(device_id, sizeof(device_id));
    obtain_time();
    xTaskCreate(&mqtt_task, "mqtt_task", 8192, NULL, 5, NULL);
    cmd_scene_init();
    cmd_timer_init();
}