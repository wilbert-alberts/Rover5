/*
 * sensors.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */


#include "sensor.h"

#include <stdio.h>

#include "rv.h"

#include "filter.h"


typedef struct
{
	double gain;
	double offset;
} GainOffset;

static GainOffset go[4];
static BWFilter   flt[4];

static int readGainOffset(GainOffset* gof);


int setupSensors()
{
	int result = OK;
	double as[3] = { 1.00000,    -1.86689,     0.87521   };
	double bs[3] = { 0.0020806,   0.0041611,   0.0020806 };

	if (result == OK)
		result = readGainOffset(go);
	if (result == OK)
		result = BWF_create(&flt[0], 3, as, bs);
	if (result == OK)
		result = BWF_create(&flt[1], 3, as, bs);
	if (result == OK)
		result = BWF_create(&flt[2], 3, as, bs);
	if (result == OK)
		result = BWF_create(&flt[3], 3, as, bs);

	return result;
}


int readGainOffset(GainOffset* gof)
{
	FILE* of = fopen("RV_GainOffset.txt", "r");

	if (of == NULL) {
		fprintf(stderr, "Error: unable to open file for reading: RV_GainOffset.txt\n");
		return -1;
	}

	fscanf(of, "%lf\n", &gof[0].gain);
	fscanf(of, "%lf\n", &gof[0].offset);

	fscanf(of, "%lf\n", &gof[1].gain);
	fscanf(of, "%lf\n", &gof[1].offset);

	fscanf(of, "%lf\n", &gof[2].gain);
	fscanf(of, "%lf\n", &gof[2].offset);

	fscanf(of, "%lf\n", &gof[3].gain);
	fscanf(of, "%lf\n", &gof[3].offset);

	fclose(of);
	return 0;
}


int getSensors(SensorStruct* s)
{
	int result = OK;
	RV_LineSensors ls;

	result = RV_getLineSensors(&ls);
	if (result != OK)
		return result;

	BWF_put(flt[0], ls.N.active * go[0].gain + go[0].offset);
	BWF_put(flt[1], ls.E.active * go[1].gain + go[1].offset);
	BWF_put(flt[2], ls.S.active * go[2].gain + go[2].offset);
	BWF_put(flt[3], ls.W.active * go[3].gain + go[3].offset);

	BWF_get(flt[0], &s->N);
	BWF_get(flt[1], &s->E);
	BWF_get(flt[2], &s->S);
	BWF_get(flt[3], &s->W);

	return OK;
}
