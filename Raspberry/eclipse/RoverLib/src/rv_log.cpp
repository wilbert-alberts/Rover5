/*
 * rv_log.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#include <stdarg.h>
#include <stdio.h>


static bool rv_logging = false;

extern int RV_LoggingOn()
{
	rv_logging = true;
}

extern int RV_LoggingOff()
{
	rv_logging = false;
}

extern int RV_LogEntry(const char* fname, const char *fmt, ...) {
	if (rv_logging)
	{
	va_list argp;
	va_start(argp, fmt);

	fprintf(stderr, "%s >(", fname);
	if (fmt != NULL) {
		vfprintf(stderr, fmt, argp);
	}
	fprintf(stderr, ")\n");
	}
}

extern int RV_LogExit(const char* fname, int result, const char *fmt, ...) {
	if (rv_logging)
	{
	va_list argp;
	va_start(argp, fmt);

	fprintf(stderr, "%s <(", fname);
	if (fmt != NULL) {
		vfprintf(stderr, fmt, argp);
	}
	fprintf(stderr, ") = 0x%0x\n", result);
}
}

