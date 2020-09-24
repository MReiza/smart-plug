#define __TV_DB_FORM_C__
#define test_db 0
#include "tv_db_form.h"

#if !test_db
TV_DB_FORM TV_DbForm[] = 
{
{16,    TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,    {0x2,   0x7,    0xB,    0x1,    0x17,   0x1B,   0x1C,   0x1D,   0x2A},  0x00},  //  CITIZEN
{28,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x8,    0x8,    {0x0F,  0x0C,   0x0D,   0x15,   0x14,   0x15,   0x11,   0x1D,   0x1B},  0x00},  //  ANAM
{34,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x6,    0x6,    {0x0F,  0x0C,   0x0D,   0x12,   0xFF,   0xFF,   0xFF,   0x1E,   0xFF},  0x00},  //  CITIZEN
{36,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x6,    0x6,    {0x14,  0x10,   0x11,   0x15,   0xFF,   0xFF,   0x0F,   0x0E,   0xFF},  0x00},  //  DAEWOO
{52,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x8,    0x8,    {0x0F,  0x10,   0x11,   0x14,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{56,    TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x11,   0x11,   {0x2,   0x7,    0x0B,   0x0,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  INKEL
{71,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x0,    0x0,    {0x0F,  0x0C,   0x0D,   0x12,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  EMERSON
{72,    TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x16,   0x16,   {0x0F,  0x0C,   0x0D,   0xFF,   0xFF,   0xFF,   0x0D,   0x0C,   0x18},  0x00},  //  EMERSON
{101,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x11,   0x11,   {0x2,   0x7,    0x0B,   0x1,    0x60,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TELEFUNKEN BLE
//{101,   UNKNOWN4,   T2D_CON,    FLASH_PULSE,    0x11,   0x11,   {0x1F,  0x20,   0x21,   0x22,   0x23,   0x24,   0x25,   0x26,   0x27},  0x00},  //  TOSHIBA IR
{104,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x11,   0x11,   {0x2,   0x7,    0x0B,   0x1,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TELEFUNKEN BLE
//{104,   UNKNOWN4,   T2D_CON,    FLASH_PULSE,    0x11,   0x11,   {0x28,  0x29,   0x2A,   0x2B,   0xFF,   0x2C,   0xFF,   0xFF,   0xFF},  0x00},  //  TOSHIBA IR
{105,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x17,   0x17,   {0x14,  0x12,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  MGA
{117,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x11,   0x11,   {0x2,   0x7,    0x0B,   0x1,    0x60,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TELEFUNKEN
{118,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x11,   0x11,   {0x2,   0x7,    0x0B,   0x1,    0x60,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TELEFUNKEN
{129,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x8,    0x8,    {0x0F,  0x0C,   0x0D,   0x12,   0xFF,   0xFF,   0x18,   0x16,   0x26},  0x00},  //  한국전자
{171,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x7,    0x7,    {0x2,   0x7,    0x0B,   0x1,    0x60,   0x61,   0x65,   0x62,   0x68},  0x00},  //  SAMSUNG
{172,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x7,    0x7,    {0x2,   0x7,    0x0B,   0x1,    0x17,   0x1B,   0x1C,   0x1D,   0x2A},  0x00},  //  SAMSUNG
{173,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x7,    0x7,    {0x2,   0x7,    0x0B,   0x1,    0x17,   0x1B,   0x1C,   0x1D,   0x29},  0x00},  //  CITIZEN
{174,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x0E,   0x0E,   {0x14,  0x12,   0x13,   0x16,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SAMSUNG
{175,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x0E,   0x0E,   {0x14,  0x12,   0x13,   0x16,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SAMSUNG
{176,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x5,    0x5,    {0x2,   0x7,    0x0B,   0x1,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SAMSUNG
{177,   TOSHIBA,    LC8C8D8DB8E_CON,    0,  0x5,    0x5,    {0x2,   0x7,    0x0B,   0x24,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SAMSUNG
{178,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x20,   0x20,   {0x2,   0x7,    0x0B,   0x1,    0x78,   0x75,   0x17,   0x13,   0x16},  0x00},  //  SAMSUNG
{187,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x0E,   0x0E,   {0x0C,  0x14,   0x15,   0x0F,   0x42,   0x43,   0x44,   0x45,   0x46},  0x00},  //  DAEWOO LUCOMS
{211,   TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x0E,   0x0E,   {0x0B,  0x13,   0x12,   0x0F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  UNIKRO
{227, TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x42,   0x42,   {0x0B,  0x12,   0x10,   0x0C,   0x0E,   0x13,   0x10,   0x12,   0x11},  0x00},  //  LG PILLIPS LCD
                                                                    
{87,    NEC_F40,    LC8C8D8DB8E_CON,    0,  0xAA,   0xBB,   {0x8E,  0x85,   0x0B,   0x19,   0x83,   0x9,    0xFF,   0xFF,   0x94},  0x00},  //  PIONEER
{1, NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x4,    0x10,   {0x0F,  0x0C,   0x0D,   0x12,   0x0A,   0x0B,   0x0D,   0x0C,   0x14},  0x00},  //  CINEMA PULS
{2, NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x20,   0x5F,   {0x0,   0x11,   0x12,   0x15,   0x10,   0x13,   0x12,   0x11,   0x40},  0x00},  //  금영
{6, NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0B,   0x6,    0x7,    0x5,    0x4,    0x4C},  0x00},  //  DAEWOO
{7, NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0B,   0x71,   0x72,   0x7,    0x6,    0x44},  0x00},  //  AOC
{22,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0A,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  AOC
{23,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x38,   0,  {0x12,  0x0E,   0x0F,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{25,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x18,   0,  {0x8,   0x2,    0x3,    0x53,   0x46,   0x47,   0x49,   0x48,   0x0A},  0x00},  //  DAEWOO
{26,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x81,   0x78,   {0x9C,  0x80,   0x88,   0x97,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{27,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x20,   0,  {0x17,  0x12,   0x0A,   0x1B,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{37,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x81,   0x78,   {0x18,  0x12,   0x13,   0x1A,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{38,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x81,   0x78,   {0x9D,  0x81,   0x89,   0xD6,   0x91,   0x95,   0x88,   0x80,   0x8D},  0x00},  //  DAEWOO
{44,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x81,   0x7A,   {0x9C,  0x80,   0x88,   0x97,   0x90,   0x94,   0x93,   0xD0,   0x8C},  0x00},  //  DAEWOO
{57,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x83,   0x46,   {0x40,  0x44,   0x45,   0x47,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  INKEL
{63,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x40,   0,  {0x12,  0x1A,   0x1E,   0x0F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SEARS
{66,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x40,   0,  {0x12,  0x1A,   0x1E,   0x14,   0x5B,   0x5F,   0xFF,   0xFF,   0xFF},  0x00},  //  TOSHIBA
{70,    NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x83,   0x7A,   {0x8,   0x2,    0x3,    0x0C,   0x0F,   0xFF,   0x81,   0x80,   0x4},   0x00},  //  EMERSON
{74,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x53,   0,  {0x17,  0x12,   0x15,   0x9,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HITACHI
{75,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x50,   0,  {0x17,  0x12,   0x15,   0x9,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HITACHI
{76,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x51,   0,  {0x8,   0x0A,   0x0B,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HITACHI
{81,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x15,   0,  {0x12,  0x16,   0x17,   0x0,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  CANDLE
//{83,    NEC,    LC8CB8D8DB8E_REP_NEC,   EXT_CUSTOM+0,   0x6E,   0,  {0x14,  0x2,    0x3,    0x15,   0x0B,   0x1,    0x2,    0x3,    0x8},   0x00},  //  SYLVANIA
{85,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0xA0,   0,  {0x0B,  0x1E,   0x1F,   0x0,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  PIONEER
{86,    NEC,    LC8CB8D8DB8E_CON,   0,  0xAA,   0,  {0x1C,  0x0A,   0x0B,   0x0C,   0x56,   0x57,   0x42,   0x41,   0xFF},  0x00},  //  NAD
{91,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x10,   0,  {0x11,  0x12,   0x0A,   0x6,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  GE
{92,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x20,   0,  {0x11,  0x12,   0x0A,   0x6,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  GE
{94,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x28,   0,  {0x0B,  0x0E,   0x0F,   0x14,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  RADIO SHACK
{95,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x38,   0,  {0x1C,  0x16,   0x17,   0x14,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
{96,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x30,   0,  {0x0,   0x9,    0x0A,   0x8,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
{97,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x0,    0,  {0x0B,  0x0E,   0x0F,   0x1C,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SHARP
{100,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x38,   0,  {0x14,  0x0E,   0x0F,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
{115,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0xC4,   0,  {0x18,  0x12,   0x13,   0x17,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  JVC
//{120,   NEC,    LC8CB8D8DB8E_REP_NEC,   EXT_CUSTOM+2,   0x4,    0,  {0x8,   0x2,    0x3,    0x9,    0x0E,   0x4,    0x0D,   0x0C,   0x5},   0x00},  //  KENWOOD
{121,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x38,   0,  {0x12,  0x0E,   0x0F,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
{122,   NEC_F36,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0B,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TAEKWANG
{124,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x0,    0x7F,   {0x12,  0x1A,   0x1E,   0x55,   0x1B,   0x1F,   0x1E,   0x1A,   0x17},  0x00},  //  AVITRON
{127,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1C,   0,  {0x0A,  0x15,   0x13,   0x40,   0x11,   0x17,   0x13,   0x15,   0x14},  0x00},  //  AVITRON
{128,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0E,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  KTV
{130,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x2,    0xFF,   {0x14,  0x16,   0x15,   0x17,   0x10,   0x0E,   0x15,   0x16,   0x13},  0x00},  //  디지털 싸이노스
{132,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x8,    0,  {0x5,   0x0,    0x1,    0x40,   0x6,    0x7,    0x41,   0x43,   0x42},  0x00},  //  Trutech
{133,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x40,   0,  {0x12,  0x1A,   0x1E,   0x14,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SEARS
{139,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x96,   0,  {0x0C,  0x0F,   0x0D,   0x32,   0x14,   0x10,   0x34,   0x5E,   0x18},  0x00},  //  오르콤
{140,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x96,   0,  {0x0C,  0x0F,   0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  오르콤
{142,   NEC,    LC8C8D8DB8E_CON,    0,  0x45,   0x16,   {0x10,  0x20,   0x21,   0x17,   0x18,   0x19,   0x1A,   0x1B,   0x17},  0x00},  //  현대아이티
{143,   NEC,    LC8C8D8DB8E_CON,    0,  0xAA,   0x3,    {0x10,  0x26,   0x28,   0x0E,   0x1C,   0x1D,   0x21,   0x20,   0xFF},  0x00},  //  현대아이티
{144,   NEC,    LC8CB8D8DB8E_CON,   0,  0x77,   0,  {0x80,  0x91,   0x92,   0xAC,   0x8D,   0x8E,   0x8F,   0x90,   0x8C},  0x00},  //  현대아이티
{146,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x40,   0x2,    {0x8,   0x2,    0x3,    0x0B,   0xEE,   0xEE,   0x2D,   0x2C,   0x44},  0x00},  //  PRISM(프리즘)
{147,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x2,    0,  {0x2,   0x7,    0x0B,   0x1,    0x13,   0x10,   0x0B,   0x7,    0x1E},  0x00},  //  시그마컴
{148,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x12,  0x5,    0x0C,   0x4E,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  MOTV
{150,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x8,    0,  {0x0C,  0x15,   0x16,   0x0B,   0x17,   0x18,   0x16,   0x15,   0x14},  0x00},  //  LS전자
{151,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x8,    0,  {0x0C,  0x15,   0x16,   0x0B,   0x17,   0x18,   0x16,   0x15,   0x14},  0x00},  //  Trutech
{152,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x40,   0x3F,   {0x0A,  0x15,   0x1C,   0x0F,   0x0B,   0x0E,   0x10,   0x11,   0x0D},  0x00},  //  Volimtle
{153,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x42,   0,  {0x0B,  0x16,   0x1A,   0x0C,   0x0E,   0x13,   0x10,   0x12,   0x11},  0x00},  //  유투스
{154,   NEC,    LC8C8D8DB8E_CON,    0,  0x45,   0x16,   {0x10,  0x20,   0x21,   0x17,   0x18,   0x19,   0x1A,   0x1B,   0x17},  0x00},  //  ANAM
{155,   NEC,    LC8C8D8DB8E_CON,    0,  0xAA,   0x3,    {0x10,  0x26,   0x28,   0x0E,   0x1C,   0x1D,   0x21,   0x20,   0xFF},  0x00},  //  현대아이티
{156,   NEC,    LC8CB8D8DB8E_CON,   0,  0x77,   0,  {0x80,  0x91,   0x92,   0xAC,   0x8D,   0x8E,   0x8F,   0x90,   0x8C},  0x00},  //  보림전자
{158,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x44,   0x9B,   {0x0B,  0x58,   0x54,   0x17,   0x0E,   0x0A,   0x1A,   0x12,   0x6},   0x00},  //  BTC
{159,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x40,   0x3F,   {0x0A,  0x15,   0x1C,   0x0F,   0x0B,   0x0E,   0x10,   0x11,   0x0D},  0x00},  //  Trutech
{160,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x40,   0x2,    {0x8,   0x2,    0x3,    0x0A,   0x34,   0x35,   0x32,   0x33,   0x44},  0x00},  //  인디텍
{162,   NEC,    LC8CB8D8DB8E_CON,   0,  0x3,    0,  {0x11,  0x53,   0x57,   0x13,   0x47,   0x4B,   0x57,   0x53,   0x4},   0x00},  //  ALBAS
{165,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x0B,   0x40,   0x41,   0x7,    0x6,    0x44},  0x00},  //  LG
{166,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0,  {0x8,   0x2,    0x3,    0x98,   0x0,    0x1,    0x3,    0x2,    0x44},  0x00},  //  LG
{170,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x6E,   0,  {0x14,  0x58,   0x59,   0x5A,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  LG
{180,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x0,    0x7F,   {0x0B,  0x1,    0x2,    0x34,   0x15,   0x17,   0x12,   0x1A,   0x16},  0x00},  //  TIVA
{181,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x8,    0,  {0xD7,  0x83,   0x86,   0xC3,   0x92,   0xD8,   0x97,   0x9F,   0x9B},  0x00},  //  SKYMEDIA
{182,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x40,   0x40,   {0x0A,  0x15,   0x1C,   0x42,   0x0B,   0x0E,   0x10,   0x11,   0x0D},  0x00},  //  DAEWOO LUCOMS
{184,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x0,    0,  {0x11,  0x1C,   0x1D,   0x48,   0x44,   0x42,   0x41,   0x49,   0x45},  0x00},  //  SKYMEDIA
{185,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x0,    0x7F,   {0x0A,  0x1E,   0x5F,   0x53,   0x5E,   0x56,   0x5B,   0x58,   0x50},  0x00},  //  DAEWOO LUCOMS
{186,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x2,    0x7D,   {0x46,  0x0C,   0x19,   0x4B,   0x48,   0x4D,   0x4E,   0x49,   0x4A},  0x00},  //  DAEWOO LUCOMS
{188,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x20,   0,  {0x52,  0x2,    0x9,    0x40,   0x47,   0x4D,   0x49,   0x4B,   0x4A},  0x00},  //  DAEWOO LUCOMS
{189,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1C,   0,  {0x0A,  0x5C,   0x5D,   0x17,   0x5A,   0x5B,   0x5D,   0x5C,   0x1C},  0x00},  //  HAIER
{190,   NEC,    LC8CB8D8DB8E_CON,   0,  0x83,   0,  {0x11,  0x53,   0x57,   0x6,    0x47,   0x4B,   0x57,   0x53,   0x4},   0x00},  //  HAIER
{191,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1C,   0,  {0x0A,  0x5C,   0x5D,   0x40,   0x5E,   0x5F,   0x12,   0x11,   0x1C},  0xC0},  //  HAIER
{192,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x0,    0x7F,   {0x0A,  0x1E,   0x5F,   0x53,   0x5E,   0x56,   0x5B,   0x58,   0x5A},  0x00},  //  시피앤씨
{202,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x8,    0,  {0xD7,  0x83,   0x86,   0xC0,   0x92,   0xD8,   0x97,   0x9F,   0x9B},  0x00},  //  TG삼보컴퓨터
{203,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1,    0,  {0x4A,  0x44,   0x0F,   0x7,    0x12,   0x16,   0x6,    0x1A,   0x0E},  0x00},  //  에드뷰
{209,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1C,   0,  {0x0A,  0x15,   0x13,   0x40,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  WORLD
{222,   NEC,    LC8C8D8DB8E_CON,    0,  0x7B,   0x80,   {0xC3,  0x63,   0x67,   0x3B,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  AIWA
//{222,   NEC_42_Bits,    LC8C8C8C1D8DB8E_CON,    EXT_CODE+11,    0x7B,   0x80,   {0x1,   0x31,   0x33,   0x1D,   0xFF,   0xFF,   0xFF,   0xFF,   0x9F},  0x00},      
{226,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x3,    0,  {0x11,  0x53,   0x57,   0x4F,   0x47,   0x4B,   0x57,   0x53,   0x4},   0x00},  //  ATEC
{233,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x1,    0x3E,   {0x0A,  0x1E,   0x5F,   0x50,   0x5E,   0x56,   0x5B,   0x58,   0x5A},  0x00},  //  토마토디스플레이
{239,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x3,    0,  {0x11,  0x48,   0x4D,   0x10,   0x8,    0x5E,   0x4D,   0x48,   0x4},   0x00},  //  LG PILLIPS LCD
{241,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x1C,   0,  {0x0A,  0x5C,   0x5D,   0x40,   0x5A,   0x5B,   0x5D,   0x5C,   0x1C},  0x00},  //  CIMA DIGITEK
{242,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x41,   0,  {0x0C,  0x2,    0x3,    0x7,    0x0,    0x1,    0x3,    0x2,    0x1F},  0x00},  //  CIMA DIGITEK
{243,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x42,   0xFD,   {0x0B,  0x12,   0x10,   0x0C,   0x0E,   0x13,   0x10,   0x12,   0x11},  0x00},  //  CIMA DIGITEK
{245,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x77,   0,  {0x80,  0x91,   0x92,   0xAC,   0x8D,   0x8E,   0x8F,   0x90,   0x8C},  0x00},  //  삼인 디스플레이
{246,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x10,   0,  {0x0A,  0x2,    0x3,    0x0C,   0xFF,   0xFF,   0xFF,   0xFF,   0x1F},  0x00},  //  삼인 디스플레이
{247,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x0,    0xBD,   {0x1,   0x0C,   0x10,   0x8,    0x0B,   0x0F,   0x49,   0x4A,   0x0D},  0x00},  //  TPV(이마트 Dream View)
{248,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x0,    0,  {0x1F,  0x54,   0x17,   0x5C,   0x12,   0x10,   0x15,   0x19,   0x11},  0x00},  //  통큰TV(롯데마트)
                                                                    
{18,    M50560_F40, LC8SD8E_CON,    0,  0x3A,   0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0x1D,   0x1C,   0x0B},  0x00},  //  ANAM
{29,    M50560, LC8SD8E_CON,    0,  0x2,    0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{31,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x1F,  0x18,   0x1C,   0x1E,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{32,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x15,  0x0A,   0x0B,   0x8,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
//{33,    M50560_F35, LC8SD8E_CON,    EXT_CUSTOM+4,   0x14,   0,  {0x15,  0x13,   0x14,   0x15,   0x11,   0x0D,   0x10,   0x0F,   0x0E},  0x00},  //  DAEWOO
{35,    M50560, LC8SD8E_CON,    0,  0x15,   0,  {0x15,  0x13,   0x14,   0x16,   0x1C,   0x1D,   0x0D,   0x1B,   0x19},  0x00},  //  DAEWOO
{39,    M50560_F40, LC8SD8E_CON,    0,  0x14,   0,  {0x15,  0x13,   0x14,   0x16,   0x1C,   0x1D,   0x26,   0xFF,   0x23},  0x00},  //  DAEWOO
{40,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0x5,    0x0,    0xFF},  0x00},  //  DAEWOO
{41,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x19,  0x11,   0x15,   0x1D,   0x13,   0x17,   0x1F,   0x1E,   0x1A},  0x00},  //  DAEWOO
{42,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x0C,  0x2,    0x3,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{43,    M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x8,   0x2,    0x3,    0x0A,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
//{45,    M50560_REP60,   LC8SD8E_CON,    EXT_CUSTOM+6,   0x14,   0,  {0x15,  0x13,   0x14,   0x16,   0x11,   0x0D,   0x10,   0x0F,   0x0E},  0x00},  //  DAEWOO
{51,    M50560, LC8SD8E_CON,    0,  0x2,    0,  {0x0E,  0x3,    0x4,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{53,    M50560_F40, LC8SD8E_CON,    0,  0x3A,   0,  {0x0E,  0x3,    0x4,    0x0D,   0x24,   0x25,   0x3F,   0x3E,   0x0F},  0x00},  //  ANAM
{54,    M50560_F40, LC8SD8E_CON,    0,  0x2,    0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{55,    M50560, LC8SD8E_CON,    0,  0xEE,   0,  {0x0E,  0x3,    0x4,    0x0D,   0x24,   0x25,   0x3F,   0x3E,   0x34},  0x00},  //  ANAM
{69,    M50560_F40, LC8SD8E_CON,    0,  0x3A,   0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0x1D,   0x1C,   0xFF},  0x00},  //  ANAM
{134,   M50560_F40, LC8SD8E_CON,    0,  0x14,   0,  {0x15,  0x13,   0x14,   0x16,   0x1B,   0x2F,   0x3D,   0x27,   0x52},  0x00},  //  DAEWOO
{135,   M50560, LC8SD8E_CON,    0,  0xEE,   0,  {0x0E,  0x3,    0x4,    0x0D,   0x1A,   0x1B,   0x62,   0x63,   0x0},   0x00},  //  ANAM
{167,   M50560, LC8SD8E_CON,    0,  0x1,    0,  {0x0E,  0x3,    0x4,    0x0D,   0x30,   0x29,   0x28,   0x22,   0x2E},  0x00},  //  LG
{168,   M50560, LC8SD8E_CON,    0,  0x1,    0,  {0x0E,  0x3,    0x4,    0x0D,   0x7,    0x8,    0x5,    0x6,    0xFF},  0x00},  //  LG
{169,   M50560, LC8SD8E_CON,    0,  0x2,    0,  {0x0E,  0x3,    0x4,    0x0D,   0x2B,   0xFF,   0x28,   0x22,   0x2E},  0x00},  //  ANAM
{183,   M50560_REP60,   LC8SD8E_CON,    0,  0xEE,   0,  {0x0E,  0x3,    0x4,    0x0D,   0x1A,   0x1B,   0x62,   0x63,   0x0},   0x00},  //  SKYMEDIA
{204,   M50560, LC8SD8E_CON,    0,  0x15,   0,  {0x15,  0x3A,   0x3E,   0x16,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO(VIDEO)
{225,   M50560, LC8SD8E_CON,    0,  0x14,   0,  {0x15,  0x13,   0x19,   0x62,   0x20,   0x24,   0x22,   0x23,   0x21},  0x00},  //  디바이스
                                                                    
{68,    RC5_F36,    L2T1C5D6E_CON,  0,  0x0,    0,  {0x0C,  0x2,    0x22,   0x7,    0x0A,   0x2A,   0x2E,   0x0E,   0x39},  0x00},  //  SYLVANIA
//{123,   RC5_F36,    L2T1C5D6E_CON,  EXT_CUSTOM+8,   0x0,    0,  {0x0C,  0x2,    0x22,   0x3F,   0x35,   0x0D,   0x22,   0x2,    0x3F},  0x00},  //  MARANTZ
{131,   RC5_F36,    L2T1C5D6E_CON,  0,  0x0,    0,  {0x0C,  0x2,    0x22,   0x7,    0x2,    0x22,   0x2A,   0x1A,   0x27},  0x00},  //  PHILIPS
{149,   RC5_F36,    L2T1C5D6E_CON,  0,  0x0C,   0,  {0x34,  0x13,   0x33,   0x2,    0xFF,   0xFF,   0x3C,   0x0A,   0x2},   0x00},  //  디지털 디바이스
{215,   RC5_F36,    L2T1C5D6E_CON,  0,  0x0,    0,  {0x0C,  0x2,    0x22,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DIBOSS
{216,   RC5,    L2T1C5D6E_CON,  0,  0x0,    0,  {0x0C,  0x2,    0x22,   0x7,    0x0E,   0x2E,   0x0D,   0x35,   0x37},  0x00},  //  DIBOSS
                                                                    
{10,    UPS1986,    L3D5_CON,   0,  0,  0,  {0x1F,  0x19,   0x18,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x80},  //  ANAM
{12,    UPS1986,    L3D5_CON,   0,  0,  0,  {0x1F,  0x19,   0x18,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x80},  //  DAEWOO
{64,    UPS1986,    L3D5_CON,   0,  0,  0,  {0x12,  0x19,   0x18,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x80},  //  SEARS
{99,    UPS1986,    L3D5_CON,   0,  0,  0,  {0x1D,  0x1B,   0x1A,   0x1F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0xA0},  //  TEKNIKA
                                                                    
{46,    MN6030_F35, LC5D6CB5DB6E_CON,   0,  0x1F,   0,  {0x1F,  0x1B,   0x1A,   0x32,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{49,    MN6030_F35, LC5D6CB5DB6E_CON,   0,  0x0,    0,  {0x20,  0x24,   0x25,   0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{50,    MN6030_F35, LC5D6CB5DB6E_CON,   0,  0x0,    0,  {0x20,  0x1,    0x0D,   0x26,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  ANAM
{67,    MN6030_F36_5,   LC5D6CB5DB6E_CON,   0,  0x0,    0,  {0x20,  0x24,   0x25,   0x0D,   0x1A,   0x1B,   0x7,    0x6,    0x0E},  0x00},  //  QUASAR
{106,   MN6014_F36_8,   LC6D6CB6DB6E_CON,   0,  0x21,   0,  {0x2E,  0x20,   0x21,   0x26,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
{116,   MN6030_F57, LC5D6CB5DB6E_CON,   0,  0x0,    0,  {0x20,  0x24,   0x25,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  TECHNICS
                                                                    
{193,   CX7947_F40, LD7C5_CON,  0,  0x0,    0,  {0x95,  0x92,   0x93,   0xA5,   0x98,   0x99,   0xD9,   0xD8,   0x8B},  0x00},  //  SONY
{194,   CX7947_F40, LD7C5_CON,  0,  0x0,    0,  {0x95,  0x92,   0x93,   0xAA,   0xD8,   0xD9,   0xF5,   0xF4,   0x8B},  0x00},  //  SONY
{195,   CX7947_F40, LD7C5_CON,  0,  0x0,    0,  {0x95,  0x92,   0x93,   0xA5,   0xF4,   0xF5,   0xB4,   0xB3,   0xE5},  0x00},  //  SONY
{196,   CX7947_F40, LD7C5_CON,  0,  0x0,    0,  {0xAE,  0x92,   0x93,   0xAF,   0xF4,   0xF5,   0xCD,   0xCE,   0xE5},  0x00},  //  SONY
                                                                    
{79,    JVC_F33_2,  C8D8E_CON,  0,  0x47,   0,  {0x2,   0x22,   0x2A,   0x7,    0x33,   0x3B,   0x2B,   0x23,   0x34},  0x00},  //  MITSUBISHI
{112,   JVC_F33_2,  C8D8E_CON,  0,  0x47,   0,  {0x2,   0x22,   0x2A,   0x7,    0x34,   0x3C,   0x1A,   0x3A,   0x77},  0x00},  //  TEKNIKA
                                                                    
{58,    M50125P_F40,    D6E_CON,    0,  0,  0,  {0x0E,  0x3,    0x4,    0x1F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HITACHI
{65,    M50125P,    D6E_CON,    0,  0,  0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  MITSUBISHI
{80,    M50125P_F33_4,  D6E_CON,    0,  0,  0,  {0x0E,  0x3,    0x4,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  MGA
{103,   M50125P,    D6E_CON,    0,  0,  0,  {0x0E,  0x3,    0x4,    0x0D,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
                                                                    
{93,    M50139, C5D8C2EC5DB8CB2E_CON,   0,  0x0D,   0x01,   {0x16,  0x14,   0x15,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SHARP
{98,    M50139, C5D8C2EC5DB8CB2E_CON,   0,  0x1,    0x01,   {0x16,  0x14,   0x15,   0x13,   0x32,   0x34,   0x50,   0x4F,   0x33},  0x00},  //  SHARP
{113,   M50139, C5D8C2EC5DB8CB2E_CON,   0,  0x1,    0x01,   {0x16,  0x14,   0x15,   0x13,   0x4F,   0x50,   0x21,   0x26,   0x71},  0x00},  //  SHARP
                                                                    
{30,    GEMINI_C7_F31_3,    LC7ELD7E_CON,   0,  0x2,    0,  {0xFE,  0x6E,   0x6C,   0x70,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
                                                                    
{47,    MN6025A,    D5DB5E_CON, 0,  0,  0,  {0x11,  0x1E,   0x0E,   0x13,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0xA0},  //  ANAM
                                                                    
//{48,    PANASONIC_F36_6,    LC32D16E_CON,   REP_END+EXT_CODE+1, 0x2,    0x20,   {0x3D,  0x20,   0x21,   0x05,   0x4A,   0x4B,   0x4E,   0x4F,   0x7F},  0x00},  //  ANAM
//{137,   PANASONIC_F37_1,    LC32D16E_CON,   REP_END+EXT_CODE+2, 0x2,    0x20,   {0x3D,  0x20,   0x21,   0x05,   0x4A,   0x4B,   0x4E,   0x4F,   0x49},  0x00},  //  PANASONIC
                                                                    
//{84,    SAA1250_FLASH,  LC5D6E2_CON,    FLASH_PULSE,    0x1C,   0,  {0x1,   0x2,    0x3,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  MAGNAVOX
//{110,   SAA1250_FLASH,  LC5D6E2_CON,    FLASH_PULSE,    0x1C,   0,  {0x4,   0x5,    0x6,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SYLVANIA
//{217,   SAA1250_FLASH,  LC5D6E2_CON,    FLASH_PULSE,    0x12,   0,  {0x7,   0x8,    0x9,    0xFF,   0x19,   0x1A,   0x1B,   0x1C,   0x1D},  0x00},  //  BARCO
                                                                    
{82,    SAF1039_F46_9,  L6D8E_CON,  0,  0,  0,  {0x82,  0x28,   0x20,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SYLVANIA
                                                                    
{88,    ZENITH_S7_F40,  L1D7_CON,   0,  0,  0,  {0x6E,  0x6A,   0x68,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SIGNATURE
{89,    ZENITH_D5_F40,  L2D5_CON,   0,  0,  0,  {0x0E,  0x0A,   0x8,    0x0F,   0x6,    0x18,   0x0C,   0x1E,   0x17},  0x00},  //  ZENITH
                                                                    
//{14,    TC9148, C3D3D8SC3D3D8_CON,  EXT_CODE+3, 0x5,    0,  {0x10,  0x08,   0x04,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x80},  //  EMERSON
//{73,    TC9148, C3D3D8SC3D3D8_CON,  EXT_CODE+4, 0x0,    0,  {0x20,  0x1,    0x2,    0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x80},  //  EMERSON
{24,    UNKNOW2_F65,    LC8D8SC8D8E_CON,    0,  0x14,   0,  {0x15,  0x13,   0x14,   0x16,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  DAEWOO
{77,    RCA_F41,    C4D6SC4D6E_REP, 0,  0x08,   0,  {0x0A,  0x1A,   0x12,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  RCA
{78,    RCA_F41,    C4D6SC4D6E_REP, 0,  0x08,   0,  {0x0A,  0x1A,   0x16,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HITACHI
{102,   JVC_F38,    LC8D8E_REP, 0,  0x03,   0,  {0x17,  0x1E,   0x1F,   0x13,   0x5A,   0x5B,   0x51,   0x50,   0x0A},  0x00},  //  JVC
{114,   SANYO_F45,  D6SD6_CON,  0,  0,  0,  {0x0F,  0x3F,   0x1F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  SANYO
//{119,   GEMINI10_F30,   LC5C5SLD5D5_REP,    EXT_CODE+5, 0x00,   0x00,   {0x1A,  0x0E,   0x0C,   0x1F,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  BLAUPUNKT
{145,   M50560_REP60,   LC8SD8E_CON,    0,  0x3A,   0,  {0x0E,  0x03,   0x04,   0x0D,   0x24,   0x25,   0x3F,   0x3E,   0x0F},  0x00},  //  ANAM
//{157,   BORIM32,    LC8D8C8D8E_CON, REP_END+EXT_CODE+6, 0xF6,   0xF9,   {0x2A,  0xF0,   0xEF,   0xDB,   0xEA,   0xE9,   0xE7,   0xE8,   0xE6},  0x00},  //  보림전자
{200,   HANS_GLASS, LD8_CON,    0,  0,  0,  {0x20,  0xA0,   0xC0,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},  //  HANS GLASS
{207,   M50560_REP60,   LC8SD8E_CON,    0,  0xEE,   0,  {0x80,  0x8A,   0x8B,   0x8E,   0x96,   0x97,   0x9A,   0x91,   0x9B},  0x00},  //  ANAM
{212,   NEC24_REP,  LC8D8DB8E_REP,  0,  0x8,    0,  {0x0F,  0x0A,   0x0B,   0x03,   0x02,   0x01,   0x1B,   0x19,   0x09},  0x00},  //  UNIKRO
{223,   NEC24_CON,  LC8D8DB8E_CON,  0,  0x8,    0,  {0x0F,  0x6A,   0x6B,   0x69,   0x0C,   0x0D,   0x0B,   0x0A,   0x13},  0x00},  //  ERAE
{224,   NEC24_CON,  LC8D8DB8E_CON,  0,  0x8,    0,  {0x0F,  0x6A,   0x6B,   0x67,   0x0C,   0x0D,   0x0B,   0x0A,   0x13},  0x00},  //  ERAE
{228,   TCL24,  LC4D8CB4DB8E_CON,   0,  0x0F,   0,  {0x54,  0xF4,   0x74,   0xC5,   0x9A,   0x1A,   0x6A,   0xEA,   0x2F},  0x00},  //  TCL
//{237,   XEVA32_2,   LC8D8C8D8E_CON, REP_END+EXT_CODE+7, 0xFC,   0xF3,   {0x2A,  0xF0,   0xEF,   0xDB,   0xEA,   0xE9,   0xE7,   0xE8,   0xE6},  0x00},  //  XEVA
//{244,   XEVA32, LC8D8C8D8E_CON, REP_END+EXT_CODE+8, 0xFC,   0xF3,   {0x2A,  0xF0,   0xEF,   0xDB,   0xEA,   0xE9,   0xE7,   0xE8,   0xE6},  0x00},  //  XEVA
//{141,   XEVA32, LC8D8C8D8E_CON, REP_END+EXT_CODE+9, 0xFB,   0xF4,   {0x2A,  0xF0,   0xEF,   0xDB,   0xEA,   0xE9,   0xE7,   0xE8,   0xE6},  0x00},  //  오르콤
                                                                    
//{164,   NEC_TOSH,   LC16D8DB8ELC16D8DB8E,   EXT_CODE+10,    0x04,   0xFB,   {0x08,  0x02,   0x03,   0x0B,   0x40,   0x41,   0x07,   0x06,   0x44},  0xBF},  //  LG_SAMSUNG
                                                                    
{3, M50560, LC8SD8E_CON,    0,  0x1,    0x0,    {0x0E,  0x3,    0x4,    0x0D,   0x0B,   0x1F,   0x6,    0x5,    0x0},   0x00},      
{8, M50560, LC8SD8E_CON,    0,  0x1,    0x0,    {0x0E,  0x3,    0x4,    0x0D,   0x5,    0x6,    0xFF,   0xFF,   0x0C},  0x00},      
{11,    NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0x0,    {0x8,   0x2,    0x3,    0x0B,   0x6,    0x7,    0x1A,   0xFF,   0x4C},  0x00},      
{138,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x4,    0x0,    {0x8,   0x2,    0x3,    0x51,   0x4,    0x5,    0x7,    0x6,    0x44},  0x00},      
{201,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x10,   0x0,    {0x0F,  0xFF,   0xFF,   0x18,   0x0A,   0x0B,   0x0D,   0x0C,   0x11},  0x00},      
{205,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x6E,   0x0,    {0x7D,  0x58,   0x59,   0x5A,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},      
{208,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x61,   0x22,   {0x0A,  0x40,   0x41,   0x45,   0x48,   0x4A,   0x4A,   0x4B,   0x4C},  0x00},      
{213,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x2,    0x0,    {0x0A,  0x6,    0x7,    0x1,    0x2,    0x3,    0x7,    0x6,    0x5},   0x00},      
{214,   NEC,    LC8CB8D8DB8E_REP_NEC,   0,  0x2,    0x0,    {0x0A,  0x6,    0x7,    0x10,   0x2,    0x3,    0x7,    0x6,    0x5},   0x00},      
{253,   NEC,    LC8C8D8DB8E_REP_NEC,    0,  0x61,   0x22,   {0x0A,  0x40,   0x41,   0x45,   0x48,   0x49,   0x4A,   0x4B,   0x4C},  0x00},      
{163,   NEC_2,  LC8C8D8DB8E_REP_NEC_2,  0,  0x40,   0x2,    {0x8,   0x2,    0x3,    0x0B,   0x0,    0x1,    0x3,    0x2,    0x44},  0x00},      
{206,   NEC_3,  LC8C8D8DB8E_CON,    0,  0xC1,   0xA5,   {0x0,   0x71,   0x72,   0x70,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},  0x00},      
//{227,   NEC_2,  LC8C8D8DB8E_REP_NEC_2,  0,  0x42,   0xFD,   {0x0B,  0x12,   0x10,   0x0C,   0x0E,   0x13,   0x10,   0x12,   0x11},  0x00},  //  TOSHIBA modify
//{9, SAA1250_FLASH_Rep128,   LC5D6E2_CON,    FLASH_PULSE+OTHER_REP,  0x1C,   0,  {0xA,   0xC,    0xE,    0x10,   0x12,   0x14,   0xFF,   0xFF,   0xFF},  0x00},      
{13,    UNKNOWN3,   LT1C4D5E_CON,   0,  0x0F,   0x0,    {0x9,   0x8,    0x18,   0x19,   0x2,    0x12,   0x1A,   0x0A,   0xFF},  0x00},      

};

#else
TV_DB_FORM TV_DbForm[] = 
{
   {11, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {11, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {11, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {11, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {11, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {16, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x17,   0x1B,   0x1C,   0x1D,   0x2A},   0x00},   //  CITIZEN
   {28, TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x8,    0x8,   {0x0F,   0x0C,   0x0D,   0x15,   0x14,   0x15,   0x11,   0x1D,   0x1B},   0x00},   //  ANAM
   {34, TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x6,    0x6,   {0x0F,   0x0C,   0x0D,   0x12,   0xFF,   0xFF,   0xFF,   0x1E,   0xFF},   0x00},   //  CITIZEN
   {36, TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x6,    0x6,   {0x14,   0x10,   0x11,   0x15,   0xFF,   0xFF,   0x0F,   0x0E,   0xFF},   0x00},   //  DAEWOO
   {52, TOSHIBA,    LC8C8D8DB8E_REP,    0,  0x8,    0x8,   {0x0F,   0x10,   0x11,   0x14,   0xFF,   0xFF,   0xFF,   0xFF,   0xFF},   0x00},   //  ANAM
   {171, TOSHIBA,   LC8C8D8DB8E_REP,    0,  0x7,    0x7,   {0x2,    0x7,    0xB,    0x1,    0x60,   0x61,   0x65,   0x62,   0x68},   0x00},   //  CITIZEN
   {1,NEC, LC8C8D8DB8E_REP_NEC,    0,  0x4,    0x10,   {0x0F,   0x0C,   0x0D,   0x12,   0x0A,   0x0B,   0x0D,   0x0C,   0x14},   0x00},   //  CINEMA PULS
};
#endif

STB_DB_FORM STB_DbForm[] = 
{
  {1 , NEC,  LC8C8D8DB8E_REP_NEC,  0,  0x39,   0x15,  { 0x00,   0x71,   0x72,   0x1B,   0x41,   0x20,   0x1F,   0x40,   0x49,   0x4A,   0x13,   0x11,   0x12,   0x15,   0x14,   0x61,   0x18,   0x62,   0x65,   0x60,   0x63,   0x64,   0x0C,   0x03,   0x04,   0x05,   0x06,   0x07,   0x08,   0x09,   0x0A,   0x0B,   0x4E,   0x4F,   0x74,   0x16,   0x0D,   0x1C,   0x1D,   0x1A,   0x1E,   0x6A,   0x76,   0x43,   0x75}},//KT IR Settop

};

#if test_db
WAVE_FORM TV_WaveForm[] = {
//TOSHIBA
    {{71, 142},
    {1082, 1082},
    {{{HIGH, 580},    {LOW,    540},    {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{HIGH,   580},    {LOW,    1650},   {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{HIGH,   4500},   {LOW,    4500},   {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0}}}},
//NEC
    {{71, 142},
    {1082, 1082},
    {{{HIGH, 580},    {LOW,    538},    {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{HIGH,   580},    {LOW,    1680},   {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{HIGH,   9000},   {LOW,    4433},   {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{HIGH,  9000},  {LOW,  2250},  {0,  0},  {0,  0},  {0,  0},  {0,  0}},
    {{0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0},  {0,  0}}}},
};

#else
WAVE_FORM TV_WaveForm[] = {
    //TOSHIBA,                                          
    {{71,   142},                                       
    {1082,  1082},                                      
    {{{HIGH,    580},   {LOW,   540},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 580},   {LOW,   1650},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 4500},  {LOW,   4500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC,                                          
    {{71,   142},                                       
    {1082,  1082},                                      
    {{{HIGH,    580},   {LOW,   538},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 580},   {LOW,   1680},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   4433},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   2250},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_F36,                                          
    {{74,   148},                                       
    {1080,  1080},                                      
    {{{HIGH,    575},   {LOW,   540},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 575},   {LOW,   1680},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9060},  {LOW,   4500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9060},  {LOW,   2250},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_F40,                                          
    {{66,   130},                                       
    {1200,  1200},                                      
    {{{HIGH,    516},   {LOW,   522},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 516},   {LOW,   1580},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 8380},  {LOW,   4274},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   2250},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50560,                                           
    {{71,   142},                                       
    {632,   632},                                       
    {{{HIGH,    538},   {LOW,   495},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 538},   {LOW,   1550},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 8450},  {LOW,   4210},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 538},   {LOW,   4210},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50560_F35,                                           
    {{76,   152},                                       
    {595,   595},                                       
    {{{HIGH,    530},   {LOW,   470},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 530},   {LOW,   1470},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 8000},  {LOW,   4000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 530},   {LOW,   4000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50560_F40,                                           
    {{67,   133},                                       
    {595,   595},                                       
    {{{HIGH,    520},   {LOW,   490},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 520},   {LOW,   1500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 8000},  {LOW,   4000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 520},   {LOW,   4000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //RC5,                                          
    {{71,   142},                                       
    {1088,  1088},                                      
    {{{HIGH,    855},   {LOW,   846},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  846},   {HIGH,  855},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  847},   {HIGH,  855},   {LOW,   846},   {HIGH,  855},   {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //RC5_F36,                                          
    {{74,   149},                                       
    {1135,  1135},                                      
    {{{HIGH,    895},   {LOW,   910},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  910},   {HIGH,  895},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  910},   {HIGH,  895},   {LOW,   910},   {HIGH,  895},   {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //UPS1986,                                          
    {{71,   142},                                       
    {715,   715},                                       
    {{{LOW, 1100},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 1118},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 1118},  {LOW,   1100},  {HIGH,  1118},  {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  27200}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //MN6030_F35,                                           
    {{76,   153},                                       
    {1088,  1088},                                      
    {{{HIGH,    920},   {LOW,   900},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 920},   {LOW,   2760},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3650},  {LOW,   3600},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //MN6030_F36_5,                                         
    {{74,   146},                                       
    {1048,  1048},                                      
    {{{HIGH,    870},   {LOW,   840},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 870},   {LOW,   2620},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3450},  {LOW,   3440},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //MN6014_F36_8,                                         
    {{72,   146},                                       
    {1035,  1035},                                      
    {{{HIGH,    920},   {LOW,   870},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 920},   {LOW,   2590},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3500},  {LOW,   3500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //MN6030_F57,                                           
    {{46,   92},                                        
    {1008,  1008},                                      
    {{{HIGH,    850},   {LOW,   830},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 850},   {LOW,   2500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3330},  {LOW,   3330},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //CX7947,                                           
    {{71,   142},                                       
    {876,   876},                                       
    {{{LOW, 595},   {HIGH,  608},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  595},   {HIGH,  1192},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 2380},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  25280}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //CX7947_F40,                                           
    {{67,   133},                                       
    {443,   443},                                       
    {{{LOW, 595},   {HIGH,  610},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  595},   {HIGH,  1200},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 2368},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //JVC_F33_6,                                            
    {{79,   158},                                       
    {600,   600},                                       
    {{{HIGH,    280},   {LOW,   820},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 280},   {LOW,   1950},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //JVC_F33_2,                                            
    {{81,   161},                                       
    {595,   595},                                       
    {{{HIGH,    310},   {LOW,   810},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 310},   {LOW,   1950},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50125P,                                          
    {{71,   142},                                       
    {497,   497},                                       
    {{{HIGH,    540},   {LOW,   1570},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 540},   {LOW,   3650},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50125P_F33_4,                                            
    {{80,   160},                                       
    {567,   567},                                       
    {{{HIGH,    610},   {LOW,   1800},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 610},   {LOW,   4200},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50125P_F40,                                          
    {{67,   133},                                       
    {471,   471},                                       
    {{{HIGH,    510},   {LOW,   1500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 510},   {LOW,   3500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50139,                                           
    {{71,   142},                                       
    {1319,  1319},                                      
    {{{HIGH,    273},   {LOW,   784},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 273},   {LOW,   1823},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 273},   {LOW,   42767}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //GEMINI_C7_F31_3,                                          
    {{85,   170},                                       
    {1753,  1753},                                      
    {{{HIGH,    520},   {LOW,   476},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  476},   {HIGH,  520},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 520},   {LOW,   2525},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  25000}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //MN6025A,                                          
    {{71,   142},                                       
    {1000,  1000},                                      
    {{{HIGH,    872},   {LOW,   2500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 872},   {LOW,   5850},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //PANASONIC_F36_6,                                          
    {{73,   146},                                       
    {542,   542},                                       
    {{{HIGH,    480},   {LOW,   420},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 480},   {LOW,   1320},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3608},  {LOW,   1756},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //PANASONIC_F37_1,                                          
    {{72,   144},                                       
    {741,   741},                                       
    {{{HIGH,    471},   {LOW,   427},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 471},   {LOW,   1325},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3437},  {LOW,   1716},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //SAA1250_FLASH,                                            
    {{0,    0},                                     
    {1336,  1336},                                      
    {{{HIGH,    9}, {LOW,   92},    {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   190},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   250},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   270},   {HIGH,  9}, {LOW,   92},    {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //SAF1039_F46_9,                                            
    {{57,   114},                                       
    {870,   870},                                       
    {{{HIGH,    540},   {LOW,   2195},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  2765},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 540},   {LOW,   2195},  {HIGH,  540},   {LOW,   7642},  {0, 0}, {0, 0}},
    {{HIGH, 540},   {LOW,   2195},  {HIGH,  540},   {LOW,   4928},  {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //ZENITH_S7_F40,                                            
    {{67,   133},                                       
    {1777,  1777},                                      
    {{{HIGH,    512},   {LOW,   5110},  {HIGH,  512},   {LOW,   521},   {HIGH,  512},   {LOW,   4185}},
    {{HIGH, 512},   {LOW,   521},   {HIGH,  512},   {LOW,   4185},  {HIGH,  512},   {LOW,   5110}},
    {{HIGH, 512},   {LOW,   5110},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //ZENITH_D5_F40,                                            
    {{67,   133},                                       
    {1795,  1795},                                      
    {{{HIGH,    513},   {LOW,   5134},  {HIGH,  513},   {LOW,   495},   {HIGH,  513},   {LOW,   4135}},
    {{HIGH, 513},   {LOW,   495},   {HIGH,  513},   {LOW,   4136},  {HIGH,  513},   {LOW,   5134}},
    {{HIGH, 513},   {LOW,   495},   {HIGH,  513},   {LOW,   4136},  {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //TC9148,                                           
    {{71,   142},                                       
    {1609,  1609},                                      
    {{{HIGH,    430},   {LOW,   1240},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 1265},  {LOW,   407},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  34175}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //UNKNOW2_F65,                                          
    {{40,   82},                                        
    {355,   355},                                       
    {{{HIGH,    312},   {LOW,   289},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 312},   {LOW,   845},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 4795},  {LOW,   2460},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 312},   {LOW,   2446},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 312},   {LOW,   13000}, {HIGH,  4813},  {LOW,   2426},  {0, 0}, {0, 0}}}},
                                                
    //RCA_F41,                                          
    {{64,   129},                                       
    {1883,  1890},                                      
    {{{HIGH,    978},   {LOW,   955},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 978},   {LOW,   2930},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 978},   {LOW,   35700}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //JVC_F38,                                          
    {{70,   140},                                       
    {584,   454},                                       
    {{{HIGH,    545},   {LOW,   492},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 545},   {LOW,   1552},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 8402},  {LOW,   4225},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //SANYO_F45,                                            
    {{59,   118},                                       
    {445,   445},                                       
    {{{LOW, 2834},  {LOW,   2836},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 2834},  {LOW,   2836},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  11091}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //GEMINI10_F30,                                         
    {{88,   176},                                       
    {1672,  1338},                                      
    {{{HIGH,    542},   {LOW,   527},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  527},   {HIGH,  542},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 540},   {LOW,   2657},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{LOW,  19400}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //BORIM32,                                          
    {{70,   141},                                       
    {144,   144},                                       
    {{{HIGH,    513},   {LOW,   681},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 513},   {LOW,   1502},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3785},  {LOW,   3775},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //HANS_GLASS,                                           
    {{70,   141},                                       
    {708,   708},                                       
    {{{LOW, 1955},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 2000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 4000},  {LOW,   1955},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC24_CON,                                            
    {{70,   141},                                       
    {788,   788},                                       
    {{{HIGH,    566},   {LOW,   546},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 566},   {LOW,   1686},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9033},  {LOW,   4435},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //TCL24,                                            
    {{70,   141},                                       
    {670,   670},                                       
    {{{HIGH,    517},   {LOW,   996},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 517},   {LOW,   2006},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 4006},  {LOW,   4010},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //XEVA32,                                           
    {{70,   141},                                       
    {23,    23},                                        
    {{{HIGH,    516},   {LOW,   837},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 516},   {LOW,   2000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 4007},  {LOW,   3990},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_TOSH,                                         
    {{70,   141},                                       
    {3185,  3185},                                      
    {{{HIGH,    562},   {LOW,   540},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 562},   {LOW,   1655},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   4500},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 562},   {LOW,   45750}, {LOW,   45750}, {HIGH,  4500},  {LOW,   4500},  {0, 0}},
    {{HIGH, 562},   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //XEVA32_2,                                         
    {{70,   141},                                       
    {142,   142},                                       
    {{{HIGH,    542},   {LOW,   679},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 542},   {LOW,   1473},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 3743},  {LOW,   3668},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC24_REP,                                            
    {{70,   141},                                       
    {1075,  1075},                                      
    {{{HIGH,    566},   {LOW,   546},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 566},   {LOW,   1686},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9033},  {LOW,   4435},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9033},  {LOW,   2231},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //M50560_REP60,                                         
    {{71,   142},                                       
    {590,   590},                                       
    {{{HIGH,    508},   {LOW,   495},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 508},   {LOW,   1480},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 7950},  {LOW,   3974},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 508},   {LOW,   4010},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_2,                                            
    {{70,   140},                                       
    {1082,  1082},                                      
    {{{HIGH,    590},   {LOW,   545},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 590},   {LOW,   1655},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   4450},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   2235},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 560},   {LOW,   545},   {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_3,                                            
    {{70,   140},                                       
    {1065,  1065},                                      
    {{{HIGH,    300},   {LOW,   680},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 300},   {LOW,   2000},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   4560},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   2235},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //SAA1250_FLASH_Rep128,                                         
    {{0,    0},                                     
    {1284,  1284},                                      
    {{{HIGH,    9}, {LOW,   92},    {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   190},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   250},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   270},   {HIGH,  9}, {LOW,   92},    {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //SAA1250_FLASH_Rep103,                                         
    {{0,    0},                                     
    {1030,  1030},                                      
    {{{HIGH,    9}, {LOW,   92},    {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   190},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   250},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   270},   {HIGH,  9}, {LOW,   92},    {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //UNKNOWN3,                                         
    {{71,   142},                                       
    {1209,  1209},                                      
    {{{HIGH,    166},   {LOW,   4875},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 164},   {LOW,   7356},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 0}, {LOW,   0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 0}, {LOW,   0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //NEC_42_Bits,                                          
    {{71,   142},                                       
    {1081,  1081},                                      
    {{{HIGH,    562},   {LOW,   545},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 562},   {LOW,   1680},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9000},  {LOW,   4425},  {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 562},   {LOW,   1680},  {HIGH,  562},   {0, 0}, {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
                                                
    //UNKNOWN4,                                         
    {{0,    0},                                     
    {1379,  1379},                                      
    {{{HIGH,    9}, {LOW,   92},    {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   190},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   250},   {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{HIGH, 9}, {LOW,   270},   {HIGH,  9}, {LOW,   92},    {0, 0}, {0, 0}},
    {{0,    0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}}},
};
#endif

#if test_db
DATA_FORM TV_DataForm[] = {
//LC8C8D8DB8E_REP
        {{{LEADER,	2,	0,	0},
        {CUSTOM,	0,	0,	8},
        {CUSTOM,	0,	1,	8},
        {DATA,	0,	2,	8},
        {DATA+BAR,	0,	2,	8},
        {END,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0}},
                        
        {{LEADER,	2,	0,	0},
        {END,	0,	0,	0},
        {END,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0}}},


//LC8C8D8DB8E_REP_NEC
        {{{LEADER,	2,	0,	0},
        {CUSTOM,	0,	0,	8},
        {CUSTOM,	0,	1,	8},
        {DATA,	0,	2,	8},
        {DATA+BAR,	0,	2,	8},
        {END,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0}},
                        
        {{LEADER,	3,	0,	0},
        {END,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0},
        {0,	0,	0,	0}}},
};

#else

DATA_FORM TV_DataForm[] = {
    //LC8C8D8DB8E_REP,          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {END,   0,  0,  0},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8C8D8DB8E_CON,          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8CB8D8DB8E_REP,         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM+BAR,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8CB8D8DB8E_CON,         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM+BAR,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM+BAR,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8SD8E_CON(M50560),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {END,   3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {END,   3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //L2T1C5D6E_CON(RC5),           
    {{{LEADER,  2,  0,  0},
    {TOGGLE,    0,  0,  0},
    {SYSTEM,    0,  0,  5},
    {DATA,  0,  2,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {TOGGLE,    0,  0,  0},
    {SYSTEM,    0,  0,  5},
    {DATA,  0,  2,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //L3D5_CON(UPS1986),            
    {{{LEADER,  2,  0,  0},
    {DATA,  0,  2,  5},
    {END,   3,  0,  0},
    {LEADER,    2,  0,  0},
    {DATA,  0,  2,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  2,  5},
    {END,   3,  0,  0},
    {LEADER,    2,  0,  0},
    {DATA,  0,  2,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC5D6CB5DB6E_CON(MN6030),         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  5},
    {DATA,  0,  2,  6},
    {CUSTOM+BAR,    0,  0,  5},
    {DATA+BAR,  0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  5},
    {DATA,  0,  2,  6},
    {CUSTOM+BAR,    0,  0,  5},
    {DATA+BAR,  0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC6D6CB6DB6E_CON(MN6014),         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  6},
    {DATA,  0,  2,  6},
    {CUSTOM+BAR,    0,  0,  6},
    {DATA+BAR,  0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  6},
    {DATA,  0,  2,  6},
    {CUSTOM+BAR,    0,  0,  6},
    {DATA+BAR,  0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LD7C5_CON(CX7947),            
    {{{LEADER,  2,  0,  0},
    {DATA,  0,  2,  8},
    {CUSTOM,    0,  0,  4},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  2,  8},
    {CUSTOM,    0,  0,  4},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //C8D8E_CON(JVC),           
    {{{CUSTOM,  0,  0,  8},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{CUSTOM,   0,  0,  8},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //D6E_CON(M50125P),         
    {{{DATA,    0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{DATA, 0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //C5D8C2EC5DB8CB2E_CON(M50139),         
    {{{CUSTOM,  0,  0,  5},
    {DATA,  0,  2,  8},
    {CUSTOM,    0,  1,  2},
    {END,   2,  0,  0},
    {CUSTOM,    0,  0,  5},
    {DATA+BAR,  0,  2,  8},
    {CUSTOM+BAR,    0,  1,  2},
    {END,   0,  0,  0},
    {0, 0,  0,  0}},
                
    {{CUSTOM,   0,  0,  5},
    {DATA,  0,  2,  8},
    {CUSTOM,    0,  1,  2},
    {END,   2,  0,  0},
    {CUSTOM,    0,  0,  5},
    {DATA+BAR,  0,  2,  8},
    {CUSTOM+BAR,    0,  1,  2},
    {END,   0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC7ELD7E_CON(GEMINI_C7),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  7},
    {END,   3,  0,  0},
    {LEADER,    2,  0,  0},
    {DATA,  0,  2,  7},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  7},
    {END,   3,  0,  0},
    {LEADER,    2,  0,  0},
    {DATA,  0,  2,  7},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //D5DB5E_CON(MN6025A),          
    {{{DATA,    0,  2,  5},
    {DATA+BAR,  0,  2,  5},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{DATA, 0,  2,  5},
    {DATA+BAR,  0,  2,  5},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC32D16E_CON(PANASONIC),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {CUSTOM,    0,  2,  8},
    {CUSTOM,    0,  3,  8},
    {DATA,  0,  4,  8},
    {DATA,  0,  5,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {CUSTOM,    0,  2,  8},
    {CUSTOM,    0,  3,  8},
    {DATA,  0,  4,  8},
    {DATA,  0,  5,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC5D6E2_CON(SAA1250_FLASH),           
    {{{LEADER,  0,  0,  0},
    {LEADER,    1,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    3,  0,  0},
    {LEADER,    4,  0,  0},
    {LEADER,    5,  0,  0},
    {LEADER,    6,  0,  0},
    {LEADER,    7,  0,  0},
    {LEADER,    8,  0,  0}},
                
    {{LEADER,   0,  0,  0},
    {LEADER,    1,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    3,  0,  0},
    {LEADER,    4,  0,  0},
    {LEADER,    5,  0,  0},
    {LEADER,    6,  0,  0},
    {LEADER,    7,  0,  0},
    {LEADER,    8,  0,  0}}},
                
    //L6D8E_CON(SAF1039_F46_9),         
    {{{LEADER,  2,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    2,  0,  0},
    {LEADER,    3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //L1D7_CON(ZENITH_S7_F40),          
    {{{LEADER,  2,  0,  0},
    {DATA,  0,  2,  7},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  2,  7},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //L2D5_CON(ZENITH_D5_F40),          
    {{{LEADER,  2,  0,  0},
    {DATA,  0,  2,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  2,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //C3D3D8SC3D3D8_CON(TC9148),            
    {{{CUSTOM,  0,  0,  3},
    {DATA,  0,  5,  3},
    {DATA,  0,  4,  6},
    {END,   2,  0,  0},
    {CUSTOM,    0,  0,  3},
    {DATA,  0,  5,  3},
    {DATA,  0,  4,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{CUSTOM,   0,  0,  3},
    {DATA,  0,  5,  3},
    {DATA,  0,  4,  6},
    {END,   2,  0,  0},
    {CUSTOM,    0,  0,  3},
    {DATA,  0,  5,  3},
    {DATA,  0,  4,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8D8SC8D8E_CON(UNKNOW2_F65),         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {END,   3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   3,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {END,   3,  0,  0},
    {DATA,  0,  2,  8},
    {END,   3,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //C4D6SC4D6E_REP(RCA_F41),          
    {{{CUSTOM,  0,  0,  4},
    {DATA,  0,  2,  6},
    {END,   2,  0,  0},
    {CUSTOM,    0,  0,  4},
    {DATA,  0,  2,  6},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{CUSTOM,   0,  0,  4},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8D8E_REP(JVC_F38),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{CUSTOM,   0,  0,  8},
    {DATA,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //D6SD6_CON,(SANYO_F45),            
    {{{DATA,    0,  2,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{DATA, 0,  2,  6},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC5C5SLD5D5_REP(GEMINI10_F30),            
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  5},
    {CUSTOM,    0,  1,  5},
    {END,   3,  0,  0},
    {LEADER,    2,  0,  0},
    {DATA,  0,  4,  5},
    {DATA,  0,  5,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  4,  5},
    {DATA,  0,  5,  5},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8D8C8D8E_CON(BORIM32),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  4,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  5,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  4,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  5,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LD8_CON,(HANS_GLASS),         
    {{{LEADER,  2,  0,  0},
    {DATA,  0,  2,  8},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {DATA,  0,  2,  8},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8D8DB8E_REP,(NEC24),            
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   3,  0,  0},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8D8DB8E_CON(NEC24),         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC4D8CB4DB8E_CON(TCL24),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  4},
    {DATA,  0,  2,  8},
    {CUSTOM+BAR,    0,  0,  4},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  4},
    {DATA,  0,  2,  8},
    {CUSTOM+BAR,    0,  0,  4},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8C8D8DB8E_REP_NEC,          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   3,  0,  0},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8CB8D8DB8E_REP_NEC,         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM+BAR,    0,  0,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   3,  0,  0},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC16D8DB8ELC16D8DB8E (NEC_TOSH),          
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  16},
    {DATA,  0,  4,  8},
    {DATA+BAR,  0,  4,  8},
    {END,   3,  0,  0},
    {CUSTOM,    0,  2,  16},
    {DATA,  0,  5,  8},
    {DATA+BAR,  0,  5,  8},
    {END,   4,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  16},
    {DATA,  0,  4,  8},
    {DATA+BAR,  0,  4,  8},
    {END,   3,  0,  0},
    {CUSTOM,    0,  2,  16},
    {DATA,  0,  5,  8},
    {DATA+BAR,  0,  5,  8},
    {END,   4,  0,  0}}},
                
    //LC8C8D8DB8E_REP_NEC_2         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {DATA,  0,  2,  8},
    {DATA+BAR,  0,  2,  8},
    {END,   4,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   3,  0,  0},
    {END,   4,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LT1C4D5E_CON(UNKNOWN2),           
    {{{LEADER,  1,  0,  0},
    {TOGGLE,    0,  0,  0},
    {CUSTOM,    0,  0,  4},
    {DATA,  0,  2,  5},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   1,  0,  0},
    {TOGGLE,    0,  0,  0},
    {CUSTOM,    0,  0,  4},
    {DATA,  0,  2,  5},
    {END,   0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
                
    //LC8C8C8C1D8DB8E_CON(NEC_42_Bits),         
    {{{LEADER,  2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {CUSTOM,    0,  2,  8},
    {CUSTOM,    0,  3,  1},
    {DATA,  0,  4,  8},
    {DATA+BAR,  0,  4,  8},
    {END,   3,  0,  0},
    {0, 0,  0,  0}},
                
    {{LEADER,   2,  0,  0},
    {CUSTOM,    0,  0,  8},
    {CUSTOM,    0,  1,  8},
    {CUSTOM,    0,  2,  8},
    {CUSTOM,    0,  3,  1},
    {DATA,  0,  4,  8},
    {DATA+BAR,  0,  4,  8},
    {END,   3,  0,  0},
    {0, 0,  0,  0}}},
                
    //T2D_CON(UNKNOWN4),            
    {{{TOGGLE,  0,  0,  0},
    {LEADER,    2,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}},
                
    {{TOGGLE,   0,  0,  0},
    {LEADER,    2,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0},
    {0, 0,  0,  0}}},
};

#endif


unsigned int tvdb_size = (sizeof(TV_DbForm)/sizeof(TV_DbForm[0]));
unsigned int stbdb_size = (sizeof(STB_DbForm)/sizeof(STB_DbForm[0]));

