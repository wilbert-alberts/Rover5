/*
 * actuator.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <math.h>
#include <stdlib.h>

#include <rv.h>

#include "actuator.h"


int setActuators(ActuatorStruct* a)
{

	double pwmL = 25 + a->left*50;
	double pwmR = 25 + a->right*50;

	int ipwmL = abs(round(pwmL));
	int ipwmR = abs(round(pwmR));

	int dirL = pwmL > 0 ? RV_FORWARD : RV_BACKWARD;
	int dirR = pwmR > 0 ? RV_FORWARD : RV_BACKWARD;

	int result = RV_move(dirL, dirR, ipwmL, ipwmR);

	return result;
}
