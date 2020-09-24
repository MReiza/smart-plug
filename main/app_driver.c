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
#define TIMER_TIMEOUT_US 1000000

static relay_handle_t relay_handle;
static led_handle_t led_handle;
static esp_timer_handle_t publish_timer;

static void publish_timer_cb(void *arg)
{
    cloud_publish();
}

static void publish_timer_init(void)
{
    const esp_timer_create_args_t publish_timer_args = {
        .callback = &publish_timer_cb,
        .name = "once",
    };
    ESP_ERROR_CHECK(esp_timer_create(&publish_timer_args, &publish_timer));
}

static void button_tap_cb(void *arg)
{
    bool relay_state = relay_get_state();
    relay_set_state(!relay_state);
}

static void button_press_5s_cb(void *arg)
{
    nvs_flash_erase();
    esp_restart();
}

esp_err_t relay_on(void)
{
    return iot_relay_state_write(relay_handle, RELAY_STATUS_CLOSE);
}

esp_err_t relay_off(void)
{
    return iot_relay_state_write(relay_handle, RELAY_STATUS_OPEN);
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

    esp_err_t err = esp_timer_start_once(publish_timer, TIMER_TIMEOUT_US);
    if (err != ESP_OK)
    {
        ESP_ERROR_CHECK(esp_timer_stop(publish_timer));
        err = esp_timer_start_once(publish_timer, TIMER_TIMEOUT_US);
    }
    ESP_ERROR_CHECK(err);
}

bool relay_get_state(void)
{
    relay_status_t relay_status = iot_relay_state_read(relay_handle);
    return relay_status == RELAY_STATUS_CLOSE;
}

esp_err_t led_off(void)
{
    return iot_led_state_write(led_handle, LED_OFF);
}

esp_err_t led_on(void)
{
    return iot_led_state_write(led_handle, LED_ON);
}

esp_err_t led_quick_blink(void)
{
    return iot_led_state_write(led_handle, LED_QUICK_BLINK);
}

esp_err_t led_slow_blink(void)
{
    return iot_led_state_write(led_handle, LED_SLOW_BLINK);
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

    publish_timer_init();
    rmt_tx_init();
}
