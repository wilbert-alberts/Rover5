#include "REG.h"
#include "COMM.h"
#include "MTR.h"
#include "POS.h"
#include "LINE.h"
#include "COL.h"
#include "MDC.h"


void setup() {
  // put your setup code here, to run once
  MDC_setup();
  REG_setup();
  COL_setup();
  LINE_setup();
  MTR_setup();
  POS_setup();
  COMM_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  COL_loop();
  LINE_loop();
  POS_loop();
  MTR_loop();
  COMM_loop();
}


