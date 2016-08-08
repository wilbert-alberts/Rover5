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
	    trRaw[i] = new Trace("s_Raw_"+orients[i]);
	    trFiltered[i] = new Trace("S_Filt_"+orients[i]);
	}
}


Sensors::~Sensors()
{
    for (int i=0; i<nrOrients; i++) {
        delete sensors[i].flt;
        delete trRaw[i];
        delete trFiltered[i];
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

    trRaw[0]->add(ls.N.active);
    trRaw[1]->add(ls.E.active);
    trRaw[2]->add(ls.S.active);
    trRaw[3]->add(ls.W.active);

	sensors[0].flt->put((ls.N.active + sensors[0].go.offset) * sensors[0].go.gain);
	sensors[1].flt->put((ls.E.active + sensors[0].go.offset) * sensors[0].go.gain);
	sensors[2].flt->put((ls.S.active + sensors[0].go.offset) * sensors[0].go.gain);
	sensors[3].flt->put((ls.W.active + sensors[0].go.offset) * sensors[0].go.gain);

	for (int i=0; i<nrOrients; i++) {
	    trFiltered[i]->add(sensors[i].flt->get());
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
