/* Code written by Chia Jiun Wei @ 27 Mar 2017
 * <J.W.Chia@tudelft.nl>
 
 * AB1805: a library to provide high level APIs to interface with the 
 * Abracon Corporation Real-time Clock. It is possible to use this library 
 * in Energia (the Arduino port for MSP microcontrollers) or in other 
 * toolchains.
 
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3, both as published by the Free Software Foundation.
  
 */
 
#ifndef AB1805_H
#define AB1805_H

#include <Energia.h>
#include <DWire.h>

//Slave addresses
#define I2C_ADDRESS 			0x69	//b1101001

//Registers
#define HUNDRED_SEC_REG			0x00	//hundredth of seconds
#define SEC_REG					0x01	//seconds
#define MIN_REG					0x02	//minutes
#define HOUR_REG				0x03	//hour
#define DATE_REG				0x04	//date
#define	MONTH_REG				0x05	//month
#define YEAR_REG				0x06	//year
#define DAY_REG					0x07	//day of week
#define HUNDRED_SEC_ALARM_REG	0x08	//hundredth seconds alarm
#define SEC_ALARM_REG			0x09	//seconds alarm
#define MIN_ALARM_REG			0x0A	//minutes alarm
#define HOUR_ALARM_REG			0x0B	//hour alarm
#define DATE_ALARM_REG			0x0C	//date alarm
#define MONTH_ALARM_REG			0x0D	//months alarm
#define WEEKDAY_ALARM_REG		0x0E	//weekday alarm
#define STATUS_REG				0x0F	//status

#define CONTROL_1_REG			0x10	//control 1
#define CONTROL_2_REG			0x11	//control 2
#define INT_MASK_REG			0x12	//interrupt Mask
#define SQW_REG					0x13	//square wave output
#define CAL_XT_REG				0x14	//calibration XT
#define CAL_RC_HI_REG			0x15	//calibration RC HIGH
#define CAL_RC_LOW_REG			0x16	//calibration RC LOW
#define SLP_CTRL_REG			0x17	//sleep control
#define TMR_CTRL_REG			0x18	//timer control
#define TMR_REG					0x19	//timer
#define TMR_INIT_REG			0x1A	//initial timer
#define WDT_REG					0x1B	//watchdog timer
#define OSC_CTRL_REG			0x1C	//oscillator control
#define OSC_STATUS_REG			0x1D	//oscillator status
#define CONFIG_REG				0x1F	//configuration
#define TRICKLE_REG				0x20	//trickle charger
#define BREF_CTRL_REG			0x21	//BREF control 
#define AFCTRL_REG				0x26	//autocalibration filter control
#define BATMODE_IO_REG			0x27	//batmode I/O
#define ID0_REG					0x28	//ID0
#define ID1_REG					0x29	//ID1
#define ID2_REG					0x2A	//ID2
#define ID3_REG					0x2B	//ID3
#define ID4_REG					0x2C	//ID4
#define ID5_REG					0x2D	//ID5
#define ID6_REG					0x2E	//ID6
#define ASTAT_REG				0x2F	//ASTAT
#define OCTRL_REG				0x30	//OCTRL

//Control Register
#define EN_OSCILLATOR			0x00	
#define DIS_OSCILLATOR			0x80
#define HOUR_MODE_12			0x40
#define HOUR_MODE_24			0x00
#define WRTC					0x01

//Interrupt Mask
#define EN_CENTURY				0x80
#define INT_MODE_1				0x00	//static for both XT and RC
#define INT_MODE_2				0x20	// 1/8192 for XT, 1/64 for RC
#define INT_MODE_3				0x40	// 1/64 for XT and RC
#define INT_MODE_4				0x60	// 1/4 for XT and RC

//Time format
#define AM						0x00
#define PM						0x20


class AB1805

{
protected:
	DWire &wire;
    unsigned char address;
	
public:
	unsigned char date[8];
	
	AB1805(DWire &i2c);
	virtual ~AB1805( ) {};
	
	void init(unsigned char time_format);
	void init_int();
	void init_time(unsigned char * init_time);
	void init_time_done();
	unsigned char get_time();
	unsigned char hex2dec(unsigned char val);
	unsigned char dec2hex(unsigned char val);
	unsigned char get_status();
	
	// read and write from the register
	unsigned char readRegister(unsigned char reg);
	void writeRegister(unsigned char reg, unsigned char val);
	
private:
	
};

#endif  // AB1805_H