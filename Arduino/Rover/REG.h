#ifndef REG_H
#define REG_H

#include <stdint.h>

#define REG_HEADER        (0)
#define REG_MICROS        (1)
#define REG_MILLIS        (2)
#define REG_LEFTDIR       (3)
#define REG_LEFTDC        (4)
#define REG_RIGHTDIR      (5)
#define REG_RIGHTDC       (6)
#define REG_COLLISION     (7)
#define REG_LINE          (8)
#define REG_LEFTPOS       (9)
#define REG_RIGHTPOS      (10)
#define REG_AMBOFFSET     (11)
#define REG_AMB_COL_NE    (12)
#define REG_AMB_COL_SE    (13)
#define REG_AMB_COL_SW    (14)
#define REG_AMB_COL_NW    (15)
#define REG_AMB_LINE_NE   (16)
#define REG_AMB_LINE_EN   (17)
#define REG_AMB_LINE_ES   (18)
#define REG_AMB_LINE_SE   (19)
#define REG_AMB_LINE_SW   (20)
#define REG_AMB_LINE_WS   (21)
#define REG_AMB_LINE_WN   (22)
#define REG_AMB_LINE_NW   (23)
#define REG_IR_COL_NE     (24)
#define REG_IR_COL_SE     (25)
#define REG_IR_COL_SW     (26)
#define REG_IR_COL_NW     (27)
#define REG_IR_LINE_NE    (28)
#define REG_IR_LINE_EN    (29)
#define REG_IR_LINE_ES    (30)
#define REG_IR_LINE_SE    (31)
#define REG_IR_LINE_SW    (32)
#define REG_IR_LINE_WS    (33)
#define REG_IR_LINE_WN    (34)
#define REG_IR_LINE_NW    (35)
#define REG_IR_TRAILER    (35)
#define REG_MAX           (36)

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
  uint32_t HEADER;
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
  uint16_t AMBOFFSET;
  uint16_t AMB_LINE_NE;
  uint16_t AMB_LINE_EN;
  uint16_t AMB_LINE_ES;
  uint16_t AMB_LINE_SE;
  uint16_t AMB_LINE_SW;
  uint16_t AMB_LINE_WS;
  uint16_t AMB_LINE_WN;
  uint16_t AMB_LINE_NW;
  uint16_t AMB_COL_NE;
  uint16_t AMB_COL_SE;
  uint16_t AMB_COL_SW;
  uint16_t AMB_COL_NW;
  uint16_t IR_LINE_NE;
  uint16_t IR_LINE_EN;
  uint16_t IR_LINE_ES;
  uint16_t IR_LINE_SE;
  uint16_t IR_LINE_SW;
  uint16_t IR_LINE_WS;
  uint16_t IR_LINE_WN;
  uint16_t IR_LINE_NW;
  uint16_t IR_COL_NE;
  uint16_t IR_COL_SE;
  uint16_t IR_COL_SW;
  uint16_t IR_COL_NW;
  uint32_t TRAILER;
} REG_map;


extern void REG_setup();
extern void REG_loop();

extern void REG_write8(int id, uint8_t val);
extern void REG_write16(int id, uint16_t val);
extern void REG_write32(int id, int32_t val);

extern void REG_read8(int id, uint8_t* dst);
extern void REG_read16(int id, uint16_t* dst);
extern void REG_read32(int id, int32_t* dst);

extern void REG_readAll(REG_map* dst);
extern void REG_writeAll(REG_map* src);

extern void REG_logAll(REG_map* src);
#endif
