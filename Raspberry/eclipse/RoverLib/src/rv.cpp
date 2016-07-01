#include <stdio.h>
#include "rv_log.h"
#include "rv_exchange.h"
#include "rv_loop.h"
#include "rv_reg.h"
#include "rv.h"

#define rv_IntToDirection(d,i,r)\
{ 								\
	if (r == OK) {				\
	switch (i) { 				\
	case RV_FORWARD: 			\
	case RV_BACKWARD:			\
		d = (uint8_t) (i); 		\
		break; 					\
	default: 					\
		r = RV_ILLEGALVALUE;    \
		break;					\
	} 							\
	} 							\
}

#define rv_IntToDC(dc, i,r) 	\
{								\
	if (r == OK) {				\
	if ((i<0) || (i>255))     	\
		r = RV_ILLEGALVALUE; 	\
    else						\
    	dc = (uint8_t) (i);		\
	}							\
}

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}

extern int RV_start() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	SAFE_INVOKE(RV_exchangeSetup(), result, RV_START_FAILED)
	SAFE_INVOKE(RV_startLoop(), result, RV_START_FAILED)
	SAFE_INVOKE(REG_setup(), result, RV_START_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_stop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	SAFE_INVOKE(RV_stopLoop(), result, RV_STOP_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_loggingOn() {
	int result = OK;
	RV_LogEntry(__func__, NULL);
	SAFE_INVOKE(RV_SetLogging(pthread_self(), true), result, RV_LOGGING_ON_FAILED)
	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_loggingOff() {
	int result = OK;
	RV_LogEntry(__func__, NULL);
	SAFE_INVOKE(RV_SetLogging(pthread_self(), false), result, RV_LOGGING_OFF_FAILED)
	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_getPosition(long* left, long* right) {
	int32_t lft = 0;
	int32_t rgt = 0;

	int result = OK;
	RV_LogEntry(__func__, "left: %p, right: %p", left, right);

	SAFE_INVOKE(REG_read32(REG_LEFTPOS, &lft), result, RV_GET_POSITION_FAILED)
	SAFE_INVOKE(REG_read32(REG_RIGHTPOS, &rgt), result, RV_GET_POSITION_FAILED)

	if (result == OK) {
		*left = (long) lft;
		*right = (long) rgt;
	}

	RV_LogExit(__func__, result, "*left: %d, *right: %d", *left, *right);
	return result;
}

extern int RV_move(int leftDirection, int rightDirection, int leftDC,
		int rightDC) {
	uint8_t ld;
	uint8_t rd;
	uint8_t ldc;
	uint8_t rdc;

	int result = OK;
	RV_LogEntry(__func__,
			"leftDirection: %d, rightDirection: %d, leftDC: %d, rightDC: %d",
			leftDirection, rightDirection, leftDC, rightDC);

	rv_IntToDirection(ld, leftDirection, result)
	rv_IntToDirection(rd, rightDirection, result)
	rv_IntToDC(ldc, leftDC, result)
	rv_IntToDC(rdc, rightDC, result)

	SAFE_INVOKE(REG_write8(REG_LEFTDIR, ld), result, RV_MOVE_FAILED)
	SAFE_INVOKE(REG_write8(REG_LEFTDC, ldc), result, RV_MOVE_FAILED)
	SAFE_INVOKE(REG_write8(REG_RIGHTDIR, rd), result, RV_MOVE_FAILED)
	SAFE_INVOKE(REG_write8(REG_RIGHTDC, rdc), result, RV_MOVE_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_getLine(uint8_t* r) {
	int result = OK;
	RV_LogEntry(__func__, "r: %p", r);

	SAFE_INVOKE(REG_read8(REG_LINE, r), result, RV_GET_LINE_FAILED)

	RV_LogExit(__func__, result, "*r: 0x%0x", r);
	return result;
}

extern int RV_getCollision(uint8_t* r) {
	int result = OK;
	RV_LogEntry(__func__, "r: %p", r);

	SAFE_INVOKE(REG_read8(REG_COLLISION, r), result, RV_GET_COLLISION_FAILED)

	RV_LogExit(__func__, result, "*r: 0x%0x", r);
	return result;
}
