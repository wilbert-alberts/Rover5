#ifndef RV_H
#define RV_H

/*
 * rv.h - public API for Rover clients.
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

#include <stdint.h>

#define OK (0)
#define RV_ILLEGALVALUE (-1)


#define RV_FORWARD   (0)
#define RV_BACKWARD  (1)

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

extern int RV_LoopLoggingOn();
extern int RV_LoopLoggingOff();
extern int RV_LoggingOn();
extern int RV_LoggingOff();

extern int RV_start();
extern int RV_setFrequency(int herz);

extern int RV_stop();

extern int RV_waitForNewData();

extern int RV_getPosition(long* leftPos, long* rightPos);

extern int RV_move(int leftDirection, // FORWARD, BACKWARD
				   int rightDirection,
				   int leftDC,        // from 0 to 255
				   int rightDC);

extern int RV_getCollision(uint8_t* mask);

extern int RV_getLine(uint8_t* mask);

#endif
