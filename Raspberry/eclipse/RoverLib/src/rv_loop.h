#ifndef RV_LOOP_H
#define RV_LOOP_H

/*
 * rv_loop.h - operations that start and implement the periodical loop.
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */


/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */

/* LP_start spawns thread that executes periodical loop */
extern int LP_start();

/* LP_stop sets boolean that is used by the periodical loop to decide
 * whether to stop. Note that this operation does not actively kill
 * the thread executing the periodical loop.
 */
extern int LP_stop();

/* Enable/disable logging of the periodical loop. */
extern int LP_loggingOn();
extern int LP_loggingOff();

/* Set the frequency of the periodical loop. */
extern int LP_setFrequency(int herz);

/* Block the current thread until the next period of the periodical loop
 * has run.
 */
extern int LP_waitForNewData();

#endif
