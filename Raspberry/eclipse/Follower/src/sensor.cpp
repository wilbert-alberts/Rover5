/*
 * sensors.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */


#include "sensor.h"

#include <stdio.h>
#include <string>

#include "rv.h"


Sensors::Sensors()
{
	int result = OK;
	double as[3] = { 1.00000,    -1.86689,     0.87521   };
	double bs[3] = { 0.0020806,   0.0041611,   0.0020806 };

	if (result == OK)
		result = readGainOffset();

	for (int i=0; i<nrOrients; i++) {
	    sensors[i].flt = new IIRFilter(3, as, bs);
	}

    RV_addTraceVariable("S_inRaw_N", &raw[0]);
    RV_addTraceVariable("S_inRaw_E", &raw[1]);
    RV_addTraceVariable("S_inRaw_S", &raw[2]);
    RV_addTraceVariable("S_inRaw_W", &raw[3]);

    RV_addTraceVariable("S_inAfterGO_N", &afterGO[0]);
    RV_addTraceVariable("S_inAfterGO_E", &afterGO[1]);
    RV_addTraceVariable("S_inAfterGO_S", &afterGO[2]);
    RV_addTraceVariable("S_inAfterGO_W", &afterGO[3]);

    RV_addTraceVariable("S_inFiltered_N", &filtered[0]);
    RV_addTraceVariable("S_inFiltered_E", &filtered[1]);
    RV_addTraceVariable("S_inFiltered_S", &filtered[2]);
    RV_addTraceVariable("S_inFiltered_W", &filtered[3]);

}


Sensors::~Sensors()
{
    for (int i=0; i<nrOrients; i++) {
        delete sensors[i].flt;
    }
}


int Sensors::readGainOffset()
{
	FILE* of = fopen("RV_GainOffset.txt", "r");

	if (of == NULL) {
		fprintf(stderr, "Error: unable to open file for reading: RV_GainOffset.txt\n");
		return -1;
	}

	fscanf(of, "%lf\n", &sensors[0].go.gain);
	fscanf(of, "%lf\n", &sensors[0].go.offset);

	fscanf(of, "%lf\n", &sensors[1].go.gain);
	fscanf(of, "%lf\n", &sensors[1].go.offset);

	fscanf(of, "%lf\n", &sensors[2].go.gain);
	fscanf(of, "%lf\n", &sensors[2].go.offset);

	fscanf(of, "%lf\n", &sensors[3].go.gain);
	fscanf(of, "%lf\n", &sensors[3].go.offset);

	fclose(of);
	return 0;
}


int Sensors::process()
{
	int result = OK;
	RV_LineSensors ls;

	result = RV_getLineSensors(&ls);
	if (result != OK)
		return result;

    raw[0] = ls.N.active;
    raw[1] = ls.E.active;
    raw[2] = ls.S.active;
    raw[3] = ls.W.active;

    for (int i=0; i<nrOrients; i++) {
        afterGO[i] = (raw[i] + sensors[i].go.offset) * sensors[i].go.gain;
        sensors[i].flt->put(afterGO[i]);
        filtered[i] =sensors[i].flt->get();
    }

	return OK;
}

double Sensors::getN()
{
    return sensors[0].flt->get();
}

double Sensors::getE()
{
    return sensors[1].flt->get();
}

double Sensors::getS()
{
    return sensors[2].flt->get();
}

double Sensors::getW()
{
    return sensors[3].flt->get();
}
