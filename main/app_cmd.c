#include "global.h"
#include "app_cmd.h"
#include "app_driver.h"
#include "app_scene.h"
#include "ac_ir_tx.h"
#include "tv_ir_tx.h"

static esp_timer_handle_t relay_timer;
static int state, current_model = -1;

static const char *TAG = "CMD";

static void relay_timer_cb(void *arg)
{
    relay_set_state(state);
}

void cmd_relay(void)
{
    bool relay_state = relay_get_state();
    relay_set_state(!relay_state);
}

void cmd_timer_init(void)
{
    const esp_timer_create_args_t relay_timer_args = {
        .callback = &relay_timer_cb,
    };
    ESP_ERROR_CHECK(esp_timer_create(&relay_timer_args, &relay_timer));
}

void cmd_timer(cJSON *cmd_message)
{
    cJSON *timer_en = cJSON_GetObjectItem(cmd_message, "enable");
    if (cJSON_IsTrue(timer_en))
    {
        int timeout = cJSON_GetObjectItem(cmd_message, "timeout")->valueint;
        state = cJSON_GetObjectItem(cmd_message, "state")->valueint;
        esp_err_t err = esp_timer_start_once(relay_timer, timeout * 1000000);
        if (err == ESP_ERR_INVALID_STATE)
        {
            ESP_ERROR_CHECK(esp_timer_stop(relay_timer));
            err = esp_timer_start_once(relay_timer, timeout * 1000000);
        }
        ESP_ERROR_CHECK(err);
        ESP_LOGI(TAG, "Timer start: %d seconds", timeout);
    }
    else
    {
        esp_timer_stop(relay_timer);
        ESP_LOGI(TAG, "Timer stop");
    }
}

void cmd_scene_init(void)
{
    scene_init();
}

void cmd_scene(cJSON *cmd_message)
{
    int timeron_hour = cJSON_GetObjectItem(cmd_message, "timeron_hour")->valueint;
    int timeron_min = cJSON_GetObjectItem(cmd_message, "timeron_min")->valueint;
    int timeroff_hour = cJSON_GetObjectItem(cmd_message, "timeroff_hour")->valueint;
    int timeroff_min = cJSON_GetObjectItem(cmd_message, "timeroff_min")->valueint;
    int timer_ena = cJSON_GetObjectItem(cmd_message, "timer_ena")->valueint;
    int weekday_mask = cJSON_GetObjectItem(cmd_message, "weekday_mask")->valueint;

    scene_change_setting(timeron_hour, timeron_min, timeroff_hour, timeroff_min, timer_ena, weekday_mask);
}

void cmd_rmt(cJSON *cmd_message)
{
    char *group = cJSON_GetObjectItem(cmd_message, "group")->valuestring;
    char *keycode = cJSON_GetObjectItem(cmd_message, "keycode")->valuestring;
    int model = cJSON_GetObjectItem(cmd_message, "model")->valueint;
    if (strcmp(group, "aircon") == 0)
    {
        if (current_model != model)
        {
            AC_Brand_Set(model);
            current_model = model;
        }

        if (strcmp(keycode, "AC_POWER_ON") == 0)
            ac_power_on();
        else if (strcmp(keycode, "AC_POWER_OFF") == 0)
            ac_power_off();
        else if (strcmp(keycode, "AC_TEMP_UP") == 0)
            ac_temp_up();
        else if (strcmp(keycode, "AC_TEMP_DN") == 0)
            ac_temp_down();
        else if (strcmp(keycode, "AC_SWING_ON") == 0)
            ac_swing_on();
        else if (strcmp(keycode, "AC_SWING_OFF") == 0)
            ac_swing_off();
        else if (strcmp(keycode, "AC_MODE_AUTO") == 0)
            ac_mode_auto();
        else if (strcmp(keycode, "AC_MODE_COOL") == 0)
            ac_mode_cool();
        else if (strcmp(keycode, "AC_MODE_DRY") == 0)
            ac_mode_dry();
        else if (strcmp(keycode, "AC_MODE_WARM") == 0)
            ac_mode_warm();
        else if (strcmp(keycode, "AC_FAN_AUTO") == 0)
            ac_fan_auto();
        else if (strcmp(keycode, "AC_FAN_LOW") == 0)
            ac_fan_low();
        else if (strcmp(keycode, "AC_FAN_MID") == 0)
            ac_fan_mid();
        else if (strcmp(keycode, "AC_FAN_HIGH") == 0)
            ac_fan_high();
        else if (strcmp(keycode, "AC_FAN_TURBO") == 0)
            ac_fan_turbo();
    }
    else if (strcmp(group, "tv") == 0)
    {
        TV_Brand_Set(model);
        int cmd = 0;

        if (strcmp(keycode, "KEY_TV_POWER") == 0)
            cmd = TV_POWER;
        else if (strcmp(keycode, "KEY_TV_SOURCE") == 0)
            cmd = TV_SOURCE;
        else if (strcmp(keycode, "KEY_TV_VOLUP") == 0)
            cmd = TV_VOL_UP;
        else if (strcmp(keycode, "KEY_TV_VOLDN") == 0)
            cmd = TV_VOL_DOWN;
        else if (strcmp(keycode, "KEY_TV_UP") == 0)
            cmd = TV_UP;
        else if (strcmp(keycode, "KEY_TV_DOWN") == 0)
            cmd = TV_DOWN;
        else if (strcmp(keycode, "KEY_TV_LEFT") == 0)
            cmd = TV_LEFT;
        else if (strcmp(keycode, "KEY_TV_RIGHT") == 0)
            cmd = TV_RIGHT;
        else if (strcmp(keycode, "KEY_TV_OK") == 0)
            cmd = TV_OK;

        TV_IR_TX(cmd);
    }
    led_blink(2, 100);
}

void cmd_ota(void)
{
    check_firmware_update();
}