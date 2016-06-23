#ifndef ROVER_H
#define ROVER_H

typedef void (*RV_collisionCB)(uint8_t* collisionRegister);

extern int RV_start();
extern int RV_stop();

extern int RV_getPosition(long* leftPos, long* rightPos);
extern int RV_move(char leftDirection, char rightDirection, int leftDC, int rightDC);

extern int RV_registerCollisionCB(RV_collisionCB);
extern int RV_deregistercollisionCB(RV_collisionCB);
extern int RV_registerLineCB(RV_lineCB);
extern int RV_deregisterLineCB(RV_lineCB);




#endif
