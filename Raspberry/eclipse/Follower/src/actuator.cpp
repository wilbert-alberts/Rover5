/*
 * actuator.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <math.h>

#include <rv.h>

#include "actuator.h"


int setActuators(ActuatorStruct* a)
{
	// Assuming a settable range between 25 and 50.
	// Consequence: straight line is 37.5

	double pwmL = 37.5 + a->left*12.5;
	double pwmR = 37.5 + a->right*12.5;

	int ipwmL = abs(round(pwmL));
	int ipwmR = abs(round(pwmR));

	int dirL = pwmL > 0 ? RV_FORWARD : RV_BACKWARD;
	int dirR = pwmR > 0 ? RV_FORWARD : RV_BACKWARD;

	int result = RV_move(dirL, dirR, ipwmL, ipwmR);

	return result;
}
