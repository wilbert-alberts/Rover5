/*
 * control.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include "pid.h"
#include "controller.h"
#include "actuator.h"

#include "rv.h"

Controller::Controller(double frequency,
        double kp,
        double ki,
        double kd,
        Sensors& ss,
        Actuators& as)
: pid("C_PID", 1.0/frequency, kp, ki, kd)
, sensors(ss)
, actuators(as)
, error(0.0)
, outRaw(0.0)
, outNormalized(0.0)
{
    RV_addTraceVariable("C_outError", &error);
    RV_addTraceVariable("C_outRaw", &outRaw);
    RV_addTraceVariable("C_outNormalized", &outNormalized);
}

Controller::~Controller()
{
}

int Controller::process()
{
	error = sensors.getE() - sensors.getW();

	pid.calculate(error, &outRaw);

	outNormalized = outRaw;
	if (outNormalized > 1.0)
		outNormalized = 1.0;
	if (outNormalized < -1.0)
	    outNormalized = -1.0;


	actuators.setRight(outNormalized);
	actuators.setLeft(-outNormalized);

	return 0;
}
