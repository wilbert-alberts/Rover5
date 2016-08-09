/*
 * PID.h
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#ifndef PID_H_
#define PID_H_

#include <string>

class PID
{
public:
	PID(const std::string& id, double period, double kp, double ki, double kd);
	virtual ~PID();

	void calculate(double err, double* out);

private:
	double period;
	double Kp;
	double Kd;
	double Ki;

	double p;
	double d;
	double i;

	double in;
	double out;

	std::string id_in;
    std::string id_p;
    std::string id_i;
    std::string id_d;
    std::string id_out;

	double integral;
	double prevErr;
};

#endif /* PID_H_ */
