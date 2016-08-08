/*
 * PID.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#include "pid.h"

PID::PID(double _period, double _kp, double _ki, double _kd)
{
	// TODO Auto-generated constructor stub
	period = _period;
	Kd = _kd;
	Kp = _kp;
	Ki = _ki;

	prevErr = 0.0;
	integral = 0.0;
}

PID::~PID()
{
	// TODO Auto-generated destructor stub
}

void PID::calculate(double err, double* tL)
{
	integral += (err * period);

	double p = err * Kp;
	double d = (err - prevErr) / period * Kd;
	double i = integral * Ki;

	prevErr = err;
	*tL = p+d+i;
}
