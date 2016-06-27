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
	wiringPiSPIDataRW (SPICHANNEL, (unsigned char*)&rv_exchangeBuffer, sizeof(rv_exchangeBuffer));
	REG_writeAll(&rv_exchangeBuffer);
}
