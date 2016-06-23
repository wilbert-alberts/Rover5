#ifndef REG_H
#define REG_H

#include <stdint.h>

#define REG_MICROS  0
#define REG_MILLIS  1
#define REG_LEFTDIR 2
#define REG_LEFTDC  3
#define REG_RIGHTDIR  4
#define REG_RIGHTDC 5
#define REG_COLLISION 6
#define REG_LINE  7
#define REG_LEFTPOS 8
#define REG_RIGHTPOS  9
#define REG_AMBOFFSET 10
#define REG_AMB_COL_NE  11
#define REG_AMB_COL_SE  12
#define REG_AMB_COL_SW  13
#define REG_AMB_COL_NW  14
#define REG_AMB_LINE_NE 15
#define REG_AMB_LINE_EN 16
#define REG_AMB_LINE_ES 17
#define REG_AMB_LINE_SE 18
#define REG_AMB_LINE_SW 19
#define REG_AMB_LINE_WS 20
#define REG_AMB_LINE_WN 21
#define REG_AMB_LINE_NW 22
#define REG_IR_COL_NE 23
#define REG_IR_COL_SE 24
#define REG_IR_COL_SW 25
#define REG_IR_COL_NW 26
#define REG_IR_LINE_NE  27
#define REG_IR_LINE_EN  28
#define REG_IR_LINE_ES  29
#define REG_IR_LINE_SE  30
#define REG_IR_LINE_SW  31
#define REG_IR_LINE_WS  32
#define REG_IR_LINE_WN  33
#define REG_IR_LINE_NW  34
#define REG_MAX 35

// Mask for the LINE register
#define REG_LINE_NE (1<<0)
#define REG_LINE_EN (1<<1)
#define REG_LINE_ES (1<<2)
#define REG_LINE_SE (1<<3)
#define REG_LINE_SW (1<<4)
#define REG_LINE_WS (1<<5)
#define REG_LINE_WN (1<<6)
#define REG_LINE_NW (1<<7)

// Mask for the COLLISION register
#define REG_COL_NE (1<<0)
#define REG_COL_SE (1<<1)
#define REG_COL_SW (1<<2)
#define REG_COL_NW (1<<3)

typedef struct
{
  uint32_t MICROS;
  uint32_t MILLIS;
  uint8_t  LEFTDIR;
  uint8_t  LEFTDC;
  uint8_t  RIGHTDIR;
  uint8_t  RIGHTDC;
  uint8_t  COLLISION;
  uint8_t  LINE;
  int32_t  LEFTPOS;
  int32_t  RIGHTPOS;
  uint8_t AMBOFFSET;
  uint8_t AMB_LINE_NE;
  uint8_t AMB_LINE_EN;
  uint8_t AMB_LINE_ES;
  uint8_t AMB_LINE_SE;
  uint8_t AMB_LINE_SW;
  uint8_t AMB_LINE_WS;
  uint8_t AMB_LINE_WN;
  uint8_t AMB_LINE_NW;
  uint8_t AMB_COL_NE;
  uint8_t AMB_COL_SE;
  uint8_t AMB_COL_SW;
  uint8_t AMB_COL_NW;
  uint8_t IR_LINE_NE;
  uint8_t IR_LINE_EN;
  uint8_t IR_LINE_ES;
  uint8_t IR_LINE_SE;
  uint8_t IR_LINE_SW;
  uint8_t IR_LINE_WS;
  uint8_t IR_LINE_WN;
  uint8_t IR_LINE_NW;
  uint8_t IR_COL_NE;
  uint8_t IR_COL_SE;
  uint8_t IR_COL_SW;
  uint8_t IR_COL_NW;
} REG_map;


void REG_setup();

void REG_write8(int id, uint8_t val);
void REG_write16(int id, uint8_t val);
void REG_write32(int id, int32_t val);

void REG_read8(int id, uint8_t* dst);
void REG_read16(int id, uint8_t* dst);
void REG_read32(int id, int32_t* dst);

void REG_readAll(REG_map* dst);
void REG_writeAll(REG_map* src);

#endif
