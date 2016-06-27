/*
 * rv_log.h
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#ifndef RV_LOG_H_
#define RV_LOG_H_


extern int RV_LogEntry(const char* fname, const char *fmt, ...);
extern int RV_LogExit(const char* fname, int result, const char *fmt, ...);



#endif /* RV_LOG_H_ */
