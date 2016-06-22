#ifndef ROVER_H
#define ROVER_H

typedef 

extern int RV_start();
extern int RV_stop();

extern int RV_getPosition(long* leftPos, long* rightPos);
extern int RV_move(char leftDirection, char rightDirection, int leftDC, int rightDC);


#endif
