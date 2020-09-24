#pragma once

#define RMT_TX_CHANNEL 0
#define RMT_TX_GPIO_NUM 27
#define RMT_CLK_DIV 100
#define RMT_TICK_10_US 8
#define RMT_DATA_ITEM_NUM 258

void rmt_tx_init(void);
void rmt_set_num(unsigned short Set_Num, unsigned short *DbAccessPtr);
