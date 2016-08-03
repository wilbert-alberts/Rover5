/*
 * rv_filter.h
 *
 *  Created on: Aug 3, 2016
 *      Author: walberts
 */

#ifndef RV_FILTER_H_
#define RV_FILTER_H_

#include "rv.h"

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 80 Hz

* 0 Hz - 2 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 0.625094254797975 dB

* 5 Hz - 40 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -56.334870712791876 dB

*/

#define LPFILTER80FILTER_TAP_NUM 64

typedef struct {
  double history[LPFILTER80FILTER_TAP_NUM];
  unsigned int last_index;
} LPFilter80Filter;

void LPFilter80Filter_init(LPFilter80Filter* f);
void LPFilter80Filter_put(LPFilter80Filter* f, double input);
double LPFilter80Filter_get(LPFilter80Filter* f);


int RV_initLineSensorFilters();
int RV_filterLineSensorFilter();
int RV_getFilteredLineSensors(RV_LineSensors* lineSensors);


#endif /* RV_FILTER_H_ */
