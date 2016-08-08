/*
 * actuator.hpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "tracing.h"


typedef struct
{
	double left;    // Normalized between -1.0 (backwards)
	double right;   // and 1.0 (forwards)
} ActuatorStruct;

class Actuators
{
public:
    Actuators();
    virtual ~Actuators();

    // setLeft and setRight accept values between -1.0
    // (backwards) and 1.0 (fowrads)
    void setLeft(double v);
    void setRight(double v);

    int process();
private:
    double left;
    double right;
    Trace  trLeft;
    Trace  trRight;
};

#endif /* ACTUATOR_H_ */
