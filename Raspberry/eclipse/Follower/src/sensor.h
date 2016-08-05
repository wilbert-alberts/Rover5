/*
 * sensors.h
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef SENSOR_H_
#define SENSOR_H_



typedef struct
{
	double N;
	double E;
	double S;
	double W;
} SensorStruct;

int setupSensors();
int getSensors(SensorStruct* s);


#endif /* SENSOR_H_ */
