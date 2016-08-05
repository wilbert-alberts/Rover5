/*
 * filter.h
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#ifndef FILTER_H_
#define FILTER_H_

typedef void* BWFilter;

extern int BWF_create(BWFilter* flt, int order, double* As, double* Bs);
extern int BWF_destroy(BWFilter* flt);
extern int BWF_put(BWFilter flt, double val);
extern int BWF_get(BWFilter flt, double* r);


#endif /* FILTER_H_ */
