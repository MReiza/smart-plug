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
unsigned short TV_DbAccessCode = 0;


static TV_DB_FORM *DbPtr;
static WAVE_FORM *WavePtr;
static DATA_FORM *DataPtr;

static unsigned char data[4];
static unsigned char ToggleFlag = 0;
bool flag = false;

static void IR_Generate(unsigned char keycode);
static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item);

static void find_set_num_short(unsigned short Set_Num, unsigned int db_list_size, unsigned short *DbAccessPtr)
{
    for (int i = 0; i < db_list_size; i++)
    {
        if (TV_DbForm[i].Num == Set_Num)
        {
            *DbAccessPtr = i;
            break;
        }
    }
}

void TV_Brand_Set(unsigned short Set_Num)
{
    find_set_num_short(Set_Num, tvdb_size, &TV_DbAccessCode);
    DbPtr = &TV_DbForm[TV_DbAccessCode];
    WavePtr = &TV_WaveForm[DbPtr->WaveForm];
    DataPtr = &TV_DataForm[DbPtr->DataForm];
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = WavePtr->Freq.Low * 10;
}


/*
 * @brief IR Signal Generate Function
 */
unsigned char TV_IR_TX(unsigned char keycode)
{
    DbPtr = &TV_DbForm[TV_DbAccessCode];
    WavePtr = &TV_WaveForm[DbPtr->WaveForm];
    DataPtr = &TV_DataForm[DbPtr->DataForm];

    if (DbPtr->Key[keycode] == 0xff)
        return UNUSED_KEY;
    else
        IR_Generate(keycode);
    return 1;
}

static int rmt_build_items(rmt_item32_t *item, unsigned char keycode)
{
    int i = 0;
    unsigned char dSeq = 0, dType, dSize, dPulse, dArray, db_data;
    data[0] = DbPtr->Custom1;
    data[1] = DbPtr->Custom2;
    data[2] = DbPtr->Key[keycode];
    data[3] = DbPtr->Default[1];
    while (dSeq < DATA_MAP_SIZE)
    {
        dType = DataPtr->DataMap[dSeq].Type;
        dSize = DataPtr->DataMap[dSeq].Size;
        dPulse = DataPtr->DataMap[dSeq].Pulse;
        dArray = DataPtr->DataMap[dSeq].Array;

        switch (dType)
        {
        case LEADER:
        case END:
            Pulse_Gen(dPulse, item++);
            i++;
            break;

        case DATA:
            db_data = data[dArray];
            while (dSize)
            {
                if (db_data & 0x01)
                    Pulse_Gen(1, item);
                else
                    Pulse_Gen(0, item);
                db_data >>= 1;
                dSize--;
                item++;
                i++;
            }
            break;

        case BAR:
            db_data = data[dArray];
            while (dSize)
            {
                if (db_data & 0x01)
                    Pulse_Gen(0, item);
                else
                    Pulse_Gen(1, item);
                db_data >>= 1;
                ;
                dSize--;
                item++;
                i++;
            }
            break;

            // case TOGGLE:
            //     ToggleFlag ^= 0x01;
            //     Pulse_Gen(ToggleFlag, item);
            //     item++;
            //     i++;
            //     break;

        case TOGGLE:
            if (flag)
            {
                db_data = data[dArray];
                flag = false;
            }
            else
            {
                db_data = ~data[dArray];
                flag = true;
            }
            while (dSize)
            {
                if (db_data & 0x01)
                    Pulse_Gen(1, item);
                else
                    Pulse_Gen(0, item);
                db_data >>= 1;
                dSize--;
                item++;
                i++;
            }
        }
        dSeq++;
    }
    return i;
}

/*
 * @brief IR Signal Generate Function
 */
static void IR_Generate(unsigned char keycode)
{
    int item_num = 0;
    size_t size = (sizeof(rmt_item32_t) * RMT_DATA_ITEM_NUM);
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);
    item_num = rmt_build_items(item, keycode);
    gpio_matrix_out(RMT_TX_EXTENDER_GPIO, RMT_TX_SIGNAL_IDX, false, false);
    rmt_write_items(RMT_TX_CHANNEL, item, item_num, true);
    free(item);
}

/*
 * @brief Pulse Generation
 */
static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item)
{
    unsigned char seq = 0;
    unsigned short *time;
    unsigned char *pol;

    time = &(WavePtr->Pulse[pulse][seq].Time);
    pol = &(WavePtr->Pulse[pulse][seq].Pol);

    item->level0 = (*pol) - 1;
    item->duration0 = (*time) / 10 * RMT_TICK_10_US;
    pol += 4;
    time += 2;
    item->level1 = (*pol) - 1;
    item->duration1 = (*time) / 10 * RMT_TICK_10_US;
}
