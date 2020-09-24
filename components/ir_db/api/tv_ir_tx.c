#define __TV_IR_TX_C__

#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"
#include "tv_db_form.h"
#include "tv_ir_tx.h"
#include "ir_remote.h"

static const char *TAG = "TV_IR_TX";
unsigned char TV_DbAccessCode = 0;
unsigned char STB_DbAccessCode = 0;
int channel;

static STB_DB_FORM *DbPtr;
static WAVE_FORM *WavePtr;
static DATA_FORM *DataPtr;

static unsigned char data[3];
static unsigned char ToggleFlag = 0;

static void IR_Generate(unsigned char keycode);
static inline void Pulse_Gen(unsigned char pulse);

/*
 * @brief RMT transmitter initialization
 */
void tv_tx_init()
{
    rmt_config_t rmt_tx;
    rmt_tx.channel = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = RMT_CLK_DIV;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_level = 1;
    rmt_tx.tx_config.carrier_en = RMT_TX_CARRIER_EN;
    rmt_tx.tx_config.idle_level = 0;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_tx.rmt_mode = 0;

    rmt_tx.tx_config.carrier_freq_hz = 38500;
    rmt_tx.tx_config.carrier_duty_percent = 30;

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = TV_WaveForm[TV_DbForm[TV_DbAccessCode].WaveForm].Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = TV_WaveForm[TV_DbForm[TV_DbAccessCode].WaveForm].Freq.Low * 10;

    rmt_config(&rmt_tx);

    rmt_driver_install(rmt_tx.channel, 0, 0);
}

void stb_tx_init()
{
    rmt_config_t rmt_tx;
    rmt_tx.channel = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = RMT_CLK_DIV;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_level = 1;
    rmt_tx.tx_config.carrier_en = RMT_TX_CARRIER_EN;
    rmt_tx.tx_config.idle_level = 0;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_tx.rmt_mode = 0;

    rmt_tx.tx_config.carrier_freq_hz = 38500;
    rmt_tx.tx_config.carrier_duty_percent = 30;

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = TV_WaveForm[STB_DbForm[STB_DbAccessCode].WaveForm].Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = TV_WaveForm[STB_DbForm[STB_DbAccessCode].WaveForm].Freq.Low * 10;

    rmt_config(&rmt_tx);

    rmt_driver_install(rmt_tx.channel, 0, 0);
}

void TV_Brand_Set(unsigned char Set_Num)
{
    find_set_num(Set_Num, &TV_DbForm[0].Num, tvdb_size, sizeof(TV_DB_FORM), &TV_DbAccessCode);
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = TV_WaveForm[TV_DbForm[TV_DbAccessCode].WaveForm].Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = TV_WaveForm[TV_DbForm[TV_DbAccessCode].WaveForm].Freq.Low * 10;
}

void STB_Brand_Set(unsigned char Set_Num)
{
    find_set_num(Set_Num, &STB_DbForm[0].Num, stbdb_size, sizeof(STB_DB_FORM), &STB_DbAccessCode);
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = TV_WaveForm[STB_DbForm[STB_DbAccessCode].WaveForm].Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = TV_WaveForm[STB_DbForm[STB_DbAccessCode].WaveForm].Freq.Low * 10;
}

void find_set_num(unsigned char Set_Num, unsigned char *setnum_ptr, unsigned char db_list_size, unsigned char db_size, unsigned char *DbAccessPtr)
{
    for (int i = 0; i < db_list_size; i++)
    {
        if (*setnum_ptr == Set_Num)
        {
            *DbAccessPtr = i;
            break;
        }
        setnum_ptr += db_size;
    }
}

/*
 * @brief IR Signal Generate Function
 */
unsigned char TV_IR_TX(unsigned char keycode)
{
    DbPtr = &TV_DbForm[TV_DbAccessCode];
    WavePtr = &TV_WaveForm[DbPtr->WaveForm];
    DataPtr = &TV_DataForm[DbPtr->DataForm];
    channel = RMT_TX_CHANNEL;

    if (DbPtr->Key[keycode] == 0xff)
        return UNUSED_KEY;
    else
        IR_Generate(keycode);
    return 1;
}

/*
 * @brief IR Signal Generate Function
 */
unsigned char STB_IR_TX(unsigned char keycode)
{
    DbPtr = &STB_DbForm[STB_DbAccessCode];
    WavePtr = &TV_WaveForm[DbPtr->WaveForm];
    DataPtr = &TV_DataForm[DbPtr->DataForm];
    channel = RMT_TX_CHANNEL;

    if (DbPtr->Key[keycode] == 0xff)
        return UNUSED_KEY;
    else
        IR_Generate(keycode);
    return 1;
}

/*
 * @brief IR Signal Generate Function
 */
static void IR_Generate(unsigned char keycode)
{
    unsigned char dSeq = 0, dType, dSize, dPulse, dArray, db_data;
    data[0] = DbPtr->Custom1;
    data[1] = DbPtr->Custom2;
    data[2] = DbPtr->Key[keycode];

    while (dSeq < DATA_MAP_SIZE)
    {
        dType = DataPtr->DataMap[dSeq].Type;
        dSize = DataPtr->DataMap[dSeq].Size;
        dPulse = DataPtr->DataMap[dSeq].Pulse;
        dArray = DataPtr->DataMap[dSeq].Array;

        switch (dType & 0xF0)
        {
        case LEADER:
        case END:
            Pulse_Gen(dPulse);
            break;
        case DATA:
            db_data = data[dArray];
            while (dSize)
            {
                if (dType & BAR)
                {
                    if (db_data & 0x01)
                        Pulse_Gen(0);
                    else
                        Pulse_Gen(1);
                }
                else
                {
                    if (db_data & 0x01)
                        Pulse_Gen(1);
                    else
                        Pulse_Gen(0);
                }
                dSize--;
                db_data >>= 1;
            }
            break;
        case TOGGLE:
            //if(rep == 0)ToggleFlag ^= 0x01;
            ToggleFlag ^= 0x01;
            Pulse_Gen(ToggleFlag);
            break;
        }
        dSeq++;
    }
}

/*
 * @brief Pulse Generation
 */
static inline void Pulse_Gen(unsigned char pulse)
{
    unsigned char seq = 0;
    unsigned short *time;
    unsigned char *pol;
    rmt_item32_t *item = malloc(sizeof(rmt_item32_t));

    time = &(WavePtr->Pulse[pulse][seq].Time);
    pol = &(WavePtr->Pulse[pulse][seq].Pol);

    while ((*pol))
    {
        item->level0 = (*pol) - 1;
        item->duration0 = (*time) / 10 * RMT_TICK_10_US;
        pol += 4;
        time += 2;
        seq++;

        item->level1 = (*pol) - 1;
        item->duration1 = (*time) / 10 * RMT_TICK_10_US;
        rmt_write_items(channel, item, 1, true);
        rmt_write_items(1, item, 1, true);
        seq++;

        if (seq >= 6)
            break;
        pol += 4;
        time += 2;
    }
}
