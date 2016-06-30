#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_exchange.h"

static int rv_frequency = 2;

static bool RV_running;
static pthread_t rv_loopTID;
static sem_t rv_loopWaitSem;
static sem_t rv_loopWaitProtectionSem;
static bool rv_loopWaiterBlocked;

static void* rv_loop(void* args);

extern int RV_startLoop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	sem_init(&rv_loopWaitProtectionSem, 0, 1);
	sem_init(&rv_loopWaitSem, 0, 0);
	pthread_create(&rv_loopTID, NULL, rv_loop, NULL);

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_stopLoop() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	RV_running = false;

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_loopLoggingOn() {
	RV_SetLogging(rv_loopTID, true);
	return OK;
}

extern int  RV_loopLogginOff() {
	RV_SetLogging(rv_loopTID, false);
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

	sem_wait(&rv_loopWaitProtectionSem);
	rv_loopWaiterBlocked = true;
	sem_post(&rv_loopWaitProtectionSem);
	sem_wait(&rv_loopWaitSem);

	RV_LogExit(__func__, result, NULL);
	return result;
}

static void rv_notifyWaiters() {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	sem_wait(&rv_loopWaitProtectionSem);
	if (rv_loopWaiterBlocked) {
		rv_loopWaiterBlocked = false;
		sem_post(&rv_loopWaitSem);
	}
	sem_post(&rv_loopWaitProtectionSem);

	RV_LogExit(__func__, result, NULL);
}

static void* rv_loop(void* args) {
	int result = OK;
	RV_LogEntry(__func__, NULL);

	RV_running = true;
	while (RV_running) {
		RV_exchangeWithMega();
		rv_notifyWaiters();
		usleep(1000 / rv_frequency);
	}

	RV_LogExit(__func__, result, NULL);
	return NULL;
}

