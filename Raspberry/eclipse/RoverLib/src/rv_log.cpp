/*
 * rv_log.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
 */
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

#include "rv.h"
#include "rv_log.h"

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */

#define MAX_NR_THREADS (3) // Actually 2 should suffice.

/*
 * ---------------------------------------------------------------------------
 *               Static module data
 * ---------------------------------------------------------------------------
 */

/* rv_loggingThreads represents an arry of thread id's of threads that
 * have enabled logging. Unused entries of the array are set to 0. Note that
 * in order to decide effiently whether tracing is enabled for a thread,
 * size of this array should be as small as possible and the entries should
 * be stored without gaps. (I.e, when an entry is removed, the gap needs to be
 * filled by moving the subsequent ones, one place back.)
 */

static pthread_t rv_loggingThreads[MAX_NR_THREADS];

/*
 * ---------------------------------------------------------------------------
 *               Static function declarations.
 * ---------------------------------------------------------------------------
 */

static bool rv_isLogginOn(pthread_t tid);

/*
 * ---------------------------------------------------------------------------
 *               Module implementation
 * ---------------------------------------------------------------------------
 */

extern int LG_setLogging(pthread_t tid, bool on) {

	printf("Setting logging for: %lud\n", (unsigned long int)tid);
	if (on) {
	    /* In case logging needs to be enabled, search for the
	     * first non zero entry in the loggingThreads array.
	     */

		for (int i = 0; i < MAX_NR_THREADS; i++)
			if (rv_loggingThreads[i] == 0) {
			    /* Found emtpy entry, fill it. */
				rv_loggingThreads[i] = tid;
				return OK;
			}

	} else {

	    /* In case logging needs to be disabled, search
	     * for the corresponding entry in the loggingThreads
	     * array and set it to zero.
	     */
		int i;
		for (i = 0; i < MAX_NR_THREADS; i++)
			if (rv_loggingThreads[i] == tid)
				rv_loggingThreads[i] = 0;

		/* Shift following entries one back in order to
		 * 'fill the gap'.
		 */
		i++;
		while (i < MAX_NR_THREADS - 1) {
			rv_loggingThreads[i] = rv_loggingThreads[i + 1];
		}
		rv_loggingThreads[MAX_NR_THREADS - 1] = 0;

	}
	return OK;
}

/*
 * ---------------------------------------------------------------------------
 */
static bool rv_isLogginOn(pthread_t tid) {
    /* Note: we might want optimize this function by simply returning
     * false in case no tracing is desired at all.
     */

    /* Search the array for tid */
	for (int i = 0; i < MAX_NR_THREADS; i++) {
		//printf("rv_logginTrhreads %d, %d vs %d\n", i, rv_loggingThreads[i], tid);
		if (rv_loggingThreads[i] == tid)
			return true;
	}
	/* tid is not found in the array, tracing is not desired */
	return false;
}

/*
 * ---------------------------------------------------------------------------
 */
extern void LG_logEntry(const char* fname, const char *fmt, ...) {
	if (rv_isLogginOn(pthread_self())) {
		va_list argp;
		va_start(argp, fmt);

		fprintf(stderr, "%s >(", fname);
		if (fmt != NULL) {
			vfprintf(stderr, fmt, argp);
		}
		fprintf(stderr, ")\n");
	}
}

/*
 * ---------------------------------------------------------------------------
 */
extern void LG_logExit(const char* fname, int result, const char *fmt, ...) {
	if (rv_isLogginOn(pthread_self())) {
		va_list argp;
		va_start(argp, fmt);

		fprintf(stderr, "%s <(", fname);
		if (fmt != NULL) {
			vfprintf(stderr, fmt, argp);
		}
		fprintf(stderr, ") = %d\n", result);
	}
}

