/*
 * main.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <stdio.h>


#include <rv.h>

#include "sensor.h"
#include "controller.h"
#include "actuator.h"

#define COLLISIONSENS (130)
#define FREQUENCY     (100)
#define MAXDURATION   (10)

int mustStop();


int main(int argc, char** argv)
{
	char c;
	SensorStruct sensors;
	ActuatorStruct actuators;

	if (setupSensors()!=0) {
		printf("Unable to setup sensors.\n");
		return -1;
	}
	if (controlSetup(FREQUENCY) != 0) {
		printf("Unable to setup controller.\n");
		return -1;
	}


	RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(FREQUENCY);

    printf("Position car such that it on top of and aligned with the line and press enter\n");
    scanf("%c", &c);

    RV_start();

	for (int i=0; i<MAXDURATION*FREQUENCY; i++)
	{
		getSensors(&sensors);

		controlLoop(&sensors, &actuators);

		setActuators(&actuators);

		if (mustStop())
			break;
		RV_waitForNewData();
	}

	RV_stop();

	RV_dumpBuffersToFile("RV_trace.txt");
}


int mustStop()
{
    RV_CollisionSensors s;

    RV_getCollisionSensors(&s);

    
    if ((s.NE.active > COLLISIONSENS) ||
        //(s.SE.active > COLLISIONSENS) ||
        //(s.SW.active > COLLISIONSENS) ||
        (s.NW.active > COLLISIONSENS)) 
    {
    	RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
	return 1;
    }
    return 0;
}
