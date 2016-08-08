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

Actuators::Actuators()
: left(0.0)
, right(0.0)
, trLeft("A_Torqueleft")
, trRight("A_TorqueRight")
{
}

Actuators::~Actuators()
{
}

void Actuators::setLeft(double v)
{
    left = v;
}

void Actuators::setRight(double v)
{
    right = v;
}

int Actuators::process()
{
    double pwmL = 25 + left*50;
    double pwmR = 25 + right*50;

    int ipwmL = abs(round(pwmL));
    int ipwmR = abs(round(pwmR));

    int dirL = pwmL > 0 ? RV_FORWARD : RV_BACKWARD;
    int dirR = pwmR > 0 ? RV_FORWARD : RV_BACKWARD;

    trLeft.add(pwmL);
    trRight.add(pwmR);

    int result = RV_move(dirL, dirR, ipwmL, ipwmR);

    return result;
}

