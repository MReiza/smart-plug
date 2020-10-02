#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "ir_remote.h"
#include "ac_db_form.h"

void rmt_tx_init()
{
    rmt_config_t rmt_tx_config = RMT_DEFAULT_CONFIG_TX(RMT_TX_GPIO, RMT_TX_CHANNEL);
    rmt_tx_config.tx_config.carrier_en = true;
    rmt_tx_config.clk_div = 100;
    rmt_config(&rmt_tx_config);
    rmt_driver_install(rmt_tx_config.channel, 0, 0);
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