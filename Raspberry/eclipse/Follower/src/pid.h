/*
 * PID.h
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#ifndef PID_H_
#define PID_H_

class PID
{
public:
	PID(double period, double kp, double ki, double kd);
	virtual ~PID();

	void calculate(double err, double* out);

private:
	double period;
	double Kp;
	double Kd;
	double Ki;

	double integral;
	double prevErr;
};

#endif /* PID_H_ */
