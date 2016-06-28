#include <stdio.h>
#include "rv_log.h"
#include "rv_exchange.h"
#include "rv_loop.h"
#include "rv_reg.h"
#include "rv.h"

#define rv_IntToDirection(d,i) 	\
{ 								\
	switch (i) { 				\
	case RV_FORWARD: 			\
	case RV_BACKWARD:			\
		d = (uint8_t) (i); 		\
		break; 					\
	default: 					\
		return RV_ILLEGALVALUE; \
	} 							\
}

#define rv_IntToDC(dc, i) 		\
{								\
	if ((i<0) || (i>255))     	\
      return RV_ILLEGALVALUE; 	\
    dc = (uint8_t) (i);			\
}


extern int RV_start() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	RV_exchangeSetup();
	RV_startLoop();
	REG_setup();

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_stop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	RV_stopLoop();

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_LoggingOn()
{
	RV_SetLogging(pthread_self(), true);
}
extern int RV_LoggingOff()
{
	RV_SetLogging(pthread_self(), false);
}

extern int RV_getPosition(long* left, long* right) {
	int32_t lft;
	int32_t rgt;

	int result = OK;
	RV_LogEntry(__func__, "left: %p, right: %p", left, right);

	REG_read32(REG_LEFTPOS, &lft);
	REG_read32(REG_RIGHTPOS, &rgt);

	*left = (long) lft;
	*right = (long) rgt;

	RV_LogExit(__func__, result, "*left: %d, *right: %d", *left, *right);
	return result;
}

extern int RV_move(int leftDirection, int rightDirection, int leftDC, int rightDC) {
	uint8_t ld;
	uint8_t rd;
	uint8_t ldc;
	uint8_t rdc;

	int result = OK;
	RV_LogEntry(__func__, "leftDirection: %d, rightDirection: %d, leftDC: %d, rightDC: %d",
			leftDirection, rightDirection, leftDC, rightDC);

	rv_IntToDirection(ld, leftDirection)
	rv_IntToDirection(rd, rightDirection)
	rv_IntToDC(ldc, leftDC)
	rv_IntToDC(rdc, rightDC)

	REG_write8(REG_LEFTDIR, ld);
	REG_write8(REG_LEFTDC, ldc);
	REG_write8(REG_RIGHTDIR, rd);
	REG_write8(REG_RIGHTDC, rdc);

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_getLine(uint8_t* r) {
	int result = OK;
	RV_LogEntry(__func__, "r: %p", r);

	REG_read8(REG_LINE, r);

	RV_LogExit(__func__, result, "*r: 0x%0x", r);
	return result;
}

extern int RV_getCollision(uint8_t* r) {
	int result = OK;
	RV_LogEntry(__func__, "r: %p", r);

	REG_read8(REG_COLLISION, r);

	RV_LogExit(__func__, result, "*r: 0x%0x", r);
	return result;
}
