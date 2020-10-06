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

static void button_tap_cb(void *arg)
{
    bool output_state = relay_get_state();
    relay_set_state(!output_state);
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
        led_off();
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
    iot_led_state_write(led_handle, LED_QUICK_BLINK);
}

void led_slow_blink(void)
{
    iot_led_state_write(led_handle, LED_SLOW_BLINK);
}

void led_medium_blink(void)
{
    iot_led_state_write(led_handle, LED_MEDIUM_BLINK);
}

void led_recv_message(void)
{
    led_status_t prev_led_state = iot_led_state_read(led_handle);
    for (int i = 0; i < 5; i++)
    {
        iot_led_state_write(led_handle, i % 2);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    iot_led_state_write(led_handle, prev_led_state);
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
