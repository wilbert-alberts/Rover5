#include <Arduino.h>

#include "HSI.h"
#include "REG.h"

#define LINE_NR_SENSORS (4)

static uint16_t line_ambient[LINE_NR_SENSORS];
static uint16_t line_active[LINE_NR_SENSORS];

static void line_setLights(int value);
static void line_readSensors(uint16_t* dst);
static void line_digitalize();
static void line_writeRegisters();


extern void LINE_setup()
{
  pinMode(PIN_LINE_N, INPUT);
  pinMode(PIN_LINE_E, INPUT);
  pinMode(PIN_LINE_S, INPUT);
  pinMode(PIN_LINE_W, INPUT);

  pinMode(PIN_LINE_LIGHT, OUTPUT);
}

extern void LINE_loop()
{
   line_readSensors(line_ambient);
   line_setLights(HIGH);
   delayMicroseconds(50);
   line_readSensors(line_active);
   line_setLights(LOW);
   line_writeRegisters();
}


extern void LINE_test() 
{
  Serial.println("Testing line sensors");
  Serial.println("1: Turning all line lights on");
  line_setLights(HIGH);
  delay(3000);
  Serial.println("2: Turning all line lights off");
  line_setLights(LOW);
  delay(3000);  
}
static void line_setLights(int value)
{
  digitalWrite(PIN_LINE_LIGHT, value);
}

static void line_readSensors(uint16_t* dst)
{
  static const int pins[] = { 
    PIN_LINE_N,
    PIN_LINE_E,
    PIN_LINE_S,
    PIN_LINE_W,
  }; 
  int v;
  
  for (int i=0; i<LINE_NR_SENSORS; i++) {
    v=analogRead(pins[i]);
    *dst = v;
    dst++;
  }
  
}

static void line_writeRegisters()
{
  static const int registersAmb[] = { 
    REG_AMB_LINE_N,
    REG_AMB_LINE_E,
    REG_AMB_LINE_S,
    REG_AMB_LINE_W,
  };
  static const int registersIR[] = { 
    REG_IR_LINE_N,
    REG_IR_LINE_E,
    REG_IR_LINE_S,
    REG_IR_LINE_W,
    };

  static int8_t mask = 0;
  
  for (int i=0; i<LINE_NR_SENSORS; i++) {
    REG_write16(registersAmb[i], line_ambient[i]);
    REG_write16(registersIR[i], line_active[i]);
  }
}

