/*
 * PID.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#include "PID.h"

PID::PID(double kp, double ki, double kd, double period)
{
	// TODO Auto-generated constructor stub
	this->period = period;
	this->Kd = kd;
	this->Kp = kp;
	this->Ki = ki;

	this->prevErr = 0.0;
	this->integral = 0.0;
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
