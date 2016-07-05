/*
 * rv_exchange.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 */

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_reg.h"


#define REQEXC (5)
#define ACKEXC (6)
#define SS     (4)


#define SPICHANNEL (0)
#define SPISPEED   (2000000UL) // 1Mhz

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}

static int rv_exchangeSPI();
static int rv_exchangeFillHeaderTrailer(REG_map* m);
static int rv_exchangeCheckHeaderTrailer(REG_map* m);
static void rv_exchangeLogBuffer(REG_map* m);


static REG_map rv_exchangeBuffer;

extern int RV_exchangeSetup()
{
	int result = OK;
	int fd = 0;
	RV_LogEntry(__func__, NULL);

	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	pinMode(REQEXC, OUTPUT);
	digitalWrite(REQEXC, LOW);
	pinMode(ACKEXC, INPUT);

	fd = wiringPiSPISetup (SPICHANNEL, SPISPEED);
	result = (fd==-1) ? RV_EXCHANGE_SETUP_FAILED : OK ;

	RV_LogExit(__func__, result, NULL);
	return result;
}

extern int RV_exchangeWithMega()
{
	int result = OK;
	RV_LogEntry(__func__, NULL);

	digitalWrite(REQEXC, HIGH);

	while ((result == OK ) and (digitalRead(ACKEXC)==LOW))
		;// Busy wait

	SAFE_INVOKE(rv_exchangeSPI(), result, RV_EXCHANGE_FAILED)

	while ((result == OK) and (digitalRead(ACKEXC)==HIGH))
		; // Busy wait
	digitalWrite(REQEXC, LOW);

	RV_LogExit(__func__, result, NULL);
	return result;
}

static int rv_exchangeSPI()
{
	int result = OK;
	RV_LogEntry(__func__, NULL);
	uint8_t* b;
	const struct timespec delta = { 0, 20000 };

	REG_readAll(&rv_exchangeBuffer);
	SAFE_INVOKE(rv_exchangeFillHeaderTrailer(&rv_exchangeBuffer), result, RV_EXCHANGESPI_FAILED)

        digitalWrite(SS, LOW);

	b = (uint8_t*)&rv_exchangeBuffer;
	for (unsigned int i=0; i<sizeof(rv_exchangeBuffer); i++) {
		wiringPiSPIDataRW (SPICHANNEL, (unsigned char*)(b+i), 1);
		for (volatile long d=0; d<1000; d++) {
			volatile long dd;
			dd=d;
		}
		//nanosleep(&delta, NULL);
	}

	//wiringPiSPIDataRW (SPICHANNEL, (unsigned char*)&rv_exchangeBuffer, sizeof(rv_exchangeBuffer));
	digitalWrite(SS, HIGH);


	// rv_exchangeLogBuffer(&rv_exchangeBuffer);
	SAFE_INVOKE(rv_exchangeCheckHeaderTrailer(&rv_exchangeBuffer), result, RV_EXCHANGESPI_FAILED)
	if (result != OK) {
		rv_exchangeLogBuffer(&rv_exchangeBuffer);
		REG_logAll(&rv_exchangeBuffer);
	}
	SAFE_INVOKE(REG_writeAll(&rv_exchangeBuffer), result, RV_EXCHANGESPI_FAILED)

	RV_LogExit(__func__, result, NULL);
	return result;
}

static int rv_exchangeFillHeaderTrailer(REG_map* m)
{
  uint8_t* header = (uint8_t*)(&m->HEADER);
  uint8_t* trailer= (uint8_t*)(&m->TRAILER);

  header[0] = 0x00;
  header[1] = 0xA5;
  header[2] = 0xFF;
  header[3] = 0x5A;

  // TODO: remove
  for (unsigned int i=4; i<sizeof(REG_map)-4; i++) {
	  header[i] = i;
  }

  trailer[0] = 0xFF;
  trailer[1] = 0xA5;
  trailer[2] = 0x00;
  trailer[3] = 0x5A;

  return OK;
}

static int rv_exchangeCheckHeaderTrailer(REG_map* m)
{
  uint8_t* header = (uint8_t*)(&m->HEADER);
  uint8_t* trailer= (uint8_t*)(&m->TRAILER);

  if (header[0] != 0x00)  return -1001;
  if (header[1] != 0xA5)  return -1002;
  if (header[2] != 0xFF ) return -1003;
  if (header[3] != 0x5A)  return -1004;
  if (trailer[0] != 0xFF)  return -1020;
  if (trailer[1] != 0xA5)  return -1030;
  if (trailer[2] != 0x00)  return -1040;
  if (trailer[3] != 0x5A)  return -1050;
  return OK;
}

static void rv_exchangeLogBuffer(REG_map* m)
{
	uint8_t* p = (uint8_t*)m;
	for (unsigned int i=0; i<sizeof(REG_map); i++) {
		printf("%d: %d\n", i, p[i]);
	}
}
