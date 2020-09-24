#pragma once

#include "cJSON.h"

void cmd_ota(void);
void cmd_relay(void);
void cmd_timer_init(void);
void cmd_scene_init(void);
void cmd_timer(cJSON *cmd_message);
void cmd_scene(cJSON *cmd_message);
void cmd_rmt(cJSON *cmd_message);