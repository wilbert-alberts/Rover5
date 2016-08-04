#ifndef REG_H
#define REG_H

/*
 * rv_reg.h - operations to manipulate the registermap
 *
 * This module provides functionality to read and write
 * registers to the registermap. It also maintains the
 * current copy of the registermap.
 *
 * Access to the registers via the regular read/write operations
 * is proctected by a semaphore in order to make this module thread
 * safe.
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */


/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
 */

#include <stdint.h>

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */

/* Each register is identified by one of the following defines. */
#define REG_HEADER          (0)
#define REG_MICROS          (1)
#define REG_MILLIS          (2)
#define REG_LEFTDIR         (3)
#define REG_LEFTDC          (4)
#define REG_RIGHTDIR        (5)
#define REG_RIGHTDC         (6)
#define REG_LEFTPOS         (7)
#define REG_RIGHTPOS        (8)
#define REG_AMB_COL_NE      (9)
#define REG_AMB_COL_SE      (10)
#define REG_AMB_COL_SW      (11)
#define REG_AMB_COL_NW      (12)
#define REG_AMB_LINE_N      (13)
#define REG_AMB_LINE_E      (14)
#define REG_AMB_LINE_S      (15)
#define REG_AMB_LINE_W      (16)
#define REG_IR_COL_NE       (17)
#define REG_IR_COL_SE       (18)
#define REG_IR_COL_SW       (19)
#define REG_IR_COL_NW       (20)
#define REG_IR_LINE_N       (21)
#define REG_IR_LINE_E       (22)
#define REG_IR_LINE_S       (23)
#define REG_IR_LINE_W       (24)
#define REG_TRAILER         (25)
#define REG_MAX             (26)

/*
 * ---------------------------------------------------------------------------
 *               Type definitions
 * ---------------------------------------------------------------------------
 */

/* The REG_map struct actually represents the registermap. Each register
 * is represented by a collection of bytes. Note that the layout of this
 * structure is shared with the AVR and any other application that requires
 * manipulating it.
 *
 * The structure starts with a register representing a header (and ends
 * with a registers representing a trailer). These registers are expected
 * to be filled with a magical number used to verify integrity of the data
 * exchange.
 */
typedef struct
{
	  uint8_t HEADER[4];
	  uint8_t MICROS[4];
	  uint8_t MILLIS[4];
	  uint8_t LEFTDIR;
	  uint8_t LEFTDC;
	  uint8_t RIGHTDIR;
	  uint8_t RIGHTDC;
	  int8_t  LEFTPOS[4];
	  int8_t  RIGHTPOS[4];
	  uint8_t AMB_LINE_N[2];
	  uint8_t AMB_LINE_E[2];
	  uint8_t AMB_LINE_S[2];
	  uint8_t AMB_LINE_W[2];
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

/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */


/* Initialize the register map administration. It initializes
 * a map allowing efficient reading and writing of registers
 * and it also initializes a map allowing retrieval of
 * register names by register index.
 */
extern int REG_setup();

/* The following write operations do exactly that, they write
 * a value in one of the registers. The differences result
 * from the size of the data being written.
 */
extern int REG_write8(int id, uint8_t val);
extern int REG_write16(int id, uint16_t val);
extern int REG_write32(int id, int32_t val);

/* The following read operations do exactly that, they read
 * a value from one of the registers. The differences result
 * from the size of the data being read.
 */
extern int REG_read8(int id, uint8_t* dst);
extern int REG_read16(int id, uint16_t* dst);
extern int REG_read32(int id, int32_t* dst);

/* REG_readLong reads an arbitrary register from the registermap
 * and stores it into a long variable. This allows generic handling
 * of register values for instance for tracing. Note however that
 * this operation contains a costly switch statement and should
 * not be used when other REG_read operations can be used.
 */
 extern int REG_readLong(REG_map* src, int id, long* val);

 /* REG_readAll copies the current register map into the one
  * referred to by 'dst'.
  */
extern int REG_readAll(REG_map* dst);

/* REG_writeAll copies the content of the registermap referred to by
 * src into the current registermap.
 */
extern int REG_writeAll(REG_map* src);

/* REG_logAll dumps the content of the registers represented by
 * src to stdout.
 */
extern void REG_logAll(REG_map* src);

/* REG_getRegisterName returns a char* to the name of the register
 * represented by idx.
 */
extern const char* REG_getRegistername(int idx);

#endif
