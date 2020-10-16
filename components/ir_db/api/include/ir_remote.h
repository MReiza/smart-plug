#pragma once

#define RMT_TX_CHANNEL 0
#define RMT_TX_SIGNAL_IDX 87
#define RMT_TX_GPIO 27
#define RMT_TX_EXTENDER_GPIO 19
#define RMT_TICK_10_US 8
#define RMT_DATA_ITEM_NUM 256

void rmt_tx_init(void);
void rmt_set_num(unsigned short Set_Num,unsigned short *setnum_ptr,unsigned short db_list_size,unsigned short db_size,unsigned short *DbAccessPtr);
