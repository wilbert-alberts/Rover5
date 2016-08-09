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
#include "pid.h"

int controlSetup(int frequency);

int controlLoop(Sensors& s, ActuatorStruct* a);

class Controller
{
public:
    Controller(double frequency,
               double kp,
               double ki,
               double kd,
               Sensors& sensors,
               Actuators& actuators);

    virtual ~Controller();

    int process();

private:
    PID        pid;
    Sensors&   sensors;
    Actuators& actuators;
    double     error;
    double     outRaw;
    double     outNormalized;
};

#endif /* CONTROLLER_H_ */
