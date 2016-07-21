#include <Arduino.h>

#include "HSI.h"
#include "REG.h"

static int32_t pos_Left = 0;
static int32_t pos_Right = 0;

static void pos_isrLeftAUp();
static void pos_isrLeftADown();
static void pos_isrLeftBUp();
static void pos_isrLeftBDown();
static void pos_isrRightAUp();
static void pos_isrRightADown();
static void pos_isrRightBUp();
static void pos_isrRightBDown();



extern void POS_setup()
{
  pinMode(PIN_ENC_LEFT_A, INPUT);
  pinMode(PIN_ENC_LEFT_B, INPUT);
  pinMode(PIN_ENC_RIGHT_A, INPUT);
  pinMode(PIN_ENC_RIGHT_B, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_ENC_LEFT_A), pos_isrLeftAUp, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_LEFT_A), pos_isrLeftADown, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_LEFT_B), pos_isrLeftBUp, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_LEFT_B), pos_isrLeftBDown, FALLING);

  attachInterrupt(digitalPinToInterrupt(PIN_ENC_RIGHT_A), pos_isrRightAUp, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_RIGHT_A), pos_isrRightADown, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_RIGHT_B), pos_isrRightBUp, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_RIGHT_B), pos_isrRightBDown, FALLING);
}

extern void POS_test()
{
  Serial.print("Positiont Left: ");
  Serial.println(pos_Left);
  Serial.print("Positiont Right: ");
  Serial.println(pos_Right);
}

extern long POS_getLeft()
{
  return pos_Left;
}

extern long POS_getRight()
{
  return pos_Right;  
}

extern void POS_loop()
{
  REG_write32(REG_LEFTPOS, pos_Left);
  REG_write32(REG_RIGHTPOS, pos_Right);

  //Serial.println(pos_Left);
  //Serial.println(pos_Right);
}

static void pos_isrLeftAUp()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_LEFT_B) ==  LOW)
    pos_Left++;
  else
    pos_Left--;
}

static void pos_isrLeftADown()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_LEFT_B) ==  LOW)
    pos_Left--;
  else
    pos_Left++;
}

static void pos_isrLeftBUp()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_LEFT_A) ==  HIGH)
    pos_Left++;
  else
    pos_Left--;
}

static void pos_isrLeftBDown()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_LEFT_A) ==  HIGH)
    pos_Left--;
  else
    pos_Left++;
}

static void pos_isrRightAUp()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_RIGHT_B) ==  LOW)
    pos_Right++;
  else
    pos_Right--;
}

static void pos_isrRightADown()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_RIGHT_B) ==  LOW)
    pos_Right--;
  else
    pos_Right++;
}

static void pos_isrRightBUp()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_RIGHT_A) ==  HIGH)
    pos_Right++;
  else
    pos_Right--;
}

static void pos_isrRightBDown()
{
  // TODO: check signs
  if (digitalRead(PIN_ENC_RIGHT_A) ==  HIGH)
    pos_Right--;
  else
    pos_Right++;
}


