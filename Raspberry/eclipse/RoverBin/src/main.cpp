#include <stdio.h>
#include <unistd.h>

#include "rv.h"


int main (int argc, char* argv[])
{
    long millis;
    long micros;
    RV_loggingOn();
    //RV_loopLoggingOn();
    RV_setFrequency(2);
    RV_start();
    

    sleep(1);
    for (int i=0; i<4; i++) {
      RV_getAVRTime(&millis, &micros);
      printf("millis: %ld, micros: %ld\n", millis, micros);
      //sleep(1);
      RV_waitForNewData();
    }
    
    RV_dumpBuffersToFile("RV_trace.txt");

    RV_stop();
}
