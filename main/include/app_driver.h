#pragma once

void driver_init(void);
void relay_set_state(bool state);
bool relay_get_state(void);
esp_err_t led_off(void);
esp_err_t led_on(void);
esp_err_t led_quick_blink(void);
esp_err_t led_slow_blink(void);
esp_err_t led_medium_blink(void);
