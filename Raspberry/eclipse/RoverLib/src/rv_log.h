/*
 * rv_log.h
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#ifndef RV_LOG_H_
#define RV_LOG_H_

#include <pthread.h>

extern void LG_logEntry(const char* fname, const char *fmt, ...);
extern void LG_logExit(const char* fname, int result, const char *fmt, ...);

extern int LG_setLogging(pthread_t tid, bool on);


#endif /* RV_LOG_H_ */
