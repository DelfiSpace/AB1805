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
 
  #include "AB1805.h"
  
/**  AB1805 class creator function
 *
 *   Parameters:
 *   DWire &i2c             I2C object
 *
 */
AB1805::AB1805(DWire &i2c): wire(i2c)
{
    address = I2C_ADDRESS;
}

/**  Initialise the value of control register
 *   
 *	 Parameters:
 *	 unsigned char time_format	12/24 hour format
 *
 *   Control register is initialise to oscillator enable, user defined time format and write counter register
 *
 */
void AB1805::init(unsigned char time_format)
{
	writeRegister(CONTROL_1_REG, (time_format | EN_OSCILLATOR | WRTC));
}

/**  Initialise the value of interrupt mask register
 *
 *   Interrupt mask register is initialise to century enable, 1/4 seconds for XT and RC, and all other interrupt disable
 *
 */
void AB1805::init_int()
{
	writeRegister(INT_MASK_REG, (0x00 | EN_CENTURY | INT_MODE_4));
}

/**  Initialise the time
 *   
 *   Parameters:
 *   unsigned char year					year
 *	 unsigned char month				month
 *   unsigned char date					date
 *   unsigned char day_of_week			day of the week, SUNDAY: 0, MONDAY: 1, TUESDAY:2, ... 
 *   unsinged char hr					hour 
 *   unsigned char min 					minutes 
 *   unsigned char sec					seconds
 *	 unsigned char hundredth_sec		hundredth of second 
 *
 * 	 NOTE: 
 *   1. Initialization parameters can be found in "AB1805.h" under time settings
 *   2. All the input parameters except day_of_week require a combination  
 *      of zero order and tenth order time settings Eg: FOURTY | THREE
 *
 */
void AB1805::init_time(unsigned char year, unsigned char month, unsigned char date, unsigned char day_of_week, unsigned char hr, unsigned char min, unsigned char sec, unsigned char hundredth_sec)
{
	writeRegister(HUNDRED_SEC_REG, hundredth_sec);
	writeRegister(SEC_REG, sec);
	writeRegister(MIN_REG, min);
	writeRegister(HOUR_REG, hr);
	writeRegister(DAY_REG, day_of_week);
	writeRegister(DATE_REG, date);
	writeRegister(MONTH_REG, month);
	writeRegister(YEAR_REG, year);	
}

/**  Complete time initialisation
 *   
 */
void AB1805::init_time_done()
{
	unsigned char reg_save;
	
	reg_save = readRegister(CONTROL_1_REG);
	writeRegister(CONTROL_1_REG, (reg_save & ~WRTC));
}

/**  Get time
 *
 *	 Returns:
 *   unsigned char			0: 12-hour mode, AM
 *							1: 12-hour mode, PM
 *							2: 24-hour mode
 */
unsigned char AB1805::get_time()
{
	unsigned char ret;
	unsigned char time_format;
	
	time_format = (readRegister(CONTROL_1_REG) & 0x40);
	
	date[0] = readRegister(HUNDRED_SEC_REG);
	date[1] = readRegister(SEC_REG);
	date[2] = readRegister(MIN_REG);
	date[3] = readRegister(HOUR_REG);
	date[4] = readRegister(DAY_REG);
	date[5] = readRegister(DATE_REG);
	date[6] = readRegister(MONTH_REG);
	date[7] = readRegister(YEAR_REG);
	
	//Time processing 
	date[0] = conv_time(date[0]);
	date[1] = conv_time(date[1]);
	date[2] = conv_time(date[2]);
	date[5] = conv_time(date[5]);
	date[6] = conv_time(date[6]);
	date[7] = conv_time(date[7]);
	
	if (time_format == HOUR_MODE_24)
	{
		date[3] = conv_time(date[3]);
		ret = 2;
		return ret;
	}
	else
	{	
		ret = (date[3] & 0x20 )>> 5;
		date[3] = conv_time(date[3] & 0x1F);
		return ret;
	}	
}

/**  Convert the date from HEX to DEC
 *
 *   Parameters:
 *	 unsigned char val		value to be convert
 *
 *	 Returns:
 *	 unsigned char			converted value
 *
 */
unsigned char AB1805::conv_time(unsigned char val)
{
	val = val - 6 * (val >> 4);
	return val;
}
  
/**  Get the status of the RTC
 *
 *	 Returns:
 *	 unsigned char			status register value
 *
 */
unsigned char AB1805::get_status()
{
	unsigned char ret;
	
	ret = readRegister(STATUS_REG);
	return ret;
}  
  
  
  
  
  
/**  Returns the value (1 byte) of the selected internal register
 *
 *   Parameters:
 *   unsigned char reg     register number
 *
 *   Returns:
 *   unsigned char         register value
 *
 */
unsigned char AB1805::readRegister(unsigned char reg)
{
    unsigned char ret = -1;
    wire.beginTransmission(address);
    wire.write(reg);

    unsigned char res = wire.requestFrom(address, 1);
    if (res == 1)
    {
		ret = wire.read();
    }

    return ret;
}


/**  Sets the value (1 byte) of the selected internal register
 *   
 *   Parameters:
 *   unsigned char reg     register number
 *   unsigned char val     register value
 *
 */
void AB1805::writeRegister(unsigned char reg, unsigned char val)
{
    wire.beginTransmission(address);
    wire.write(reg);
    wire.write(val & 0xFF);      
    wire.endTransmission();
}