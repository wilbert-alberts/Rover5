/*
 * control.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include "pid.h"
#include "controller.h"
#include "actuator.h"

Controller::Controller(double frequency,
        double kp,
        double ki,
        double kd,
        Sensors& ss,
        Actuators& as)
: pid(1.0/frequency, kp, ki, kd)
, sensors(ss)
, actuators(as)
, trError("C_Error")
, trOut("C_Out")
{
}

Controller::~Controller()
{
}

int Controller::process()
{
	double err = sensors.getE() - sensors.getW();
	double out;

	pid.calculate(err, &out);

	trError.add(err);
	trOut.add(out);

	if (out > 1.0)
		out = 1.0;
	if (out < -1.0)
		out = -1.0;

	actuators.setRight(out);
	actuators.setLeft(-out);

	//printf("%lf\t%lf\t%lf\t%lf\n", s.getE(), s.getW(), a->right, a->left);

	return 0;
}
