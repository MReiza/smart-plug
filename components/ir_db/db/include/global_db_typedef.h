#ifndef __GLOBAL_DB_TYPEDEF_H__
#define __GLOBAL_DB_TYPEDEF_H__

//Wave Option
#define LOW 1
#define HIGH 2

//Db Option
#define REP 0x01
#define NAV 0x02

#define PULSE_SIZE 5
#define PULSE_SEQ 6

typedef struct
{
    unsigned char High;
    unsigned char Low;

} FREQ;

typedef struct
{
    unsigned short Time1;
    unsigned short Time2;
} REPEAT;

typedef struct
{
    unsigned char Pol;
    unsigned short Time;
} PULSE;

typedef struct
{
    unsigned char Type;
    unsigned char Pulse;
    unsigned char Array;
    unsigned char Size;
} DATA_MAP;

typedef struct
{
    FREQ Freq;
    REPEAT Repeat;
    PULSE Pulse[PULSE_SIZE][PULSE_SEQ];
} WAVE_FORM;

void find_set_num(unsigned char Set_Num, unsigned char *setnum_ptr, unsigned char db_list_size, unsigned char db_size, unsigned char *DbAccessPtr);

#endif
