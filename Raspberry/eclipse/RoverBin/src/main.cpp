#include <stdio.h>
#include <unistd.h>

#include "rv.h"


int main (int argc, char* argv[])
{
    long left;
    long right;
    //RV_loggingOn();
    //RV_loopLoggingOn();
    RV_setFrequency(2);
    RV_start();
    

    sleep(1);
    for (int i=0; i<40; i++) {
      RV_getPosition(&left, &right);
      printf("left: %ld, right: %ld\n", left, right);
      //sleep(1);
      RV_waitForNewData();
    }
    
    //RV_dumpBuffersToFile("RV_trace.txt");

    RV_stop();
}
