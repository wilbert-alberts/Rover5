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
#define SS     	(4)
//#define RTS 	(3)  
//#define RECEIVING	(7)

#define SPICHANNEL (0)
#define SPISPEED   (2000000UL) // 2Mhz

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) \
			r = c; \
	}

static int ex_communicateSPI();
static int ex_fillHeaderTrailer(REG_map* m);
static int ex_checkHeaderTrailer(REG_map* m);
static void ex_logBuffer(REG_map* m);
static void ex_mysleep(int ndelay);

int ex_spiByteDelay = 7000; // in nanoseconds; e.g. 7 us

static REG_map ex_buffer;

extern int EX_setup() {
	int result = OK;
	int fd = 0;
	LG_logEntry(__func__, NULL);

	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);

	pinMode(REQEXC, OUTPUT);
	digitalWrite(REQEXC, LOW);

	pinMode(ACKEXC, INPUT);

	fd = wiringPiSPISetup(SPICHANNEL, SPISPEED);
	result = (fd == -1) ? RV_EXCHANGE_SETUP_FAILED : OK;

	LG_logExit(__func__, result, NULL);
	return result;
}

extern int EX_communicate() {
	int result = OK;
	LG_logEntry(__func__, NULL);

	digitalWrite(REQEXC, HIGH);
	while ((result == OK) and (digitalRead(ACKEXC) == LOW))
		; // Busy wait

	SAFE_INVOKE(ex_communicateSPI(), result, RV_EXCHANGE_FAILED)

	digitalWrite(REQEXC, LOW);
	while ((digitalRead(ACKEXC) == HIGH))
		; // Busy wait

	LG_logExit(__func__, result, NULL);
	return result;
}


static int ex_communicateSPI() {
	int result = OK;
	LG_logEntry(__func__, NULL);
	uint8_t* b;
	// const struct timespec delta = { 0, 1 };

	REG_readAll(&ex_buffer);
	SAFE_INVOKE(ex_fillHeaderTrailer(&ex_buffer), result,
			RV_EXCHANGESPI_FAILED)

	digitalWrite(SS, LOW);

	b = (uint8_t*) &ex_buffer;
	for (unsigned int i = 0; i < sizeof(ex_buffer); i++) {

		wiringPiSPIDataRW(SPICHANNEL, (unsigned char*) (b + i), 1);

		///We need to sleep here for a little while ~20us
		ex_mysleep(ex_spiByteDelay);
	}

	digitalWrite(SS, HIGH);

	SAFE_INVOKE(ex_checkHeaderTrailer(&ex_buffer), result,
			RV_EXCHANGESPI_FAILED)
	if (result != OK) {
		ex_logBuffer(&ex_buffer);
		REG_logAll(&ex_buffer);
	} 
	if (result == OK) {
	        uint8_t dcl;
		uint8_t dcr;
		uint8_t dirl;
		uint8_t dirr;
		REG_read8(REG_LEFTDIR, &dirl);
		REG_read8(REG_LEFTDC, &dcl);
		REG_read8(REG_RIGHTDIR, &dirr);
		REG_read8(REG_RIGHTDC, &dcr);
		REG_writeAll(&ex_buffer);
		REG_write8(REG_LEFTDIR, dirl);
		REG_write8(REG_LEFTDC, dcl);
		REG_write8(REG_RIGHTDIR, dirr);
		REG_write8(REG_RIGHTDC, dcr);
	}
	
	//SAFE_INVOKE(REG_writeAll(&ex_buffer), result, RV_EXCHANGESPI_FAILED)

	LG_logExit(__func__, result, NULL);
	return result;
}

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

static void ex_logBuffer(REG_map* m) {
	uint8_t* p = (uint8_t*) m;
	for (unsigned int i = 0; i < sizeof(REG_map); i++) {
		printf("%d: %d\n", i, p[i]);
	}
}

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
