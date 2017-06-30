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
 *   unsigned char init_time[0]			year
 *	 unsigned char init_time[1]			month
 *   unsigned char init_time[2]			date
 *   unsigned char init_time[3]			day of the week, SUNDAY: 0, MONDAY: 1, TUESDAY:2, ... 
 *   unsinged char init_time[4]			hour 
 *   unsigned char init_time[5] 		minutes 
 *   unsigned char init_time[6]			seconds
 *	 unsigned char init_time[7]			hundredth of second 
 *
 * 	 NOTE: 
 *   1. Initialization parameters can be found in "AB1805.h" under time settings
 *   2. All the input parameters except day_of_week require a combination  
 *      of zero order and tenth order time settings Eg: FOURTY | THREE
 *
 */
 void AB1805::init_time(unsigned char * init_time)
{	
	writeRegister(HUNDRED_SEC_REG, dec2hex(init_time[7]));
	writeRegister(SEC_REG, dec2hex(init_time[6]));
	writeRegister(MIN_REG, dec2hex(init_time[5]));
	writeRegister(HOUR_REG, dec2hex(init_time[4]));
	writeRegister(DAY_REG, dec2hex(init_time[3]));
	writeRegister(DATE_REG, dec2hex(init_time[2]));
	writeRegister(MONTH_REG, dec2hex(init_time[1]));
	writeRegister(YEAR_REG, dec2hex(init_time[0]));
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
	
	date[0] = readRegister(YEAR_REG);
	date[1] = readRegister(MONTH_REG);
	date[2] = readRegister(DATE_REG);
	date[3] = readRegister(DAY_REG);
	date[4] = readRegister(HOUR_REG);
	date[5] = readRegister(MIN_REG);
	date[6] = readRegister(SEC_REG);
	date[7] = readRegister(HUNDRED_SEC_REG);
	
	//Time processing 
	date[0] = hex2dec(date[0]);
	date[1] = hex2dec(date[1]);
	date[2] = hex2dec(date[2]);
	date[5] = hex2dec(date[5]);
	date[6] = hex2dec(date[6]);
	date[7] = hex2dec(date[7]);
	
	if (time_format == HOUR_MODE_24)
	{
		date[4] = hex2dec(date[4]);
		ret = 2;
		return ret;
	}
	else
	{	
		ret = (date[4] & 0x20 )>> 5;
		date[4] = hex2dec(date[4] & 0x1F);
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
unsigned char AB1805::hex2dec(unsigned char val)
{
	val = val - 6 * (val >> 4);
	return val;
}

/**  Convert the date from DEC to HEX
 *
 *   Parameters:
 *	 unsigned char val		value to be convert
 *
 *	 Returns:
 *	 unsigned char			converted value
 *
 */
unsigned char AB1805::dec2hex(unsigned char val)
{
	val = val + 6 * (val / 10);
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