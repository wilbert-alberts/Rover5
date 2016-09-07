/*
 * PID.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: walberts
 */

#include "pid.h"

#include "rv.h"

PID::PID(const std::string& _id, double _period, double _kp, double _ki, double _kd)
: id_in(_id+"_in")
, id_p(_id+"_p")
, id_i(_id+"_i")
, id_d(_id+"_d")
, id_out(_id+"_out")
{
	// TODO Auto-generated constructor stub
	period = _period;
	Kd = _kd;
	Kp = _kp;
	Ki = _ki;

	prevErr = 0.0;
	integral = 0.0;

	RV_addTraceVariable(id_in.c_str(), &in);
    RV_addTraceVariable(id_p.c_str(), &p);
    RV_addTraceVariable(id_i.c_str(), &i);
    RV_addTraceVariable(id_d.c_str(), &d);
    RV_addTraceVariable(id_out.c_str(), &out);
    RV_addTraceVariable("PID_P", &Kp);
    RV_addTraceVariable("PID_I", &Ki);
    RV_addTraceVariable("PID_D", &Kd);
}

PID::~PID()
{
	// TODO Auto-generated destructor stub
}

void PID::calculate(double err, double* tL)
{
    in = err;
    if (((err>0.0) && (prevErr<0.0)) ||
        ((err<0.0) && (prevErr>0.0)))
        integral = 0.0;
    else
        integral += (err * period);

	p = err * Kp;
	d = (err - prevErr) / period * Kd;
	i = integral * Ki;

	prevErr = err;
	out = p+d+i;
	*tL = out;
}
