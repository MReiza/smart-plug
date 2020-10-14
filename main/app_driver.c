#include "global.h"
#include "app_driver.h"
#include "app_cloud.h"
#include "iot_button.h"
#include "iot_led.h"
#include "iot_relay.h"
#include "ir_remote.h"

#define BUTTON_IO_NUM 26
#define BUTTON_ACTIVE_LEVEL 0
#define RELAY_IO_NUM 4
#define LED_IO_NUM 33

static relay_handle_t relay_handle;
static led_handle_t led_handle;
static bool relay_state;

static void button_tap_cb(void *arg)
{
    relay_state = relay_get_state();
    relay_set_state(!relay_state);
}

static void button_press_5s_cb(void *arg)
{
    nvs_flash_erase();
    esp_restart();
}

void relay_on(void)
{
    iot_relay_state_write(relay_handle, RELAY_STATUS_CLOSE);
}

void relay_off(void)
{
    iot_relay_state_write(relay_handle, RELAY_STATUS_OPEN);
}

void relay_set_state(bool state)
{
    if (state)
    {
        relay_on();
        led_on();
    }
    else
    {
        relay_off();
        switch (app_event)
        {
        case APP_EVENT_SMARTCONFIG:
            led_slow_blink();
            break;
        case APP_EVENT_WIFI_CONNECTING:
        case APP_EVENT_WIFI_DISCONNECTED:
            led_medium_blink();
            break;
        case APP_EVENT_WIFI_CONNECTED:
        case APP_EVENT_IOTC_DISCONNECTED:
            led_quick_blink();
            break;
        default:
            led_off();
            break;
        }
    }

    cloud_publish();
}

bool relay_get_state(void)
{
    return iot_relay_state_read(relay_handle) == RELAY_STATUS_CLOSE ? true : false;
}

void led_off(void)
{
    iot_led_state_write(led_handle, LED_OFF);
}

void led_on(void)
{
    iot_led_state_write(led_handle, LED_ON);
}

void led_quick_blink(void)
{
    relay_state = relay_get_state();
    if (!relay_state)
    {
        iot_led_state_write(led_handle, LED_QUICK_BLINK);
    }
}

void led_slow_blink(void)
{
    relay_state = relay_get_state();
    if (!relay_state)
    {
        iot_led_state_write(led_handle, LED_SLOW_BLINK);
    }
}

void led_medium_blink(void)
{
    relay_state = relay_get_state();
    if (!relay_state)
    {
        iot_led_state_write(led_handle, LED_MEDIUM_BLINK);
    }
}

void led_blink(int num, int delay)
{
    led_status_t led_state = iot_led_state_read(led_handle);
    for (int i = 0; i < num * 2; i++)
    {
        iot_led_state_write(led_handle, i % 2);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
    iot_led_state_write(led_handle, led_state);
}

void driver_init(void)
{
    relay_io_t relay_io = {
        .single_io = {
            .ctl_io_num = RELAY_IO_NUM,
        },
    };

    relay_handle = iot_relay_create(relay_io, RELAY_CLOSE_HIGH, RELAY_GPIO_CONTROL, RELAY_IO_NORMAL);

    led_handle = iot_led_create(LED_IO_NUM, LED_DARK_HIGH);

    button_handle_t button_handle = iot_button_create(BUTTON_IO_NUM, BUTTON_ACTIVE_LEVEL);
    iot_button_set_evt_cb(button_handle, BUTTON_CB_RELEASE, button_tap_cb, NULL);
    iot_button_add_custom_cb(button_handle, 5, button_press_5s_cb, NULL);

    rmt_tx_init();
}
