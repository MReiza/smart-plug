#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "ir_remote.h"
#include "ac_db_form.h"

void rmt_tx_init()
{
    rmt_config_t rmt_tx_0, rmt_tx_1;
    rmt_tx_0.channel = RMT_TX_CHANNEL;
    rmt_tx_0.gpio_num = RMT_TX_IO_NUM;
    rmt_tx_0.mem_block_num = 1;
    rmt_tx_0.clk_div = RMT_CLK_DIV;
    rmt_tx_0.tx_config.loop_en = false;
    rmt_tx_0.tx_config.carrier_level = 1;
    rmt_tx_0.tx_config.carrier_en = true;
    rmt_tx_0.tx_config.idle_level = 0;
    rmt_tx_0.tx_config.idle_output_en = true;
    rmt_tx_0.rmt_mode = 0;
    rmt_tx_0.tx_config.carrier_freq_hz = 38000;
    rmt_tx_0.tx_config.carrier_duty_percent = 30;

    rmt_tx_1.channel = RMT_EXT_CHANNEL;
    rmt_tx_1.gpio_num = RMT_EXT_IO_NUM;
    rmt_tx_1.mem_block_num = 1;
    rmt_tx_1.clk_div = RMT_CLK_DIV;
    rmt_tx_1.tx_config.loop_en = false;
    rmt_tx_1.tx_config.carrier_level = 1;
    rmt_tx_1.tx_config.carrier_en = true;
    rmt_tx_1.tx_config.idle_level = 0;
    rmt_tx_1.tx_config.idle_output_en = true;
    rmt_tx_1.rmt_mode = 0;
    rmt_tx_1.tx_config.carrier_freq_hz = 38000;
    rmt_tx_1.tx_config.carrier_duty_percent = 30;

    rmt_config(&rmt_tx_0);
    rmt_config(&rmt_tx_1);

    rmt_driver_install(rmt_tx_0.channel, 0, 0);
    rmt_driver_install(rmt_tx_1.channel, 0, 0);
}

void rmt_set_num(unsigned short Set_Num, unsigned short *DbAccessPtr)
{
    for (int i = 0; i < acdb_size; i++)
    {
        if (AC_DbForm[i].Num == Set_Num)
        {
            *DbAccessPtr = i;
            break;
        }
    }
}