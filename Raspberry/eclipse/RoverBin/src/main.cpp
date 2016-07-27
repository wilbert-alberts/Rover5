#include <stdio.h>
#include <unistd.h>

#include "rv.h"


int mustStop(int s[4])
{
    for (int i=0; i<4; i++) {
    	if (s[i]>500)
	    return 1;
    }
    return 0;
}


int main (int argc, char* argv[])
{
    long left;
    long right;
    int sig[4];
    int amb[4];
    RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(100);
    RV_start();
    RV_setColAmbOffset(300);
    while(21);
    // RV_move(RV_BACKWARD, RV_BACKWARD, 70,70);
    for (int i=0; i<10000; i++) {
      //RV_getPosition(&left, &right);
      //printf("left: %ld, right: %ld\n", left, right);
      
      //RV_getAnalogCollision(sig, amb);
      //printf("left: %ld, right: %ld, NE: %d, SE: %d, SW: %d, NW: %d\n", 
      // 	     left, right, sig[0], sig[1], sig[2], sig[3]);
      
      //if (mustStop(sig)) {
      //  RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
      //  i=100000;
      //}      

      RV_getAnalogLine(sig, amb);
      printf("left: %ld, right: %ld, N: %d, E: %d, S: %d, W: %d\n", 
      	     left, right, amb[0], amb[1], amb[2], amb[3]);
      printf("left: %ld, right: %ld, N: %d, E: %d, S: %d, W: %d\n", 
      	     left, right, sig[0], sig[1], sig[2], sig[3]);
      
      RV_waitForNewData();
    }
    
    //RV_dumpBuffersToFile("RV_trace.txt");

    RV_stop();
}
