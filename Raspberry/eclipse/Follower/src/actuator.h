/*
 * actuator.hpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_


typedef struct
{
	double left;    // Normalized between -1.0 (backwards)
	double right;   // and 1.0 (forwards)
} ActuatorStruct;


int setActuators(ActuatorStruct* a);

#endif /* ACTUATOR_H_ */
