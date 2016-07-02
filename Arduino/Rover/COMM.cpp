#include <Arduino.h>
#include <SPI.h>

#include "REG.h"
#include "HSI.h"
#include "MDC.h"

static REG_map comm_SendBuffer;
static REG_map comm_ReceiveBuffer;

static void comm_doExchange();
static void comm_FillSendBuffer();
static void comm_AcknowledgeRequest();
static void comm_Exchange();
static void comm_EmptyReceiveBuffer();
static void comm_EndExchange();
static void comm_FillHeaderTrailer(REG_map* m);
static int comm_CheckHeaderTrailer(REG_map* m);

extern void COMM_setup()
{
  static const SPISettings comm_SPISettings(16000000, MSBFIRST, SPI_MODE3);  // TODO check spi mode

  pinMode(PIN_REQEXC, INPUT);
  pinMode(PIN_ACKEXC, OUTPUT);

  SPI.begin();
  SPI.beginTransaction(comm_SPISettings);
}

extern void COMM_loop()
{
  if (digitalRead(PIN_REQEXC)==HIGH) {
     comm_doExchange();
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
  if (header[0] != 0xA5)  return -2;  
  if (header[0] !=0xFF )  return -3;  
  if (header[0] != 0x5A)  return -4;  
  if (trailer[0] != 0xFF)  return -20;  
  if (trailer[0] != 0xA5)  return -30;  
  if (trailer[0] != 0x00)  return -40;  
  if (trailer[0] != 0x5A)  return -50;  
  return 0;
}

static void comm_AcknowledgeRequest()
{
  digitalWrite(PIN_ACKEXC, HIGH);
}

static void comm_Exchange()
{
  unsigned int i=0;
  byte* dst = (byte*) &comm_ReceiveBuffer;
  byte* src = (byte*) &comm_SendBuffer;

  while ((i<sizeof(comm_SendBuffer) && digitalRead(PIN_REQEXC) == HIGH)) {
       *dst = SPI.transfer(*src);
       dst++;
       src++;

       MDC_checkAlive();
  }
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
  while (digitalRead(PIN_REQEXC)==HIGH)
       MDC_checkAlive();

  digitalWrite(PIN_ACKEXC, LOW);
}

