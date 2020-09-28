#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "driver/periph_ctrl.h"
#include "soc/rmt_reg.h"
#include "ir_remote.h"
#include "ac_db_form.h"
#include "ac_ir_tx.h"

unsigned char *output_buf;
unsigned char present_temp, present_mode, present_fan, present_swing, present_power;
unsigned char m_mode, m_fan;
static unsigned char do_verify, do_verify2;

static AC_DB_FORM *DbPtr;
static WAVE_FORM *WavePtr;
static DATA_FORM *DataPtr;

static void Verify_data();
static void IR_Generate();
static void Pulse_Gen(unsigned char pulse, rmt_item32_t *item);
static void Init_Output_Buf();

void AC_Brand_Set(unsigned short Set_Num)
{
    unsigned short DbAccessCode = 0;
    do_verify = 0;
    present_temp = 6;
    m_mode = 0;
    m_fan = 14;

    rmt_set_num(Set_Num, &DbAccessCode);
    DbPtr = &AC_DbForm[DbAccessCode];
    WavePtr = &AC_WaveForm[DbPtr->WaveForm];
    DataPtr = &AC_DataForm[DbPtr->DataForm];

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = WavePtr->Freq.Low * 10;
    RMT.carrier_duty_ch[1].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[1].low = WavePtr->Freq.Low * 10;
    Init_Output_Buf();
}

static void Init_Output_Buf()
{
    unsigned char dSeq = 1, dArray;

    output_buf = calloc(DATA_MAP_SIZE, sizeof(unsigned char));

    while (dSeq < DATA_MAP_SIZE)
    {
        dArray = DataPtr->DataMap[dSeq].Array;
        switch (dArray)
        {
        case 1:
            output_buf[dArray] = DbPtr->Data[m_mode + m_fan];
            break;
        case 2:
            output_buf[dArray] = DbPtr->Data[POWER_ON];
            break;
        case 3:
            output_buf[dArray] = DbPtr->Data[POWER_ON];
            break;
        case 4:
            output_buf[dArray] = DbPtr->Data[MODE_COOL];
            present_mode = DbPtr->Data[MODE_COOL];
            break;
        case 5:
            output_buf[dArray] = DbPtr->Data[SWING_OFF];
            break;
        case 6:
            output_buf[dArray] = DbPtr->Data[FAN_HIGH];
            present_fan = DbPtr->Data[FAN_HIGH];
            break;
        case 7:
            output_buf[dArray] = DbPtr->Temp[present_temp];
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            output_buf[dArray] = DbPtr->Default[dArray - 8];
            break;
        case 18:
            do_verify = 1;
            break;
        case 19:
            do_verify2 = 1;
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            output_buf[dArray] = DbPtr->Default[dArray - 11];
            break;
        }
        dSeq++;
    }
    if (do_verify)
        Verify_data();
}

unsigned char reverse_bit(unsigned char num, int nbit)
{
    unsigned char reverse_num = 0;
    for (int i = 0; i < nbit; i++)
    {
        reverse_num <<= 1;
        reverse_num |= num & 1;
        num >>= 1;
    }
    return reverse_num;
}

unsigned char count_one_bit(unsigned char num)
{
    unsigned char count = 0;
    while (num)
    {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

void Verify_data()
{
    unsigned char dSeq = 0, dType, dSize, dArray, offset, mask, db_data, cnt = 0;
    if (do_verify)
    {
        switch (DbPtr->Verify)
        {
        case vestel_v_1:
            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                switch (dType)
                {
                case DATA:
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        cnt += count_one_bit(db_data);
                    }
                    break;
                }
                dSeq++;
            }
            output_buf[18] = ~cnt;
            break;

        case custom46_v_1:
            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                if (dType == DATA)
                {
                    if (dArray != 18)
                        cnt += count_one_bit(output_buf[dArray]);
                    else
                        dSize = DataPtr->DataMap[dSeq].Size;
                }
                dSeq++;
            }
            output_buf[18] = reverse_bit(cnt, dSize);
            break;

        /* Sum all bytes */
        case electra_v_1:
        case mitsubishi_v_1:
        case tcl_v_1:
        case custom48_v_1:
        case custom56_v_2:
            if (DbPtr->DataForm == TCL112AC4_DM)
                dSeq = 9;

            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                if (dType == DATA)
                {
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        while (dSize < 8)
                        {
                            dSeq++;
                            offset = DataPtr->DataMap[dSeq].Size;
                            mask = DataPtr->DataMap[dSeq].Array;
                            db_data += (output_buf[mask] << dSize);
                            dSize += offset;
                        }
                        cnt += db_data;
                    }
                }
                dSeq++;
            }
            output_buf[18] = cnt;
            break;

        /* Sum all reverse-bytes */
        case haier_v_1:
        case custom88_v_1:
            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                if (dType == DATA)
                {
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        while (dSize < 8)
                        {
                            dSeq++;
                            offset = DataPtr->DataMap[dSeq].Size;
                            mask = DataPtr->DataMap[dSeq].Array;
                            db_data += (output_buf[mask] << dSize);
                            dSize += offset;
                        }
                        cnt += reverse_bit(db_data, 8);
                    }
                }
                dSeq++;
            }
            output_buf[18] = reverse_bit(cnt, 8);
            break;

        /* Sum all nibbles */
        case century_v_1:
        case samsung_v_1:
        case winia_v_1:
        case winia_v_2:
        case winia_v_3:
        case custom112_v_1:
        case custom136_v_1:
        case custom136_v_2:
        case custom176_v_1:
        case custom32_v_1:
        case custom40_v_1:
        case custom64_v_1:
        case custom64_v_2:
        case custom88_v_2:
            if (DbPtr->DataForm == CUSTOM32AC4_DM)
                dSeq = 3;
            else if (DbPtr->DataForm == CUSTOM152AC3_DM)
                dSeq = 9;
            else if (DbPtr->Verify == custom136_v_1)
                dSeq = 5;
            else if (DbPtr->Verify == custom88_v_2)
                dSeq = 5;
            else if (DbPtr->Verify == century_v_1)
                dSeq = 3;
            else if (DbPtr->Verify == winia_v_1)
                dSeq = 3;

            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                if (dType == DATA)
                {
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        while (dSize % 4 != 0)
                        {
                            dSeq++;
                            offset = DataPtr->DataMap[dSeq].Size;
                            mask = DataPtr->DataMap[dSeq].Array;
                            db_data += (output_buf[mask] << dSize);
                            dSize += offset;
                        }
                        if (dSize == 8)
                        {
                            db_data = ((db_data >> 4) + (db_data & 0x0f));
                        }
                        cnt += db_data;
                    }
                }
                dSeq++;
            }
            output_buf[18] = cnt;
            if (DbPtr->Verify == custom136_v_2)
                output_buf[18] = (cnt << 4) | (cnt >> 4);
            break;

        /* Sum all reverse-nibbles */
        case lg_v_1:
            dSeq = 3;
            if (DbPtr->DataForm == LG28AC3_DM)
                dSeq = 2;

            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                if (dType == DATA)
                {
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        while (dSize % 4 != 0)
                        {
                            dSeq++;
                            offset = DataPtr->DataMap[dSeq].Size;
                            mask = DataPtr->DataMap[dSeq].Array;
                            db_data += (output_buf[mask] << dSize);
                            dSize += offset;
                        }
                        if (dSize == 8)
                        {
                            db_data = ((db_data >> 4) + (db_data & 0x0f));
                        }
                        cnt += reverse_bit(db_data, 4);
                    }
                }
                dSeq++;
            }
            output_buf[18] = reverse_bit(cnt, 4);
            break;

        case custom56_v_1:
        case custom60_v_1:
            while (dSeq < DATA_MAP_SIZE)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                switch (dType)
                {
                case DATA:
                    if (dArray != 18)
                        cnt += reverse_bit(output_buf[dArray], 4);
                    break;
                }
                dSeq++;
            }
            output_buf[18] = reverse_bit(cnt >> 4, 8) | reverse_bit(cnt, 4);
            break;

        case coolix_v_1:
            output_buf[18] = ~((output_buf[4] << 4) + output_buf[7]);
            break;

        case gree_v_1:
        case custom126_v_1a:
            output_buf[18] = 0x0a;
            output_buf[18] += output_buf[3] << 3 | output_buf[4];
            output_buf[18] += output_buf[7] & 0x0f;
            output_buf[18] += output_buf[8] & 0x0f;
            output_buf[18] += output_buf[9] & 0x0f;
            output_buf[18] += output_buf[5] >> 4;
            output_buf[18] += output_buf[11] >> 4;
            break;

        /* Copy from Reiza */
        case whirlpool_v_1:
            dSeq = 3;
            while (dSeq < 12)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                switch (dType)
                {
                case DATA:
                    db_data = output_buf[dArray];
                    while (dSize % 8 != 0 && dSize < 8)
                    {
                        offset = DataPtr->DataMap[dSeq + 1].Size;
                        mask = DataPtr->DataMap[dSeq + 1].Array;
                        db_data += (output_buf[mask] << dSize);
                        dSeq++;
                        dSize += offset;
                    }
                    cnt ^= db_data;
                    break;
                }
                dSeq++;
            }
            output_buf[18] = cnt;
            break;

        case custom88_v_3:
            dSeq = 5;
            while (dSeq < 12)
            {
                dType = DataPtr->DataMap[dSeq].Type;
                dArray = DataPtr->DataMap[dSeq].Array;
                dSize = DataPtr->DataMap[dSeq].Size;
                switch (dType)
                {
                case DATA:
                    if (dArray != 18)
                    {
                        db_data = output_buf[dArray];
                        if (dSize == 8)
                        {
                            db_data = (output_buf[dArray] >> 4) + (output_buf[dArray] & 0x0f);
                        }
                        cnt += db_data;
                    }
                    break;
                }
                dSeq++;
            }
            output_buf[18] = cnt;
            break;
            /* End of copy */
        }
    }

    /* 2nd VERIFY */
    if (do_verify2)
    {
        switch (DbPtr->Verify1)
        {
        case custom126_v_1b:
            output_buf[19] = 0x0a;
            output_buf[19] += output_buf[3] << 3 | output_buf[4];
            output_buf[19] += output_buf[7] & 0x0f;
            output_buf[19] += output_buf[6];
            break;
        }
    }
}

static int rmt_build_items(rmt_item32_t *item)
{
    int i = 0;
    unsigned char dSeq = 0, dType, dSize, dPulse, dArray, db_data;
    unsigned char *output_ptr = output_buf;

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
            db_data = output_ptr[dArray];
            while (dSize)
            {
                if (db_data & 0x01)
                    Pulse_Gen(1, item);
                else
                    Pulse_Gen(0, item);
                dSize--;
                db_data >>= 1;
                item++;
                i++;
            }
            break;
        case BAR:
            db_data = output_ptr[dArray];
            while (dSize)
            {
                if (db_data & 0x01)
                    Pulse_Gen(0, item);
                else
                    Pulse_Gen(1, item);
                dSize--;
                db_data >>= 1;
                item++;
                i++;
            }
            break;
        }
        dSeq++;
    }
    return i;
}

static void IR_Generate()
{
    int item_num = 0;
    size_t size = (sizeof(rmt_item32_t) * RMT_DATA_ITEM_NUM);
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);
    item_num = rmt_build_items(item);
    rmt_write_items(RMT_TX_CHANNEL, item, item_num, true);
    rmt_wait_tx_done(RMT_TX_CHANNEL, portMAX_DELAY);
    rmt_write_items(RMT_EXT_CHANNEL, item, item_num, true);
    rmt_wait_tx_done(RMT_EXT_CHANNEL, portMAX_DELAY);
    free(item);
}

static void Pulse_Gen(unsigned char pulse, rmt_item32_t *item)
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

unsigned char ac_power_on()
{
    if (DbPtr->Data[POWER_ON] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        output_buf[2] = DbPtr->Data[POWER_ON];
        output_buf[3] = DbPtr->Data[POWER_ON];

        switch (DbPtr->DataForm)
        {
        case LG28AC_DM:
        case LG28AC2_DM:
            output_buf[4]--;
            Verify_data();
            IR_Generate();
            output_buf[4]++;
            break;

        case HAIER112AC_DM:
            output_buf[14] = 0xa0;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM56AC_DM:
            output_buf[12] = 0x0c;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0f;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC4_DM:
            output_buf[5] = output_buf[5] ^ 0x8;
            Verify_data();
            IR_Generate();
            output_buf[5] = output_buf[5] ^ 0x08;
            break;

        case CUSTOM40AC3_DM:
        case CUSTOM72AC3_DM:
        case CUSTOM40AC6_DM:
            output_buf[13] = 0x08;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM72AC4_DM:
        case CUSTOM72AC_DM:
            output_buf[10] = 0x04;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM72AC6_DM:
            output_buf[10] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM72AC5_DM:
            output_buf[14] = 0x08;
            Verify_data();
            IR_Generate();
            output_buf[14] = DbPtr->Default[6];
            break;

        case CUSTOM56AC3_DM:
        case CUSTOM60AC_DM:
            output_buf[12] = 0x0c;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case NECAC5_DM:
        case NECAC8_DM:
        case NECAC_DM:
            output_buf[10] = 0x05;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case NECAC4_DM:
            output_buf[10] = 0x01;
            output_buf[11] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[11] = DbPtr->Default[3];
            break;

        case NECAC3_DM:
            output_buf[10] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case NECAC9_DM:
            output_buf[10] = 0x05;
            output_buf[11] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[11] = DbPtr->Default[3];
            break;

        case NECAC10_DM:
        case NECAC6_DM:
            output_buf[10] = 0x05;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x05;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM112AC_DM:
            output_buf[12] = 0x08;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[10] = 0x01;
            output_buf[13] = 0x01;
            output_buf[15] = 0x39;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[10] = 0x01;
            output_buf[13] = 0x01;
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM152AC3_DM:
        case CUSTOM136AC4_DM:
        case CUSTOM136AC3_DM:
        case CUSTOM136AC_DM:
            output_buf[13] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM136AC2_DM:
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case CUSTOM36AC_DM:
            output_buf[12] = 0x05;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case CUSTOM32AC5_DM:
            if (DbPtr->Num == 51)
            {
                output_buf[12] = 0x05;
                output_buf[13] = 0x00;
                Verify_data();
                IR_Generate();
                output_buf[12] = DbPtr->Default[4];
                output_buf[13] = DbPtr->Default[5];
            }
            else
            {
                output_buf[12] = 0x05;
                Verify_data();
                IR_Generate();
                output_buf[12] = DbPtr->Default[4];
            }
            break;

        case CUSTOM168AC_DM:
            output_buf[10] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM56AC5_DM:
			output_buf[12] = 0x01;
			Verify_data();
			IR_Generate();
			break;

        case CUSTOM32AC4_DM:
			output_buf[10] = 0x00;
			Verify_data();
			IR_Generate();
			break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x00;
            output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_power_off()
{
    if (DbPtr->Data[POWER_OFF] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        output_buf[2] = DbPtr->Data[POWER_OFF];
        output_buf[3] = DbPtr->Data[POWER_OFF];

        switch (DbPtr->DataForm)
        {
        case LG28AC_DM:
        case LG28AC2_DM:
            output_buf[4] = 0x00;
            output_buf[7] = 0x00;
            output_buf[6] = 0x0a;
            Verify_data();
            IR_Generate();
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case HAIER112AC_DM:
            output_buf[14] = 0xa0;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x00;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM46AC2_DM:
        case CUSTOM46AC3_DM:
            output_buf[4] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[4] = present_mode;
            break;

        case CUSTOM56AC_DM:
            output_buf[12] = 0x0c;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case CUSTOM88AC2_DM:
            output_buf[10] = 0x29;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM88AC3_DM:
            output_buf[10] = 0x29;
            output_buf[13] = 0x09;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM88AC4_DM:
            output_buf[10] = 0x29;
            output_buf[14] = 0x80;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[14] = DbPtr->Default[6];
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0f;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC6_DM:
        case CUSTOM40AC3_DM:
            output_buf[13] = 0x08;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM40AC4_DM:
            output_buf[5] = output_buf[5] ^ 0x8;
            Verify_data();
            IR_Generate();
            output_buf[5] = output_buf[5] ^ 0x08;
            break;

        case CUSTOM72AC4_DM:
        case CUSTOM72AC_DM:
            output_buf[10] = 0x08;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM72AC6_DM:
            output_buf[10] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM72AC3_DM:
            output_buf[13] = 0x04;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM72AC5_DM:
            output_buf[14] = 0x04;
            Verify_data();
            IR_Generate();
            output_buf[14] = DbPtr->Default[6];
            break;

        case CUSTOM56AC3_DM:
        case CUSTOM60AC_DM:
            output_buf[12] = 0x0c;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case NECAC5_DM:
        case NECAC8_DM:
        case NECAC_DM:
            output_buf[10] = 0x0b;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case NECAC4_DM:
            output_buf[10] = 0x01;
            output_buf[11] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[11] = DbPtr->Default[3];
            break;

        case NECAC9_DM:
            output_buf[10] = 0x05;
            output_buf[11] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[11] = DbPtr->Default[3];
            break;

        case NECAC10_DM:
        case NECAC6_DM:
            output_buf[10] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x05;
            Verify_data();
            IR_Generate();
            break;

        case LG28AC3_DM:
            output_buf[4] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[4] = present_mode;
            break;

        case COOLIX48AC_DM:
            output_buf[13] = 0x0d;
            output_buf[6] = 0x06;
            output_buf[7] = 0x07;
            output_buf[4] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            output_buf[6] = present_fan;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[4] = present_mode;
            break;

        case CUSTOM112AC_DM:
            output_buf[12] = 0x04;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[10] = 0x01;
            output_buf[13] = 0x01;
            output_buf[15] = 0x29;
            output_buf[14] = 0x28;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[14] = DbPtr->Default[6];
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[10] = 0x01;
            output_buf[13] = 0x01;
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM136AC2_DM:
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case CUSTOM136AC3_DM:
        case CUSTOM136AC4_DM:
        case CUSTOM152AC3_DM:
        case CUSTOM136AC_DM:
            output_buf[13] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM36AC_DM:
            output_buf[12] = 0x0b;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case CUSTOM32AC5_DM:
            if (DbPtr->Num == 51)
            {
                output_buf[12] = 0x05;
                output_buf[13] = 0x01;
                Verify_data();
                IR_Generate();
                output_buf[12] = DbPtr->Default[4];
                output_buf[13] = DbPtr->Default[5];
            }
            else
            {
                output_buf[12] = 0x0b;
                Verify_data();
                IR_Generate();
                output_buf[12] = DbPtr->Default[4];
            }
            break;

        case CUSTOM168AC_DM:
            output_buf[10] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM56AC5_DM:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

        case CUSTOM32AC4_DM:
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x00;
            output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        output_buf[3] = DbPtr->Data[POWER_ON];
        return TX_OK;
    }
}

unsigned char ac_mode_cool()
{
    if (DbPtr->Data[MODE_COOL] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_mode = 0;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[MODE_COOL];
        output_buf[4] = DbPtr->Data[MODE_COOL];
        present_mode = DbPtr->Data[MODE_COOL];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x60;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0a;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x06;
            output_buf[3] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case COOLIX48AC_DM:
            output_buf[6] = present_fan;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM64AC2_DM:
            output_buf[11] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x3e;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM152AC3_DM:
        case CUSTOM136AC3_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM136AC4_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[25] = DbPtr->Default[14];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM176AC_DM:
            output_buf[11] = DbPtr->Default[3];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM168AC_DM:
            output_buf[13] = DbPtr->Default[5];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}
unsigned char ac_mode_warm()
{
    if (DbPtr->Data[MODE_WARM] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_mode = 4;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[MODE_WARM];
        output_buf[4] = DbPtr->Data[MODE_WARM];
        present_mode = DbPtr->Data[MODE_WARM];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x60;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0a;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x06;
            output_buf[3] = 0x30;
            Verify_data();
            IR_Generate();
            break;

        case COOLIX48AC_DM:
            output_buf[6] = present_fan;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x3e;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM152AC3_DM:
        case CUSTOM136AC3_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM136AC4_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[25] = 0x02;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM176AC_DM:
            output_buf[11] = DbPtr->Default[3];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM168AC_DM:
            output_buf[13] = DbPtr->Default[5];
            Verify_data();
            IR_Generate();
            break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}
unsigned char ac_mode_dry()
{
    if (DbPtr->Data[MODE_DRY] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_mode = 8;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[MODE_DRY];
        output_buf[4] = DbPtr->Data[MODE_DRY];
        present_mode = DbPtr->Data[MODE_DRY];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x60;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0a;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x06;
            output_buf[3] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case COOLIX48AC_DM:
            output_buf[6] = 0x00;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM64AC2_DM:
            output_buf[11] = 0x00;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x3e;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM152AC3_DM:
        case CUSTOM136AC3_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM136AC4_DM:
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[25] = DbPtr->Default[14];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM176AC_DM:
            output_buf[11] = 0x00;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM168AC_DM:
            output_buf[13] = DbPtr->Default[5];
            Verify_data();
            IR_Generate();
            break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_mode_auto()
{
    if (DbPtr->Data[MODE_AUTO] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_mode = 12;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[MODE_AUTO];
        output_buf[4] = DbPtr->Data[MODE_AUTO];
        present_mode = DbPtr->Data[MODE_AUTO];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x60;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0a;
            output_buf[6] = 0x0a;
            output_buf[7] = 0xf0;
            Verify_data();
            IR_Generate();
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case CUSTOM88AC7_DM:
            output_buf[11] = 0x0a;
            output_buf[6] = 0x0a;
            output_buf[7] = 0xf3;
            Verify_data();
            IR_Generate();
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x06;
            output_buf[3] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case COOLIX48AC_DM:
            output_buf[6] = 0x00;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM64AC2_DM:
            output_buf[11] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x3e;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x06;
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM152AC3_DM:
        case CUSTOM136AC3_DM:
            output_buf[7] = 0xa8;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM136AC4_DM:
            output_buf[7] = 0xa8;
            output_buf[25] = DbPtr->Default[14];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM176AC_DM:
            output_buf[11] = DbPtr->Default[3];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM168AC_DM:
            output_buf[13] = 0x08;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_fan_low()
{
    if (DbPtr->Data[FAN_LOW] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_fan = 12;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[FAN_LOW];
        output_buf[6] = DbPtr->Data[FAN_LOW];
        present_fan = DbPtr->Data[FAN_LOW];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x29;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x11;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x03;
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_fan_mid()
{
    if (DbPtr->Data[FAN_MID] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_fan = 13;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[FAN_MID];
        output_buf[6] = DbPtr->Data[FAN_MID];
        present_fan = DbPtr->Data[FAN_MID];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x29;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x11;
            Verify_data();
            IR_Generate();
            break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_fan_high()
{
    if (DbPtr->Data[FAN_HIGH] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_fan = 14;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[FAN_HIGH];
        output_buf[6] = DbPtr->Data[FAN_HIGH];
        present_fan = DbPtr->Data[FAN_HIGH];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x29;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x11;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x03;
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_fan_auto()
{
    if (DbPtr->Data[FAN_AUTO] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        m_fan = 11;
        output_buf[1] = DbPtr->Data[m_mode + m_fan];

        output_buf[2] = DbPtr->Data[FAN_AUTO];
        output_buf[6] = DbPtr->Data[FAN_AUTO];
        present_fan = DbPtr->Data[FAN_AUTO];

        switch (DbPtr->DataForm)
        {
        case HAIER112AC_DM:
            output_buf[14] = 0x20;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x0c;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x29;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x11;
            output_buf[15] = 0x11;
            Verify_data();
            IR_Generate();
            break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_fan_turbo()
{
    if (DbPtr->Data[FAN_TURBO] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        output_buf[2] = DbPtr->Data[FAN_TURBO];
        output_buf[6] = DbPtr->Data[FAN_TURBO];
        Verify_data();
        IR_Generate();
        return TX_OK;
    }
}

unsigned char ac_swing_on()
{
    if (DbPtr->Data[SWING_ON] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        output_buf[2] = DbPtr->Data[SWING_ON];
        output_buf[5] = DbPtr->Data[SWING_ON];

        switch (DbPtr->DataForm)
        {
        case LG28AC_DM:
            output_buf[10] = 0x02;
            output_buf[4] = output_buf[7] = output_buf[6] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case LG28AC2_DM:
            output_buf[10] = 0x02;
            output_buf[4] = 0x0c;
            output_buf[7] = 0x08;
            output_buf[6] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case HAIER112AC_DM:
            output_buf[14] = 0x40;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x02;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x18;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case NECAC7_DM:
            output_buf[10] = 0x02;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x09;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC3_DM:
            output_buf[13] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM40AC5_DM:
            output_buf[12] = 0x0f;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case CUSTOM72AC2_DM:
        case CUSTOM72AC6_DM:
        case CUSTOM72AC_DM:
        case CUSTOM72AC5_DM:
            output_buf[13] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM72AC3_DM:
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case NECAC8_DM:
            output_buf[10] = 0x0d;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case NECAC3_DM:
            output_buf[10] = 0x03;
            Verify_data();
            IR_Generate();
            break;

        case NECAC10_DM:
        case NECAC6_DM:
            output_buf[10] = 0x0d;
            Verify_data();
            IR_Generate();
            break;

        case NECAC5_DM:
            if (DbPtr->Num == 61)
            {
                output_buf[10] = 0x0d;
                Verify_data();
                IR_Generate();
            }
            else
            {
                output_buf[10] = 0x0d;
                Verify_data();
                IR_Generate();
                output_buf[10] = DbPtr->Default[2];
            }
            break;

        case NECAC9_DM:
            output_buf[10] = 0x0b;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x02;
            Verify_data();
            IR_Generate();
            break;

        case LG28AC3_DM:
            output_buf[9] = 0x08;
            output_buf[4] = 0x00;
            output_buf[7] = 0x08;
            output_buf[6] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[9] = DbPtr->Default[1];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case COOLIX48AC_DM:
            output_buf[12] = 0x00;
            output_buf[13] = 0x0d;
            output_buf[6] = 0x06;
            output_buf[7] = 0x07;
            output_buf[4] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            output_buf[13] = DbPtr->Default[5];
            output_buf[6] = present_fan;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[4] = present_mode;
            break;

        case CUSTOM112AC_DM:
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x00;
            output_buf[15] = 0x38;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x07;
            output_buf[15] = 0x07;
            output_buf[10] = 0x20;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM136AC4_DM:
        case CUSTOM136AC_DM:
            output_buf[24] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[24] = DbPtr->Default[13];
            break;

        case CUSTOM152AC3_DM:
            output_buf[17] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[17] = DbPtr->Default[9];
            break;

        case CUSTOM32AC5_DM:
            if (DbPtr->Num == 51)
                output_buf[12] = 0x0b;
            else
                output_buf[12] = 0x0d;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x03;
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_swing_off()
{
    if (DbPtr->Data[SWING_OFF] == 0xFF)
        return NOT_USED_KEY;
    else
    {
        output_buf[2] = DbPtr->Data[SWING_OFF];
        output_buf[5] = DbPtr->Data[SWING_OFF];

        switch (DbPtr->DataForm)
        {
        case LG28AC_DM:
            output_buf[10] = 0x02;
            output_buf[4] = output_buf[7] = output_buf[6] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case LG28AC2_DM:
            output_buf[10] = 0x02;
            output_buf[4] = 0x0c;
            output_buf[7] = 0x08;
            output_buf[6] = 0x0a;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case HAIER112AC_DM:
            output_buf[14] = 0x40;
            Verify_data();
            IR_Generate();
            break;

        case HAIER72AC_DM:
            output_buf[9] = 0x0a;
            Verify_data();
            IR_Generate();
            break;

        case TCL112AC2_DM:
            output_buf[16] = 0x18;
            Verify_data();
            IR_Generate();
            break;

        /* Copy from Reiza */
        case NECAC7_DM:
            output_buf[10] = DbPtr->Default[2];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM88AC7_DM:
        case CUSTOM88AC6_DM:
            output_buf[11] = 0x09;
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC3_DM:
            output_buf[13] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM40AC5_DM:
            output_buf[12] = 0x0f;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            break;

        case CUSTOM72AC2_DM:
        case CUSTOM72AC6_DM:
        case CUSTOM72AC_DM:
        case CUSTOM72AC5_DM:
            output_buf[13] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[13] = DbPtr->Default[5];
            break;

        case CUSTOM72AC3_DM:
            output_buf[15] = 0x06;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case NECAC8_DM:
            output_buf[10] = 0x0d;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case NECAC9_DM:
            output_buf[10] = 0x09;
            Verify_data();
            IR_Generate();
            break;

        case NECAC5_DM:
            if (DbPtr->Num == 61)
            {
                output_buf[10] = 0x09;
                Verify_data();
                IR_Generate();
            }
            else
            {
                output_buf[10] = 0x0d;
                Verify_data();
                IR_Generate();
                output_buf[10] = DbPtr->Default[2];
            }
            break;

        case NECAC3_DM:
            output_buf[10] = 0x02;
            Verify_data();
            IR_Generate();
            break;

        case NECAC10_DM:
        case NECAC6_DM:
            output_buf[10] = 0x05;
            Verify_data();
            IR_Generate();
            break;

        case ELECTRA104AC_DM:
            output_buf[15] = 0x04;
            Verify_data();
            IR_Generate();
            break;

        case LG28AC3_DM:
            output_buf[9] = 0x08;
            output_buf[4] = 0x00;
            output_buf[7] = 0x08;
            output_buf[6] = 0x0a;
            Verify_data();
            IR_Generate();
            output_buf[9] = DbPtr->Default[1];
            output_buf[4] = present_mode;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[6] = present_fan;
            break;

        case COOLIX48AC_DM:
            output_buf[12] = 0x00;
            output_buf[13] = 0x0d;
            output_buf[6] = 0x06;
            output_buf[7] = 0x07;
            output_buf[4] = 0x00;
            Verify_data();
            IR_Generate();
            output_buf[12] = DbPtr->Default[4];
            output_buf[13] = DbPtr->Default[5];
            output_buf[6] = present_fan;
            output_buf[7] = DbPtr->Temp[present_temp];
            output_buf[4] = present_mode;
            break;

        case CUSTOM112AC_DM:
            output_buf[15] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[15] = DbPtr->Default[7];
            break;

        case WHIRLPOOL168AC_DM:
            output_buf[13] = 0x00;
            output_buf[15] = 0x38;
            Verify_data();
            IR_Generate();
            break;

        case WHIRLPOOL168AC2_DM:
            output_buf[13] = 0x07;
            output_buf[15] = 0x07;
            output_buf[10] = 0x20;
            Verify_data();
            IR_Generate();
            output_buf[10] = DbPtr->Default[2];
            break;

        case CUSTOM136AC4_DM:
        case CUSTOM136AC_DM:
            output_buf[24] = 0x01;
            Verify_data();
            IR_Generate();
            output_buf[24] = DbPtr->Default[13];
            break;

        case CUSTOM152AC3_DM:
            output_buf[17] = 0x02;
            Verify_data();
            IR_Generate();
            output_buf[17] = DbPtr->Default[9];
            break;

        case CUSTOM32AC5_DM:
            output_buf[12] = DbPtr->Default[4];
            Verify_data();
            IR_Generate();
            break;

        case CUSTOM40AC7_DM:
			output_buf[14] = 0x03;
			output_buf[15] = present_fan + (present_swing << 2) + (present_power << 3);
			Verify_data();
			IR_Generate();
			break;
            /* End of copy */

        default:
            Verify_data();
            IR_Generate();
            break;
        }

        return TX_OK;
    }
}

unsigned char ac_temp_up()
{
    if (DbPtr->Data[TEMP_UP] == 0xFF)
    {
        if (present_temp < 12)
        {
            present_temp++;
        }
        if (DbPtr->Temp[present_temp] == 0xFF)
            return NOT_USED_KEY;
        else
        {
            output_buf[7] = DbPtr->Temp[present_temp];
        }
    }
    else
    {
        output_buf[2] = DbPtr->Data[TEMP_UP]; //TEMP UP
    }

    switch (DbPtr->DataForm)
    {
    case HAIER112AC_DM:
        output_buf[14] = 0x00;
        Verify_data();
        IR_Generate();
        break;

    case HAIER72AC_DM:
        output_buf[9] = 0x06;
        Verify_data();
        IR_Generate();
        break;

    case TCL112AC2_DM:
        output_buf[16] = 0x14;
        Verify_data();
        IR_Generate();
        break;

    /* Copy from Reiza */
    case CUSTOM40AC6_DM:
        output_buf[13] = 0x01;
        Verify_data();
        IR_Generate();
        output_buf[13] = 0x00;
        break;

    case CUSTOM40AC7_DM:
        output_buf[10] = 0x0a;
        output_buf[4] = output_buf[14] = output_buf[15] = 0x0f;
        output_buf[13] = DbPtr->Temp[present_temp] & 0x0f;
        output_buf[6] = DbPtr->Temp[present_temp] >> 4;
        output_buf[5] = DbPtr->Temp[present_temp] >> 6;
        output_buf[3] = DbPtr->Temp[present_temp] >> 7;
        Verify_data();
        IR_Generate();
        output_buf[10] = DbPtr->Default[2];
        output_buf[13] = DbPtr->Default[5];
        output_buf[3] = present_power;
        output_buf[4] = present_mode;
        output_buf[5] = present_swing;
        output_buf[6] = present_fan;
        break;

    case CUSTOM88AC7_DM:
    case CUSTOM88AC6_DM:
        output_buf[11] = 0x0c;
        Verify_data();
        IR_Generate();
        break;

    case ELECTRA104AC_DM:
        output_buf[15] = 0x00;
        Verify_data();
        IR_Generate();
        break;

    case WHIRLPOOL168AC_DM:
        output_buf[13] = 0x02;
        output_buf[15] = 0x3a;
        Verify_data();
        IR_Generate();
        break;

    case WHIRLPOOL168AC2_DM:
        output_buf[13] = 0x02;
        output_buf[15] = 0x02;
        Verify_data();
        IR_Generate();
        break;
        /* End of copy */

    default:
        Verify_data();
        IR_Generate();
        break;
    }

    return TX_OK;
}

unsigned char ac_temp_down()
{
    if (DbPtr->Data[TEMP_DOWN] == 0xFF)
    {
        if (present_temp > 0)
        {
            present_temp--;
        }
        if (DbPtr->Temp[present_temp] == 0xFF)
            return NOT_USED_KEY;
        else
        {
            output_buf[7] = DbPtr->Temp[present_temp];
        }
    }
    else
    {
        output_buf[2] = DbPtr->Data[TEMP_DOWN]; //TEMP UP
    }

    switch (DbPtr->DataForm)
    {
    case HAIER112AC_DM:
        output_buf[14] = 0x80;
        Verify_data();
        IR_Generate();
        break;

    case HAIER72AC_DM:
        output_buf[9] = 0x0e;
        Verify_data();
        IR_Generate();
        break;

    case TCL112AC2_DM:
        output_buf[16] = 0x10;
        Verify_data();
        IR_Generate();
        break;

    /* Copy from Reiza */
    case CUSTOM40AC6_DM:
        output_buf[13] = 0x01;
        Verify_data();
        IR_Generate();
        output_buf[13] = 0x00;
        break;

    case CUSTOM40AC7_DM:
        output_buf[10] = 0x0a;
        output_buf[4] = output_buf[14] = output_buf[15] = 0x0f;
        output_buf[13] = DbPtr->Temp[present_temp] & 0x0f;
        output_buf[6] = DbPtr->Temp[present_temp] >> 4;
        output_buf[5] = DbPtr->Temp[present_temp] >> 6;
        output_buf[3] = DbPtr->Temp[present_temp] >> 7;
        Verify_data();
        IR_Generate();
        output_buf[10] = DbPtr->Default[2];
        output_buf[13] = DbPtr->Default[5];
        output_buf[3] = present_power;
        output_buf[4] = present_mode;
        output_buf[5] = present_swing;
        output_buf[6] = present_fan;
        break;

    case CUSTOM88AC7_DM:
    case CUSTOM88AC6_DM:
        output_buf[11] = 0x0d;
        Verify_data();
        IR_Generate();
        break;

    case ELECTRA104AC_DM:
        output_buf[15] = 0x01;
        Verify_data();
        IR_Generate();
        break;

    case WHIRLPOOL168AC_DM:
        output_buf[13] = 0x02;
        output_buf[15] = 0x3a;
        Verify_data();
        IR_Generate();
        break;

    case WHIRLPOOL168AC2_DM:
        output_buf[13] = 0x02;
        output_buf[15] = 0x02;
        Verify_data();
        IR_Generate();
        break;
        /* End of copy */

    default:
        Verify_data();
        IR_Generate();
        break;
    }

    return TX_OK;
}
