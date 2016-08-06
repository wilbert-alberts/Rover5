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
#define AMBDURATION   (5)
#define NRAMBSAMPLES  (AMBDURATION * FREQUENCY)
#define NRSAMPLES     (FREQUENCY * MAXDURATION)
#define COLLISIONSENS (130)


typedef struct
{
	double gain;
	double offset;
} GainOffset;

int mustStop();
void determineGainOffset(GainOffset* go, 
		RV_LineSensors* ambient,
		int nrAmbientSamples,
		RV_LineSensors* ls, 
		int nrsamples);
void writeGainOffset(GainOffset* gof);


int main(int argc, char** argv)
{
	char c;
    int i;
    RV_LineSensors lineSensors[NRSAMPLES];
    RV_LineSensors ambient[NRAMBSAMPLES];
    
    GainOffset gof[4];

    RV_loggingOff();
    RV_loopLoggingOff();
    RV_setFrequency(FREQUENCY);

    printf("Position car such that it crosses the line and press enter\n");
    scanf("%c", &c);

    RV_start();
    for (i=0; i<NRAMBSAMPLES; i++) {
      RV_waitForNewData();
      RV_getLineSensors(&ambient[i]);
    }
      
    for (i=0; i<NRSAMPLES; i++) {
      RV_waitForNewData();

      if (mustStop())
        break;

      if (i == FREQUENCY)
    	  RV_move(RV_FORWARD, RV_FORWARD, 30,30);

      RV_getLineSensors(&lineSensors[i]);
    }

    RV_dumpBuffersToFile("RV_trace.txt");
    determineGainOffset(gof, ambient, NRAMBSAMPLES, lineSensors, i);

    writeGainOffset(gof);

    RV_stop();
}


void updateMinMax(int* min, int* max, int val)
{
	*min = val < *min ? val : *min;
	*max = val > *max ? val : *max;
}

void determineGainOffset(GainOffset* go, 
		RV_LineSensors* ambient,
		int nrAmbientSamples,
		RV_LineSensors* ls, 
		int nrsamples)
{
	int min[4];
	int max[4];
	long long sum[4] = { 0,0,0,0 };
	
	for (int i=0; i<nrAmbientSamples; i++) {
		sum[0] += ambient[i].N.active;
		sum[1] += ambient[i].E.active;
		sum[2] += ambient[i].S.active;
		sum[3] += ambient[i].W.active;
	}
	
	for (int i=0; i<4; i++) {
		go[i].offset = -sum[i]/nrAmbientSamples;
	}
	
	for (int i=0; i<4; i++) {
		min[i] = INT_MAX;
		max[i] = INT_MIN;
	}

	for (int i=4; i<nrsamples; i++) {
		updateMinMax(&min[0],&max[0], ls[i].N.active);
		updateMinMax(&min[1],&max[1], ls[i].E.active);
		updateMinMax(&min[2],&max[2], ls[i].S.active);
		updateMinMax(&min[3],&max[3], ls[i].W.active);
	}

	for (int i=0; i<4; i++) {
		go[i].gain = 1.0 / (max[i] - go[i].offset);
	}
}


int mustStop()
{
    RV_CollisionSensors s;

    RV_getCollisionSensors(&s);

    
    if ((s.NE.active > COLLISIONSENS) ||
        (s.SE.active > COLLISIONSENS) ||
        (s.SW.active > COLLISIONSENS) ||
        (s.NW.active > COLLISIONSENS)) 
    {
    	RV_move(RV_FORWARD, RV_FORWARD, 0, 0);
	return 1;
    }
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
