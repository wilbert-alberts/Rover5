#ifndef RV_LOG_H_
#define RV_LOG_H_

/*
 * rv_log.h - facility to log function entry/exits.
 *
 *
 * This module facilitates tracing function entry and exit
 * by printing a message to stdout whenever a function is
 * entered/exited.
 *
 * For this to work, each function should start with invoking
 * LG_logEntry and LG_logExit. The first argument represents the
 * function name (which can be provided by the preprocessor using
 * __func__. The second argument is a printf like format string
 * that can be used to specify how the variable list of arguments
 * following it should be formatted to stdout.
 *
 * Logging is enabled per thread. Therefore, in order to enable
 * disable logging the thread id needs to be provided to LG_setLogging.
 *
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
#include <pthread.h>

/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */
extern void LG_logEntry(const char* fname, const char *fmt, ...);
extern void LG_logExit(const char* fname, int result, const char *fmt, ...);

extern int LG_setLogging(pthread_t tid, bool on);


#endif /* RV_LOG_H_ */
