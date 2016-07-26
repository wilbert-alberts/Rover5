#ifndef REG_H
#define REG_H

#include <stdint.h>

#define REG_HEADER          (0)
#define REG_MICROS          (1)
#define REG_MILLIS          (2)
#define REG_LEFTDIR         (3)
#define REG_LEFTDC          (4)
#define REG_RIGHTDIR        (5)
#define REG_RIGHTDC         (6)
#define REG_COLLISION       (7)
#define REG_LINE            (8)
#define REG_LEFTPOS         (9)
#define REG_RIGHTPOS        (10)
#define REG_AMB_COL_OFFSET  (11)
#define REG_AMB_COL_NE      (12)
#define REG_AMB_COL_SE      (13)
#define REG_AMB_COL_SW      (14)
#define REG_AMB_COL_NW      (15)
#define REG_AMB_LINE_OFFSET (16)
#define REG_AMB_LINE_N      (17)
#define REG_AMB_LINE_E      (18)
#define REG_AMB_LINE_S      (19)
#define REG_AMB_LINE_W      (20)
#define REG_IR_COL_NE       (21)
#define REG_IR_COL_SE       (22)
#define REG_IR_COL_SW       (23)
#define REG_IR_COL_NW       (24)
#define REG_IR_LINE_N       (25)
#define REG_IR_LINE_E       (26)
#define REG_IR_LINE_S       (27)
#define REG_IR_LINE_W       (28)
#define REG_TRAILER         (29)
#define REG_MAX             (30)

// Mask for the LINE register
#define REG_LINE_N (1<<0)
#define REG_LINE_E (1<<1)
#define REG_LINE_S (1<<2)
#define REG_LINE_W (1<<3)

// Mask for the COLLISION register
#define REG_COL_NE (1<<0)
#define REG_COL_SE (1<<1)
#define REG_COL_SW (1<<2)
#define REG_COL_NW (1<<3)

typedef struct
{
	  uint8_t HEADER[4];
	  uint8_t MICROS[4];
	  uint8_t MILLIS[4];
	  uint8_t LEFTDIR;
	  uint8_t LEFTDC;
	  uint8_t RIGHTDIR;
	  uint8_t RIGHTDC;
	  uint8_t COLLISION;
	  uint8_t LINE;
	  int8_t  LEFTPOS[4];
	  int8_t  RIGHTPOS[4];
	  uint8_t AMB_LINE_OFFSET[2];
	  uint8_t AMB_LINE_N[2];
	  uint8_t AMB_LINE_E[2];
	  uint8_t AMB_LINE_S[2];
	  uint8_t AMB_LINE_W[2];
	  uint8_t AMB_COL_OFFSET[2];
	  uint8_t AMB_COL_NE[2];
	  uint8_t AMB_COL_SE[2];
	  uint8_t AMB_COL_SW[2];
	  uint8_t AMB_COL_NW[2];
	  uint8_t IR_LINE_N[2];
	  uint8_t IR_LINE_E[2];
	  uint8_t IR_LINE_S[2];
	  uint8_t IR_LINE_W[2];
	  uint8_t IR_COL_NE[2];
	  uint8_t IR_COL_SE[2];
	  uint8_t IR_COL_SW[2];
	  uint8_t IR_COL_NW[2];
	  uint8_t TRAILER[4];
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
