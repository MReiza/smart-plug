#ifndef __TV_IR_TX_H__
#define __TV_IR_TX_H__

#define RMT_TX_CARRIER_EN   1   /*!< Enable carrier for IR transmitter test with IR led */
#define TV_TX_CHANNEL       2     /*!< TV channel for transmitter */                                                      
#define STB_TX_CHANNEL      3     /*!< STB channel for transmitter */                                                      
#define RMT_TX_GPIO_NUM     21     /*!< GPIO number for transmitter signal */                                               
#define RMT_CLK_DIV         100    /*!< RMT counter clock divider */                                                        
#define RMT_TICK_10_US    (80000000/RMT_CLK_DIV/100000)   /*!< RMT counter value for 10 us.(Source clock is APB clock) */
#define UNUSED_KEY 0

//TV Key List
typedef enum
{
        TV_POWER = 0,
        TV_VOL_UP,
        TV_VOL_DOWN,
        TV_SOURCE,
        TV_UP,
        TV_DOWN,
        TV_LEFT,
        TV_RIGHT,
        TV_OK,
}TV_KEY_INDEX;

//STB Key List
typedef enum
{
        STB_POWER = 0,
        MOVIE,
        TV_REVIEW,
        TV_CHART,
        MENU,
        FAV_CHANNEL,
        VOL_UP,
        VOL_DOWN,
        CH_UP,
        CH_DOWN,
        STB_OK,
        UP,
        LEFT,
        DOWN,
        RIGHT,
        GO_BACK,
        MUTE,
        EXIT,
        REW,
        PLAY,
        STOP,
        FF,
        zero,
        one,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ASTERISK,
        HASH,
        SEARCH,
        ERASE,
        KO_EN,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MY_MENU,
        SHOPPING,
        WIDGET,
        APP_STORE,
}STB_KEY_INDEX;

unsigned char TV_IR_TX(unsigned char keycode);
unsigned char STB_IR_TX(unsigned char keycode);
void tv_tx_init();
void stb_tx_init();
void TV_Brand_Set(unsigned char Set_Num);
void STB_Brand_Set(unsigned char Set_Num);
#endif
