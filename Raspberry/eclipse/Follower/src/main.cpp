/*
 * main.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <stdio.h>

#include <cstdlib>
#include <iostream>

#include <rv.h>

#include "sensor.h"
#include "controller.h"
#include "actuator.h"
#include "tracing.h"

#define COLLISIONSENS (130)
#define FREQUENCY     (100)
#define MAXDURATION   (130)

int mustStop();

int main(int argc, char** argv)
{
	char c;

	RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(FREQUENCY);

	Sensors sensors;
	Actuators actuators;
	Controller controller(FREQUENCY, 2.5, 0, 0.2 , sensors, actuators);


    RV_start();
    printf("Position car such that it on top of and aligned with the line and press enter\n");
    scanf("%c", &c);

//    RV_start();

	for (int i=0; i<MAXDURATION*FREQUENCY; i++)
	{
		sensors.process();
		controller.process();
		//actuators.process();

		if (mustStop())
			break;
		Trace::stepTime();
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
        (s.SE.active > COLLISIONSENS) ||
        (s.SW.active > COLLISIONSENS) ||
        (s.NW.active > COLLISIONSENS)) 
    {
    	RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
	return 1;
    }
    return 0;
}
