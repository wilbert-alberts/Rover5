/*
 * rv_filter.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: walberts
 */


#include "rv_filter.h"
#include "rv_reg.h"
#include "rv.h"

static double filter_taps[LPFILTER80FILTER_TAP_NUM] = {
  0.0010020696070727152,
  0.0004985322164226077,
  0.00048448554732087644,
  0.0003353064829782722,
  0.000007854016538004452,
  -0.0005333357647385379,
  -0.0013117083005676656,
  -0.0023309360922054516,
  -0.0035692257345106638,
  -0.004975278446513338,
  -0.006465778050264458,
  -0.007925213625846922,
  -0.009208998017137279,
  -0.01015010890027469,
  -0.010568681487132154,
  -0.010283788386286187,
  -0.009126872551322475,
  -0.0069562941767722115,
  -0.00367194043712656,
  0.0007717230253083472,
  0.006355781978940095,
  0.012991186096423402,
  0.020517585940159913,
  0.028706877255374207,
  0.03727181424589789,
  0.04587988139203232,
  0.05417172402273162,
  0.0617823396546555,
  0.06836285340869895,
  0.07360135938260018,
  0.07724237924312997,
  0.07911050871955384,
  0.07911050871955384,
  0.07724237924312997,
  0.07360135938260018,
  0.06836285340869895,
  0.0617823396546555,
  0.05417172402273162,
  0.04587988139203232,
  0.03727181424589789,
  0.028706877255374207,
  0.020517585940159913,
  0.012991186096423402,
  0.006355781978940095,
  0.0007717230253083472,
  -0.00367194043712656,
  -0.0069562941767722115,
  -0.009126872551322475,
  -0.010283788386286187,
  -0.010568681487132154,
  -0.01015010890027469,
  -0.00920899801713728,
  -0.007925213625846922,
  -0.006465778050264458,
  -0.004975278446513338,
  -0.0035692257345106638,
  -0.0023309360922054516,
  -0.0013117083005676656,
  -0.0005333357647385414,
  0.000007854016538004452,
  0.00033530648297827565,
  0.00048448554732087644,
  0.0004985322164226086,
  0.0010020696070727152
};

void LPFilter80Filter_init(LPFilter80Filter* f) {
  int i;
  for(i = 0; i < LPFILTER80FILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void LPFilter80Filter_put(LPFilter80Filter* f, double input) {
  f->history[(f->last_index++) & 63] = input;
}

double LPFilter80Filter_get(LPFilter80Filter* f) {
  double acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < LPFILTER80FILTER_TAP_NUM; ++i) {
    acc += f->history[(index--) & 63] * filter_taps[i];
  };
  return acc;
}



static LPFilter80Filter rv_lineSensorFilters[4];


int RV_initLineSensorFilters()
{
    int result = OK;

    for (int i=0; i<4; i++) {
        LPFilter80Filter_init(&rv_lineSensorFilters[i]);
    }

    return result;
}

int RV_filterLineSensorFilter()
{
    int result = OK;

    uint16_t v;
    result = REG_read16(REG_IR_LINE_N, &v);
    LPFilter80Filter_put(&rv_lineSensorFilters[0], (double)v);
    result = REG_read16(REG_IR_LINE_E, &v);
    LPFilter80Filter_put(&rv_lineSensorFilters[1], (double)v);
    result = REG_read16(REG_IR_LINE_S, &v);
    LPFilter80Filter_put(&rv_lineSensorFilters[2], (double)v);
    result = REG_read16(REG_IR_LINE_W, &v);
    LPFilter80Filter_put(&rv_lineSensorFilters[3], (double)v);

    return result;
}

int RV_getFilteredLineSensors(RV_LineSensors* lineSensors)
{
    int result = OK;
    double v;
    v = LPFilter80Filter_get(&rv_lineSensorFilters[0]);
    lineSensors->N.active = (int)v;
    lineSensors->N.ambient = 0;
    v = LPFilter80Filter_get(&rv_lineSensorFilters[1]);
    lineSensors->E.active = (int)v;
    lineSensors->E.ambient = 0;
    v = LPFilter80Filter_get(&rv_lineSensorFilters[2]);
    lineSensors->S.active = (int)v;
    lineSensors->S.ambient = 0;
    v = LPFilter80Filter_get(&rv_lineSensorFilters[3]);
    lineSensors->W.active = (int)v;
    lineSensors->W.ambient = 0;

    return result;
}
