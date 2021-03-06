/*
 * sensors.h
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <string>

#include "filter.h"

static const std::string orients[] = { "N", "E", "S", "W" };
static const int nrOrients = sizeof(orients) / sizeof(std::string);


class Sensors {
public:
    Sensors();
    virtual ~Sensors();

    int process();

    double getN();
    double getE();
    double getS();
    double getW();

private:
    typedef struct
    {
        double gain;
        double offset;
    } GainOffset;

    typedef struct  {
        GainOffset  go;
        IIRFilter*  flt;
    } SensorConfig;

    SensorConfig sensors[nrOrients];

    double raw[nrOrients];
    double afterGO[nrOrients];
    double filtered[nrOrients];

    int readGainOffset();
};

#endif /* SENSOR_H_ */
