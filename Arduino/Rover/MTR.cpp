#include <Arduino.h>

#include "HSI.h"
#include "REG.h"
#include "MDC.h"

static  byte mtr_ldir;
static  byte mtr_rdir;
static  byte mtr_ldc;
static  byte mtr_rdc;

extern void MTR_setup()
{
  pinMode(PIN_PWM_LEFT, OUTPUT);
  pinMode(PIN_PWM_RIGHT, OUTPUT);
  pinMode(PIN_DIR_LEFT, OUTPUT);
  pinMode(PIN_DIR_RIGHT, OUTPUT);
}

extern void MTR_loop()
{
  if (MDC_checkAlive()) {
    REG_read8(REG_LEFTDIR, &mtr_ldir);
    REG_read8(REG_RIGHTDIR, &mtr_rdir);
    REG_read8(REG_LEFTDC, &mtr_ldc);
    REG_read8(REG_RIGHTDC, &mtr_rdc);
  
    digitalWrite(PIN_DIR_LEFT, mtr_ldir);
    analogWrite(PIN_PWM_LEFT,mtr_ldc);
    
    digitalWrite(PIN_DIR_RIGHT, mtr_rdir);
    analogWrite(PIN_PWM_RIGHT,mtr_rdc);  
  }
  else {
    analogWrite(PIN_PWM_LEFT,  0);
    analogWrite(PIN_PWM_RIGHT, 0);      
  }
}


