/*
 * rv_loop.cpp - Realizing the operations for the periodical loop
 *
 * This module contains the functions that start/stop the periodical loop.
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

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#ifdef __CYGWIN__

typedef struct
{
    int __not_used__;
} LP_timerStruct;

#else
#include <sys/timerfd.h>

typedef struct
{
    int timerFD;
} LP_timerStruct;

#endif


#include "rv.h"
#include "rv_log.h"
#include "rv_exchange.h"
#include "rv_trace.h"
#include "rv_server.h"

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) { \
		        printf("error: %d\n", r); \
			r = c; \
		} \
	}

#define RV_RT_PRIO (10)

/*
 * ---------------------------------------------------------------------------
 *               Static module data.
 * ---------------------------------------------------------------------------
 */

static int lp_loopFrequency = 1;   // Frequency of periodical loop.
static bool lp_running;            // Loop running or not.
static bool lp_logging = false;    // Loop logging enabled.
static pthread_t lp_TID;           // Thread id of periodical loop.
static sem_t lp_waitSem;           // semaphore for waiters on new data.
static sem_t lp_waitProtectionSem; // Semaphore to protect waiters' semaphore
static bool lp_waiterBlocked;      // Any blocked waiters?

/*
 * ---------------------------------------------------------------------------
 *               Static function declarations.
 * ---------------------------------------------------------------------------
 */

static void* lp_main(void* args);  // Actual body of periodical loop
static int lp_notifyWaiters();
static int lp_startTimer(int frequency, LP_timerStruct* tmr);
static void lp_stopTimer(LP_timerStruct* tmr);
static int lp_waitForTimer(LP_timerStruct* tmr);

/*
 * ---------------------------------------------------------------------------
 *              Module implementation
 * ---------------------------------------------------------------------------
 */

extern int LP_start() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	pthread_attr_t  threadAttributes;

	/* Initialize the semaphore that blocks waiters for new data and
	 * the semaphore that blocks the waiter's administratin.
	 */
    SAFE_INVOKE(sem_init(&lp_waitProtectionSem, 0, 1), result, RV_SEM_INIT_FAILED)
    SAFE_INVOKE(sem_init(&lp_waitSem, 0, 0), result, RV_SEM_INIT_FAILED)

    /* Create the thread realizing the body of the periodical loop */
	pthread_attr_init(&threadAttributes);
	//result = pthread_attr_setschedpolicy(&threadAttributes, SCHED_RR); // , result, RV_SET_SCHED_POLICY_FAILED);
	SAFE_INVOKE(pthread_create(&lp_TID, &threadAttributes, lp_main, NULL), result, RV_THREAD_CREATE_FAILED)
	//result = pthread_setschedprio(loop_loopTID, RV_RT_PRIO); //, result, RV_SET_SCHED_PRIO_FAILED);

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int LP_stop() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* The periodical loop checks lp_running to decide whether or not
	 * to make a 'next' iteration. Setting it to false will end execution
	 * of the loop next time it runs.
	 */
	lp_running = false;

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 *
 * Enable/disabe logging for loop is done by first remembering that logging
 * is desired. Second, if the periodical loop is already running, it will
 * request the logging module to enable logging for the loop thread (by
 * the thread ID of the loop thread).
 */

extern int LP_loggingOn() {
	lp_logging = true;
	if (lp_TID != 0)
		LG_setLogging(lp_TID, lp_logging);
	return OK;
}

extern int  LP_loggingOff() {
	lp_logging = false;
	if (lp_TID != 0)
		LG_setLogging(lp_TID, lp_logging);
	return OK;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int LP_setFrequency(int herz)
{
	int result = OK;
	LG_logEntry(__func__, "herz: %d", herz);

	lp_loopFrequency = herz;

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int LP_waitForNewData() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* Try to get access to the waiterBlocked boolean by waiting for
	 * its protection semaphore.
	 */
	SAFE_INVOKE(sem_wait(&lp_waitProtectionSem), result, RV_SEM_WAIT_FAILED)
	lp_waiterBlocked = true;
	SAFE_INVOKE(sem_post(&lp_waitProtectionSem), result, RV_SEM_POST_FAILED)

	/* Now start waiting on the wait semaphore. This one will be
	 * released by the periodical loop whenever new data has
	 * arrived.
	 */
	SAFE_INVOKE(sem_wait(&lp_waitSem), result, RV_SEM_WAIT_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

static int lp_notifyWaiters() {
	int result = OK;
	LG_logEntry(__func__, NULL);

    /* Try to get access to the waiterBlocked boolean by waiting for
     * its protection semaphore. It might be in use the a client that
     * wants to indicate that it wants to wait for new data.
     */
	SAFE_INVOKE(sem_wait(&lp_waitProtectionSem), result, RV_SEM_WAIT_FAILED)

	/* Check whether any client is waiting for the wait semaphore */
	if (lp_waiterBlocked) {
		lp_waiterBlocked = false;
		/* Release waiters  */
		SAFE_INVOKE(sem_post(&lp_waitSem), result, RV_SEM_POST_FAILED)
	}

	/* Release the waiter's protection semaphore */
	SAFE_INVOKE(sem_post(&lp_waitProtectionSem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

static int lp_startTimer(int frequency, LP_timerStruct* timer)
{
#ifdef __CYGWIN__
    int result = OK;

    return result;
#else
    int result = OK;
    struct itimerspec tmr;
    long period = 1000000000L / lp_loopFrequency;

    /* Set period of timer struct */
    tmr.it_interval.tv_sec = period / 1000000000L;
    tmr.it_interval.tv_nsec = period % 1000000000L;
    tmr.it_value.tv_sec = period / 1000000000L;
    tmr.it_value.tv_nsec = period % 1000000000L;

    /* Create the timer and start it */
    timer->timerFD = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer->timerFD == 0)
    {
        result = RV_UNABLE_TO_START_TIMER;
        printf("RV_UNABLE_TO_START_TIMER, timer create, errno: %d\n", errno);
    }

    if (result == OK)
    {
        result = timerfd_settime(timer->timerFD, 0, &tmr, NULL);
        if (result != 0)
        {
            result = RV_UNABLE_TO_START_TIMER;
            printf("RV_UNABLE_TO_START_TIMER, timer settime, errno: %d\n",
                    errno);
        }
    }
    return result;
#endif
}

/*
 * ---------------------------------------------------------------------------
 */

static void lp_stopTimer(LP_timerStruct* timer)
{
#ifdef __CYGWIN__
#else
    struct itimerspec tmr;

    if (timer->timerFD != 0)
    {
        tmr.it_interval.tv_sec = 0;
        tmr.it_interval.tv_nsec = 0;
        tmr.it_value.tv_sec = 0;
        tmr.it_value.tv_nsec = 0;
        timerfd_settime(timer->timerFD, 0, &tmr, NULL);
        close (timer->timerFD);
    }
#endif
}

/*
 * ---------------------------------------------------------------------------
 */

static int lp_waitForTimer(LP_timerStruct* timer)
{
#ifdef __CYGWIN__

    int result = OK;

    usleep(1000000L/lp_loopFrequency);

    return result;
#else

    int result = OK;

    /* Wait for the next period by reading from the timer */
    result = read(timer->timerFD, &overruns, sizeof(uint64_t));
    if (result < 0)
    {
        result = RV_LOOP_ABORTED;
        printf("RV_LOOP_ABORTED errno: %d\n", errno);
    }
    else
    {
        if (result != 8)
        {
            result = RV_LOOP_ABORTED;
        }
        else
        {
            result = OK;
        }
    }
    /* Check whether the timer already expired. This indicates that the
     * requested frequency can not be achieved.
     */
    if (overruns > 1)
    {
        printf("Warning: overrun(s) detected: %lld\n", overruns - 1);
        result = RV_LOOP_OVERRUNS;
    }
    return result;
#endif
}

/*
 * ---------------------------------------------------------------------------
 *
 * Main body of the periodical loop.
 *
 * Each iteration we need to:
 * - communicate with the AVR (exchanging the current registermap)
 * - trace the current register map into the trace buffer
 * - write the current register to the server socket in case a client is listening
 * - notify andy waiters that new data has arrived.
 *
 * The periodicality is realized by linux timers.
 */

static void* lp_main(void* args) {
	int result = OK;
    LP_timerStruct timer;

    /* Now that lp_TID has a value, use it to enable/disable
	 * logging for this thread.
	 */
	LG_setLogging(lp_TID, lp_logging);

	LG_logEntry(__func__, NULL);

	/* Initialize the timer structs such that it periodically
	 * ticks at the desired frequency.
	 */
    result = lp_startTimer(lp_loopFrequency, &timer);

	lp_running = true;
	while ((lp_running) and (result == OK)) {
		result = EX_communicate();
		result = TR_traceRegmap();
		result = SV_send();
		result = OK;

		/* Notify any waiting clients */
		SAFE_INVOKE(lp_notifyWaiters(), result, RV_LOOP_ABORTED)

		/* Wait for the next period by reading from the timer */
        result = lp_waitForTimer(&timer);

		/* Ignore overruns */
		if (result == RV_LOOP_OVERRUNS)
		    result = OK;
	}

    lp_stopTimer(&timer);

	LG_logExit(__func__, result, NULL);
	return NULL;
}

