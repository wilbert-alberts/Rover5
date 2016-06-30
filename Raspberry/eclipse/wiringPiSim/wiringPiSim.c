/*
 * wirintPiSim.c
 *
 *  Created on: Jun 30, 2016
 *      Author: walberts
 */

/*
 * wiringPi.h:
 *	Arduino like Wiring library for the Raspberry Pi.
 *	Copyright (c) 2012-2016 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

extern const char *piModelNames    [16] ;
extern const char *piRevisionNames [16] ;
extern const char *piMakerNames    [16] ;
extern const int   piMemorySize    [ 8] ;



extern int  wiringPiSetup       (void)
{

}
extern int  wiringPiSetupSys    (void)
{

}
extern int  wiringPiSetupGpio   (void)
{

}
extern int  wiringPiSetupPhys   (void)
{

}

extern void pinModeAlt          (int pin, int mode)
{

}

extern void pinMode             (int pin, int mode)
{

}

extern void pullUpDnControl     (int pin, int pud)
{

}

extern int  digitalRead         (int pin)
{

}

extern void digitalWrite        (int pin, int value)
{

}

extern void pwmWrite            (int pin, int value)
{

}

extern int  analogRead          (int pin)
{

}

extern void analogWrite         (int pin, int value)
{

}


// On-Board Raspberry Pi hardware specific stuff

extern          int  piBoardRev          (void)
{

}

extern          void piBoardId           (int *model, int *rev, int *mem, int *maker, int *overVolted)
{

}

extern          int  wpiPinToGpio        (int wpiPin)
{

}

extern          int  physPinToGpio       (int physPin)
{

}

extern          void setPadDrive         (int group, int value)
{

}

extern          int  getAlt              (int pin)
{

}

extern          void pwmToneWrite        (int pin, int freq)
{

}

extern          void digitalWriteByte    (int value)
{

}

extern unsigned int  digitalReadByte     (void)
{

}

extern          void pwmSetMode          (int mode)
{

}

extern          void pwmSetRange         (unsigned int range)
{

}

extern          void pwmSetClock         (int divisor)
{

}

extern          void gpioClockSet        (int pin, int freq)
{

}


// Interrupts
//	(Also Pi hardware specific)

extern int  waitForInterrupt    (int pin, int mS)
{

}

extern int  wiringPiISR         (int pin, int mode, void (*function)(void))
{

}


// Threads

extern int  piThreadCreate      (void *(*fn)(void *))
{

}

extern void piLock              (int key)
{

}

extern void piUnlock            (int key)
{

}


// Schedulling priority

extern int piHiPri (const int pri)
{

}


// Extras from arduino land

extern void         delay             (unsigned int howLong)
{

}

extern void         delayMicroseconds (unsigned int howLong)
{

}

extern unsigned int millis            (void)
{

}

extern unsigned int micros            (void)
{

}



int wiringPiSPIGetFd     (int channel)
{

}
int wiringPiSPIDataRW    (int channel, unsigned char *data, int len)
{

}
int wiringPiSPISetupMode (int channel, int speed, int mode)
{

}
int wiringPiSPISetup     (int channel, int speed)
{

}
