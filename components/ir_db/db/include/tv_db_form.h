#ifndef __TV_DB_FORM_H_
#define  __TV_DB_FORM_H_

#include "global_db_typedef.h"

//Data Option
#define LEADER     0x10 
#define TOGGLE	   0x20 
#define DATA	   0x40 
#define CUSTOM     0x40 
#define SYSTEM     0x40 
#define END		   0x80	
#define BAR	       0x01 

//DB Option
#define EXT_CODE_MASK    0x0F    //Ext Code Table Mask           
#define FLASH_PULSE 0x40    //SAA1250_FLASH         
#define REP_END 0x20    //Repeat Time Point = End           
#define EXT_CUSTOM  0x10    //Change Custom         

//Datamap Index
#define LC8C8D8DB8E_REP   0   
#define LC8C8D8DB8E_CON 1   
#define LC8CB8D8DB8E_REP    2   
#define LC8CB8D8DB8E_CON    3   
#define LC8SD8E_CON 4   //M50560
#define L2T1C5D6E_CON   5   //RC5
#define L3D5_CON    6   //UPS1986
#define LC5D6CB5DB6E_CON    7   //MN6030
#define LC6D6CB6DB6E_CON    8   //MN6014
#define LD7C5_CON   9   //CX7947
#define C8D8E_CON   10  //JVC
#define D6E_CON 11  //M50125P
#define C5D8C2EC5DB8CB2E_CON    12  //M50139
#define LC7ELD7E_CON    13  //GEMINI_C7
#define D5DB5E_CON  14  //MN6025A
#define LC32D16E_CON    15  //PANASONIC
#define LC5D6E2_CON 16  //SAA1250_FLASH
#define L6D8E_CON   17  //SAF1039_F46_9
#define L1D7_CON    18  //ZENITH_S7_F40
#define L2D5_CON    19  //ZENITH_D5_F40
#define C3D3D8SC3D3D8_CON   20  //TC9148
#define LC8D8SC8D8E_CON 21  //UNKNOW2_F65
#define C4D6SC4D6E_REP  22  //RCA_F41
#define LC8D8E_REP  23  //JVC_F38
#define D6SD6_CON   24  //SANYO_F45
#define LC5C5SLD5D5_REP 25  //GEMINI10_F30
#define LC8D8C8D8E_CON  26  //BORIM32
#define LD8_CON 27  //HANS_GLASS
#define LC8D8DB8E_REP   28  //NEC24
#define LC8D8DB8E_CON   29  //NEC24
#define LC4D8CB4DB8E_CON    30  //TCL24
#define LC8C8D8DB8E_REP_NEC 31  
#define LC8CB8D8DB8E_REP_NEC    32  
#define LC16D8DB8ELC16D8DB8E    33  //LG_SAMSUNG
#define LC8C8D8DB8E_REP_NEC_2   34  
#define LT1C4D5E_CON    35  //UNKNOWN2
#define LC8C8C8C1D8DB8E_CON 36  //NEC_42_Bits
#define T2D_CON 37  //UNKNOWN4

//Waveform Index
#define TOSHIBA 0
#define NEC 1
#define NEC_F36 2
#define NEC_F40 3
#define M50560  4
#define M50560_F35  5
#define M50560_F40  6
#define RC5 7
#define RC5_F36 8
#define UPS1986 9
#define MN6030_F35  10
#define MN6030_F36_5    11
#define MN6014_F36_8    12
#define MN6030_F57  13
#define CX7947  14
#define CX7947_F40  15
#define JVC_F33_6   16
#define JVC_F33_2   17
#define M50125P 18
#define M50125P_F33_4   19
#define M50125P_F40 20
#define M50139  21
#define GEMINI_C7_F31_3 22
#define MN6025A 23
#define PANASONIC_F36_6 24
#define PANASONIC_F37_1 25
#define SAA1250_FLASH   26
#define SAF1039_F46_9   27
#define ZENITH_S7_F40   28
#define ZENITH_D5_F40   29
#define TC9148  30
#define UNKNOW2_F65 31
#define RCA_F41 32
#define JVC_F38 33
#define SANYO_F45   34
#define GEMINI10_F30    35
#define BORIM32 36
#define HANS_GLASS  37
#define NEC24_CON   38
#define TCL24   39
#define XEVA32  40
#define NEC_TOSH    41
#define XEVA32_2    42
#define NEC24_REP   43
#define M50560_REP60    44
#define NEC_2   45
#define NEC_3   46
#define SAA1250_FLASH_Rep128    47
#define SAA1250_FLASH_Rep103    48
#define UNKNOWN3    49
#define NEC_42_Bits 50
#define UNKNOWN4   51

#define DATA_MAP_SIZE		9

typedef struct{
        DATA_MAP DataMap[DATA_MAP_SIZE];
        DATA_MAP RepDataMap[DATA_MAP_SIZE];
}DATA_FORM;

typedef struct
{
        unsigned char Num;
        unsigned char WaveForm;
        unsigned char DataForm;
        unsigned char Option;
        unsigned char Custom1;
        unsigned char Custom2;
        unsigned char Key[9];
        unsigned char RepMsk;
}TV_DB_FORM;

typedef struct
{
        unsigned char Num;
        unsigned char WaveForm;
        unsigned char DataForm;
        unsigned char Option;
        unsigned char Custom1;
        unsigned char Custom2;
        unsigned char Key[45];
}STB_DB_FORM;


extern STB_DB_FORM STB_DbForm[];
extern TV_DB_FORM TV_DbForm[];
extern WAVE_FORM TV_WaveForm[];
extern DATA_FORM TV_DataForm[];
extern unsigned int tvdb_size;
extern unsigned int stbdb_size;
#endif
