/*
 * filter.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include "filter.h"

#define BWF_PLUSONE(X, Y) ((X)+1==(Y) ? 0 : (X)+1)
#define BWF_MINUSONE(X, Y) ((X)-1<0 ? (Y)-1 : (X)-1)

IIRFilter::IIRFilter(int o, double as[], double bs[])
: order(o)
, idx(0)
{
    A = new double[o];
    B = new double[o];
    for (int i = 0; i < order; i++)
    {
        A[i] = as[i];
        B[i] = bs[i];
    }
    X = new double[o];
    Y = new double[o];
}

IIRFilter::~IIRFilter()
{
    delete[] A;
    delete[] B;
    delete[] X;
    delete[] Y;
}

void IIRFilter::put(double v)
{
    X[idx] = v;

    Y[idx] = calc();

    idx = BWF_PLUSONE(idx, order);

}


double IIRFilter::calc()
{
    double r = 0.0;

    int yidx = BWF_MINUSONE(idx, order);
    for (int ai = 1; ai < order; ai++)
    {
        r -= A[ai] * Y[yidx];
        yidx = BWF_MINUSONE(yidx, order);
    }

    int xidx = idx;
    for (int bi = 0; bi < order; bi++)
    {
        r += B[bi] * X[xidx];
        xidx = BWF_MINUSONE(xidx, order);
    }
    return r;
}

double IIRFilter::get()
{
    int i = BWF_MINUSONE(idx, order);
    return Y[i];
}

