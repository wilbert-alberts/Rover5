#include <Arduino.h>

#include "REG.h"

static uint8_t* reg_address[REG_MAX];
static REG_map reg_map;

extern void REG_setup()
{
  reg_address[REG_MICROS] = (uint8_t*) &reg_map.MICROS;
  reg_address[REG_MILLIS] = (uint8_t*) &reg_map.MILLIS;
  reg_address[REG_LEFTDIR] = (uint8_t*) &reg_map.LEFTDIR;
  reg_address[REG_LEFTDC] = (uint8_t*) &reg_map.LEFTDC;
  reg_address[REG_RIGHTDIR] = (uint8_t*) &reg_map.RIGHTDIR;
  reg_address[REG_RIGHTDC] = (uint8_t*) &reg_map.RIGHTDC;
  reg_address[REG_COLLISION] = (uint8_t*) &reg_map.COLLISION;
  reg_address[REG_LINE] = (uint8_t*) &reg_map.LINE;
  reg_address[REG_LEFTPOS] = (uint8_t*) &reg_map.LEFTPOS;
  reg_address[REG_RIGHTPOS] = (uint8_t*) &reg_map.RIGHTPOS;
  reg_address[REG_AMBOFFSET] = (uint8_t*) &reg_map.AMBOFFSET;
  reg_address[REG_AMB_COL_NE] = (uint8_t*) &reg_map.AMB_COL_NE;
  reg_address[REG_AMB_COL_SE] = (uint8_t*) &reg_map.AMB_COL_SE;
  reg_address[REG_AMB_COL_SW] = (uint8_t*) &reg_map.AMB_COL_SW;
  reg_address[REG_AMB_COL_NW] = (uint8_t*) &reg_map.AMB_COL_NW;
  reg_address[REG_AMB_LINE_NE] = (uint8_t*) &reg_map.AMB_LINE_NE;
  reg_address[REG_AMB_LINE_EN] = (uint8_t*) &reg_map.AMB_LINE_EN;
  reg_address[REG_AMB_LINE_ES] = (uint8_t*) &reg_map.AMB_LINE_ES;
  reg_address[REG_AMB_LINE_SE] = (uint8_t*) &reg_map.AMB_LINE_SE;
  reg_address[REG_AMB_LINE_SW] = (uint8_t*) &reg_map.AMB_LINE_SW;
  reg_address[REG_AMB_LINE_WS] = (uint8_t*) &reg_map.AMB_LINE_WS;
  reg_address[REG_AMB_LINE_WN] = (uint8_t*) &reg_map.AMB_LINE_WN;
  reg_address[REG_AMB_LINE_NW] = (uint8_t*) &reg_map.AMB_LINE_NW;
  reg_address[REG_IR_COL_NE] = (uint8_t*) &reg_map.IR_COL_NE;
  reg_address[REG_IR_COL_SE] = (uint8_t*) &reg_map.IR_COL_SE;
  reg_address[REG_IR_COL_SW] = (uint8_t*) &reg_map.IR_COL_SW;
  reg_address[REG_IR_COL_NW] = (uint8_t*) &reg_map.IR_COL_NW;
  reg_address[REG_IR_LINE_NE] = (uint8_t*) &reg_map.IR_LINE_NE;
  reg_address[REG_IR_LINE_EN] = (uint8_t*) &reg_map.IR_LINE_EN;
  reg_address[REG_IR_LINE_ES] = (uint8_t*) &reg_map.IR_LINE_ES;
  reg_address[REG_IR_LINE_SE] = (uint8_t*) &reg_map.IR_LINE_SE;
  reg_address[REG_IR_LINE_SW] = (uint8_t*) &reg_map.IR_LINE_SW;
  reg_address[REG_IR_LINE_WS] = (uint8_t*) &reg_map.IR_LINE_WS;
  reg_address[REG_IR_LINE_WN] = (uint8_t*) &reg_map.IR_LINE_WN;
  reg_address[REG_IR_LINE_NW] = (uint8_t*) &reg_map.IR_LINE_NW;
}

extern void REG_write8(int id, uint8_t val)
{
  uint8_t* dst = (uint8_t*)(reg_address[id]);
  *dst=val;
}

extern void REG_write16(int id, uint16_t val)
{
  uint16_t* dst = (uint16_t*)(reg_address[id]);
  *dst=val;
}

extern void REG_write32(int id, int32_t val)
{
  uint32_t* dst = (uint32_t*)(reg_address[id]);
  *dst=val;
}

extern void REG_read8(int id, uint8_t* val)
{
  uint8_t* src = (uint8_t*)(reg_address[id]);
  *val=*src;
}

extern void REG_read16(int id, uint16_t* val)
{
  uint16_t* src = (uint16_t*)(reg_address[id]);
  *val=*src;
}

extern void REG_read32(int id, int32_t* val)
{
  uint32_t* src = (uint32_t*)(reg_address[id]);
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




