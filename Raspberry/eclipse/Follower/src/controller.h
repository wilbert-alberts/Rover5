/*
 * control.hpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "sensor.h"
#include "actuator.h"

int controlSetup();

int controlLoop(SensorStruct* s, ActuatorStruct* a);



#endif /* CONTROLLER_H_ */
