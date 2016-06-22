#ifndef REG_H
#define REG_H

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
  unsigned long MICROS;
  unsigned long MILLIS;
  bool  LEFTDIR;
  byte  LEFTDC;
  bool  RIGHTDIR;
  byte  RIGHTDC;
  byte  COLLISION;
  byte  LINE;
  long  LEFTPOS;
  long  RIGHTPOS;
  short AMBOFFSET;
  short AMB_LINE_NE;
  short AMB_LINE_EN;
  short AMB_LINE_ES;
  short AMB_LINE_SE;
  short AMB_LINE_SW;
  short AMB_LINE_WS;
  short AMB_LINE_WN;
  short AMB_LINE_NW;
  short AMB_COL_NE;
  short AMB_COL_SE;
  short AMB_COL_SW;
  short AMB_COL_NW;
  short IR_LINE_NE;
  short IR_LINE_EN;
  short IR_LINE_ES;
  short IR_LINE_SE;
  short IR_LINE_SW;
  short IR_LINE_WS;
  short IR_LINE_WN;
  short IR_LINE_NW;
  short IR_COL_NE;
  short IR_COL_SE;
  short IR_COL_SW;
  short IR_COL_NW;
} REG_map;


void REG_setup();

void REG_write8(int id, byte val);
void REG_write16(int id, short val);
void REG_write32(int id, long val);

void REG_read8(int id, byte* dst);
void REG_read16(int id, short* dst);
void REG_read32(int id, long* dst);

void REG_readAll(REG_map* dst);
void REG_writeAll(REG_map* src);

#endif
