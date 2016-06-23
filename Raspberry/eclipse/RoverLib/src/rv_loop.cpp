#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "rv_reg.h"

#define RV_LOOP_FREQUENCY (2) // Hz

static bool RV_running;
static pthread_t rv_loopTID;

static void* rv_loop(void* args);
static void rv_exchangeWithMega();


void RV_startLoop()
{
  pthread_create(&rv_loopTID, NULL, rv_loop, NULL);
}

void RV_stopLoop() {
  RV_running = false;
}

static void* rv_loop(void* args)
{
    RV_running = true;
    while (RV_running) {
    	rv_exchangeWithMega();
	usleep(1000/RV_LOOP_FREQUENCY);
	//printf(".\n");
    }
    
    return NULL;
}

static void rv_exchangeWithMega()
{
  static REG_map reg_map;

  reg_map.LEFTPOS = 32;
  reg_map.RIGHTPOS = 42;	
  REG_writeAll(&reg_map);	
}

