#ifndef RV_H
#define RV_H

/*
 * rv.h - public API for Rover clients.
 *
 * This file defines the user api to the Rover. It should be the only file
 * that needs to be included. The implementation is delivered in a
 * static library.
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

#include <stdint.h>

/* Result codes returned by any of the Rover's functions. Anything else then
 * zero indicates an error.
 */

#define OK                              (0)
#define RV_ILLEGALVALUE                 (-1)
#define RV_SEM_INIT_FAILED              (-2)
#define RV_THREAD_CREATE_FAILED         (-3)
#define RV_SEM_WAIT_FAILED              (-4)
#define RV_SEM_POST_FAILED              (-5)
#define RV_LOOP_ABORTED                 (-6)
#define RV_START_FAILED                 (-7)
#define RV_STOP_FAILED                 	(-8)
#define RV_LOGGING_ON_FAILED           	(-9)
#define RV_LOGGING_OFF_FAILED           (-10)
#define RV_GET_POSITION_FAILED          (-11)
#define RV_MOVE_FAILED                  (-12)
#define RV_GET_LINE_FAILED              (-13)
#define RV_GET_COLLISION_FAILED         (-13)
#define RV_EXCHANGE_SETUP_FAILED        (-14)
#define RV_EXCHANGE_FAILED              (-15)
#define RV_EXCHANGESPI_FAILED           (-16)
#define RV_SET_SCHED_POLICY_FAILED      (-17)
#define RV_SET_SCHED_PRIO_FAILED        (-18)
#define RV_GET_AVRTIME_FAILED           (-19)
#define RV_GET_ANALOGLINE_FAILED        (-20)
#define RV_GET_ANALOGCOLLISION_FAILED   (-21)
#define RV_UNABLE_TO_OPEN_FILE          (-22)
#define RV_UNABLE_TO_MALLOC             (-23)
#define RV_SET_AMB_COL_OFFSET_FAILED    (-24)
#define RV_GET_AMB_COL_OFFSET_FAILED    (-25)
#define RV_SET_AMB_LINE_OFFSET_FAILED   (-26)
#define RV_GET_AMB_LINE_OFFSET_FAILED   (-27)
#define RV_UNABLE_TO_GET_SOCKET         (-28)
#define RV_UNABLE_TO_BIND               (-29)
#define RV_UNABLE_INSTALL_SIGHANDLER    (-30)


/* RV_FORWARD and RV_BACKWARD are to be used when invoking the
 * RV_move operation in order to define the Rover's movement direction.
 */
#define RV_FORWARD   (0)
#define RV_BACKWARD  (1)

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
    int ambient;
    int active;
} RV_IRSensor;

typedef struct
{
    RV_IRSensor NE;
    RV_IRSensor SE;
    RV_IRSensor SW;
    RV_IRSensor NW;
} RV_CollisionSensors;

typedef struct
{
    RV_IRSensor N;
    RV_IRSensor E;
    RV_IRSensor S;
    RV_IRSensor W;
} RV_LineSensors;

extern int RV_loopLoggingOn();
extern int RV_loopLoggingOff();
extern int RV_loggingOn();
extern int RV_loggingOff();

extern int RV_start();
extern int RV_setFrequency(int herz);

extern int RV_stop();

extern int RV_waitForNewData();

extern int RV_getPosition(long* leftPos, long* rightPos);

extern int RV_move(int leftDirection, // RV_FORWARD, RV_BACKWARD
        int rightDirection, int leftDC,        // from 0 to 255
        int rightDC);

extern int RV_getLineSensors(RV_LineSensors* lineSensors);
extern int RV_getLineSensorsFiltered(RV_LineSensors* lineSensors);

extern int RV_getCollisionSensors(RV_CollisionSensors* collisionSensors);

/* Expert usage */
extern int RV_getAVRTime(long* millis, long* micros);

extern int RV_dumpBuffersToFile(const char* name);

#endif
