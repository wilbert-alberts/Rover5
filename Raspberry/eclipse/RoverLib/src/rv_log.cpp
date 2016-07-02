/*
 * rv_log.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

#include "rv.h"
#include "rv_log.h"

#define MAX_NR_THREADS (3) // Actually 2 should suffice.

static pthread_t rv_loggingThreads[MAX_NR_THREADS];

extern int RV_SetLogging(pthread_t tid, bool on) {
	printf("Setting logging for: %lud\n", tid);
	if (on) {
		for (int i = 0; i < MAX_NR_THREADS; i++)
			if (rv_loggingThreads[i] == 0) {
				rv_loggingThreads[i] = tid;
				return OK;
			}

	} else {
		int i;
		for (i = 0; i < MAX_NR_THREADS; i++)
			if (rv_loggingThreads[i] == tid)
				rv_loggingThreads[i] = 0;
		i++;
		while (i < MAX_NR_THREADS - 1) {
			rv_loggingThreads[i] = rv_loggingThreads[i + 1];
		}
		rv_loggingThreads[MAX_NR_THREADS - 1] = 0;

	}
	return OK;
}

extern bool rv_isLogginOn(pthread_t tid) {
	for (int i = 0; i < MAX_NR_THREADS; i++) {
		//printf("rv_logginTrhreads %d, %d vs %d\n", i, rv_loggingThreads[i], tid);
		if (rv_loggingThreads[i] == tid)
			return true;
	}
	return false;
}

extern void RV_LogEntry(const char* fname, const char *fmt, ...) {
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

extern void RV_LogExit(const char* fname, int result, const char *fmt, ...) {
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

