/*
 * tracing.h
 *
 *  Created on: Aug 8, 2016
 *      Author: walberts
 */

#ifndef TRACING_H_
#define TRACING_H_

#include <string>
#include <vector>
#include <map>


class Trace
{
public:
    Trace(const std::string& name);
    Trace();
    virtual ~Trace();

    void setName(const std::string& name);
    void add(double value);

    static void dumpTraces(const std::string& filename);
    static void stepTime();

private:
    void init(const std::string& name);

    typedef std::vector<double> TimeVector;
    typedef std::vector<TimeVector> Tracings;
    typedef std::vector<std::string> Tracenames;


    int idx;

    static Tracenames tracenames;
    static TimeVector currentTrace;
    static Tracings   traces;
    static int        nrTraces;
};


#endif /* TRACING_H_ */
