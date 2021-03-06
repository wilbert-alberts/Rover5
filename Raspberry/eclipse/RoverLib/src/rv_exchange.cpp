/*
 * rv_exchange.cpp
 *
 * This module realizes interaction with the AVR.
 *
 * Interaction with the AVR consists of the following phases:
 * - The PI indicates that it wants to exchange and waits for the AVR to
 *   become ready
 * - The AVR prepares and indicates that it is ready to exchange
 * - Pi and AVR exchange using SPI
 * - The PI indicates end of transmission and waits on AVR to acknowledge.
 * - The AVR acknowledges end of transmission
 *
 *  Created on: Jun 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 *
 *
 */

/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>

#include "rv.h"
#include "rv_log.h"
#include "rv_reg.h"

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */

#define REQEXC  (5)
#define ACKEXC  (6)
#define SS     	(4)

#define SPICHANNEL (0)
#define SPISPEED   (2000000UL) // 2Mhz

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}


/*
 * ---------------------------------------------------------------------------
 *               Static function declarations.
 * ---------------------------------------------------------------------------
 */

static int ex_communicateSPI();
static int ex_fillHeaderTrailer(REG_map* m);
static int ex_checkHeaderTrailer(REG_map* m);
static void ex_logBuffer(REG_map* m);
static void ex_mysleep(int ndelay);

/*
 * ---------------------------------------------------------------------------
 *               Static module data.
 * ---------------------------------------------------------------------------
 */

int ex_spiByteDelay = 7000; // in nanoseconds; e.g. 7 us

static REG_map ex_buffer;

/*
 * ---------------------------------------------------------------------------
 *               Module implementation
 * ---------------------------------------------------------------------------
 */

extern int EX_setup() {
	int result = OK;
	int fd = 0;
	LG_logEntry(__func__, NULL);

	/* Set data directions on a number of pins.
	 *
	 * SS - Slave select is output for PI
	 * REQEXC - Request exchange is output for PI
	 * ACKEXC - Acknowledge exchange request is input for PI.
	 */
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);

	pinMode(REQEXC, OUTPUT);
	digitalWrite(REQEXC, LOW);

	pinMode(ACKEXC, INPUT);

	/* Setup SPI channel and speed */
	fd = wiringPiSPISetup(SPICHANNEL, SPISPEED);
	result = (fd == -1) ? RV_EXCHANGE_SETUP_FAILED : OK;

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

extern int EX_communicate() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* Request exchange by setting REQEXC HIGH */
	digitalWrite(REQEXC, HIGH);
	/* Wait for AVR to acknowledge */
	while ((result == OK) and (digitalRead(ACKEXC) == LOW))
		; // Busy wait

	/* SPI exchange */
	SAFE_INVOKE(ex_communicateSPI(), result, RV_EXCHANGE_FAILED)

	/* Indicate end of exchange */
	digitalWrite(REQEXC, LOW);
	/* Wait for AVR to acknowledge */
	while ((digitalRead(ACKEXC) == HIGH))
		; // Busy wait

	LG_logExit(__func__, result, NULL);
	return result;
}


/*
 * ---------------------------------------------------------------------------
 */

static int ex_communicateSPI() {
	int result = OK;
	LG_logEntry(__func__, NULL);
	uint8_t* b;
	// const struct timespec delta = { 0, 1 };

	/* Retrieve data to be exchanged from register map */
	REG_readAll(&ex_buffer);

	/* Ensure that header and trailer are properly filled */
	SAFE_INVOKE(ex_fillHeaderTrailer(&ex_buffer), result,
			RV_EXCHANGESPI_FAILED)

	/* Pull down slave select. This signals the AVR
	 * that SPI data is coming.
	 * */
	digitalWrite(SS, LOW);

	/* Due to the fact that the AVR requires a little bit of time
	 * between two bytes, we need to perform the transfer byte
	 * by byte.
	 */
	b = (uint8_t*) &ex_buffer;
	for (unsigned int i = 0; i < sizeof(ex_buffer); i++) {

	    /* Exchange exactly one byte */
		wiringPiSPIDataRW(SPICHANNEL, (unsigned char*) (b + i), 1);

		///We need to sleep here for a little while ~20us
		ex_mysleep(ex_spiByteDelay);
	}

	/* Indicate end of SPI transfer by making Slave select high */
	digitalWrite(SS, HIGH);

	/* Check integrity of message received from AVR */
	SAFE_INVOKE(ex_checkHeaderTrailer(&ex_buffer), result,
			RV_EXCHANGESPI_FAILED)

	/* If data is not OK, log to console */
	if (result != OK) {
		ex_logBuffer(&ex_buffer);
		REG_logAll(&ex_buffer);
	} 
	else {
	    // Whatever the AVR sent back, we want to keep
	    // the DC and direction registers. So we copy
	    // the existing values over the ones we just
	    // received before updating the registermap
	    // as a whole.
		REG_read8(REG_LEFTDIR, &ex_buffer.LEFTDIR);
		REG_read8(REG_LEFTDC, &ex_buffer.LEFTDC);
		REG_read8(REG_RIGHTDIR, &ex_buffer.RIGHTDIR);
		REG_read8(REG_RIGHTDC, &ex_buffer.RIGHTDC);
		REG_writeAll(&ex_buffer);
	}
	
	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

static int ex_fillHeaderTrailer(REG_map* m) {
	uint8_t* header = (uint8_t*) (&m->HEADER);
	uint8_t* trailer = (uint8_t*) (&m->TRAILER);

	header[0] = 0x00;
	header[1] = 0xA5;
	header[2] = 0xFF;
	header[3] = 0x5A;

	// TODO: remove
	//for (unsigned int i=4; i<sizeof(REG_map)-4; i++) {
//	  header[i] = i;
	//}

	trailer[0] = 0xFF;
	trailer[1] = 0xA5;
	trailer[2] = 0x00;
	trailer[3] = 0x5A;

	return OK;
}

/*
 * ---------------------------------------------------------------------------
 */

static int ex_checkHeaderTrailer(REG_map* m) {
	uint8_t* header = (uint8_t*) (&m->HEADER);
	uint8_t* trailer = (uint8_t*) (&m->TRAILER);

	if (header[0] != 0x00)
		return -1001;
	if (header[1] != 0xA5)
		return -1002;
	if (header[2] != 0xFF)
		return -1003;
	if (header[3] != 0x5A)
		return -1004;
	if (trailer[0] != 0xFF)
		return -1020;
	if (trailer[1] != 0xA5)
		return -1030;
	if (trailer[2] != 0x00)
		return -1040;
	if (trailer[3] != 0x5A)
		return -1050;
	return OK;
}

/*
 * ---------------------------------------------------------------------------
 *
 * Dump registermap to stdout for diagnostic purposes.
 */

static void ex_logBuffer(REG_map* m) {
	uint8_t* p = (uint8_t*) m;
	for (unsigned int i = 0; i < sizeof(REG_map); i++) {
		printf("%d: %d\n", i, p[i]);
	}
}

/*
 * ---------------------------------------------------------------------------
 *
 * We need to sleep for a small delay. The linux scheduler does not allow
 * us to sleep for that duration. Therefore we use some kind of busy waiting
 * here.
 *
 */

static void ex_mysleep(int ndelay)
{
	struct timespec now;
	struct timespec end;

	clock_gettime(CLOCK_MONOTONIC, &end);
	end.tv_nsec+=ndelay;
	if (end.tv_nsec>1000000000L) {
		end.tv_nsec -= 1000000000L;
		end.tv_sec++;
	}		
	do {
		clock_gettime(CLOCK_MONOTONIC, &now);
	} while ((end.tv_sec > now.tv_sec) || 
		 ((end.tv_sec == now.tv_sec) &&
		  (end.tv_nsec > now.tv_nsec)));
}
