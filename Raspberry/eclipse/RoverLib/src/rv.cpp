#include <stdio.h>
#include <wiringPi.h>
#include "rv_log.h"
#include "rv_exchange.h"
#include "rv_loop.h"
#include "rv_reg.h"
#include "rv_trace.h"
#include "rv_server.h"
#include "rv.h"

#define TRACEBUFFERSIZE (1000)

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
	LG_logEntry(__func__, NULL);

	wiringPiSetup();
	SAFE_INVOKE(EX_setup(), result, RV_START_FAILED)
	SAFE_INVOKE(REG_setup(), result, RV_START_FAILED)
	SAFE_INVOKE(TR_setup(TRACEBUFFERSIZE), result, RV_START_FAILED)
	SAFE_INVOKE(SV_start(), result, RV_START_FAILED)
	SAFE_INVOKE(LP_start(), result, RV_START_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_stop() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	SAFE_INVOKE(LP_stop(), result, RV_STOP_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_loggingOn() {
	int result = OK;
	LG_logEntry(__func__, NULL);
	SAFE_INVOKE(LG_setLogging(pthread_self(), true), result, RV_LOGGING_ON_FAILED)
	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_loggingOff() {
	int result = OK;
	LG_logEntry(__func__, NULL);
	SAFE_INVOKE(LG_setLogging(pthread_self(), false), result, RV_LOGGING_OFF_FAILED)
	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_getPosition(long* left, long* right) {
	int32_t lft = 0;
	int32_t rgt = 0;

	int result = OK;
	LG_logEntry(__func__, "left: %p, right: %p", left, right);

	SAFE_INVOKE(REG_read32(REG_LEFTPOS, &lft), result, RV_GET_POSITION_FAILED)
	SAFE_INVOKE(REG_read32(REG_RIGHTPOS, &rgt), result, RV_GET_POSITION_FAILED)

	if (result == OK) {
		*left = (long) lft;
		*right = (long) rgt;
	}

	LG_logExit(__func__, result, "*left: %d, *right: %d", *left, *right);
	return result;
}

extern int RV_move(int leftDirection, int rightDirection, int leftDC,
		int rightDC) {
	uint8_t ld;
	uint8_t rd;
	uint8_t ldc;
	uint8_t rdc;

	int result = OK;
	LG_logEntry(__func__,
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

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_getLine(uint8_t* r) {
	int result = OK;
	LG_logEntry(__func__, "r: %p", r);

	SAFE_INVOKE(REG_read8(REG_LINE, r), result, RV_GET_LINE_FAILED)

	LG_logExit(__func__, result, "*r: 0x%0x", r);
	return result;
}

extern int RV_getCollision(uint8_t* r) {
	int result = OK;
	LG_logEntry(__func__, "r: %p", r);

	SAFE_INVOKE(REG_read8(REG_COLLISION, r), result, RV_GET_COLLISION_FAILED)

	LG_logExit(__func__, result, "*r: 0x%0x", r);
	return result;
}

extern int RV_getColAmbOffset(int* r) {
	int result = OK;
	LG_logEntry(__func__, "r: %p", r);

	uint16_t o;

	SAFE_INVOKE(REG_read16(REG_AMB_COL_OFFSET, &o), result, RV_GET_AMB_COL_OFFSET_FAILED)
	if (result == OK) {
		*r = o;
	}
	LG_logExit(__func__, result, "*r: %d", r);
	return result;
}

extern int RV_setColAmbOffset(int r) {
	int result = OK;
	uint16_t offset = (uint16_t)(r);

	LG_logEntry(__func__, "r: %d", r);

	SAFE_INVOKE(REG_write16(REG_AMB_COL_OFFSET, offset), result, RV_SET_AMB_COL_OFFSET_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_getLineAmbOffset(uint16_t* r) {
	int result = OK;
	uint16_t o;
	LG_logEntry(__func__, "r: %p", r);

	SAFE_INVOKE(REG_read16(REG_AMB_LINE_OFFSET, &o), result, RV_GET_AMB_LINE_OFFSET_FAILED)

	if (result == OK) {
		*r = o;
	}

	LG_logExit(__func__, result, "*r: %d", r);
	return result;
}

extern int RV_setLineAmbOffset(uint16_t r) {
	int result = OK;
	uint16_t offset = (uint16_t)(r);

	LG_logEntry(__func__, "r: %d", r);

	SAFE_INVOKE(REG_write16(REG_AMB_LINE_OFFSET, offset), result, RV_SET_AMB_LINE_OFFSET_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}


extern int RV_getAVRTime(long* millis, long* micros)
{
	int result = OK;
	LG_logEntry(__func__, "millis: %p, micros: %p", millis, micros);

	int32_t ml;
	int32_t ms;

	SAFE_INVOKE(REG_read32(REG_MILLIS, &ml), result, RV_GET_AVRTIME_FAILED)
	SAFE_INVOKE(REG_read32(REG_MICROS, &ms), result, RV_GET_AVRTIME_FAILED)
	
	if (result == OK) {
		*millis = ml;
		*micros = ms;
	}
	
	LG_logExit(__func__, result, "*millis: %d, *micros: %d", *millis, *micros);
	return result;
}

extern int RV_getAnalogLine(int ambient[], int active[])
{
	int result = OK;
	uint16_t v;
	
	static const int ambientRegisters[]  =  {
    REG_AMB_LINE_N,
    REG_AMB_LINE_E,
    REG_AMB_LINE_S,
    REG_AMB_LINE_W,
	};
	static const int activeRegisters[]  =  {
    REG_IR_LINE_N,
    REG_IR_LINE_E,
    REG_IR_LINE_S,
    REG_IR_LINE_W,
	};
	LG_logEntry(__func__, "NOT IMPLEMENTED");

	for (unsigned int i=0; i< sizeof(ambientRegisters)/sizeof(int); i++) {
		SAFE_INVOKE(REG_read16(ambientRegisters[i], &v), result, RV_GET_ANALOGLINE_FAILED)
		if (result == OK) {
			ambient[i] = v;
		}
	}

	for (unsigned int i=0; i< sizeof(activeRegisters)/sizeof(int); i++) {
		SAFE_INVOKE(REG_read16(activeRegisters[i], &v), result, RV_GET_ANALOGLINE_FAILED)
		if (result == OK) {
			active[i] = v;
		}
	}

	LG_logExit(__func__, result, "NO RESULT");
	return result;
}

extern int RV_getAnalogCollision(int ambient[] ,int active[])
{
	int result = OK;
	uint16_t v;

	static const int ambientRegisters[]  =  { REG_AMB_COL_NE, REG_AMB_COL_SE, REG_AMB_COL_SW, REG_AMB_COL_NW };
	static const int activeRegisters[]  = { REG_IR_COL_NE, REG_IR_COL_SE, REG_IR_COL_SW, REG_IR_COL_NW };
	LG_logEntry(__func__, "NOT IMPLEMENTED");

	for (unsigned int i=0; i< sizeof(ambientRegisters)/sizeof(int); i++) {
		SAFE_INVOKE(REG_read16(ambientRegisters[i], &v), result, RV_GET_ANALOGCOLLISION_FAILED)
		if (result == OK) {
			ambient[i] = v;
		}
	}

	for (unsigned int i=0; i< sizeof(activeRegisters)/sizeof(int); i++) {
		SAFE_INVOKE(REG_read16(activeRegisters[i], &v), result, RV_GET_ANALOGCOLLISION_FAILED)
		if (result == OK) {
			active[i] = v;
		}
	}

	LG_logExit(__func__, result, "NO RESULT");
	return result;
}

extern int RV_waitForNewData()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	result = LP_waitForNewData();

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_setFrequency(int herz)
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	result = LP_setFrequency(herz);

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int RV_loopLoggingOn()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	result = LP_loggingOn();

	LG_logExit(__func__, result, NULL);
	return result;
}
extern int RV_loopLoggingOff()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	result = LP_loggingOff();

	LG_logExit(__func__, result, NULL);
	return result;
}
