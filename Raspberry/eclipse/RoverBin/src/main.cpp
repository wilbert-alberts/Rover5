#include <stdio.h>
#include <unistd.h>

#include "rv.h"


int mustStop(RV_CollisionSensors* s)
{
    int d = 130;
    if (s->NE.active > d)
	return 1;
    if (s->SE.active > d)
	return 1;
    if (s->SW.active > d)
	return 1;
    if (s->NW.active > d)
	return 1;
    return 0;
}


int main(int argc, char* argv[])
{
    long left;
    long right;
    RV_LineSensors lineSensors;
    RV_CollisionSensors collisionSensors;

    RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(1);
    RV_start();
    RV_move(RV_FORWARD, RV_FORWARD, 0,0);
    for (int i=0; 1 || i<10000; i++) {
      RV_getPosition(&left, &right);
      //printf("left: %ld, right: %ld\n", left, right);
      
      RV_getCollisionSensors(&collisionSensors);
      //printf("left: %ld, right: %ld, NE: %d, SE: %d, SW: %d, NW: %d\n", 
      // 	     left, right, sig[0], sig[1], sig[2], sig[3]);
      
      if (mustStop(&collisionSensors)) {
        RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
        i=100000;
      }      

      RV_getLineSensors(&lineSensors);
      printf("left: %ld, right: %ld, active: N: %d, E: %d, S: %d, W: %d, ambient: N: %d, E: %d, S: %d, W: %d\n",
      	     left, right,
             lineSensors.N.active,
             lineSensors.E.active,
             lineSensors.S.active,
             lineSensors.W.active,
             lineSensors.N.ambient,
             lineSensors.E.ambient,
             lineSensors.S.ambient,
             lineSensors.W.ambient);
      RV_waitForNewData();

    }

    RV_dumpBuffersToFile("RV_trace.txt");

    RV_stop();
}
