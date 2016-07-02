#include <Arduino.h>
#include <SPI.h>

#include "REG.h"
#include "HSI.h"
#include "MDC.h"

static REG_map  comm_ReceiveBuffer;
static REG_map  comm_SendBuffer;
volatile static uint8_t  comm_NrBytesReceived;
volatile static uint8_t * comm_SendPtr;
volatile static uint8_t * comm_RecvPtr;
volatile static bool     comm_Ready = false;

static void comm_doExchange();
static void comm_FillSendBuffer();
static void comm_AcknowledgeRequest();
static void comm_Exchange();
static void comm_EmptyReceiveBuffer();
static void comm_EndExchange();
static void comm_FillHeaderTrailer(REG_map* m);
static int comm_CheckHeaderTrailer(REG_map* m);
static void comm_LogBuffer(REG_map* m);


extern void COMM_setup()
{
  pinMode(PIN_REQEXC, INPUT);
  pinMode(PIN_ACKEXC, OUTPUT);
  pinMode(MISO, OUTPUT);
  
  SPCR |= _BV(SPE);

}

extern void COMM_loop()
{
  static int s = HIGH;
  if (digitalRead(PIN_REQEXC)==HIGH) {
     comm_doExchange();
     digitalWrite(13, s);
     s = s== HIGH ? LOW : HIGH;
  }
}

static void comm_doExchange() 
{
  comm_FillSendBuffer();
  comm_AcknowledgeRequest();
  comm_Exchange();  
  comm_EmptyReceiveBuffer();
  comm_EndExchange();

  MDC_triggerAlive();
}

static void comm_FillSendBuffer()
{
  REG_readAll(&comm_SendBuffer);
  comm_FillHeaderTrailer(&comm_SendBuffer);
}

static void comm_FillHeaderTrailer(REG_map* m)
{
  byte* header = (byte*)(&m->HEADER);
  byte* trailer= (byte*)(&m->TRAILER);
  
  header[0] = 0x00;
  header[1] = 0xA5;
  header[2] = 0xFF;
  header[3] = 0x5A;

  for (unsigned int i=4; i<sizeof(REG_map) - 4; i++) {
    header[i] = i;
  }

  trailer[0] = 0xFF;
  trailer[1] = 0xA5;
  trailer[2] = 0x00;
  trailer[3] = 0x5A;
}

static int comm_CheckHeaderTrailer(REG_map* m)
{
  byte* header = (byte*)(&m->HEADER);
  byte* trailer= (byte*)(&m->TRAILER);
  
  if (header[0] != 0x00)  return -1;  
  if (header[1] != 0xA5)  return -2;  
  if (header[2] !=0xFF )  return -3;  
  if (header[3] != 0x5A)  return -4;  
  if (trailer[0] != 0xFF)  return -20;  
  if (trailer[1] != 0xA5)  return -30;  
  if (trailer[2] != 0x00)  return -40;  
  if (trailer[3] != 0x5A)  return -50;  
  return 0;
}

static void comm_AcknowledgeRequest()
{
  comm_Ready = false;
  comm_NrBytesReceived = 0;
  comm_SendPtr = (uint8_t*)(&comm_SendBuffer);
  comm_RecvPtr = (uint8_t*)(&comm_ReceiveBuffer);

  uint8_t d = SPSR; // clear any pending interrupt.
  SPDR = *comm_SendPtr;
  
  SPI.attachInterrupt();
  
  digitalWrite(PIN_ACKEXC, HIGH);
}

ISR (SPI_STC_vect)
{
    *comm_RecvPtr++ = SPDR;    
    SPDR = *(++comm_SendPtr);
    comm_NrBytesReceived++;
    comm_Ready = comm_NrBytesReceived >= sizeof(comm_SendBuffer);
}

static void comm_Exchange()
{
  while ((!comm_Ready) && (digitalRead(PIN_REQEXC) == HIGH)) {
       delay(1);
       MDC_checkAlive();
  }
  SPI.detachInterrupt();

  // comm_LogBuffer(&comm_ReceiveBuffer);
}

static void comm_EmptyReceiveBuffer()
{
  if (comm_CheckHeaderTrailer(&comm_ReceiveBuffer)==0) 
  {
    REG_write8(REG_LEFTDIR, comm_ReceiveBuffer.LEFTDIR);
    REG_write8(REG_LEFTDC, comm_ReceiveBuffer.LEFTDC);
    REG_write8(REG_RIGHTDIR, comm_ReceiveBuffer.RIGHTDIR);
    REG_write8(REG_RIGHTDC, comm_ReceiveBuffer.RIGHTDC);
  } else {
    REG_logAll(&comm_ReceiveBuffer);
  }
}

static void comm_EndExchange()
{
  digitalWrite(PIN_ACKEXC, LOW);

  while (digitalRead(PIN_REQEXC)==HIGH) {
       MDC_checkAlive();
  }
}

static void comm_LogBuffer(REG_map* m)
{
 uint8_t* p = (uint8_t*)m;
  for (unsigned int i=0; i<sizeof(REG_map); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(p[i]);
  }
}




