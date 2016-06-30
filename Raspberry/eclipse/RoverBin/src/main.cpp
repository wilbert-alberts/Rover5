#include <stdio.h>
#include <unistd.h>

#include "rv.h"


int main (int argc, char* argv[])
{
    long l;
    long r;
    RV_loopLoggingOn();
    RV_setFrequency(1);
    RV_loggingOn();
    RV_start();
    
    while (1);


    sleep(1);
    for (int i=0; i<4; i++) {
      RV_getPosition(&l, &r);
      printf("l: %ld, r: %ld\n", l,r);
      sleep(1);
    }
    
    RV_stop();
}
