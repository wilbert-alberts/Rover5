#ifndef RV_LOOP_H
#define RV_LOOP_H

extern int LP_start();
extern int LP_stop();
extern int LP_loggingOn();
extern int LP_loggingOff();
extern int LP_setFrequency(int herz);
extern int LP_waitForNewData();

#endif
