/*
 * POS.h
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#ifndef POS_H_
#define POS_H_

class POS
{
public:
	POS();
	virtual ~POS();

	void resetOffset();
	void getPosition(double* l, double* r);

private:
	long offsetL;
	long offsetR;

};

#endif /* POS_H_ */
