#include <Arduino.h>

#include "HSI.h"
#include "MTR.h"

#define MDC_ALIVEDELAY (500ul)

static unsigned long mdc_lastAlive = 0;

extern void MDC_setup() {
  mdc_lastAlive = 0;
}

extern void MDC_triggerAlive()
{
  mdc_lastAlive = millis();
}

extern bool MDC_checkAlive()
{
  unsigned long delta  = millis()-mdc_lastAlive;
  bool result = delta < MDC_ALIVEDELAY;

  if (!result)
  {
     analogWrite(PIN_PWM_LEFT,0);
     analogWrite(PIN_PWM_RIGHT,0);
//     Serial.println("Connection with Pi lost. Motors shutdown.");
//     Serial.print("Last seen: ");
//     Serial.println(delta);
  }
  return result;
}

