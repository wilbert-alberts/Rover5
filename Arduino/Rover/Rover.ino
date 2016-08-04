#include "REG.h"
#include "COMM.h"
#include "MTR.h"
#include "POS.h"
#include "LINE.h"
#include "COL.h"
#include "MDC.h"
#include "HSI.h" 


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once

  Serial.println("Setup started.");
  pinMode(PULSE, OUTPUT);

  MDC_setup();
  REG_setup();
  COL_setup();
  LINE_setup();
  MTR_setup();
  POS_setup();
  COMM_setup();

  Serial.println("Setup ready");
}


void loop() {
  // put your main code here, to run repeatedly:

  //Serial.print("loop: ");
  //Serial.println(millis());
  REG_write32(REG_MICROS, micros());
  REG_write32(REG_MILLIS, millis());

  COL_loop();
  LINE_loop();  
  POS_loop();
  MTR_loop();
  COMM_loop();
  MDC_checkAlive();

  Pulse();
}

void Pulse()
{
  static int s=1;
  s = 1-s;
  digitalWrite(PULSE, s); 
}


