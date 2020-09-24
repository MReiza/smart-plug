#include "global.h"
#include "iot_weekly_timer.h"
#include "app_driver.h"

static nvs_handle_t scene_nvs;
static weekday_mask_t w_m;
static event_time_t event_tm[2];
static weekly_timer_handle_t scene_timer;
static uint8_t relay_on_hour, relay_on_min, relay_off_hour, relay_off_min, scene_en, weekday_mask;

static const char *TAG = "SCENE";

static void scene_on_cb(void *arg)
{
    relay_set_state(true);
}

static void scene_off_cb(void *arg)
{
    relay_set_state(false);
}

static void scene_set_weekday(uint8_t mask)
{
    w_m.sunday = mask & 0x01 ? 1 : 0;
    w_m.monday = mask & 0x02 ? 1 : 0;
    w_m.tuesday = mask & 0x04 ? 1 : 0;
    w_m.wednesday = mask & 0x08 ? 1 : 0;
    w_m.thursday = mask & 0x10 ? 1 : 0;
    w_m.friday = mask & 0x20 ? 1 : 0;
    w_m.saturday = mask & 0x40 ? 1 : 0;
    w_m.enable = mask & 0x80 ? 1 : 0;
}

static void scene_create(void)
{
    event_tm[0].hour = relay_on_hour;
    event_tm[0].minute = relay_on_min;
    event_tm[0].second = 0;
    event_tm[0].tm_cb = scene_on_cb;
    event_tm[0].arg = NULL;
    event_tm[0].en = scene_en;

    event_tm[1].hour = relay_off_hour;
    event_tm[1].minute = relay_off_min;
    event_tm[1].second = 0;
    event_tm[1].tm_cb = scene_off_cb;
    event_tm[1].arg = NULL;
    event_tm[1].en = scene_en;

    bool loop_en = weekday_mask > 0 ? true : false;
    scene_set_weekday(weekday_mask);
    scene_timer = iot_weekly_timer_add(loop_en, w_m, 2, event_tm);
    if (scene_en)
    {
        iot_weekly_timer_start(scene_timer);
    }
}

static void scene_set_default(void)
{
    relay_on_hour = 0;
    relay_on_min = 0;
    relay_off_hour = 0;
    relay_off_min = 0;
    scene_en = 0;
    weekday_mask = 0;
}

static esp_err_t scene_save_setting(void)
{
    esp_err_t err = nvs_open("nvs", NVS_READWRITE, &scene_nvs);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open scene nvs, error %d", err);
    }
    else
    {
        err = nvs_set_u8(scene_nvs, "relay_on_hour", relay_on_hour);
        err = nvs_set_u8(scene_nvs, "relay_on_min", relay_on_min);
        err = nvs_set_u8(scene_nvs, "relay_off_hour", relay_off_hour);
        err = nvs_set_u8(scene_nvs, "relay_off_min", relay_off_min);
        err = nvs_set_u8(scene_nvs, "scene_en", scene_en);
        err = nvs_set_u8(scene_nvs, "weekday_mask", weekday_mask);
        err = nvs_commit(scene_nvs);
        nvs_close(scene_nvs);
    }
    return err;
}

static esp_err_t scene_load_setting(void)
{
    esp_err_t err = nvs_open("nvs", NVS_READWRITE, &scene_nvs);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open scene nvs, error %d", err);
        scene_set_default();
        scene_save_setting();
    }
    else
    {
        err = nvs_get_u8(scene_nvs, "relay_on_hour", &relay_on_hour);
        err = nvs_get_u8(scene_nvs, "relay_on_min", &relay_on_min);
        err = nvs_get_u8(scene_nvs, "relay_off_hour", &relay_off_hour);
        err = nvs_get_u8(scene_nvs, "relay_off_min", &relay_off_min);
        err = nvs_get_u8(scene_nvs, "scene_en", &scene_en);
        err = nvs_get_u8(scene_nvs, "weekday_mask", &weekday_mask);
        nvs_close(scene_nvs);
    }
    return err;
}

void scene_change_setting(uint8_t hour_0, uint8_t min_0,
                          uint8_t hour_1, uint8_t min_1,
                          uint8_t enable, uint8_t mask)
{
    iot_weekly_timer_stop(scene_timer);
    iot_weekly_timer_delete(scene_timer);

    relay_on_hour = hour_0;
    relay_on_min = min_0;
    relay_off_hour = hour_1;
    relay_off_min = min_1;
    scene_en = enable;
    weekday_mask = mask;

    scene_save_setting();
    scene_create();
}

void scene_init(void)
{
    iot_weekly_timer_init();

    scene_load_setting();
    scene_create();
}