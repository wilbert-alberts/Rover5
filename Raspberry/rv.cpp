#include <stdio.h>
#include "rv_loop.h"
#include "rv_reg.h"


extern int RV_start()
{
  printf("Starting exchange loop.");

  RV_startLoop();    
  REG_setup();
}

extern int RV_stop()
{
  RV_stopLoop();
}

extern int RV_getPosition(long* left, long* right) 
{
  int32_t lft;
  int32_t rgt;
  
  REG_read32(REG_LEFTPOS, &lft);
  REG_read32(REG_RIGHTPOS, &rgt); 
  
  *left = (long)lft;
  *right = (long) rgt;
}
