/*
 * control.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */


#include "pid.h"
#include "controller.h"
#include "actuator.h"

static PID* pid;

int controlSetup(int frequency)
{
	pid = new PID(1.0/frequency, 0.1, 0, 0 );

	return 0;
}

int controlLoop(SensorStruct* s, ActuatorStruct* a)
{
	double err = s->E - s->W;
	double out;

	pid->calculate(err, &out);

	if (out > 1.0)
		out = 1.0;
	if (out < -1.0)
		out = -1.0;

	return 0;
}
