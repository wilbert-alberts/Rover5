/*
 * rv_exchange.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#include "rv_reg.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>


#define REQEXC (5)
#define ACKEXC (6)

#define SPICHANNEL (0)
#define SPISPEED   (4000000000) // 4Mhz

static int rv_exchangeSPI();
static void rv_exchangeFillHeaderTrailer(REG_map* m);
static int rv_exchangeCheckHeaderTrailer(REG_map* m);


static REG_map rv_exchangeBuffer;

extern int RV_exchangeSetup()
{
	pinMode(REQEXC, OUTPUT);
	digitalWrite(REQEXC, LOW);
	pinMode(ACKEXC, INPUT);
	wiringPiSPISetup (SPICHANNEL, SPISPEED) ;
}

extern int RV_exchangeWithMega()
{
	digitalWrite(REQEXC, HIGH);
	while (digitalRead(ACKEXC)==LOW)
		;// Busy wait

	rv_exchangeSPI();

	while (digitalRead(ACKEXC)==HIGH)
		; // Busy wait
	digitalWrite(REQEXC, LOW);
}

static int rv_exchangeSPI()
{
	REG_readAll(&rv_exchangeBuffer);
	rv_exchangeFillHeaderTrailer(&rv_exchangeBuffer);
	wiringPiSPIDataRW (SPICHANNEL, (unsigned char*)&rv_exchangeBuffer, sizeof(rv_exchangeBuffer));
	if (rv_exchangeCheckHeaderTrailer(&rv_exchangeBuffer)==0)
		REG_writeAll(&rv_exchangeBuffer);
}

static void rv_exchangeFillHeaderTrailer(REG_map* m)
{
  uint8_t* header = (uint8_t*)(&m->HEADER);
  uint8_t* trailer= (uint8_t*)(&m->TRAILER);

  header[0] = 0x00;
  header[1] = 0xA5;
  header[2] = 0xFF;
  header[3] = 0x5A;

  trailer[0] = 0xFF;
  trailer[1] = 0xA5;
  trailer[2] = 0x00;
  trailer[3] = 0x5A;
}

static int rv_exchangeCheckHeaderTrailer(REG_map* m)
{
  uint8_t* header = (uint8_t*)(&m->HEADER);
  uint8_t* trailer= (uint8_t*)(&m->TRAILER);

  if (header[0] != 0x00)  return -1;
  if (header[0] != 0xA5)  return -2;
  if (header[0] != 0xFF ) return -3;
  if (header[0] != 0x5A)  return -4;
  if (trailer[0] != 0xFF)  return -20;
  if (trailer[0] != 0xA5)  return -30;
  if (trailer[0] != 0x00)  return -40;
  if (trailer[0] != 0x5A)  return -50;
  return 0;
}

