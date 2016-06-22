#include <stdio.h>
#include <unistd.h>

bool RV_running;



void* RV_exchangeloop(void* args)
{
    RV_running = true;
    while (RV_running) {
    	printf("rv_exchangeloop running.\n");
	sleep(1);
    }
    
    return NULL;
}
