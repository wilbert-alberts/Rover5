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

int checkCollision();


int main(int argc, char** argv)
{
	char c;
	SensorStruct sensors;
	ActuatorStruct actuators;
	bool mustStop = false;

	if (setupSensors()!=0) {
		printf("Unable to setup sensors.\n");
		return -1;
	}

	RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(100);

    printf("Position car such that it on top of and aligned with the line and press enter\n");
    scanf("%c", &c);

    RV_start();


	while (!mustStop) {

		getSensors(&sensors);

		controlLoop(&sensors, &actuators);

		setActuators(&actuators);

		mustStop = checkCollision();
	}

	RV_stop();

	RV_dumpBuffersToFile("RV_trace.txt");
}




int checkCollision()
{
	int result;
	static RV_CollisionSensors s;

	result = RV_getCollisionSensors(&s);
	if (result != OK)
		return 1;

    if (s.NE.active > COLLISIONSENS)
	return 1;
    if (s.SE.active > COLLISIONSENS)
	return 1;
    if (s.SW.active > COLLISIONSENS)
	return 1;
    if (s.NW.active > COLLISIONSENS)
	return 1;

    return 0;
}

