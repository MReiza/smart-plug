#ifndef __AC_IR_TX_H__
#define __AC_IR_TX_H__

typedef struct
{
    unsigned char id;
    unsigned char state;
} CMD;

typedef enum
{
    POWER_ON = 0,
    POWER_OFF,
    MODE_COOL,
    MODE_WARM,
    MODE_DRY,
    MODE_AUTO,
    FAN_LOW,
    FAN_MID,
    FAN_HIGH,
    FAN_AUTO,
    FAN_TURBO,
    SWING_ON = 27,
    SWING_OFF,
    TEMP_UP,
    TEMP_DOWN,
} AC_DATA_LST;

typedef enum
{
    NOT_USED_KEY = 0,
    TX_OK,
} ERROR_MSG;

void AC_Brand_Set(unsigned short Set_Num);

//air conditioner api list
unsigned char ac_power_on();
unsigned char ac_power_off();
unsigned char ac_mode_cool();
unsigned char ac_mode_warm();
unsigned char ac_mode_dry();
unsigned char ac_mode_auto();
unsigned char ac_fan_low();
unsigned char ac_fan_mid();
unsigned char ac_fan_high();
unsigned char ac_fan_auto();
unsigned char ac_fan_turbo();
unsigned char ac_swing_on();
unsigned char ac_swing_off();
unsigned char ac_temp_up();
unsigned char ac_temp_down();

#endif
