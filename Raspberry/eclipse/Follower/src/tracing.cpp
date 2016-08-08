/*
 * tracing.cpp
 *
 *  Created on: Aug 8, 2016
 *      Author: walberts
 */

#include "tracing.h"

#include <algorithm>
#include <fstream>

Trace::TimeVector Trace::currentTrace;
Trace::Tracings   Trace::traces;
Trace::Tracenames Trace::tracenames;

int        Trace::nrTraces = 0;

Trace::Trace(const std::string& n)
{
    init(n);
}

Trace::Trace()
{
    init("");
}

void Trace::init(const std::string& n)
{
    if (traces.size() > 0) {
        // TODO: report error that adding
        //       traces should not be done if
        //       data is already being added.
    }

    tracenames.push_back(n);
    idx = nrTraces;
    nrTraces++;

    currentTrace.resize(nrTraces);
}

Trace::~Trace()
{
    // TODO: warning, destructor not implemented.
    //       It is assumed that the number of
    //       traces does not decrement
}

void Trace::setName(const std::string& n)
{
    tracenames[idx] = n;
}

void Trace::add(double value)
{
    currentTrace[idx] = value;
}

void Trace::stepTime()
{
    traces.push_back(currentTrace);
    std::fill(currentTrace.begin(), currentTrace.end(), 0.0);
}

void Trace::dumpTraces(const std::string& filename)
{
    std::ofstream of(filename.c_str(), std::ios_base::app);

    for(Trace::Tracenames::iterator iter = tracenames.begin(); iter != tracenames.end(); iter++)
    {
        of << *iter << "\t";
    }
    of << std::endl;

    for(Trace::Tracings::iterator triter = traces.begin(); triter != traces.end(); triter++)
    {
        for(Trace::TimeVector::iterator tv = triter->begin(); tv != triter->end(); tv++)
        {
            of << *tv << "\t";
        }
        of << std::endl;
    }

    of.close();
}

