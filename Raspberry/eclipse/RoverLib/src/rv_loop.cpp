#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef LINUX
#include <sys/timerfd.h>
#else

/* Section to allow compilation under cygwin on windows. */

int timerfd_create(int clockid, int flags) { return 0; }
int timerfd_settime(int fd, int flags,
                    const struct itimerspec *new_value,
                    struct itimerspec *old_value) { return 0; }

int timerfd_gettime(int fd, struct itimerspec *curr_value) { return 0; }

#endif


#include "rv.h"
#include "rv_log.h"
#include "rv_exchange.h"
#include "rv_trace.h"
#include "rv_server.h"

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) { \
		        printf("error: %d\n", r); \
			r = c; \
		} \
	}

#define RV_RT_PRIO (10)

static int loop_frequency = 1;

static bool lp_running;
static bool lp_logging = false;
static pthread_t lp_TID;
static sem_t lp_waitSem;
static sem_t lp_waitProtectionSem;
static bool lp_waiterBlocked;

static void* lp_main(void* args);
static int lp_notifyWaiters();

extern int LP_start() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	pthread_attr_t  threadAttributes;
	
	pthread_attr_init(&threadAttributes);
	//result = pthread_attr_setschedpolicy(&threadAttributes, SCHED_RR); // , result, RV_SET_SCHED_POLICY_FAILED);
	SAFE_INVOKE(sem_init(&lp_waitProtectionSem, 0, 1), result, RV_SEM_INIT_FAILED)
	SAFE_INVOKE(sem_init(&lp_waitSem, 0, 0), result, RV_SEM_INIT_FAILED)
	SAFE_INVOKE(pthread_create(&lp_TID, &threadAttributes, lp_main, NULL), result, RV_THREAD_CREATE_FAILED)
	//result = pthread_setschedprio(loop_loopTID, RV_RT_PRIO); //, result, RV_SET_SCHED_PRIO_FAILED);
	LG_logExit(__func__, result, NULL);
	return result;
}

extern int LP_stop() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	lp_running = false;

	LG_logExit(__func__, result, NULL);
	return result;
}

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

extern int LP_setFrequency(int herz)
{
	int result = OK;
	LG_logEntry(__func__, "herz: %d", herz);

	loop_frequency = herz;

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int LP_waitForNewData() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	SAFE_INVOKE(sem_wait(&lp_waitProtectionSem), result, RV_SEM_WAIT_FAILED)
	lp_waiterBlocked = true;
	SAFE_INVOKE(sem_post(&lp_waitProtectionSem), result, RV_SEM_POST_FAILED)
	SAFE_INVOKE(sem_wait(&lp_waitSem), result, RV_SEM_WAIT_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

static int lp_notifyWaiters() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	SAFE_INVOKE(sem_wait(&lp_waitProtectionSem), result, RV_SEM_WAIT_FAILED)
	if (lp_waiterBlocked) {
		lp_waiterBlocked = false;
		SAFE_INVOKE(sem_post(&lp_waitSem), result, RV_SEM_POST_FAILED)
	}
	SAFE_INVOKE(sem_post(&lp_waitProtectionSem), result, RV_SEM_POST_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

static void* lp_main(void* args) {
	int result = OK;

	LG_setLogging(lp_TID, lp_logging);

	LG_logEntry(__func__, NULL);

	struct itimerspec tmr;
	int timerfd;
	long period = 1000000000L/loop_frequency;
	uint64_t overruns;

    tmr.it_interval.tv_sec = 0;
    tmr.it_interval.tv_nsec = period;
    tmr.it_value.tv_sec = 0;
    tmr.it_value.tv_nsec = period;

	timerfd = timerfd_create(CLOCK_MONOTONIC,  0);
	timerfd_settime(timerfd, 0, &tmr, NULL);

	lp_running = true;
	while ((lp_running) and (result == OK)) {
		result = EX_communicate();
		result = TR_traceRegmap();
		result = SV_send();
		result = OK;

		SAFE_INVOKE(lp_notifyWaiters(), result, RV_LOOP_ABORTED)

		result = read(timerfd, &overruns, sizeof(uint64_t));
		if (result<0) {
		    result = RV_LOOP_ABORTED;
		}
	}
	close(timerfd);
	LG_logExit(__func__, result, NULL);
	printf("Loop ended: %d\n", result);
	return NULL;
}

