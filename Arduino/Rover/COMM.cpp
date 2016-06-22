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
  if (PIN_REQEXC==HIGH) {
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
  REG_write32(REG_MICROS, micros());
  REG_write32(REG_MILLIS, millis());
  REG_readAll(&comm_SendBuffer);
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
  REG_write8(REG_LEFTDIR, comm_ReceiveBuffer.LEFTDIR);
  REG_write8(REG_LEFTDC, comm_ReceiveBuffer.LEFTDC);
  REG_write8(REG_RIGHTDIR, comm_ReceiveBuffer.RIGHTDIR);
  REG_write8(REG_RIGHTDC, comm_ReceiveBuffer.RIGHTDC);
}

static void comm_EndExchange()
{
  while (digitalRead(PIN_REQEXC)==HIGH)
       MDC_checkAlive();

  digitalWrite(PIN_ACKEXC, LOW);
}

