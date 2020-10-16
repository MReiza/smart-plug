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
unsigned char dType, dSize, dArray, db_data, mask, offset;

static AC_DB_FORM *DbPtr;
static WAVE_FORM *WavePtr;
static DATA_FORM *DataPtr;

static void Verify_data();
static void IR_Generate();
static inline void Pulse_Gen(unsigned char pulse, rmt_item32_t *item);
static void Init_Output_Buf();

void AC_Brand_Set(unsigned short Set_Num)
{
    unsigned short DbAccessCode = 0;
    do_verify = 0;
    present_temp = 6;
    m_mode = 0;
    m_fan = 14;

    rmt_set_num(Set_Num, &AC_DbForm[0].Num, acdb_size, sizeof(AC_DB_FORM), &DbAccessCode);
    DbPtr = &AC_DbForm[DbAccessCode];
    WavePtr = &AC_WaveForm[DbPtr->WaveForm];
    DataPtr = &AC_DataForm[DbPtr->DataForm];

    RMT.carrier_duty_ch[RMT_TX_CHANNEL].high = WavePtr->Freq.High * 10;
    RMT.carrier_duty_ch[RMT_TX_CHANNEL].low = WavePtr->Freq.Low * 10;
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
            present_power = output_buf[dArray];
            break;
        case 4:
            output_buf[dArray] = DbPtr->Data[MODE_COOL];
            present_mode = DbPtr->Data[MODE_COOL];
            break;
        case 5:
            output_buf[dArray] = DbPtr->Data[SWING_OFF];
            present_swing = output_buf[dArray];
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
        case 20:
			output_buf[dArray] = DbPtr->Keypress;
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

/* SUM ALL BYTE verify */
unsigned char sum_all_byte(unsigned char start_data, unsigned char end_data, bool lsb)
{
	unsigned char cnt = 0;
	unsigned char csize = 0;	  //checksum size
	while (start_data < end_data) /*end data = end of the data +1*/
	{
		dType = DataPtr->DataMap[start_data].Type;
		dArray = DataPtr->DataMap[start_data].Array;
		dSize = DataPtr->DataMap[start_data].Size;
		if (dType == DATA)
		{
			if (dArray == 18)
			{
				csize = DataPtr->DataMap[start_data].Size;
			}
			else
			{
				db_data = output_buf[dArray];
				while (dSize < 8)
				{
					offset = DataPtr->DataMap[start_data + 1].Size;
					mask = DataPtr->DataMap[start_data + 1].Array;
					db_data += (output_buf[mask] << dSize);
					start_data++;
					dSize += offset;
				}
				cnt += lsb ? reverse_bit(db_data, 8) : db_data;
				//ESP_LOGI(TAG, "%2X", db_data);
			}
		}
		start_data++;
	}
	return lsb ? reverse_bit(cnt, csize) : cnt;
}

/* SUM ALL BYTE with XOR verify */
unsigned char xsum_all_byte(unsigned char start_data, unsigned char end_data, unsigned char xor_num, bool lsb)
{
	unsigned char cnt = 0;
	unsigned char csize = 0;
	while (start_data < end_data) /*end data = end of the data +1*/
	{
		dType = DataPtr->DataMap[start_data].Type;
		dArray = DataPtr->DataMap[start_data].Array;
		dSize = DataPtr->DataMap[start_data].Size;
		if (dType == DATA || dType == BAR)
		{
			if (dArray == 18)
			{
				csize = DataPtr->DataMap[start_data].Size;
			}
			else
			{
				db_data = output_buf[dArray];

				while (dSize < 8)
				{
					offset = DataPtr->DataMap[start_data + 1].Size;
					mask = DataPtr->DataMap[start_data + 1].Array;
					db_data += (output_buf[mask] << dSize);
					start_data++;
					dSize += offset;
				}
				cnt += lsb ? reverse_bit(db_data, 8) : db_data;
				//ESP_LOGI(TAG, "%2X", db_data);
			}
		}
		start_data++;
	}
	return lsb ? (reverse_bit(cnt, csize)) ^ xor_num : cnt ^ xor_num;
}

/* SUM ALL NIBBLE verify */
unsigned char sum_all_nibble(unsigned char start_data, unsigned char end_data, bool lsb)
{
	unsigned char cnt = 0;
	unsigned char csize = 0;
	unsigned char db_data2 = 0;
	while (start_data < end_data) /*end data = end of the data +1*/
	{
		dType = DataPtr->DataMap[start_data].Type;
		dArray = DataPtr->DataMap[start_data].Array;
		dSize = DataPtr->DataMap[start_data].Size;
		if (dType == DATA)
		{
			if (dArray == 18)
			{
				csize = DataPtr->DataMap[start_data].Size;
			}
			else
			{
				db_data = output_buf[dArray];
				while (dSize % 4 != 0)
				{
					offset = DataPtr->DataMap[start_data + 1].Size;
					mask = DataPtr->DataMap[start_data + 1].Array;
					db_data += (output_buf[mask] << dSize);
					start_data++;
					dSize += offset;
				}

				if (dSize == 8)
				{
					if (lsb)
					{
						db_data2 = db_data & 0x0f;
						db_data = db_data >> 4;
					}
					else
					{
						db_data = (db_data >> 4) + (db_data & 0x0f);
					}
				}
				cnt += lsb ? reverse_bit(db_data, 4) + reverse_bit(db_data2, 4) : db_data;
				db_data2 = 0;
				//ESP_LOGI(TAG, "%2X", db_data);
			}
		}
		start_data++;
	}
	return lsb ? reverse_bit(cnt, csize) : cnt;
}

/* SUM ALL NIBBLES with XOR verify */
unsigned char xsum_all_nibble(unsigned char start_data, unsigned char end_data, unsigned char xor_num, bool lsb)
{
	unsigned char cnt = 0;
	unsigned char csize = 0;
	unsigned char db_data2 = 0;	  //for separating byte to nibble
	while (start_data < end_data) /*end data = end of the data +1*/
	{
		dType = DataPtr->DataMap[start_data].Type;
		dArray = DataPtr->DataMap[start_data].Array;
		dSize = DataPtr->DataMap[start_data].Size;
		if (dType == DATA)
		{
			if (dArray == 18)
			{
				csize = DataPtr->DataMap[start_data].Size;
			}
			else
			{
				db_data = output_buf[dArray];
				while (dSize % 4 != 0)
				{
					offset = DataPtr->DataMap[start_data + 1].Size;
					mask = DataPtr->DataMap[start_data + 1].Array;
					db_data += (output_buf[mask] << dSize);
					start_data++;
					dSize += offset;
				}
				if (dSize == 8)
				{
					if (lsb)
					{
						db_data2 = db_data & 0x0f;
						db_data = db_data >> 4;
					}
					else
					{
						db_data = (db_data >> 4) + (db_data & 0x0f);
					}
				}
				cnt += lsb ? reverse_bit(db_data, 4) + reverse_bit(db_data2, 4) : db_data;
				db_data2 = 0;
			}
		}
		start_data++;
	}
	return lsb ? reverse_bit(cnt, csize) ^ xor_num : cnt ^ xor_num;
}

/* COUNT '1' BIT verify */
unsigned char count_one_bit(unsigned start_data, unsigned end_data)
{
	unsigned char count = 0;
	while (start_data < end_data) /*end data = end of the data +1*/
	{
		dType = DataPtr->DataMap[start_data].Type;
		dArray = DataPtr->DataMap[start_data].Array;
		dSize = DataPtr->DataMap[start_data].Size;
		if (dType == DATA)
		{
			if (dArray != 18)
			{
				db_data = output_buf[dArray];
				while (0 < dSize)
				{
					count += db_data & 0x01;
					db_data >>= 1;
					dSize--;
				}
			}
		}
		start_data++;
	}
	return count;
}

/*GREE verify*/
unsigned char gree_ver()
{
	unsigned char cnt = 0;
	unsigned char dSeq = 1;
	unsigned char byte_cnt = 1;
	while (dSeq < 8)
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
					offset = DataPtr->DataMap[dSeq + 1].Size;
					mask = DataPtr->DataMap[dSeq + 1].Array;
					db_data += (output_buf[mask] << dSize);
					dSeq++;
					dSize += offset;
				}

				if (byte_cnt < 5)
					cnt += db_data & 0x0f;
				else if (4 < byte_cnt && byte_cnt < 8)
					cnt += db_data >> 4;
				byte_cnt++;
			}
		}
		dSeq++;
	}
	return 0xc + cnt;
}

void Verify_data()
{
if (do_verify)
	{ //if output frame has verify field
		switch (DbPtr->Verify)
		{
		case sob_v:
			if (DbPtr->DataForm == ROWA40AC_DM || DbPtr->DataForm == ROWA40AC_DM_2 || DbPtr->DataForm == CHANGLING40AC_DM)
				output_buf[18] = sum_all_byte(0, DATA_MAP_SIZE, false) + 0x03;
			else
				output_buf[18] = sum_all_byte(0, DATA_MAP_SIZE, false);
			break;

		case sob2_v1:
			output_buf[18] = sum_all_byte(2, DATA_MAP_SIZE, false); /*start from 2nd data*/
			break;

		case sob3_v1:
			output_buf[18] = sum_all_byte(3, DATA_MAP_SIZE, false); /*start from 3rd data*/
			break;

		case sob8_v1:
			output_buf[18] = sum_all_byte(8, DATA_MAP_SIZE, false); /*start from 8th data*/
			break;

		case sob9_v1:
			output_buf[18] = sum_all_byte(9, DATA_MAP_SIZE, false); /*start from 8th data*/
			break;

		case sob10_v1:
			output_buf[18] = sum_all_byte(10, DATA_MAP_SIZE, false);
			break;

		case sob11_v1:
			output_buf[18] = sum_all_byte(11, DATA_MAP_SIZE, false);
			break;

		case xsob_v:
			if (DbPtr->Num == 10993)
				output_buf[18] = (sum_all_byte(0, DATA_MAP_SIZE, false) + 4) ^ 0xff;
			else
				output_buf[18] = xsum_all_byte(0, DATA_MAP_SIZE, 0xff, false);
			break;

		case xsob19_v1:
			output_buf[18] = xsum_all_byte(0, 10, 0xff, false);
			break;

		case xsob111_v1:
			output_buf[18] = xsum_all_byte(0, DATA_MAP_SIZE, 0xff, false) + 1;
			break;

		case lsob_v1:
			output_buf[18] = sum_all_byte(0, DATA_MAP_SIZE, true);
			break;

		case son_v:
			if (DbPtr->DataForm == TCL128AC_DM || DbPtr->Num == 10954)
				output_buf[18] = sum_all_nibble(0, DATA_MAP_SIZE, false) + 0x2d;
			else
				output_buf[18] = sum_all_nibble(0, DATA_MAP_SIZE, false);
			break;

		case son3_v1:
			output_buf[18] = sum_all_nibble(3, DATA_MAP_SIZE, false); /*start from 3rd data*/
			break;

		case son4_v1:
			output_buf[18] = sum_all_nibble(4, DATA_MAP_SIZE, false); /*start from 4th data*/
			break;

		case son5_v1:
			output_buf[18] = sum_all_nibble(5, DATA_MAP_SIZE, false); /*start from 5th data*/
			break;

		case son6_v1:
			output_buf[18] = sum_all_nibble(6, DATA_MAP_SIZE, false); /*start from 6th data*/
			break;

		case son413_v1:
			output_buf[18] = sum_all_nibble(4, 14, false); /*start from 4th - 13rd data*/
			break;

		case lson5_v1:
			output_buf[18] = sum_all_nibble(5, DATA_MAP_SIZE, true); /*start from 5th data*/
			break;

		case xson_v:
			output_buf[18] = xsum_all_nibble(0, DATA_MAP_SIZE, 0xff, false);
			break;

		case lson_v1:
			output_buf[18] = sum_all_nibble(0, DATA_MAP_SIZE, true);
			break;
		
		case lson_v1i:
			output_buf[18] = ~(sum_all_nibble(0, DATA_MAP_SIZE, true));

		case xlson_v1:
			output_buf[18] = xsum_all_nibble(0, DATA_MAP_SIZE, 0xff, true);
			break;

		case c1_v:
			if (DbPtr->Num == 10313)
				output_buf[18] = reverse_bit(count_one_bit(0, DATA_MAP_SIZE), 4);
			else
				output_buf[18] = reverse_bit(count_one_bit(0, DATA_MAP_SIZE), 6);
			break;

		case c1_voe:
			if (count_one_bit(0, DATA_MAP_SIZE) % 2 == 0)
				output_buf[18] = 0x01;
			else
				output_buf[18] = 0x00;
			break;

		case c1_voe2:
			if (count_one_bit(0, DATA_MAP_SIZE) % 2 == 0)
				output_buf[18] = 0x00;
			else
				output_buf[18] = 0x01;
			break;

		case c1_vc:
			output_buf[18] = ~count_one_bit(0, DATA_MAP_SIZE);
			break;
		
		case c1_v1:
			output_buf[18] = reverse_bit(count_one_bit(0, DATA_MAP_SIZE), 8);	
			break; 

		case xc1_v:
			output_buf[18] = count_one_bit(0, DATA_MAP_SIZE) ^ 0xff;
			break;

		case gree_v:
			output_buf[18] = gree_ver();
			break;
		}
	}

    /* 2nd VERIFY */
    if (do_verify2)
    {
        switch (DbPtr->Verify1)
        {
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
    gpio_matrix_out(RMT_TX_EXTENDER_GPIO, RMT_TX_SIGNAL_IDX, false, false);
    rmt_write_items(RMT_TX_CHANNEL, item, item_num, true);
    free(item);
}

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
		case SHARP32AC_DM:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case HYUNDAI32AC_DM_1:
		case OTHER32AC_DM:
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM:
		case CORONA168AC_DM:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_2:
		case NEC48AC_DM_1:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case FERROLIT48AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case AUX32AC_DM:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0F;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case GREE32AC_DM:
		case GLEE32AC_DM:
		case FEILU32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x12;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x13;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x14;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x15;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x17;
			Verify_data();
			IR_Generate();
			break;

		case HAIER32AC_DM:
			output_buf[20] = 0x19;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x38;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x81;
			Verify_data();
			IR_Generate();
			break;

		case GOLDSTAR28AC_DM:
			output_buf[4] = present_mode - 0x01;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case HAIER56AC_DM:
			output_buf[4] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;
		
		case OTHER48AC_DM_4:
			output_buf[4] = 0x06;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case KTY00128AC_DM:
			output_buf[4] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case RAYBO40AC_DM_2:
			output_buf[5] = 0x03;
			output_buf[10] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[5] = present_swing;
			output_buf[10] = DbPtr->Default[2];
			break;

		case JINBEIJING32AC_DM:
		case NATIONAL26AC_DM:
			output_buf[8] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case MITSUBISHI86AC_DM_1:
			output_buf[8] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case OTHER40AC_DM:
		case XILENG40AC_DM:
		case HAIER40AC_DM:
		case MIDEA88AC_DM:
		case CHUNLAN40AC_DM:
		case GREE40AC_DM:
			output_buf[8] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case AUX140AC_DM:
			output_buf[9] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case JIALE64AC_DM:
			output_buf[9] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case TECOAC62_DM_3:
		case CHIGO56AC_DM_2:
		case MITSUBISHI48AC_DM_2:
		case ELBA48AC_DM:
		case HUABAO48AC_DM_1:
		case HUABAO48AC_DM:
		case HUABAO38AC_DM:
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case RAYBO40AC_DM_1:
		case XINLE32AC_DM:
		case MCQUAY140AC_DM:
			output_buf[10] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case MIDEA72AC_DM_4:
		case FEILU72AC_DM:
		case FUNAI72AC_DM:
		case MIDEA72AC_DM_3:
		case MIDEA72AC_DM_2:
		case MIDEA72AC_DM_1:
			output_buf[10] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SHINCO44AC_DM:
			output_buf[11] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			break;
		
		case SHARP63AC_DM_2:
		case SHARP63AC_DM_3:
			output_buf[11] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			break;

		case RAYBO64AC_DM:
			output_buf[11] = 0x01;
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			break;

		case NIKKO96AC_DM:
		case AUOMA96AC_DM_2:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case SHARP63AC_DM:
		case OTHER48AC_DM_2:
		case KELONG48AC_DM:
		case KELONG48AC_DM_1:
		case KELONG38AC_DM:
			output_buf[12] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case TECOAC62_DM_1:
			output_buf[12] = DbPtr->Default[4];
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case SHARP64AC_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case RAYBO40AC_DM:
			output_buf[12] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;
		
		case SHARP63AC_DM_1:
			output_buf[12] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case TCL112AC_DM_4:
			output_buf[13] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case AUCMA136AC_DM_1:
			output_buf[13] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case TEAC96AC_DM:
			output_buf[13] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case DONGXINBAO136AC_DM:
			output_buf[14] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;
		
		case HISENSE136AC_DM_4:
		case HISENSE136AC_DM_3:
		case HISENSE136AC_DM_2:
		case HISENSE136AC_DM_5:
		case SANYO136AC_DM_2:
		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
		case TADAIR136AC_DM_1:
		case AMCOR136AC_DM:
		case CHANGHONG136AC_DM_3:
		case CHANGHONG136AC_DM_2:
		case CHANGHONG136AC_DM_1:
		case CHANGHONG136AC_DM:
		case HISENSE136AC_DM_1:
		case HITACHI104AC_DM_1:
			output_buf[14] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case OTHER44AC_DM:
			output_buf[14] = 0x05;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case MIDEA72AC_DM:
		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[14] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case AUOMA136AC_DM_1:
			output_buf[14] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case WHIRLPOOL140AC_DM:
			output_buf[15] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case NEC96AC_DM:
		case NEC64AC_DM_1:
		case NEC64AC_DM_3:
		case SANYO64AC_DM_1:
		case NEC64AC_DM:
		case NEC104_AC_DM:
			output_buf[16] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
			output_buf[17] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
			output_buf[21] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case NEC72AC_DM_5:
			output_buf[22] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[22] = DbPtr->Default[11];
			break;

		case SANYO104_AC_DM_1:
			output_buf[23] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[23] = DbPtr->Default[12];
			break;

		case MEILING106AC_DM:
			output_buf[9] = 0x02;
			output_buf[12] = 0x01;
			output_buf[15] = 0x87;
			output_buf[16] = 0xfe;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			output_buf[12] = DbPtr->Default[4];
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
			break;

		case TRANE110AC_DM:
			output_buf[9] = 0x02;
			output_buf[11] = 0x02;
			output_buf[14] = 0x0e;
			output_buf[16] = 0x0e;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			output_buf[11] = DbPtr->Default[3];
			output_buf[14] = DbPtr->Default[6];
			output_buf[16] = DbPtr->Default[8];
			break;

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
		case SHARP32AC_DM:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case HYUNDAI32AC_DM_1:
		case OTHER32AC_DM:
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_2:
		case NEC48AC_DM_1:
		case HYUNDAI32AC_DM:
		case CORONA168AC_DM:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case AUX32AC_DM:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0F;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case GREE32AC_DM:
		case GLEE32AC_DM:
		case FEILU32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x12;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x13;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x14;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x15;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x17;
			Verify_data();
			IR_Generate();
			break;

		case HAIER32AC_DM:
			output_buf[20] = 0x19;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x38;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x81;
			Verify_data();
			IR_Generate();
			break;

		case OTHER48AC_DM_4:
		case KLIMATAIR48AC_DM:
		case HAIER64AC_DM:
		case CHANGFU56AC_DM_1:
		case SIGMA56AC_DM:
		case HUAWEI56AC_DM:
		case HAIER56AC_DM:
		case TONGLI23AC_DM:
		case CHANGFU112AC_DM:
		case CHANGLING48AC_DM_1:
			output_buf[4] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case SAMSUNG23AC_DM:
		case XINLE23AC_DM:
		case CHANGLING23AC_DM:
			output_buf[4] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case OTHER24AC_DM:
		case OTHER17AC_DM:
		case INYCIN17AC_DM:
		case WANBAO17AC_DM:
		case TEAC17AC_DM:
			output_buf[4] = 0x05;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			break;

		case HUAKE26AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			break;

		case FIRST36AC_DM:
		case MEILING32AC_DM:
			output_buf[5] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[5] = present_swing;
			break;

		case RAYBO40AC_DM_2:
			output_buf[5] = 0x03;
			output_buf[10] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[5] = present_swing;
			output_buf[10] = DbPtr->Default[2];
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			output_buf[4] = 0x00;
			output_buf[6] = 0x0E;
			output_buf[7] = 0x07;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case CHANGHONG48AC_DM_2:
		case CHANGHONG48AC_DM:
			output_buf[7] = 0x60;
			Verify_data();
			IR_Generate();
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case NEC48AC_DM_4:
		case NEC48AC_DM:
			output_buf[7] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = DbPtr->Keypress;
			break;

		case JINBEIJING32AC_DM:
		case NATIONAL26AC_DM:
			output_buf[8] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case MITSUBISHI86AC_DM_1:
			output_buf[8] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case OTHER40AC_DM:
		case XILENG40AC_DM:
		case HAIER40AC_DM:
		case MIDEA88AC_DM:
		case CHUNLAN40AC_DM:
			output_buf[8] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case GREE40AC_DM:
			output_buf[8] = 0x0b;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			break;

		case AUX140AC_DM:
			output_buf[9] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case JIALE64AC_DM:
			output_buf[9] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case TECOAC62_DM_3:
		case MITSUBISHI48AC_DM_2:
		case ELBA48AC_DM:
		case HUABAO48AC_DM_1:
		case HUABAO48AC_DM:
		case HUABAO38AC_DM:
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case RAYBO40AC_DM_1:
		case XINLE32AC_DM:
		case MCQUAY140AC_DM:
		case CHIGO56AC_DM_2:
			output_buf[10] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SHINCO44AC_DM:
			output_buf[11] = 0x01;
		case SHINCO44AC_DM_2:
			output_buf[4] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[11] = DbPtr->Default[3];
			break;

		case RAYBO64AC_DM:
			output_buf[11] = 0x01;
			output_buf[12] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			break;

		case NIKKO96AC_DM:
		case AUOMA96AC_DM_2:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case OTHER48AC_DM_2:
		case KELONG48AC_DM:
		case KELONG48AC_DM_1:
		case KELONG38AC_DM:
			output_buf[12] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case SHARP64AC_DM_1:
			output_buf[12] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case SHARP63AC_DM:
		case RAYBO40AC_DM:
			output_buf[12] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case TECOAC62_DM_1:
			output_buf[12] = DbPtr->Default[4];
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case TCL112AC_DM_4:
			output_buf[13] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case AUCMA136AC_DM_1:
			output_buf[13] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case TEAC96AC_DM:
			output_buf[13] = 0x05;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case HISENSE136AC_DM_4:
		case HISENSE136AC_DM_3:
		case HISENSE136AC_DM_2:
		case HISENSE136AC_DM_5:
		case OTHER44AC_DM:
		case SANYO136AC_DM_2:
		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
		case TADAIR136AC_DM_1:
		case AMCOR136AC_DM:
		case DONGXINBAO136AC_DM:
		case CHANGHONG136AC_DM_3:
		case CHANGHONG136AC_DM_2:
		case CHANGHONG136AC_DM_1:
		case CHANGHONG136AC_DM:
		case HISENSE136AC_DM_1:
			output_buf[14] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case HITACHI104AC_DM_1:
			output_buf[14] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case MIDEA72AC_DM:
			output_buf[14] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[14] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case AUOMA136AC_DM_1:
			output_buf[14] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
			output_buf[15] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case WHIRLPOOL140AC_DM:
			output_buf[15] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case NEC96AC_DM:
		case NEC64AC_DM_1:
		case NEC64AC_DM_3:
		case SANYO64AC_DM_1:
		case NEC64AC_DM:
		case NEC104_AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
			output_buf[17] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

		case MIDEA72AC_DM_4:
			output_buf[17] = 0x00;
		case FEILU72AC_DM:
		case FUNAI72AC_DM:
		case MIDEA72AC_DM_3:
		case MIDEA72AC_DM_2:
		case MIDEA72AC_DM_1:
			output_buf[10] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			output_buf[17] = DbPtr->Default[9];
			break;

		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
			output_buf[21] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case NEC72AC_DM_5:
			output_buf[22] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[22] = DbPtr->Default[11];
			break;

		case SANYO104_AC_DM_1:
			output_buf[23] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[23] = DbPtr->Default[12];
			break;

		case GOLDSTAR28AC_DM:
			output_buf[10] = 0x03;
			output_buf[4] = 0x00;
			output_buf[7] = 0x00;
			output_buf[6] = DbPtr->Data[FAN_AUTO];
			output_buf[18] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			output_buf[4] = present_mode;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[6] = present_fan;
			break;

		case KTY00128AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x0a;
			output_buf[7] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case MEILING106AC_DM:
			output_buf[9] = 0x02;
			output_buf[12] = 0x01;
			output_buf[15] = 0x87;
			output_buf[16] = 0xfe;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			output_buf[12] = DbPtr->Default[4];
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
			break;

		case TRANE110AC_DM:
			output_buf[9] = 0x02;
			output_buf[11] = 0x02;
			output_buf[14] = 0x0e;
			output_buf[16] = 0x0e;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			output_buf[11] = DbPtr->Default[3];
			output_buf[14] = DbPtr->Default[6];
			output_buf[16] = DbPtr->Default[8];
			break;

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

		case TECOAC62_DM_3:
			output_buf[20] = DbPtr->Keypress;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x41;
			Verify_data();
			IR_Generate();
			break;

		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case OTHER80AC_DM:
		case XILENG80AC_DM:
		case YUTU80AC_DM_2:
			output_buf[7] = DbPtr->Temp[present_temp];
		case AUOMA80AC_DM_1:
		case DELONGHI71AC_DM:
		case RHOSS72AC_DM:
		case YUTU80AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case HUABAO72AC_DM:
		case HUABAO70AC_DM:
		case SOVA72AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case OTHER32AC_DM:
		case FERROLIT48AC_DM:
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case JINDA96AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[12] = DbPtr->Default[4];
		case NATIONAL32AC_DM_1:
		case FEILU32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[13] = DbPtr->Default[5];
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case DAIKIN176AC_DM_1:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case HAIER32AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x0C;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case AUX32AC_DM:
			output_buf[20] = 0x12;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
			output_buf[20] = 0x1B;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1e;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0xb0;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x8f;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x91;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU112AC_DM:
		case OTHER56AC_DM:
			output_buf[3] = present_power;
			Verify_data();
			IR_Generate();
			break;

		case TRANE32AC_DM_1:
			output_buf[5] = present_swing;
			Verify_data();
			IR_Generate();
			break;

		case RAYBO64AC_DM:
			output_buf[6] = DbPtr->Data[MODE_COOL];
			Verify_data();
			IR_Generate();
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			output_buf[7] = DbPtr->Temp[present_temp];
		case ELECTROLUX40AC_DM_1:
			output_buf[6] = present_fan;
			Verify_data();
			IR_Generate();
			break;

		case MEILING44AC_DM:
		case SAJODENKI44AC_DM_1:
			output_buf[6] = present_fan;
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE136AC_DM_4:
			output_buf[21] = DbPtr->Default[10];
		case HISENSE136AC_DM_5:
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
		case SHARP63AC_DM_2:
		case SHARP63AC_DM_3:
		case SHARP63AC_DM_1:
		case SHARP63AC_DM:
		case GOLDSTAR28AC_DM:
		case HISENSE136AC_DM_1:
		case GREE68AC_DM_1:
		case GREE43AC_DM:
		case AKIRA35AC_DM:
		case GREE35AC_DM:
		case TEAC96AC_DM:
		case GREE67AC_DM_3:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
			output_buf[8] = DbPtr->Default[0];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_2:
			output_buf[9] = DbPtr->Default[1];
			output_buf[10] = DbPtr->Default[2];
			Verify_data();
			IR_Generate();
			break;

		case RAYBO40AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
		case CHANGHONG72AC_DM_2:
		case MITSUBISHI48AC_DM_3:
		case CHANGHONG48AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case NEC96AC_DM:
		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
		case NEC72AC_DM_1:
		case NEC72AC_DM_2:
		case SANYO72AC_DM_1:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case SANYO64AC_DM_1:
		case NEC48AC_DM_4:
		case NEC48AC_DM_2:
		case NEC48AC_DM_1:
		case SANYO104_AC_DM_1:
		case NEC104_AC_DM:
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case OTHER112AC_DM_1:
		case HUABAO56AC_DM:
		case CHANGLING46AC_DM:
			output_buf[13] = DbPtr->Default[5];
			Verify_data();
			IR_Generate();
			break;

		case KELONG48AC_DM:
		case AUCMA136AC_DM_1:
		case MITSUBISHI86AC_DM_1:
		case NISO56AC_DM:
			output_buf[14] = DbPtr->Default[6];
			Verify_data();
			IR_Generate();
			break;

		case AUOMA136AC_DM_1:
			output_buf[16] = 0x07;
			output_buf[17] = 0x0f;
		case HISENSE136AC_DM_3:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC216AC_DM_1:
		case PANASONIC216AC_DM:
		case NATIONAL216AC_DM_1:
		case NATIONAL216AC_DM:
			output_buf[15] = DbPtr->Default[7];
		case SHARP32AC_DM:
		case JINBEIJING32AC_DM:
		case KTY00128AC_DM:
		case HUAKE26AC_DM:
		case OTHER24AC_DM:
		case TONGLI23AC_DM:
		case INYCIN17AC_DM:
		case WANBAO17AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
		case SANYO136AC_DM_2:
		case TADAIR136AC_DM_1:
			output_buf[16] = DbPtr->Default[8];
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
			output_buf[16] = DbPtr->Default[8];
			output_buf[17] = DbPtr->Default[9];
			output_buf[21] = DbPtr->Default[10];
			Verify_data();
			IR_Generate();
			break;

		case AMCOR136AC_DM:
			output_buf[17] = DbPtr->Default[9];
			Verify_data();
			IR_Generate();
			break;

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

		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case OTHER80AC_DM:
		case XILENG80AC_DM:
		case YUTU80AC_DM_2:
			output_buf[7] = DbPtr->Temp[present_temp];
		case AUOMA80AC_DM_1:
		case DELONGHI71AC_DM:
		case RHOSS72AC_DM:
		case YUTU80AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x41;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case HUABAO72AC_DM:
		case HUABAO70AC_DM:
		case SOVA72AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case FERROLIT48AC_DM:
		case GREE48AC_DM_2:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case JINDA96AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[12] = DbPtr->Default[4];
		case NATIONAL32AC_DM_1:
		case OTHER32AC_DM:
		case FEILU32AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[13] = DbPtr->Default[5];
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case DAIKIN176AC_DM_1:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case TCL112AC_DM_3:
		case HAIER32AC_DM:
			output_buf[20] = 0x8;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x0C;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case CHIGO96AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case AUX32AC_DM:
		case QIXING32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1e;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0xb0;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x91;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x92;
			Verify_data();
			IR_Generate();
			break;

		case OTHER56AC_DM:
			output_buf[3] = present_power;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU112AC_DM:
			output_buf[3] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case TRANE32AC_DM_1:
			if (present_swing == DbPtr->Data[SWING_ON])
				output_buf[5] = ~DbPtr->Data[SWING_OFF];
			else
				output_buf[5] = ~DbPtr->Data[SWING_ON];
			Verify_data();
			IR_Generate();
			break;

		case RAYBO64AC_DM:
			output_buf[6] = DbPtr->Data[MODE_WARM];
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_5:
			output_buf[15] = 0x06;
			output_buf[16] = 0x0d;
		case SHARP63AC_DM_2:
		case SHARP63AC_DM_3:
		case SHARP63AC_DM_1:
		case SHARP63AC_DM:
		case GOLDSTAR28AC_DM:
		case HISENSE136AC_DM_1:
		case GREE68AC_DM_1:
		case GREE43AC_DM:
		case AKIRA35AC_DM:
		case GREE35AC_DM:
		case TEAC96AC_DM:
		case GREE67AC_DM_3:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
			output_buf[8] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_1:
			output_buf[9] = 0x47;
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_2:
			output_buf[9] = 0x07;
			output_buf[10] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case RAYBO40AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG72AC_DM_2:
			if (DbPtr->Num == 10079)
				output_buf[10] = 0x07;
			else
				output_buf[10] = 0x0c;
			output_buf[11] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
		case MITSUBISHI48AC_DM_3:
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG48AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case NEC96AC_DM:
			output_buf[11] = 0x02;
			output_buf[12] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case SANYO104_AC_DM_1:
		case NEC104_AC_DM:
			output_buf[11] = 0x05;
			output_buf[12] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_2:
			output_buf[11] = 0x09;
			output_buf[12] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case SANYO64AC_DM_1:
			output_buf[11] = 0x08;
			output_buf[12] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case NEC72AC_DM_3:
		case NEC64AC_DM_4:
			if (DbPtr->Num == 10525 || DbPtr->Num == 10527)
			{
				output_buf[11] = 0x04;
				output_buf[12] = 0x0c;
			}
			else if (DbPtr->Num == 10533)
			{
				output_buf[11] = 0x0d;
				output_buf[12] = 0x06;
			}
			else
			{
				output_buf[11] = 0x01;
				output_buf[12] = 0x0c;
			}
			Verify_data();
			IR_Generate();
			break;

		case SANYO72AC_DM_1:
		case NEC72AC_DM_2:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
			if (DbPtr->Num == 10539)
			{
				output_buf[11] = DbPtr->Default[3];
				output_buf[12] = DbPtr->Default[4];
			}
			else
			{
				output_buf[11] = 0x0c;
				output_buf[12] = 0x04;
			}
			Verify_data();
			IR_Generate();
			break;

		case NEC72AC_DM_1:
			output_buf[11] = 0x07;
			output_buf[12] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
			if (DbPtr->Num == 10535 || DbPtr->Num == 10538)
			{
				output_buf[11] = DbPtr->Default[3];
				output_buf[12] = DbPtr->Default[4];
			}
			else
			{
				output_buf[11] = 0x02;
				output_buf[12] = 0x08;
			}
			Verify_data();
			IR_Generate();
			break;

		case SANYO72AC_DM_3:
			if (DbPtr->Num == 10526)
			{
				output_buf[11] = 0x04;
				output_buf[12] = 0x0C;
			}
			else if (DbPtr->Num == 10529)
			{
				output_buf[11] = 0x01;
			}
			else if (DbPtr->Num == 10542)
			{
				output_buf[11] = DbPtr->Default[3];
				output_buf[12] = DbPtr->Default[4];
			}
			else
			{
				output_buf[11] = 0x01;
				output_buf[12] = 0x04;
			}

			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_4:
		case NEC48AC_DM_1:
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case MEILING44AC_DM:
		case SAJODENKI44AC_DM_1:
			output_buf[12] = DbPtr->Default[4];
		case MIDEA48AC_DM_1:
		case TOSHIBA48AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case ELECTROLUX40AC_DM_1:
			output_buf[6] = present_fan;
			Verify_data();
			IR_Generate();
			break;

		case CHANGLING46AC_DM:
			output_buf[13] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case OTHER112AC_DM_1:
		case HUABAO56AC_DM:
			output_buf[13] = DbPtr->Default[5];
			Verify_data();
			IR_Generate();
			break;

		case KELONG48AC_DM:
		case AUCMA136AC_DM_1:
		case MITSUBISHI86AC_DM_1:
		case NISO56AC_DM:
			output_buf[14] = DbPtr->Default[6];
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC216AC_DM_1:
		case PANASONIC216AC_DM:
		case NATIONAL216AC_DM_1:
		case NATIONAL216AC_DM:
			output_buf[15] = DbPtr->Default[7];
		case SHARP32AC_DM:
		case JINBEIJING32AC_DM:
		case KTY00128AC_DM:
		case HUAKE26AC_DM:
		case OTHER24AC_DM:
		case INYCIN17AC_DM:
		case WANBAO17AC_DM:
		case TONGLI23AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM_2:
			output_buf[15] = 0x06;
			output_buf[16] = 0x0d;
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_4:
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = 0x00;
			output_buf[21] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case AUOMA136AC_DM_1:
			output_buf[16] = 0x06;
			output_buf[17] = 0x0d;
		case HISENSE136AC_DM_3:
		case CHANGHONG136AC_DM:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case TADAIR136AC_DM_1:
		case SANYO136AC_DM_2:
			output_buf[16] = DbPtr->Default[8];
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
			output_buf[22] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[22] = DbPtr->Default[11];
			break;

		case AMCOR136AC_DM:
			output_buf[17] = DbPtr->Default[9];
			Verify_data();
			IR_Generate();
			break;

		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
			output_buf[16] = DbPtr->Default[8];
			output_buf[17] = 0x06;
			output_buf[21] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

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
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[20] = 0x41;
			Verify_data();
			IR_Generate();
			break;

		case HUABAO70AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM:
		case FERROLIT48AC_DM:
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case JINDA96AC_DM:
			output_buf[7] = 0x2b;
			output_buf[12] = 0x02;
		case NATIONAL32AC_DM_1:
		case FEILU32AC_DM:
		case GREE48AC_DM_2:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[13] = DbPtr->Default[5];
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case DAIKIN176AC_DM_1:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case OTHER32AC_DM_2:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HAIER32AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
			output_buf[7] = 0x03;
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x0C;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
		case AUX32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1e;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x80;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x89;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0xb0;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU112AC_DM:
			output_buf[3] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case OTHER56AC_DM:
			output_buf[3] = DbPtr->Data[POWER_OFF];
			Verify_data();
			IR_Generate();
			break;

		case TRANE32AC_DM_1:
			output_buf[5] = present_swing;
			Verify_data();
			IR_Generate();
			break;

		case RAYBO64AC_DM:
			output_buf[6] = DbPtr->Data[MODE_DRY];
			Verify_data();
			IR_Generate();
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[6] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case HISENSE136AC_DM_4:
			output_buf[21] = DbPtr->Default[10];
		case HISENSE136AC_DM_5:
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
		case SHARP63AC_DM_2:
		case SHARP63AC_DM:
		case GOLDSTAR28AC_DM:
		case HISENSE136AC_DM_1:
		case GREE68AC_DM_1:
		case GREE43AC_DM:
		case AKIRA35AC_DM:
		case GREE35AC_DM:
		case TEAC96AC_DM:
		case GREE67AC_DM_3:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case HUAKE26AC_DM:
		case TONGLI23AC_DM:
			output_buf[7] = 0x00;
			Verify_data();
			IR_Generate();
			break;
		
		case SHARP63AC_DM_3:
		case SHARP63AC_DM_1:
			output_buf[7] = DbPtr->Temp[0];
			Verify_data();
			IR_Generate();
			break;

		case SOVA72AC_DM:
		case HUABAO72AC_DM:
			output_buf[7] = 0x0e;
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case OTHER80AC_DM:
		case XILENG80AC_DM:
		case YUTU80AC_DM_2:
			output_buf[7] = 0x03;
		case AUOMA80AC_DM_1:
		case DELONGHI71AC_DM:
		case RHOSS72AC_DM:
		case YUTU80AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
			output_buf[8] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case RAYBO40AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
		case HISENSE72AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
		case CHANGHONG72AC_DM_2:
		case CHANGHONG48AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case MITSUBISHI48AC_DM_3:
			output_buf[10] = 0x3c;
			output_buf[11] = 0xc3;
			Verify_data();
			IR_Generate();
			break;

		case NEC96AC_DM:
		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
		case NEC72AC_DM_1:
		case NEC72AC_DM_2:
		case SANYO72AC_DM_1:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case SANYO64AC_DM_1:
		case NEC48AC_DM_4:
		case NEC48AC_DM_2:
		case NEC48AC_DM_1:
		case SANYO104_AC_DM_1:
		case NEC104_AC_DM:
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case SAJODENKI44AC_DM_1:
		case MEILING44AC_DM:
			output_buf[12] = DbPtr->Default[4];
		case ELECTROLUX40AC_DM_1:
			output_buf[6] = present_fan;
			Verify_data();
			IR_Generate();
			break;

		case OTHER112AC_DM_1:
		case CHANGLING46AC_DM:
			output_buf[13] = DbPtr->Default[5];
			Verify_data();
			IR_Generate();
			break;
		
		case HUABAO56AC_DM:
			output_buf[13] = 0x07;
			Verify_data();
			IR_Generate();
			break;

		case NISO56AC_DM:
			output_buf[14] = 0x07;
			Verify_data();
			IR_Generate();
			break;

		case AUCMA136AC_DM_1:
		case KELONG48AC_DM:
		case MITSUBISHI86AC_DM_1:
			output_buf[14] = DbPtr->Default[6];
			Verify_data();
			IR_Generate();
			break;

		case AUOMA136AC_DM_1:
			output_buf[16] = 0x07;
			output_buf[17] = 0x0f;
		case HISENSE136AC_DM_3:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC216AC_DM_1:
		case PANASONIC216AC_DM:
		case NATIONAL216AC_DM_1:
		case NATIONAL216AC_DM:
			output_buf[15] = DbPtr->Default[7];
		case SHARP32AC_DM:
		case JINBEIJING32AC_DM:
		case KTY00128AC_DM:
		case OTHER24AC_DM:
		case INYCIN17AC_DM:
		case WANBAO17AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
		case TADAIR136AC_DM_1:
		case SANYO136AC_DM_2:
			output_buf[16] = DbPtr->Default[8];
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
			Verify_data();
			IR_Generate();
			break;

		case AMCOR136AC_DM:
			output_buf[17] = DbPtr->Default[9];
			Verify_data();
			IR_Generate();
			break;

		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
			output_buf[16] = DbPtr->Default[8];
			output_buf[17] = DbPtr->Default[9];
			output_buf[21] = DbPtr->Default[10];
			Verify_data();
			IR_Generate();
			break;

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
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[7] = 0x00;
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[7] = 0x04;
			output_buf[20] = 0x41;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_3:
			output_buf[20] = DbPtr->Keypress;
			Verify_data();
			IR_Generate();
			break;

		case SOVA72AC_DM:
		case HUABAO72AC_DM:
		case HUABAO70AC_DM:
			output_buf[7] = 0x0e;
		case GREE48AC_DM_2:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM:
		case FERROLIT48AC_DM:
		case CORONA96AC_DM:
		case ELECTROLUX96AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case JINDA96AC_DM:
			output_buf[7] = 0x2b;
			output_buf[12] = 0x02;
		case NATIONAL32AC_DM_1:
		case FEILU32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
			output_buf[7] = 0x3f;
			output_buf[13] = 0x03;
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case DAIKIN176AC_DM_1:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case ELECTROLUX104AC_DM_1:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case HAIER32AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
			output_buf[7] = 0x09;
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x0C;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case AUX32AC_DM:
			output_buf[20] = 0x12;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
			output_buf[20] = 0x1B;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1e;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x8f;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x91;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0xb0;
			Verify_data();
			IR_Generate();
			break;

		case OTHER56AC_DM:
			output_buf[3] = present_power;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU112AC_DM:
			output_buf[3] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case TRANE32AC_DM_1:
			if (present_swing == DbPtr->Data[SWING_ON])
				output_buf[5] = ~DbPtr->Data[SWING_OFF];
			else
				output_buf[5] = ~DbPtr->Data[SWING_ON];
			Verify_data();
			IR_Generate();
			break;

		case RAYBO64AC_DM:
			output_buf[6] = DbPtr->Data[MODE_AUTO];
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX40AC_DM_1:
			output_buf[6] = DbPtr->Data[FAN_HIGH];
			Verify_data();
			IR_Generate();
			break;

		case MEILING44AC_DM:
		case SAJODENKI44AC_DM_1:
			output_buf[6] = DbPtr->Data[FAN_AUTO];
			output_buf[12] = 0x40;
			Verify_data();
			IR_Generate();
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			if (DbPtr->Num == 10056)
				output_buf[7] = DbPtr->Temp[present_temp];
			else 
				output_buf[7] = DbPtr->Temp[10];
			output_buf[6] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case SHARP32AC_DM:
		case JINBEIJING32AC_DM:
		case GREE68AC_DM_1:
		case GREE43AC_DM:
		case AKIRA35AC_DM:
		case GREE35AC_DM:
		case GREE67AC_DM_3:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[7] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case KTY00128AC_DM:
		case GOLDSTAR28AC_DM:
			output_buf[7] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case SHARP63AC_DM_2:
		case HUAKE26AC_DM:
		case TEAC96AC_DM:
			output_buf[7] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case OTHER24AC_DM:
		case INYCIN17AC_DM:
		case WANBAO17AC_DM:
			output_buf[7] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case SHARP63AC_DM_3:
		case SHARP63AC_DM_1:
		case SHARP63AC_DM:
			output_buf[7] = 0x1f;
			Verify_data();
			IR_Generate();
			break;

		case HISENSE136AC_DM_5:
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
		case HISENSE136AC_DM_1:
			output_buf[7] = 0xa8;
			Verify_data();
			IR_Generate();
			break;

		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
		case AUOMA80AC_DM_1:
		case DELONGHI71AC_DM:
		case RHOSS72AC_DM:
		case OTHER80AC_DM:
		case XILENG80AC_DM:
		case YUTU80AC_DM:
		case YUTU80AC_DM_2:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case TONGLI23AC_DM:
			output_buf[7] = DbPtr->Temp[present_temp];
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
			output_buf[8] = DbPtr->Default[0];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE72AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
		case HISENSE72AC_DM_1:
			output_buf[9] = DbPtr->Default[1];
			Verify_data();
			IR_Generate();
			break;

		case RAYBO40AC_DM_1:
			output_buf[9] = 0x00;
			output_buf[11] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG48AC_DM_2:
			output_buf[10] = 0x09;
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
			output_buf[10] = 0x01;
			output_buf[11] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case MITSUBISHI48AC_DM_3:
		case CHANGHONG72AC_DM_2:
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			Verify_data();
			IR_Generate();
			break;

		case NEC96AC_DM:
		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
		case NEC72AC_DM_1:
		case NEC72AC_DM_2:
		case SANYO72AC_DM_1:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case SANYO64AC_DM_1:
		case SANYO104_AC_DM_1:
		case NEC104_AC_DM:
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_4:
		case NEC48AC_DM_1:
			output_buf[11] = 0x09;
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_2:
			output_buf[11] = 0x09;
			output_buf[12] = 0x07;
			Verify_data();
			IR_Generate();
			break;

		case HUABAO56AC_DM:
		case CHANGLING46AC_DM:
			output_buf[13] = DbPtr->Default[5];
			Verify_data();
			IR_Generate();
			break;
		
		case OTHER112AC_DM_1:
			output_buf[13] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case NISO56AC_DM:
			output_buf[14] = DbPtr->Default[6];
			Verify_data();
			IR_Generate();
			break;

		case AUCMA136AC_DM_1:
			output_buf[14] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case KELONG48AC_DM:
			output_buf[14] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case MITSUBISHI86AC_DM_1:
			output_buf[14] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case AUOMA136AC_DM_1:
			output_buf[16] = 0x07;
			output_buf[17] = 0x0f;
		case HISENSE136AC_DM_3:
		case CHANGHONG136AC_DM:
			output_buf[15] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC216AC_DM_1:
		case PANASONIC216AC_DM:
		case NATIONAL216AC_DM_1:
		case NATIONAL216AC_DM:
			output_buf[15] = 0x03;
			output_buf[7] = 0x20;
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG136AC_DM_2:
			output_buf[15] = DbPtr->Default[7];
			output_buf[16] = DbPtr->Default[8];
			Verify_data();
			IR_Generate();
			break;
		
		case HISENSE136AC_DM_4:
			output_buf[15] = 0x01;
			output_buf[16] = DbPtr->Default[8];
			output_buf[21] = DbPtr->Default[10];
			Verify_data();
			IR_Generate();
			break;

		case HISENSE136AC_DM_2:
			output_buf[15] = 0x01;
			output_buf[22] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case SANYO136AC_DM_2:
		case TADAIR136AC_DM_1:
			output_buf[16] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case AMCOR136AC_DM:
			output_buf[17] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
			output_buf[16] = 0x02;
			output_buf[17] = DbPtr->Default[9];
			output_buf[21] = DbPtr->Default[10];
			Verify_data();
			IR_Generate();
			break;

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

		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case HAIER32AC_DM:
		case AUX32AC_DM:
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case CORONA96AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case BLUESTAR32AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
		case YUTU84AC_DM:
		case OTHER32AC_DM:
		case AUOMA84AC_DM_1:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case CHIGO96AC_DM:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x16;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x1b;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x30;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x42;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x81;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x99;
			Verify_data();
			IR_Generate();
			break;
		
		case SHARP52AC_DM:
			output_buf[4] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case BOYIN81AC_DM:
			output_buf[16] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

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
		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case HAIER32AC_DM:
		case AUX32AC_DM:
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case CORONA96AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case BLUESTAR32AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case OTHER32AC_DM:
		case AUOMA84AC_DM_1:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x16;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x1b;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x30;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x42;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x82;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x99;
			Verify_data();
			IR_Generate();
			break;
		
		case SHARP52AC_DM:
			output_buf[4] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case BOYIN81AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

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
		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case HAIER32AC_DM:
		case AUX32AC_DM:
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case CORONA96AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case BLUESTAR32AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case OTHER32AC_DM:
		case AUOMA84AC_DM_1:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x16;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x1b;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x30;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x42;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x83;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x99;
			Verify_data();
			IR_Generate();
			break;
		
		case SHARP52AC_DM:
			output_buf[4] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case BOYIN81AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

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
		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO32AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case HAIER32AC_DM:
		case AUX32AC_DM:
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case CORONA96AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case BLUESTAR32AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
		case YUTU84AC_DM:
		case OTHER32AC_DM:
		case AUOMA84AC_DM_1:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
		case OTHER32AC_DM_2:
		case CHIGO96AC_DM:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case DELONGHI71AC_DM:
		case TCL112AC_DM_3:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x16;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x1b;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x30;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x42;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x99;
			Verify_data();
			IR_Generate();
			break;
		
		case SHARP52AC_DM:
			output_buf[4] = 0x04;
			Verify_data();
			IR_Generate();
			break;
			
		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case BOYIN81AC_DM:
			output_buf[16] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

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
        present_swing = DbPtr->Data[SWING_ON];

        switch (DbPtr->DataForm)
		{
		case HISENSE136AC_DM_4:
		case HISENSE136AC_DM_3:
		case HISENSE136AC_DM_2:
		case HISENSE136AC_DM_5:
		case HISENSE136AC_DM_1:
		case AUOMA136AC_DM_1:
			output_buf[20] = DbPtr->Data[SWING_ON];
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case GREE67AC_DM_3:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case GREE68AC_DM_1:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case OTHER32AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;
		
		case HUALING112AC_DM_1:
			output_buf[20] = DbPtr->Keypress;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
		case CORONA96AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case AUOMA84AC_DM_1:
		case FERROLIT48AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case RHOSS72AC_DM:
		case HYUNDAI32AC_DM_1:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case DELONGHI71AC_DM:
			output_buf[20] = 0x07;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case AUCMA136AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case AUX32AC_DM:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case QIXING32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x14;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x18;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1f;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x81;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x88;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x9d;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
			output_buf[3] = 0x00;
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[3] = present_power;
			output_buf[16] = DbPtr->Default[8];
			break;

		case CHANGLING112AC_DM:
		case BOSHIAGO112AC_DM_1:
			output_buf[6] = present_fan;
			Verify_data();
			IR_Generate();
			break;

		case SAJODENKI44AC_DM_1:
		case MEILING44AC_DM:
			output_buf[9] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case MIDEA88AC_DM:
			output_buf[9] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case AITE56AC_DM:
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SIGMA56AC_DM:
		case CHANGFU112AC_DM:
			output_buf[10] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case RAYBO64AC_DM:
			output_buf[10] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SHARP63AC_DM_1:
		case ELBA48AC_DM:
		case HUABAO48AC_DM:
		case HUABAO38AC_DM:
			output_buf[11] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			break;

		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU56AC_DM_1:
		case HUAWEI56AC_DM:
		case HAIER56AC_DM:
		case OTHER56AC_DM:
			output_buf[12] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case TEAC96AC_DM:
		case FEILU72AC_DM:
		case FUNAI72AC_DM:
			output_buf[12] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case MITSUBISHI88AC_DM_2:
			output_buf[13] = DbPtr->Default[5];
			Verify_data();
			IR_Generate();
			break;

		case TEAC56AC_DM:
			output_buf[13] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case TEAC56AC_DM_2:
			output_buf[13] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case OTHER48AC_DM_2:
		case KELONG48AC_DM_1:
		case KELONG38AC_DM:
			output_buf[13] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case MIDEA72AC_DM_1:
			output_buf[13] = 0x0f;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case TEAC56AC_DM_3:
			output_buf[14] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case MIDEA72AC_DM_4:
		case MIDEA72AC_DM_3:
		case MIDEA72AC_DM_2:
			output_buf[14] = 0x0f;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[6];
			break;

		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[15] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case CHIGO56AC_DM_2:
			output_buf[15] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case HITACHI104AC_DM_1:
			output_buf[15] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
		case NEC48AC_DM_2:
		case CHANGHONG136AC_DM_1:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case MIDEA72AC_DM:
			output_buf[16] = 0x06;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case NEC72AC_DM_5:
		case NEC104_AC_DM:
		case GREE67AC_DM_2:
			output_buf[17] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

		case GOLDSTAR28AC_DM:
			output_buf[10] = 0x08;
			output_buf[4] = 0x00;
			output_buf[7] = 0x00;
			output_buf[6] = DbPtr->Data[FAN_LOW];
			output_buf[18] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			output_buf[4] = present_mode;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[6] = present_fan;
			break;

		case NATIONAL32AC_DM_1:
			output_buf[8] = 0x00;
			output_buf[11] = 0x00;
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			output_buf[11] = DbPtr->Default[3];
			break;

		case NEC96AC_DM:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case DONGXINBAO136AC_DM:
		case CHANGHONG136AC_DM_3:
		case CHANGHONG136AC_DM_2:
			output_buf[21] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case NEC64AC_DM_3:
			output_buf[21] = 0x07;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case CHANGHONG136AC_DM:
		case SANYO104_AC_DM_1:
			output_buf[22] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[22] = DbPtr->Default[11];
			break;

		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
		case AMCOR136AC_DM:
			output_buf[23] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[23] = DbPtr->Default[12];
			break;

		case SANYO136AC_DM_2:
		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
		case TADAIR136AC_DM_1:
			output_buf[24] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[24] = DbPtr->Default[13];
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			output_buf[3] = DbPtr->Data[POWER_OFF];
			output_buf[4] = 0x00;
			output_buf[6] = 0x06;
			output_buf[7] = 0x07;
			Verify_data();
			IR_Generate();
			output_buf[3] = present_power;
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case RAYBO40AC_DM:
			output_buf[6] = 0x02;
			output_buf[12] = 0x02;
			output_buf[4] = 0x0f;
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[6] = present_fan;
			output_buf[12] = DbPtr->Default[4];
			output_buf[4] = present_mode;
			output_buf[10] = DbPtr->Default[2];
			break;

		case KTY00128AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x00;
			output_buf[7] = 0x00;
			output_buf[10] = 0x02;
			output_buf[18] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[10] = DbPtr->Default[2];
			break;

		case TCL32AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x08;
			output_buf[7] = 0x00;
			output_buf[8] = 0x03;
			output_buf[9] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			output_buf[9] = DbPtr->Default[1];
			break;

		case JINBEIJING32AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x08;
			output_buf[7] = 0x00;
			output_buf[8] = 0x00;
			output_buf[9] = 0x0d;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			output_buf[9] = DbPtr->Default[1];
			break;

		case SHARP32AC_DM:
			output_buf[20] = 0x00;
			output_buf[4] = 0x00;
			output_buf[6] = 0x0e;
			output_buf[7] = 0x00;
			output_buf[8] = 0x0d;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			break;

		case JIALE64AC_DM:
			output_buf[4] = 0x0f;
			output_buf[6] = 0x0f;
			output_buf[7] = 0x0f;
			output_buf[8] = 0x0f;
			output_buf[9] = 0x0f;
			output_buf[10] = 0x0f;
			output_buf[11] = 0x0f;
			output_buf[12] = 0x0f;
			output_buf[13] = 0x0f;
			output_buf[14] = 0x0f;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			output_buf[9] = DbPtr->Default[1];
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			output_buf[13] = DbPtr->Default[5];
			output_buf[14] = DbPtr->Default[6];
			break;

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
        present_swing = DbPtr->Data[SWING_OFF];

        switch (DbPtr->DataForm)
		{
		case HISENSE136AC_DM_4:
		case HISENSE136AC_DM_3:
		case HISENSE136AC_DM_2:
		case HISENSE136AC_DM_5:
		case AUOMA136AC_DM_1:
			output_buf[20] = DbPtr->Data[SWING_OFF];
			Verify_data();
			IR_Generate();
			if (DbPtr->Num == 10232 || DbPtr->Num == 10234)
				output_buf[20] = 0x02;
			else
				output_buf[20] = 0x03;
			break;
		
		case HISENSE136AC_DM_1:
			output_buf[20] = DbPtr->Data[SWING_OFF];
			Verify_data();
			IR_Generate();
			output_buf[20] = 0x04;
			break;

		case TECOAC62_DM_3:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case GREE67AC_DM_3:
		case GREE67AC_DM_2:
		case GREE67AC_DM_1:
		case GREE67AC_DM:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case GREE68AC_DM_1:
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case OTHER32AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case HUALING112AC_DM_1:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
		case CORONA96AC_DM:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case HYUNDAI32AC_DM_1:
		case DELONGHI71AC_DM:
			output_buf[20] = 0x07;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case AUOMA84AC_DM_1:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case CHUNLAN32AC_DM:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case AUCMA136AC_DM_1:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Default[10];
			break;

		case AUX32AC_DM:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case FEILU32AC_DM:
			output_buf[20] = 0x0e;
			Verify_data();
			IR_Generate();
			break;

		case ELECTROLUX96AC_DM:
			output_buf[20] = 0x0f;
			Verify_data();
			IR_Generate();
			break;

		case CHANGHONG32AC_DM:
			output_buf[20] = 0x11;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x13;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x14;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x18;
			Verify_data();
			IR_Generate();
			break;

		case BLUESTAR32AC_DM:
			output_buf[20] = 0x1f;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x81;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x88;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x9d;
			Verify_data();
			IR_Generate();
			break;

		case NEC48AC_DM_3:
			output_buf[3] = 0x00;
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[3] = present_power;
			output_buf[16] = DbPtr->Default[8];
			break;

		case AUX44AC_DM:
			output_buf[5] = DbPtr->Data[SWING_OFF];
			Verify_data();
			IR_Generate();
			output_buf[5] = 0x02;
			break;

		case CHANGLING112AC_DM:
		case BOSHIAGO112AC_DM_1:
			output_buf[6] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case SAJODENKI44AC_DM_1:
		case MEILING44AC_DM:
		case MIDEA88AC_DM:
			output_buf[9] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[9] = DbPtr->Default[1];
			break;

		case AITE56AC_DM:
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SIGMA56AC_DM:
		case CHANGFU112AC_DM:
			output_buf[10] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case RAYBO64AC_DM:
			output_buf[10] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			break;

		case SHARP63AC_DM_1:
		case ELBA48AC_DM:
		case HUABAO48AC_DM:
		case HUABAO38AC_DM:
			output_buf[11] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[11] = DbPtr->Default[3];
			break;

		case TECOAC62_DM_1:
			output_buf[12] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case CHANGFU56AC_DM_1:
		case HUAWEI56AC_DM:
		case HAIER56AC_DM:
		case OTHER56AC_DM:
			output_buf[12] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case TEAC96AC_DM:
			output_buf[12] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[4];
			break;

		case FEILU72AC_DM:
		case FUNAI72AC_DM:
			output_buf[12] = 0x09;
			Verify_data();
			IR_Generate();
			output_buf[12] = DbPtr->Default[6];
			break;

		case RAYBO40AC_DM_1:
			output_buf[13] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case TEAC56AC_DM:
			output_buf[13] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case TEAC56AC_DM_2:
			output_buf[13] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case OTHER48AC_DM_2:
		case KELONG48AC_DM_1:
		case KELONG38AC_DM:
			output_buf[13] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[5];
			break;

		case MIDEA72AC_DM_1:
			output_buf[13] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[13] = DbPtr->Default[6];
			break;

		case MITSUBISHI88AC_DM_2:
			output_buf[13] = 0x17;
			Verify_data();
			IR_Generate();
			break;

		case TEAC56AC_DM_3:
			output_buf[14] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case MIDEA72AC_DM_4:
		case MIDEA72AC_DM_3:
			output_buf[14] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[8];
			break;

		case MIDEA72AC_DM_2:
			output_buf[14] = 0x0e;
			Verify_data();
			IR_Generate();
			output_buf[14] = DbPtr->Default[8];
			break;

		case HITACHI104AC_DM:
		case CHANGLING104AC_DM:
			output_buf[15] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case CHIGO56AC_DM_2:
			output_buf[15] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case HITACHI104AC_DM_1:
			output_buf[15] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case NEC72AC_DM_4:
		case SANYO72AC_DM_4:
		case NEC48AC_DM_2:
		case CHANGHONG136AC_DM_1:
			output_buf[16] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case MIDEA72AC_DM:
			output_buf[16] = 0x06;
			Verify_data();
			IR_Generate();
			output_buf[16] = DbPtr->Default[8];
			break;

		case NEC72AC_DM_5:
		case NEC104_AC_DM:
			output_buf[17] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

		case NEC96AC_DM:
		case NEC64AC_DM_2:
		case SANYO64AC_DM_2:
		case NEC64AC_DM_4:
		case DONGXINBAO136AC_DM:
		case CHANGHONG136AC_DM_3:
		case CHANGHONG136AC_DM_2:
			output_buf[21] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case NEC64AC_DM_3:
			output_buf[21] = 0x07;
			Verify_data();
			IR_Generate();
			output_buf[21] = DbPtr->Default[10];
			break;

		case CHANGHONG136AC_DM:
		case SANYO104_AC_DM_1:
			output_buf[22] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[22] = DbPtr->Default[11];
			break;

		case SANYO72AC_DM_3:
		case NEC72AC_DM_3:
		case AMCOR136AC_DM:
			output_buf[23] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[23] = DbPtr->Default[12];
			break;

		case GOLDSTAR28AC_DM:
			output_buf[10] = 0x08;
			output_buf[4] = 0x00;
			output_buf[7] = 0x00;
			output_buf[6] = DbPtr->Data[FAN_LOW];
			output_buf[18] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[10] = DbPtr->Default[2];
			output_buf[4] = present_mode;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[6] = present_fan;
			break;

		case NATIONAL32AC_DM_1:
			output_buf[8] = 0x00;
			output_buf[11] = 0x00;
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[8] = DbPtr->Default[0];
			output_buf[11] = DbPtr->Default[3];
			break;

		case RAYBO40AC_DM:
			output_buf[6] = 0x02;
			output_buf[12] = 0x02;
			output_buf[4] = 0x0e;
			output_buf[8] = 0x01;
			output_buf[10] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[6] = present_fan;
			output_buf[12] = DbPtr->Default[4];
			output_buf[4] = present_mode;
			output_buf[8] = DbPtr->Default[0];
			output_buf[10] = DbPtr->Default[2];
			break;

		case SANYO136AC_DM_2:
		case NEC136AC_DM_1:
		case SANYO136AC_DM_1:
		case AMCOR136AC_DM_1:
		case TADAIR136AC_DM_1:
			output_buf[24] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[24] = DbPtr->Default[13];
			break;

		case KTY00128AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x00;
			output_buf[7] = 0x00;
			output_buf[10] = 0x02;
			output_buf[18] = 0x08;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[10] = DbPtr->Default[2];
			break;

		case TOSHIBA48AC_DM:
		case MIDEA48AC_DM_1:
			if (DbPtr->Num == 10056)
			{
				output_buf[3] = DbPtr->Data[POWER_ON];
				output_buf[6] = 0x00;
			}
			else if (DbPtr->Num == 10047)
			{
				output_buf[3] = DbPtr->Data[POWER_OFF];
				output_buf[6] = 0x06;
			}
			else 
			{
				output_buf[3] = DbPtr->Data[POWER_OFF];
				output_buf[6] = 0x06;
			}
			output_buf[4] = 0x00;
			output_buf[7] = 0x07;
			Verify_data();
			IR_Generate();
			output_buf[3] = present_power;
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case TCL32AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x00;
			output_buf[7] = 0x02;
			output_buf[8] = 0x03;
			output_buf[9] = 0x03;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			output_buf[9] = DbPtr->Default[1];
			break;

		case JINBEIJING32AC_DM:
			output_buf[6] = 0x0e;
			output_buf[7] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			break;

		case SHARP32AC_DM:
			output_buf[20] = 0x00;
			output_buf[4] = 0x00;
			output_buf[6] = 0x0e;
			output_buf[7] = 0x00;
			output_buf[8] = 0x0d;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			break;

		case JIALE64AC_DM:
			output_buf[4] = 0x00;
			output_buf[6] = 0x01;
			output_buf[7] = 0x01;
			output_buf[8] = 0x00;
			output_buf[9] = 0x01;
			output_buf[10] = 0x00;
			output_buf[11] = 0x00;
			output_buf[12] = 0x00;
			output_buf[13] = 0x00;
			output_buf[14] = 0x00;
			Verify_data();
			IR_Generate();
			output_buf[4] = present_mode;
			output_buf[6] = present_fan;
			output_buf[7] = DbPtr->Temp[present_temp];
			output_buf[8] = DbPtr->Default[0];
			output_buf[9] = DbPtr->Default[1];
			output_buf[10] = DbPtr->Default[2];
			output_buf[11] = DbPtr->Default[3];
			output_buf[12] = DbPtr->Default[4];
			output_buf[13] = DbPtr->Default[5];
			output_buf[14] = DbPtr->Default[6];
			break;

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
		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
		case BLUESTAR32AC_DM:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
		case OTHER48AC_DM_3:
		case TECOAC62_DM_2:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case CORONA96AC_DM:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case NATIONAL32AC_DM_1:
			output_buf[20] = 0x04;
			Verify_data();
			IR_Generate();
			break;

		case HAIER32AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x08;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case ELECTROLUX96AC_DM:
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x09;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x0a;
			Verify_data();
			IR_Generate();
			break;

		case FERROLIT48AC_DM:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case CHIGO96AC_DM:
		case DELONGHI71AC_DM:
		case AUX32AC_DM:
			output_buf[20] = 0x0d;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x14;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case QIXING32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x18;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x19;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case FEILU32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x1a;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
			output_buf[20] = 0x1C;
			Verify_data();
			IR_Generate();
			break;

		case CHUNLAN32AC_DM:
			output_buf[20] = 0x28;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x44;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x96;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_1:
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[15] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case MITSUBISHI112AC_DM_2:
			output_buf[17] = 0x10;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

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

		case PANASONIC32AC_DM_2:
			output_buf[20] = 0x00;
			Verify_data();
			IR_Generate();
			break;

		case SHARP31AC_DM:
		case OTHER48AC_DM_3:
		case TECOAC62_DM_2:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			output_buf[20] = DbPtr->Keypress;
			break;

		case ELECTROLUX104AC_DM_1:
		case ELECTROLUX104AC_DM:
		case AUX104AC_DM:
		case SANZUAN104AC_DM:
		case CHANGFENG104AC_DM:
			output_buf[20] = 0x01;
			Verify_data();
			IR_Generate();
			break;

		case YUTU80AC_DM_2:
		case YUTU80AC_DM:
		case XILENG80AC_DM:
		case OTHER80AC_DM:
		case SHENGFENG80AC_DM:
		case FEILU80AC_DM:
		case AUOMA80AC_DM_1:
			output_buf[20] = 0x02;
			Verify_data();
			IR_Generate();
			break;

		case RHOSS72AC_DM:
		case CORONA96AC_DM:
		case DELONGHI71AC_DM:
			output_buf[20] = 0x03;
			Verify_data();
			IR_Generate();
			break;

		case GREE48AC_DM_2:
		case OTHER96AC_DM:
		case JINDA96AC_DM:
		case NATIONAL32AC_DM_1:
		case ELECTROLUX96AC_DM:
		case AUX32AC_DM:
			output_buf[20] = 0x05;
			Verify_data();
			IR_Generate();
			break;

		case YUTU84AC_DM:
		case AUOMA84AC_DM_1:
			output_buf[20] = 0x06;
			Verify_data();
			IR_Generate();
			break;
		
		case HAIER32AC_DM:
			output_buf[20] = 0x9;
			Verify_data();
			IR_Generate();
			break;

		case HYUNDAI32AC_DM_1:
		case OTHER32AC_DM:
		case FERROLIT48AC_DM:
		case BLUESTAR32AC_DM:
		case DONGXINBAO32AC_DM:
		case AUOMA32AC_DM_1:
			output_buf[20] = 0x0b;
			Verify_data();
			IR_Generate();
			break;

		case CHIGO96AC_DM:
			output_buf[20] = 0x0c;
			Verify_data();
			IR_Generate();
			break;

		case TCL112AC_DM_3:
			output_buf[20] = 0x10;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_2:
		case FEILU32AC_DM:
		case CHANGHONG32AC_DM:
			output_buf[20] = 0x13;
			Verify_data();
			IR_Generate();
			break;

		case OTHER32AC_DM_3:
		case KELONG32AC_DM:
		case CHUNLAN32AC_DM:
			output_buf[20] = 0x18;
			Verify_data();
			IR_Generate();
			break;

		case GREE32AC_DM:
		case GLEE32AC_DM:
			output_buf[20] = 0x19;
			Verify_data();
			IR_Generate();
			break;

		case QIXING32AC_DM:
			output_buf[20] = 0x1b;
			Verify_data();
			IR_Generate();
			break;

		case FEILU32AC_DM_2:
			output_buf[20] = 0x1d;
			Verify_data();
			IR_Generate();
			break;

		case HITACHI264AC_DM_1:
		case HITACHI264AC_DM_2:
		case HITACHI264AC_DM:
			output_buf[20] = 0x43;
			Verify_data();
			IR_Generate();
			break;

		case PANASONIC32AC_DM:
			output_buf[20] = 0x97;
			Verify_data();
			IR_Generate();
			break;

		case TECOAC62_DM_1:
			output_buf[12] = DbPtr->Default[4];
			Verify_data();
			IR_Generate();
			break;

		case DONGXINBAO136AC_DM:
			output_buf[15] = 0x02;
			Verify_data();
			IR_Generate();
			output_buf[15] = DbPtr->Default[7];
			break;

		case MITSUBISHI112AC_DM_2:
			output_buf[17] = 0x10;
			Verify_data();
			IR_Generate();
			output_buf[17] = DbPtr->Default[9];
			break;

		default:
			Verify_data();
			IR_Generate();
			break;
		}
	
    return TX_OK;
}
