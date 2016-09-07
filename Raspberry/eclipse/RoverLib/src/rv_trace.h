#ifndef RV_TRACE_H_
#define RV_TRACE_H_

/*
 * rv_trace.h - operations to trace REG_maps.
 *
 *  Created on: Jul 15, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */


/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */

/* Allocate memory for 'size' trace records and initialize
 * administration for tracing.
 */
extern int TR_setup(int size);

/* Retrieve current register map and append it to the tracebuffer. Note that
 * tracebuffer is cyclic. When it's full, the oldest records will be overwritten
 * by newer ones .
 */
extern int TR_traceRegmap();

/* Write tracebuffers to file indicated by of.
 *
 */
extern int TR_dumpBuffers(FILE* of);

/* Register a user trace variable by name and source
 *
 */
extern int TR_addUsrReg(const char* name, const double* var);


#endif /* RV_TRACE_H_ */
