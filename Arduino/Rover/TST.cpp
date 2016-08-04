#include <Arduino.h>

#include "HSI.h"
#include "LINE.h"
#include "COL.h"
#include "POS.h"
#include "MTR.h"

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

  //while () {
    Serial.println("Testing movement");
    Serial.print("Position (left, right): ");
    Serial.print(POS_getLeft());
    Serial.print(" ");
    Serial.println(POS_getRight());
    delay(1000);

    digitalWrite(PIN_DIR_LEFT, HIGH);
    digitalWrite(PIN_DIR_RIGHT, HIGH);
    analogWrite(PIN_PWM_LEFT, 50);
    analogWrite(PIN_PWM_RIGHT, 50);
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
    analogWrite(PIN_PWM_LEFT, 50);
    analogWrite(PIN_PWM_RIGHT, 50);
    delay(1000);
    analogWrite(PIN_PWM_LEFT, 0);
    analogWrite(PIN_PWM_RIGHT, 0);

    Serial.print("Position (left, right): ");
    Serial.print(POS_getLeft());
    Serial.print(" ");
    Serial.println(POS_getRight());
    delay(1000);
//}
}

