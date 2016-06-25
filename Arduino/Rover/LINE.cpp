#include <Arduino.h>

#include "HSI.h"
#include "REG.h"

#define LINE_NR_SENSORS (8)

static uint16_t line_ambient[LINE_NR_SENSORS];
static uint16_t line_active[LINE_NR_SENSORS];
static uint8_t  line_digitalized[LINE_NR_SENSORS];

static void line_setLights(int value);
static void line_readSensors(uint16_t* dst);
static void line_digitalize();
static void line_writeRegisters();


extern void LINE_setup()
{
  pinMode(PIN_LINE_NE, INPUT);
  pinMode(PIN_LINE_EN, INPUT);
  pinMode(PIN_LINE_ES, INPUT);
  pinMode(PIN_LINE_SE, INPUT);
  pinMode(PIN_LINE_SW, INPUT);
  pinMode(PIN_LINE_WS, INPUT);
  pinMode(PIN_LINE_WN, INPUT);
  pinMode(PIN_LINE_NW, INPUT);

  pinMode(PIN_LINE_LIGHT, OUTPUT);
}

extern void LINE_loop()
{
   line_readSensors(line_ambient);
   line_setLights(HIGH);
   delayMicroseconds(50);
   line_readSensors(line_active);
   line_setLights(LOW);
   line_digitalize();
   line_writeRegisters();
}


static void line_setLights(int value)
{
  digitalWrite(PIN_LINE_LIGHT, value);
}

static void line_readSensors(uint16_t* dst)
{
  static const int pins[] = { 
    PIN_LINE_NE,
    PIN_LINE_EN,
    PIN_LINE_ES,
    PIN_LINE_SE,
    PIN_LINE_SW,
    PIN_LINE_WS,
    PIN_LINE_WN,
    PIN_LINE_NW
  }; 
  int v;
  
  for (int i=0; i<LINE_NR_SENSORS; i++) {
    v=analogRead(pins[i]);
    *dst = v;
    dst++;
  }
  
}

static void line_digitalize()
{
  uint16_t offset;
  
  REG_read16(REG_AMBOFFSET, &offset);

  for (int i=0; i<LINE_NR_SENSORS; i++) {
    line_digitalized[i] = line_ambient[i] + offset < line_active[i];   
  }
}

static void line_writeRegisters()
{
  static const int registersAmb[] = { 
    REG_AMB_LINE_NE,
    REG_AMB_LINE_EN,
    REG_AMB_LINE_ES,
    REG_AMB_LINE_SE,
    REG_AMB_LINE_SW,
    REG_AMB_LINE_WS,
    REG_AMB_LINE_WN,
    REG_AMB_LINE_NW
  };
  static const int registersIR[] = { 
    REG_IR_LINE_NE,
    REG_IR_LINE_EN,
    REG_IR_LINE_ES,
    REG_IR_LINE_SE,
    REG_IR_LINE_SW,
    REG_IR_LINE_WS,
    REG_IR_LINE_WN,
    REG_IR_LINE_NW
    };
  static const int power[] = {
    REG_LINE_NE,
    REG_LINE_EN,
    REG_LINE_ES,
    REG_LINE_SE,
    REG_LINE_SW,
    REG_LINE_WS,
    REG_LINE_WN,
    REG_LINE_NW
  };
  static int8_t mask = 0;
  
  for (int i=0; i<LINE_NR_SENSORS; i++) {
    REG_write16(registersAmb[i], line_ambient[i]);
    REG_write16(registersIR[i], line_active[i]);
    mask |= line_digitalized[i] ? power[i] : 0;
  }
  REG_write8(REG_LINE, mask);
}

