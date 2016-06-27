#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "rv_exchange.h"

#define RV_LOOP_FREQUENCY (2) // Hz

static bool      RV_running;
static pthread_t rv_loopTID;
static sem_t     rv_loopWaitSem;
static sem_t     rv_loopWaitProtectionSem;
static bool      rv_loopWaiterBlocked;

static void* rv_loop(void* args);

void RV_startLoop()
{
	sem_init(&rv_loopWaitProtectionSem, 0, 1);
	sem_init(&rv_loopWaitSem, 0, 0);
	pthread_create(&rv_loopTID, NULL, rv_loop, NULL);
}

void RV_stopLoop() {
  RV_running = false;
}

void RV_waitForNewData()
{
	sem_wait(&rv_loopWaitProtectionSem);
	rv_loopWaiterBlocked = true;
	sem_post(&rv_loopWaitProtectionSem);
	sem_wait(&rv_loopWaitSem);
}

static void rv_notifyWaiters()
{
	sem_wait(&rv_loopWaitProtectionSem);
	if (rv_loopWaiterBlocked) {
		rv_loopWaiterBlocked = false;
		sem_post(&rv_loopWaitSem);
	}
	sem_post(&rv_loopWaitProtectionSem);
}

static void* rv_loop(void* args)
{
    RV_running = true;
    while (RV_running) {
    	RV_exchangeWithMega();
    	rv_notifyWaiters();
    	usleep(1000/RV_LOOP_FREQUENCY);
    }
    
    return NULL;
}



