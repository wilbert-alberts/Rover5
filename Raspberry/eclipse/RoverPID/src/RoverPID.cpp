//============================================================================
// Name        : RoverPID.cpp
// Author      : Wilbert Alberts
// Version     :
// Copyright   : Copyright by ASM
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "rv.h"
#include "PID.h"
#include "POS.h"
#include <math.h>
#include <ReflexxesAPI.h>

#define FREQUENCY 100



static ReflexxesAPI *RML = NULL;
static RMLPositionInputParameters *IP = NULL;
static RMLPositionOutputParameters *OP = NULL;
static RMLPositionFlags Flags;

static void SPG_setup();
static void SPG_calculate(double* left, double* right);
static void SPG_progress();
static void setTorque(double l, double r);



int main(int argc, char* argv[])
{
	int ResultValue = 0;
	double posL;
	double posR;
	double spgL;
	double spgR;
	double torqueL;
	double torqueR;

	LOOP_setFrequency(FREQUENCY);

	RV_start();

	SPG_setup();

	PID* pidR= new PID(0.0100, 0.01, 0.001, 1.0/FREQUENCY);
	PID* pidL= new PID(0.0100, 0.01, 0.001, 1.0/FREQUENCY);
	POS* pos = new POS();

	while (ResultValue != ReflexxesAPI::RML_FINAL_STATE_REACHED)
	{
		LOOP_ForNewData();

		// Retrieve new data
		pos->getPosition(&posL, &posR);
		
		// Update SPG
		SPG_calculate(&spgL, &spgR);

//		printf("left: %f (%f), right: %f (%f)", posL, spgL, posR, spgR);

		// Update PID
		pidL->calculate(spgL-posL, &torqueL);
		pidR->calculate(spgR-posR, &torqueR);

//		printf(", torque (l,r): %f, %f\n", torqueL, torqueR);

		// Send new commands
		setTorque(torqueL, torqueR);

		// Let SPG make progress
		SPG_progress();
	}
	RV_stop();
}

void SPG_setup()
{
	RML = new ReflexxesAPI(2, 1.0 / FREQUENCY);
	IP = new RMLPositionInputParameters(2);
	OP = new RMLPositionOutputParameters(2);

	IP->CurrentPositionVector->VecData[0] = 0.0;
	IP->CurrentVelocityVector->VecData[0] = 0.0;
	IP->CurrentAccelerationVector->VecData[0] = 0.0;

	IP->MaxVelocityVector->VecData[0] = 5555500;
	IP->MaxAccelerationVector->VecData[0] = 500.0;
	IP->MaxJerkVector->VecData[0] = 1000.0;

	IP->TargetPositionVector->VecData[0] = 1000.0;
	IP->TargetVelocityVector->VecData[0] = 0.0;
	IP->SelectionVector->VecData[0] = true;

	IP->CurrentPositionVector->VecData[1] = 0.0;
	IP->CurrentVelocityVector->VecData[1] = 0.0;
	IP->CurrentAccelerationVector->VecData[1] = 0.0;

	IP->MaxVelocityVector->VecData[1] = 10;
	IP->MaxAccelerationVector->VecData[1] = 1.0;
	IP->MaxJerkVector->VecData[1] = 1000.0;

	IP->TargetPositionVector->VecData[1] = 1000.0;
	IP->TargetVelocityVector->VecData[1] = 0.0;
	IP->SelectionVector->VecData[1] = true;

}


void SPG_calculate(double* left, double* right)
{
	RML->RMLPosition(*IP, OP, Flags);

	*left = OP->NewPositionVector->VecData[0];
	*right = OP->NewPositionVector->VecData[1];
}

void SPG_progress()
{
	*IP->CurrentPositionVector = *OP->NewPositionVector;
	*IP->CurrentVelocityVector = *OP->NewVelocityVector;
	*IP->CurrentAccelerationVector = *OP->NewAccelerationVector;
}

void setTorque(double l, double r)
{
	int lDir = l > 0.0 ? RV_FORWARD : RV_BACKWARD;
	l = fabs(l);
	int rDir = r > 0.0 ? RV_FORWARD : RV_BACKWARD;
	r = fabs(r);
	l = l>1.0 ? 1.0 : l;
	r = r>1.0 ? 1.0 : r;

	RV_move(lDir, rDir, (uint8_t)(l*200), (uint8_t)(r*200)) ;

}
