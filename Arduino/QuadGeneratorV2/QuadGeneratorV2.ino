#include <Arduino_FreeRTOS.h>
#include <croutine.h>
#include <event_groups.h>
#include <FreeRTOSConfig.h>
#include <FreeRTOSVariant.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <portmacro.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <StackMacros.h>
#include <task.h>
#include <timers.h>


// define two tasks for Blink & AnalogRead
void GenQuad( void *pvParameters );

typedef struct 
{
  long nrCycles;
  long cycleduration; // in microseconds
  int  pinA;
  int  pinB;
} GenQuadConfig;

static GenQuadConfig gqLeft { 100, 1000000UL, 18 , 19};
static GenQuadConfig gqRight { 100, 1000000UL, 2 , 3};

// the setup function runs once when you press reset or power the board
void setup() {

  // Now set up two tasks to run independently.
  xTaskCreate(
    GenQuad
    ,  (const portCHAR *)"GenQuadLeft"   // A name just for humans
    ,  128  // Stack size
    ,  &gqLeft
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    GenQuad
    ,  (const portCHAR *) "GenQuadRight"
    ,  128 // This stack size can be checked & adjusted by reading Highwater
    ,  &gqRight
    ,  2  // priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void GenQuad(void *pvParameters)  // This is a task.
{
  GenQuadConfig* gc = (GenQuadConfig*)pvParameters;
  long pause = gc->cycleduration/4;

  pinMode(gc->pinA, OUTPUT);
  pinMode(gc->pinB, OUTPUT);

  for (long ctr=0; ctr < gc->nrCycles; ctr++) // A Task shall never return or exit.
  {
    digitalWrite(gc->pinA, HIGH);
    delayMicroseconds(pause);
    digitalWrite(gc->pinB, HIGH);
    delayMicroseconds(pause);
    digitalWrite(gc->pinA, LOW);
    delayMicroseconds(pause);
    digitalWrite(gc->pinB, LOW);
    delayMicroseconds(pause);
  }
}

