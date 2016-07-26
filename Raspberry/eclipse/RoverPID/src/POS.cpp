/*
 * POS.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#include "POS.h"

#include "rv.h"


POS::POS()
{
	// TODO Auto-generated constructor stub
	resetOffset();
}

POS::~POS()
{
	// TODO Auto-generated destructor stub
}

void POS::resetOffset()
{
	RV_getPosition(&offsetL, &offsetR);
}

void POS::getPosition(double* left, double* right)
{
	long l;
	long r;

	RV_getPosition(&l, &r);

	*left = l - offsetL;
	*right = r - offsetR;
}
