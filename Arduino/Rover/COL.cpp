#include <Arduino.h>

#include "HSI.h"
#include "REG.h"

#define COL_NR_SENSORS (4)

static uint16_t col_ambient[COL_NR_SENSORS];
static uint16_t col_active[COL_NR_SENSORS];
static uint8_t  col_digitalized[COL_NR_SENSORS];

static void col_setLights(int value);
static void col_readSensors(uint16_t* dst);
static void col_digitalize();
static void col_writeRegisters();


extern void COL_setup()
{
  pinMode(PIN_COL_NE, INPUT);
  pinMode(PIN_COL_NW, INPUT);
  pinMode(PIN_COL_SE, INPUT);
  pinMode(PIN_COL_SW, INPUT);

  pinMode(PIN_COL_LIGHT_NE, OUTPUT);
  pinMode(PIN_COL_LIGHT_NW, OUTPUT);
  pinMode(PIN_COL_LIGHT_SE, OUTPUT);
  pinMode(PIN_COL_LIGHT_SW, OUTPUT);
}

extern void COL_loop()
{
   col_readSensors(col_ambient);
   col_setLights(HIGH);
   delayMicroseconds(50);
   col_readSensors(col_active);
   col_setLights(LOW);
   col_digitalize();
   col_writeRegisters();
}

extern void COL_test() 
{
  uint16_t buffer[4];
  Serial.println("Testing collision sensors");
  Serial.println("1: Turning all collision lights on");
  col_setLights(HIGH);
  delay(1000);
  Serial.println("2: Reading all corner sensors (with lights on)");
  col_readSensors(buffer);
  Serial.print("NE: ");
  Serial.println(buffer[0]);
  Serial.print("NW: ");
  Serial.println(buffer[1]);
  Serial.print("SE: ");
  Serial.println(buffer[2]);
  Serial.print("SW: ");
  Serial.println(buffer[3]);
  Serial.println("3: Turning all collision lights off");
  col_setLights(LOW);
  delay(1000);  
  Serial.println("4: Reading all corner sensors (with lights off)");
  col_readSensors(buffer);
  Serial.print("NE: ");
  Serial.println(buffer[0]);
  Serial.print("NW: ");
  Serial.println(buffer[1]);
  Serial.print("SE: ");
  Serial.println(buffer[2]);
  Serial.print("SW: ");
  Serial.println(buffer[3]);
  delay(1000);  
}

static void col_setLights(int value)
{
  static const int pins[] = { PIN_COL_LIGHT_NE, PIN_COL_LIGHT_NW, PIN_COL_LIGHT_SE, PIN_COL_LIGHT_SW}; 

  for (int i=0; i<COL_NR_SENSORS; i++) {
    digitalWrite(pins[i], value);
  }
}

static void col_readSensors(uint16_t* dst)
{
  static const int pins[] = { PIN_COL_NE, PIN_COL_SE, PIN_COL_SW, PIN_COL_NW }; 
/*
  static const int pins[] = { PIN_COL_NE, 
                              PIN_COL_NE, 
                              PIN_COL_NE, 
                              PIN_COL_NE }; 
                              */
  int v;
  
  for (int i=0; i<COL_NR_SENSORS; i++) {
    v=analogRead(pins[i]);
    *dst = v;
    dst++;
  }
}

static void col_digitalize()
{
  uint16_t offset;
  
  REG_read16(REG_AMBOFFSET, &offset);

  for (int i=0; i<COL_NR_SENSORS; i++) {
    col_digitalized[i] = col_ambient[i] + offset < col_active[i];   
  }
}

static void col_writeRegisters()
{
  static const int registersAmb[] = { REG_AMB_COL_NE, REG_AMB_COL_SE, REG_AMB_COL_SW, REG_AMB_COL_NW };
  static const int registersIR[] = { REG_IR_COL_NE, REG_IR_COL_SE, REG_IR_COL_SW, REG_IR_COL_NW };
  static const int power[] = { REG_COL_NE , REG_COL_SE , REG_COL_SW , REG_COL_NW };
  static byte mask = 0;
  
  for (int i=0; i<COL_NR_SENSORS; i++) {
    REG_write16(registersAmb[i], col_ambient[i]);
    REG_write16(registersIR[i], col_active[i]);
    mask |= col_digitalized[i] ? power[i] : 0;
  }
  REG_write8(REG_COLLISION, mask);
}

