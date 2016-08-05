/*
 * filter.cpp
 *
 *  Created on: 5 aug. 2016
 *      Author: wilbert
 */

#include <stdlib.h>

#define BWF_PLUSONE(X, Y) ((X)+1==(Y) ? 0 : (X)+1)
#define BWF_MINUSONE(X, Y) ((X)-1<0 ? (Y)-1 : (X)-1)

typedef void* BWFilter;

typedef struct {
	int order;
	double *A;
	double *B;
	double *x;
	double *y;

	int idx;
} BWFilter_int;

static double bwf_calc(BWFilter_int* flt);


extern int BWF_create(BWFilter* flt, int order, double* As, double* Bs) {

	BWFilter_int* r = (BWFilter_int*)calloc(1, sizeof(BWFilter_int));
	if (r != NULL) {
		r->A = (double*)calloc(order, sizeof(double));
		r->B = (double*)calloc(order, sizeof(double));
		r->x = (double*)calloc(order, sizeof(double));
		r->y = (double*)calloc(order, sizeof(double));
		r->order = order;
		r->idx = 0;

		for (int i = 0; i < order; i++) {
			r->A[i] = As[i];
			r->B[i] = Bs[i];
		}
		*flt = r;
		return 0;
	} else {
		return -1;
	}
}

extern int BWF_destroy(BWFilter* f) {
	BWFilter_int* flt= (BWFilter_int*) *f;

	if (flt != NULL)
		free(flt);
	*f = NULL;

	return 0;

}

extern int BWF_put(BWFilter f, double val) {
	BWFilter_int* flt= (BWFilter_int*) f;
	flt->x[flt->idx] = val;

	flt->y[flt->idx] = bwf_calc(flt);

	flt->idx = BWF_PLUSONE(flt->idx, flt->order);

	return 0;
}

double bwf_calc(BWFilter_int* flt) {
	double r = 0.0;

	int yidx = BWF_MINUSONE(flt->idx, flt->order);
	for (int ai = 1; ai < flt->order; ai++) {
		r -= flt->A[ai] * flt->y[yidx];
		yidx = BWF_MINUSONE(yidx, flt->order);
	}

	int xidx = flt->idx;
	for (int bi = 0; bi < flt->order; bi++) {
		r += flt->B[bi] * flt->x[xidx];
		xidx = BWF_MINUSONE(xidx, flt->order);
	}
	return r;
}

extern int BWF_get(BWFilter f, double* r) {
	BWFilter_int* flt= (BWFilter_int*) f;
	int idx = BWF_MINUSONE(flt->idx, flt->order);
	*r = flt->y[idx];

	return 0;
}



/* Filter test
int main (int argc, char** argv) {

	BWFilter f;
	double a[3];
	double b[3];
	double r;

    a[0] = 1.0;
    a[1] = -1.86689;
    a[2] = 0.87521;

    b[0] = 0.0020806;
    b[1] = 0.0041611;
    b[2] = 0.0020806;

    BWF_create(&f, 3, a, b);

    for (int i = 0; i<100; i++) {
    	BWF_put(f, i);

    	BWF_get(f, &r);

    	printf ("%d, %d, %lf\n", i, i, r);
    }

    return 0;
}
*/
