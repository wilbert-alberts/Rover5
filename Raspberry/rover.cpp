#include <stdio.h>
#include <pthread.h>
#include "rv_loop.h"

static pthread_t rv_exchangeTID;

extern int RV_start()
{
    printf("Starting exchange loop.");
    
    RV_semCreate();
    
    pthread_create(&rv_exchangeTID, NULL, RV_exchangeloop, NULL);
}

extern int RV_stop()
{
    RV_running = false;
}

extern int RV_getPosition(long* left, long* right) 
{
    RV_semTake();
    
}
