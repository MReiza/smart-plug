#ifndef __TV_IR_TX_H__
#define __TV_IR_TX_H__

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
} TV_KEY_INDEX;

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
} STB_KEY_INDEX;

unsigned char TV_IR_TX(unsigned char keycode);
unsigned char STB_IR_TX(unsigned char keycode);
void TV_Brand_Set(unsigned char Set_Num);
void STB_Brand_Set(unsigned char Set_Num);
#endif
