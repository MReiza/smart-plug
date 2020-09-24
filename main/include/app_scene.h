#pragma once

#include <stdint.h>

void scene_init(void);
void scene_change_setting(uint8_t tmr_on_h, uint8_t tmr_on_m,
                          uint8_t tmr_off_h, uint8_t tmr_off_m,
                          uint8_t tmr_en, uint8_t mask);