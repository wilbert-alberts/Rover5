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
#define RV_UNABLE_TO_START_TIMER        (-31)
#define RV_OUT_OF_USR_REGISTERS         (-32)
#define RV_LOOP_OVERRUNS                (-33)


/* RV_FORWARD and RV_BACKWARD are to be used when invoking the
 * RV_move operation in order to define the Rover's movement direction.
 */
#define RV_FORWARD   (0)
#define RV_BACKWARD  (1)


/*
 * ---------------------------------------------------------------------------
 *               Type definitions
 * ---------------------------------------------------------------------------
 */

/* RV_IRSensor struct holds values representing the output
 * of a IR sensor. As each sensor is read twice, (once
 * with illumination on and once without illumination),
 * two fields are present:
 * - active: reading with illumination ON
 * - ambient: reading with illumination OFF
 *
 * The readings are the direct outputs of the AVR AD converters.
 * As the AVR AD converters have a resolution of 10 bits,
 * the range of values varies from 0 to 1023.
 */
typedef struct
{
    int ambient;
    int active;
} RV_IRSensor;


/* RV_CollisionSensors and RV_LineSensors represent the outputs
 * of the collision and line sensors. They are organized along
 * their orientation.
 */
typedef struct
{
    RV_IRSensor NE;       // North East
    RV_IRSensor SE;       // South East
    RV_IRSensor SW;       // South West
    RV_IRSensor NW;       // North West
} RV_CollisionSensors;

typedef struct
{
    RV_IRSensor N;       // North
    RV_IRSensor E;       // East
    RV_IRSensor S;       // South
    RV_IRSensor W;       // West
} RV_LineSensors;


/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */


/* The following function enable and disable function tracing.
 * This can be done for the user side and for the thread that
 * executes the periodical loop.
 *
 * Note that these functions can be called even before
 * RV_start()
 */
extern int RV_loopLoggingOn();
extern int RV_loopLoggingOff();
extern int RV_loggingOn();
extern int RV_loggingOff();

/* RV_start initializes the hardware and starts the periodical
 * loop.
 */
extern int RV_start();

/* RV_setFrequency can be used to set the frequency of the
 * periodicaly loop.
 */
extern int RV_setFrequency(int herz);

/* RV_stop signals the periodical loop to stop. It does
 * not wait for the thread executing the loop to end.
 */
extern int RV_stop();

/* RV_waitForNewData blocks until the periodical loop has
 * executed at least once more and new data is available.
 * This function can be used to run in sync with the
 * periodical loop.
 */
extern int RV_waitForNewData();

/* RV_getPosition returns the values of the position sensors.
 * Note that these sensors measure the rotation of the wheels
 * which, due to slip, does not necessarly directly translate
 * linearly a position in the real world.
 */
extern int RV_getPosition(long* leftPos, long* rightPos);

/* RV_move instructs the Rover to move. Movement is controlled
 * by specifying a direction for each track and a duty cycle
 * for the motors for each track.
 */
extern int RV_move(int leftDirection,      // RV_FORWARD, RV_BACKWARD
                   int rightDirection,
                   int leftDC,             // from 0 to 255
                   int rightDC);

/* RV_getLineSensors returns the values of the linesensors
 * represented by a RV_LineSensors struct.
 */
extern int RV_getLineSensors(RV_LineSensors* lineSensors);

/* RV_getCollisionSensors returns the values of the collisionsensors
 * represented by a RV_CollisionSensors struct.
 */
extern int RV_getCollisionSensors(RV_CollisionSensors* collisionSensors);

/* RV_getAVRTime returns the time the AVR has been running expressed in
 * milli- and microseconds. The AVR uses 32bit integers for these values
 * so after approx 70 minutes, the micros value will wrap around.
 * After 49 days, the millis will wrap around.
 */
extern int RV_getAVRTime(long* millis, long* micros);

/* The periodical loop maintains a cyclic buffer of 1000 entries of
 * RegistersMaps. RV_dumpBuffersToFile leads to dumping their
 * content into a file named by 'name'.
 */
extern int RV_dumpBuffersToFile(const char* name);

/* Add a user register to be traced. Each time the periodic loop
 * steps, the value pointed to by var is copied into the trace
 * buffer. Note that this is the last action done in the loop.
 */
extern int RV_addTraceVariable(const char* name, double* var);

#endif
