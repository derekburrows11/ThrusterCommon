/*
Copyright 2015 - 2017 Andreas Chaitidis Andreas.Chaitidis@gmail.com

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

*/


#include "Arduino.h"

#define DEBUG

extern float fTime;
extern long lTime;
extern int msTime;


// SPI Pin definition
// nRF24: 
/* nRF24	>	Ardunio Nano	Feather 32U4
-----------------------------------------------
VCC			>	3.3 V
GND			>	GND
MOSI		>	11
MISO		>	12
SCK			>	13
IRQ			>	not connected
CE (nRF24)	>	9
CS (nRF24)	>	10
*/

/*VESC UART
VESC,SPI,I2C	7pin Conn	Nano	Feather 32U4	ESP32
----------------------------------------------------------------
5V				black	>	5V			USB			Vin
3.3V			red						not used	
GND				white	>	GND			GND			Gnd
	SCK			yellow						SCK
TX	SS?? SDA	orange	>	RX (0)		RX	SS	SDA	RX2
RX	MOSI SCL	green	>	TX (1)		TX	MO	SCL	TX2 
	MISO		blue						MI

*/

#ifndef _CONFIG_h
#define _CONFIG_h

//***** Arduino 32U4, Feather 32U4 *****
#ifdef __AVR_ATmega32U4__
// Definition of Serial ports
#define DEBUGSERIAL Serial
#define SERIALIO    Serial1
#endif  // __AVR_ATmega32U4__


//***** Arduino Nano *****
#ifdef ARDUINO_AVR_NANO
// Definition of Serial ports
#define DEBUGSERIAL Serial
#define SERIALIO    Serial
#endif  // ARDUINO_AVR_NANO


//***** Mega2560 *****
#ifdef __AVR_ATmega2560__
// Definition of Serial ports
#define DEBUGSERIAL Serial
#define SERIALIO    Serial1
#endif  // __AVR_ATmega2560__



//***** ARDUINO_ARCH_SAMD *****
#ifdef ARDUINO_ARCH_SAMD
// Definition of Serial ports
#define DEBUGSERIAL Serial
#define SERIALIO    Serial1
#endif  // ARDUINO_ARCH_SAMD


#ifdef ESP32
#define SERIALIO    Serial2
#define DEBUGSERIAL Serial


#endif


//***** Not Specified *****
#ifndef DEBUGSERIAL
#error 'Check Config'
// Definition of Serial ports
//#define DEBUGSERIAL Serial
//#define SERIALIO    Serial1
#endif  // not defined DEBUGSERIAL



#endif  // _CONFIG_h
