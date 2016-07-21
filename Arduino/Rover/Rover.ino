#include "REG.h"
#include "COMM.h"
#include "MTR.h"
#include "POS.h"
#include "LINE.h"
#include "COL.h"
#include "MDC.h"
#include "HSI.h" 

void tst_line();
void tst_line();


void setup() {
  Serial.begin(115200);
  Serial.println("Setup started.");

  tst_col();
  //tst_line();
  //tst_pos();
  //tst_mtr();
  //tst_movement();
  
  // put your setup code here, to run once
  MDC_setup();
  REG_setup();
  COL_setup();
  LINE_setup();
  MTR_setup();
  POS_setup();
  COMM_setup();
}

void tst_line()
{
  LINE_setup();
  while (1)
    LINE_test();  
}


void tst_col()
{
  COL_setup();
  while(1)
    COL_test();
}

void tst_pos()
{
  POS_setup();
  while(1)
    POS_test();
}


void tst_mtr()
{
  MTR_setup();
  while(1)
    MTR_test();
}

void tst_movement()
{
  MTR_setup();
  POS_setup();

  while (1) {
    Serial.println("Testing movement");
    Serial.print("Position (left, right): ");
    Serial.print(POS_getLeft());
    Serial.print(" ");
    Serial.println(POS_getRight());
    delay(1000);

    digitalWrite(PIN_DIR_LEFT, HIGH);
    digitalWrite(PIN_DIR_RIGHT, HIGH);
    analogWrite(PIN_PWM_LEFT, 100);
    analogWrite(PIN_PWM_RIGHT, 100);
    delay(1000);
    analogWrite(PIN_PWM_LEFT, 0);
    analogWrite(PIN_PWM_RIGHT, 0);

    Serial.print("Position (left, right): ");
    Serial.print(POS_getLeft());
    Serial.print(" ");
    Serial.println(POS_getRight());
    delay(1000);

    digitalWrite(PIN_DIR_LEFT, LOW);
    digitalWrite(PIN_DIR_RIGHT, LOW);
    analogWrite(PIN_PWM_LEFT, 100);
    analogWrite(PIN_PWM_RIGHT, 100);
    delay(1000);
    analogWrite(PIN_PWM_LEFT, 0);
    analogWrite(PIN_PWM_RIGHT, 0);

    Serial.print("Position (left, right): ");
    Serial.print(POS_getLeft());
    Serial.print(" ");
    Serial.println(POS_getRight());
    delay(1000);
}
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



