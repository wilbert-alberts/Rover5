#include <Arduino.h>

#include "REG.h"

static byte* reg_address[REG_MAX];
static REG_map reg_map;

extern void REG_setup()
{
  reg_address[REG_MICROS] = (byte*) &reg_map.MICROS;
  reg_address[REG_MILLIS] = (byte*) &reg_map.MILLIS;
  reg_address[REG_LEFTDIR] = (byte*) &reg_map.LEFTDIR;
  reg_address[REG_LEFTDC] = (byte*) &reg_map.LEFTDC;
  reg_address[REG_RIGHTDIR] = (byte*) &reg_map.RIGHTDIR;
  reg_address[REG_RIGHTDC] = (byte*) &reg_map.RIGHTDC;
  reg_address[REG_COLLISION] = (byte*) &reg_map.COLLISION;
  reg_address[REG_LINE] = (byte*) &reg_map.LINE;
  reg_address[REG_LEFTPOS] = (byte*) &reg_map.LEFTPOS;
  reg_address[REG_RIGHTPOS] = (byte*) &reg_map.RIGHTPOS;
  reg_address[REG_AMBOFFSET] = (byte*) &reg_map.AMBOFFSET;
  reg_address[REG_AMB_COL_NE] = (byte*) &reg_map.AMB_COL_NE;
  reg_address[REG_AMB_COL_SE] = (byte*) &reg_map.AMB_COL_SE;
  reg_address[REG_AMB_COL_SW] = (byte*) &reg_map.AMB_COL_SW;
  reg_address[REG_AMB_COL_NW] = (byte*) &reg_map.AMB_COL_NW;
  reg_address[REG_AMB_LINE_NE] = (byte*) &reg_map.AMB_LINE_NE;
  reg_address[REG_AMB_LINE_EN] = (byte*) &reg_map.AMB_LINE_EN;
  reg_address[REG_AMB_LINE_ES] = (byte*) &reg_map.AMB_LINE_ES;
  reg_address[REG_AMB_LINE_SE] = (byte*) &reg_map.AMB_LINE_SE;
  reg_address[REG_AMB_LINE_SW] = (byte*) &reg_map.AMB_LINE_SW;
  reg_address[REG_AMB_LINE_WS] = (byte*) &reg_map.AMB_LINE_WS;
  reg_address[REG_AMB_LINE_WN] = (byte*) &reg_map.AMB_LINE_WN;
  reg_address[REG_AMB_LINE_NW] = (byte*) &reg_map.AMB_LINE_NW;
  reg_address[REG_IR_COL_NE] = (byte*) &reg_map.IR_COL_NE;
  reg_address[REG_IR_COL_SE] = (byte*) &reg_map.IR_COL_SE;
  reg_address[REG_IR_COL_SW] = (byte*) &reg_map.IR_COL_SW;
  reg_address[REG_IR_COL_NW] = (byte*) &reg_map.IR_COL_NW;
  reg_address[REG_IR_LINE_NE] = (byte*) &reg_map.IR_LINE_NE;
  reg_address[REG_IR_LINE_EN] = (byte*) &reg_map.IR_LINE_EN;
  reg_address[REG_IR_LINE_ES] = (byte*) &reg_map.IR_LINE_ES;
  reg_address[REG_IR_LINE_SE] = (byte*) &reg_map.IR_LINE_SE;
  reg_address[REG_IR_LINE_SW] = (byte*) &reg_map.IR_LINE_SW;
  reg_address[REG_IR_LINE_WS] = (byte*) &reg_map.IR_LINE_WS;
  reg_address[REG_IR_LINE_WN] = (byte*) &reg_map.IR_LINE_WN;
  reg_address[REG_IR_LINE_NW] = (byte*) &reg_map.IR_LINE_NW;
}

extern void REG_write8(int id, byte val)
{
  byte* dst = (byte*)(reg_address[id]);
  *dst=val;
}

extern void REG_write16(int id, short val)
{
  short* dst = (short*)(reg_address[id]);
  *dst=val;
}

extern void REG_write32(int id, long val)
{
  long* dst = (long*)(reg_address[id]);
  *dst=val;
}

extern void REG_read8(int id, byte* val)
{
  byte* src = (byte*)(reg_address[id]);
  *val=*src;
}

extern void REG_read16(int id, short* val)
{
  short* src = (short*)(reg_address[id]);
  *val=*src;
}

extern void REG_read32(int id, long* val)
{
  long* src = (long*)(reg_address[id]);
  *val=*src;
}

extern void REG_readAll(REG_map* dst)
{
  *dst = reg_map;
}

extern void REG_writeAll(REG_map* src)
{
  reg_map = *src;
}




