#include "REG.h"
#include "COMM.h"
#include "MTR.h"
#include "POS.h"
#include "LINE.h"
#include "COL.h"
#include "MDC.h"


void setup() {
  Serial.begin(115200);
  Serial.println("Setup started.");
  
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
  //Serial.print("loop: ");
  //Serial.println(millis());
  // put your main code here, to run repeatedly:

  REG_write32(REG_MICROS, micros());
  REG_write32(REG_MILLIS, millis());
  
  COL_loop();
  LINE_loop();
  POS_loop();
  MTR_loop();
  COMM_loop();
}


