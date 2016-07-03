#include <Arduino.h>

#include "REG.h"

static uint8_t* reg_address[REG_MAX];
static const char* reg_name[REG_MAX];
static REG_map reg_map;

#define REG_ADDREGISTER(R) 							\
  reg_address[REG_ ## R] = (uint8_t*) &reg_map.R; 	\
  reg_name[REG_ ## R] = #R;

#define LOG_U32(R) \
{\
	uint32_t* src = (uint32_t*) (reg_address[R]);        \
	Serial.print(reg_name[R]); \
	Serial.print(": "); \
	Serial.println(*src); \
}

#define LOG_32(R) \
{\
	int32_t* src = (int32_t*) (reg_address[R]);        \
	Serial.print(reg_name[R]); \
  Serial.print(": "); \
	Serial.println(*src); \
}

#define LOG_U16(R) \
{\
	uint16_t* src = (uint16_t*) (reg_address[R]);        \
	Serial.print(reg_name[R]); \
  Serial.print(": "); \
	Serial.println(*src); \
}

#define LOG_U8(R) \
{\
	uint8_t* src = (uint8_t*) (reg_address[R]);        \
	Serial.print(reg_name[R]); \
  Serial.print(": "); \
	Serial.println(*src); \
}


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
  LOG_U32(REG_HEADER);
  LOG_U32(REG_HEADER);
  LOG_U32(REG_MICROS);
  LOG_U32(REG_MILLIS);
  LOG_U8(REG_LEFTDIR);
  LOG_U8(REG_LEFTDC);
  LOG_U8(REG_RIGHTDIR);
  LOG_U8(REG_RIGHTDC);
  LOG_U8(REG_COLLISION);
  LOG_U8(REG_LINE);
  LOG_32(REG_LEFTPOS);
  LOG_32(REG_RIGHTPOS);
  LOG_U16(REG_AMBOFFSET);
  LOG_U16(REG_AMB_LINE_NE);
  LOG_U16(REG_AMB_LINE_EN);
  LOG_U16(REG_AMB_LINE_ES);
  LOG_U16(REG_AMB_LINE_SE);
  LOG_U16(REG_AMB_LINE_SW);
  LOG_U16(REG_AMB_LINE_WS);
  LOG_U16(REG_AMB_LINE_WN);
  LOG_U16(REG_AMB_LINE_NW);
  LOG_U16(REG_AMB_COL_NE);
  LOG_U16(REG_AMB_COL_SE);
  LOG_U16(REG_AMB_COL_SW);
  LOG_U16(REG_AMB_COL_NW);
  LOG_U16(REG_IR_LINE_NE);
  LOG_U16(REG_IR_LINE_EN);
  LOG_U16(REG_IR_LINE_ES);
  LOG_U16(REG_IR_LINE_SE);
  LOG_U16(REG_IR_LINE_SW);
  LOG_U16(REG_IR_LINE_WS);
  LOG_U16(REG_IR_LINE_WN);
  LOG_U16(REG_IR_LINE_NW);
  LOG_U16(REG_IR_COL_NE);
  LOG_U16(REG_IR_COL_SE);
  LOG_U16(REG_IR_COL_SW);
  LOG_U16(REG_IR_COL_NW);
  LOG_U32(REG_TRAILER);
}




