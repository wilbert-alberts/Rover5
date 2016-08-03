#include <stdio.h>
#include <wiringPi.h>
#include "rv_log.h"
#include "rv_filter.h"
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
    SAFE_INVOKE(RV_initLineSensorFilters(), result, RV_START_FAILED)
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

extern int RV_getLineSensors(RV_LineSensors* lineSensors)
{
	int result = OK;
	uint16_t v;
	

	static const int registers[]  =  {
        REG_AMB_LINE_N,
        REG_AMB_LINE_E,
        REG_AMB_LINE_S,
        REG_AMB_LINE_W,
        REG_IR_LINE_N,
        REG_IR_LINE_E,
        REG_IR_LINE_S,
        REG_IR_LINE_W
	};
	int* destinations[]  =  {
        &lineSensors->N.ambient,
        &lineSensors->E.ambient,
        &lineSensors->S.ambient,
        &lineSensors->W.ambient,
        &lineSensors->N.active,
        &lineSensors->E.active,
        &lineSensors->S.active,
        &lineSensors->W.active
	};
	LG_logEntry(__func__, "NOT IMPLEMENTED");

	for (unsigned int i=0; i< sizeof(registers)/sizeof(int); i++) {
		SAFE_INVOKE(REG_read16(registers[i], &v), result, RV_GET_ANALOGLINE_FAILED)
		if (result == OK) {
			*(destinations[i]) = v;
		}
	}
	LG_logExit(__func__, result, "NO RESULT");
	return result;
}

extern int RV_getLineSensorsFiltered(RV_LineSensors* lineSensors)
{
    int result = OK;
    LG_logEntry(__func__, "lineSensors: %p", lineSensors);

    result = RV_getFilteredLineSensors(lineSensors);

    LG_logExit(__func__, result, "lineSensor N: %d, E: %d, S: %d, W: %d", lineSensors->N.active, lineSensors->E.active, lineSensors->S.active, lineSensors->W.active);
    return result;
}

extern int RV_getCollisionSensors(RV_CollisionSensors* collisionSensors)
{
	int result = OK;
	uint16_t v;

	static const int registers[]  =  {
        REG_AMB_COL_NE,
        REG_AMB_COL_SE,
        REG_AMB_COL_SW,
        REG_AMB_COL_NW,
        REG_IR_COL_NE,
        REG_IR_COL_SE,
        REG_IR_COL_SW,
        REG_IR_COL_NW
	};
	int* destinations[]  =  {
        &collisionSensors->NE.ambient,
        &collisionSensors->SE.ambient,
        &collisionSensors->SW.ambient,
        &collisionSensors->NW.ambient,
        &collisionSensors->NE.active,
        &collisionSensors->SE.active,
        &collisionSensors->SW.active,
        &collisionSensors->NW.active
	};

	LG_logEntry(__func__, "NOT IMPLEMENTED");
    for (unsigned int i=0; i< sizeof(registers)/sizeof(int); i++) {
        SAFE_INVOKE(REG_read16(registers[i], &v), result, RV_GET_ANALOGLINE_FAILED)
        if (result == OK) {
            *(destinations[i]) = v;
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
