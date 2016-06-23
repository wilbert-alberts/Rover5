#include <stdio.h>
#include "rv_loop.h"
#include "rv_reg.h"
#include "rv.h"

#define rv_IntToDirection(d,i) 	\
{ 								\
	switch (i) { 				\
	case RV_FORWARD: 			\
	case RV_BACKWARD:			\
		d = (uint8_t) (i); 	\
		break; 					\
	default: 					\
		return RV_ILLEGALVALUE; \
	} 							\
}

#define rv_IntToDC(dc, i) 		\
{								\
	if ((i<0) || (i>255))     	\
      return RV_ILLEGALVALUE; 	\
    dc = (uint8_t) (i);			\
}

extern int RV_start()
{
  printf("Starting exchange loop.");

  RV_startLoop();    
  REG_setup();
  return OK;
}

extern int RV_stop()
{
  RV_stopLoop();

  return OK;
}

extern int RV_getPosition(long* left, long* right) 
{
  int32_t lft;
  int32_t rgt;
  
  REG_read32(REG_LEFTPOS, &lft);
  REG_read32(REG_RIGHTPOS, &rgt); 
  
  *left = (long)lft;
  *right = (long) rgt;

  return OK;
}

extern int RV_move(int leftDirection,
				   int rightDirection,
				   int leftDC,
				   int rightDC)
{
	uint8_t ld;
	uint8_t rd;
	uint8_t ldc;
	uint8_t rdc;

	rv_IntToDirection(ld, leftDirection)
	rv_IntToDirection(rd, rightDirection)
	rv_IntToDC(ldc, leftDC)
	rv_IntToDC(rdc, rightDC)

	REG_write8(REG_LEFTDIR, ld);
	REG_write8(REG_LEFTDC, ldc);
	REG_write8(REG_RIGHTDIR, rd);
	REG_write8(REG_RIGHTDC, rdc);

    return OK;
}

extern int RV_getLine(uint8_t* r)
{
  REG_read8(REG_LINE, r);
  return OK;
}

extern int RV_getCollision(uint8_t* r)
{
  REG_read8(REG_COLLISION, r);
  return OK;
}
