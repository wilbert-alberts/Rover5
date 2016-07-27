#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

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

static int rv_frequency = 1;

static bool rv_running;
static bool rv_looplogging = false;
static pthread_t rv_loopTID;
static sem_t rv_loopWaitSem;
static sem_t rv_loopWaitProtectionSem;
static bool rv_loopWaiterBlocked;

static void* rv_loop(void* args);

extern int RV_startLoop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	pthread_attr_t  threadAttributes;
	
	pthread_attr_init(&threadAttributes);
	//result = pthread_attr_setschedpolicy(&threadAttributes, SCHED_RR); // , result, RV_SET_SCHED_POLICY_FAILED);
	SAFE_INVOKE(sem_init(&rv_loopWaitProtectionSem, 0, 1), result, RV_SEM_INIT_FAILED)
	SAFE_INVOKE(sem_init(&rv_loopWaitSem, 0, 0), result, RV_SEM_INIT_FAILED)
	SAFE_INVOKE(pthread_create(&rv_loopTID, &threadAttributes, rv_loop, NULL), result, RV_THREAD_CREATE_FAILED)
	//result = pthread_setschedprio(rv_loopTID, RV_RT_PRIO); //, result, RV_SET_SCHED_PRIO_FAILED);
	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_stopLoop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	rv_running = false;

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_loopLoggingOn() {
	rv_looplogging = true;
	if (rv_loopTID != 0)
		RV_SetLogging(rv_loopTID, rv_looplogging);
	return OK;
}

extern int  RV_loopLogginOff() {
	rv_looplogging = false;
	if (rv_loopTID != 0)
		RV_SetLogging(rv_loopTID, rv_looplogging);
	return OK;
}

extern int RV_setFrequency(int herz)
{
	int result = OK;
	RV_LogEntry(__func__, "herz: %d", herz);

	rv_frequency = herz;

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_waitForNewData() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	SAFE_INVOKE(sem_wait(&rv_loopWaitProtectionSem), result, RV_SEM_WAIT_FAILED)
	rv_loopWaiterBlocked = true;
	SAFE_INVOKE(sem_post(&rv_loopWaitProtectionSem), result, RV_SEM_POST_FAILED)
	SAFE_INVOKE(sem_wait(&rv_loopWaitSem), result, RV_SEM_WAIT_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

static int rv_notifyWaiters() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	SAFE_INVOKE(sem_wait(&rv_loopWaitProtectionSem), result, RV_SEM_WAIT_FAILED)
	if (rv_loopWaiterBlocked) {
		rv_loopWaiterBlocked = false;
		SAFE_INVOKE(sem_post(&rv_loopWaitSem), result, RV_SEM_POST_FAILED)
	}
	SAFE_INVOKE(sem_post(&rv_loopWaitProtectionSem), result, RV_SEM_POST_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

static void* rv_loop(void* args) {
	int result = OK;

	RV_SetLogging(rv_loopTID, rv_looplogging);

	RV_LogEntry(__func__, NULL);

	rv_running = true;
	while ((rv_running) and (result == OK)) {
		result = RV_exchangeWithMega();
		result = TR_traceRegmap();
		result = SV_send();
		result = OK;
		//printf("*\n");

		SAFE_INVOKE(rv_notifyWaiters(), result, RV_LOOP_ABORTED)
		usleep(1000000ul / rv_frequency);
	}

	RV_LogExit(__func__, result, NULL);
	printf("Loop ended: %d\n", result);
	return NULL;
}

