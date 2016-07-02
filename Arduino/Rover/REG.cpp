#include <Arduino.h>

#include "REG.h"

static uint8_t* reg_address[REG_MAX];
static const char* reg_name[REG_MAX];
static REG_map reg_map;

#define REG_ADDREGISTER(R) 							\
  reg_address[REG_ ## R] = (uint8_t*) &reg_map.R; 	\
  reg_name[REG_ ## R] = #R;

#define REG_LOGREGISTER(R) 					\
	    Serial.print(reg_name[REG_ ## R]); 	\
	    Serial.print(": ");					\
	    Serial.println(src->R); 			\


extern void REG_setup()
{
  REG_ADDREGISTER(HEADER)
  REG_ADDREGISTER(MICROS)
  REG_ADDREGISTER(MILLIS)
  REG_ADDREGISTER(LEFTDIR)
  REG_ADDREGISTER(LEFTDC)
  REG_ADDREGISTER(RIGHTDIR)
  REG_ADDREGISTER(RIGHTDC)
  REG_ADDREGISTER(COLLISION)
  REG_ADDREGISTER(LINE)
  REG_ADDREGISTER(LEFTPOS)
  REG_ADDREGISTER(RIGHTPOS)
  REG_ADDREGISTER(AMBOFFSET)
  REG_ADDREGISTER(AMB_COL_NE)
  REG_ADDREGISTER(AMB_COL_SE)
  REG_ADDREGISTER(AMB_COL_SW)
  REG_ADDREGISTER(AMB_COL_NW)
  REG_ADDREGISTER(AMB_LINE_NE)
  REG_ADDREGISTER(AMB_LINE_EN)
  REG_ADDREGISTER(AMB_LINE_ES)
  REG_ADDREGISTER(AMB_LINE_SE)
  REG_ADDREGISTER(AMB_LINE_SW)
  REG_ADDREGISTER(AMB_LINE_WS)
  REG_ADDREGISTER(AMB_LINE_WN)
  REG_ADDREGISTER(AMB_LINE_NW)
  REG_ADDREGISTER(IR_COL_NE)
  REG_ADDREGISTER(IR_COL_SE)
  REG_ADDREGISTER(IR_COL_SW)
  REG_ADDREGISTER(IR_COL_NW)
  REG_ADDREGISTER(IR_LINE_NE)
  REG_ADDREGISTER(IR_LINE_EN)
  REG_ADDREGISTER(IR_LINE_ES)
  REG_ADDREGISTER(IR_LINE_SE)
  REG_ADDREGISTER(IR_LINE_SW)
  REG_ADDREGISTER(IR_LINE_WS)
  REG_ADDREGISTER(IR_LINE_WN)
  REG_ADDREGISTER(IR_LINE_NW)
  REG_ADDREGISTER(TRAILER)
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

extern void REG_loop()
{
  REG_write32(REG_MICROS, micros());
  REG_write32(REG_MILLIS, millis());

  REG_logAll(&reg_map);
}

extern void REG_logAll(REG_map* src)
{
  Serial.println("reg_map: ");
  REG_LOGREGISTER(MICROS)
  REG_LOGREGISTER(MILLIS)
  REG_LOGREGISTER(LEFTDIR)
  REG_LOGREGISTER(LEFTDC)
  REG_LOGREGISTER(RIGHTDIR)
  REG_LOGREGISTER(RIGHTDC)
  REG_LOGREGISTER(COLLISION)
  REG_LOGREGISTER(LINE)
  REG_LOGREGISTER(LEFTPOS)
  REG_LOGREGISTER(RIGHTPOS)
  REG_LOGREGISTER(AMBOFFSET)
  REG_LOGREGISTER(AMB_COL_NE)
  REG_LOGREGISTER(AMB_COL_SE)
  REG_LOGREGISTER(AMB_COL_SW)
  REG_LOGREGISTER(AMB_COL_NW)
  REG_LOGREGISTER(AMB_LINE_NE)
  REG_LOGREGISTER(AMB_LINE_EN)
  REG_LOGREGISTER(AMB_LINE_ES)
  REG_LOGREGISTER(AMB_LINE_SE)
  REG_LOGREGISTER(AMB_LINE_SW)
  REG_LOGREGISTER(AMB_LINE_WS)
  REG_LOGREGISTER(AMB_LINE_WN)
  REG_LOGREGISTER(AMB_LINE_NW)
  REG_LOGREGISTER(IR_COL_NE)
  REG_LOGREGISTER(IR_COL_SE)
  REG_LOGREGISTER(IR_COL_SW)
  REG_LOGREGISTER(IR_COL_NW)
  REG_LOGREGISTER(IR_LINE_NE)
  REG_LOGREGISTER(IR_LINE_EN)
  REG_LOGREGISTER(IR_LINE_ES)
  REG_LOGREGISTER(IR_LINE_SE)
  REG_LOGREGISTER(IR_LINE_SW)
  REG_LOGREGISTER(IR_LINE_WS)
  REG_LOGREGISTER(IR_LINE_WN)
  REG_LOGREGISTER(IR_LINE_NW)
}



