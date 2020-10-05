#pragma once

void driver_init(void);
void relay_set_state(bool state);
bool relay_get_state(void);
void led_off(void);
void led_on(void);
void led_quick_blink(void);
void led_slow_blink(void);
void led_medium_blink(void);
void led_recv_message(void);
