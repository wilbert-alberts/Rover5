/*
 * main.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <stdio.h>
#include <limits.h>
#include <rv.h>

#define FREQUENCY     (100)
#define MAXDURATION   (10)
#define NRSAMPLES     (FREQUENCY * MAXDURATION)
#define COLLISIONSENS (130)


typedef struct
{
	double gain;
	double offset;
} GainOffset;

int mustStop(RV_CollisionSensors* s);
void determineGainOffset(GainOffset* go, RV_LineSensors[], int nrsamples);
void writeGainOffset(GainOffset* gof);


int main(int argc, char** argv)
{
	char c;
    int i;
    RV_LineSensors lineSensors[NRSAMPLES];
    RV_CollisionSensors collisionSensors;
    GainOffset gof[4];

    RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(100);

    printf("Position car such that it crosses the line and press enter\n");
    scanf("%c", &c);

    RV_start();
    for (i=0; i<NRSAMPLES; i++) {
      RV_getCollisionSensors(&collisionSensors);

      if (mustStop(&collisionSensors)) {
        RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
        break;
      }

      if (i == FREQUENCY)
    	  RV_move(RV_FORWARD, RV_FORWARD, 30,30);


      RV_getLineSensors(&lineSensors[i]);
      RV_waitForNewData();
    }

    RV_dumpBuffersToFile("RV_trace.txt");
    determineGainOffset(gof, lineSensors, i);

    writeGainOffset(gof);

    RV_stop();
}


void updateMinMax(int* min, int* max, int val)
{
	*min = val < *min ? val : *min;
	*max = val > *max ? val : *max;
}

void determineGainOffset(GainOffset* go, RV_LineSensors* ls, int nrsamples)
{
	int min[4];
	int max[4];

	for (int i=0; i<4; i++) {
		min[i] = INT_MAX;
		max[i] = INT_MIN;
	}

	for (int i=4; i<nrsamples; i++) {
		updateMinMax(&min[0],&max[0], ls->N.active);
		updateMinMax(&min[1],&max[1], ls->E.active);
		updateMinMax(&min[2],&max[2], ls->S.active);
		updateMinMax(&min[3],&max[3], ls->W.active);
	}

	for (int i=0; i<4; i++) {
		go[i].gain = 1.0 / (max[i] - min[i]);
		go[i].offset = min[i];
	}
}


int mustStop(RV_CollisionSensors* s)
{
    int d = COLLISIONSENS;
    if (s->NE.active > d)
	return 1;
    if (s->SE.active > d)
	return 1;
    if (s->SW.active > d)
	return 1;
    if (s->NW.active > d)
	return 1;
    return 0;
}

void writeGainOffset(GainOffset* gof)
{
	FILE* of = fopen("RV_GainOffset.txt", "w");

	if (of == NULL) {
		fprintf(stderr, "Error: unable to open file for writing: RV_GainOffset.txt\n");
		return;
	}

	fprintf(of, "%lf\n", gof[0].gain);
	fprintf(of, "%lf\n", gof[0].offset);

	fprintf(of, "%lf\n", gof[1].gain);
	fprintf(of, "%lf\n", gof[1].offset);

	fprintf(of, "%lf\n", gof[2].gain);
	fprintf(of, "%lf\n", gof[2].offset);

	fprintf(of, "%lf\n", gof[3].gain);
	fprintf(of, "%lf\n", gof[3].offset);

	fclose(of);

}
