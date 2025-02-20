/**************************************************************************************************************************************
 * File: MAX7219.c
 * Description: This file contains functions to interface with the MAX7219 
 * allow sending commands to configure the display, clear it, and show time or date information.
 * The MAX7219 is controlled via SPI and supports various functionalities like adjusting brightness, 
 * enabling/disabling the display, and decoding mode settings.
 *************************************************************************************************************************************/

#include "MAX7219.h"
#include "GPIO.h"
#include "SPI.h"
#include "CLOCK.h"


// ham gui du lieu qua SPI toi MAX7219
void MAX7219_SendCommand(uint8_t address, uint8_t data) 
{
    LPSPI0_Transmit((uint16_t)(address << 8)|data);  // gui dia chi va du lieu qua SPI
}

// MAX7219 initialization function
void MAX7219_Init(void) 
{     
		// Exit shutdown mode
		MAX7219_SendCommand(MAX7219_SHUTDOWN, 0x01); 

		// Set scan limit to display all 8 digits
		MAX7219_SendCommand(MAX7219_SCAN_LIMIT, 0x07);

		// Set intensity (brightness level)
		MAX7219_SendCommand(MAX7219_INTENSITY, 0x0F);  // Max brightness

		// ENABLE decode mode (use raw data)
		MAX7219_SendCommand(MAX7219_DECODE_MODE, 0xFF);

		// DISABLE decode mode (use raw data)
		MAX7219_SendCommand(MAX7219_DISPLAY_TEST, 0x00);
		
    // Clear all display
		//MAX7219_ClearDisplay();  
		
}		

// Clear all digits
void MAX7219_ClearDisplay(void) 
{
    for (uint8_t i = MAX7219_DIGIT0; i <= MAX7219_DIGIT7; i++) 
	  {
        MAX7219_SendCommand(i, 0xF);  // gui gia tri rong
    }
}

// Display time in HH-MM-SS format
void MAX7219_DisplayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) 
{
		MAX7219_SendCommand(MAX7219_DIGIT0, seconds % 10); // Units digit of seconds
		MAX7219_SendCommand(MAX7219_DIGIT1, seconds / 10); // Tens digit of seconds
		MAX7219_SendCommand(MAX7219_DIGIT2, 0x0A);         // Decimal point between seconds and minutes
		MAX7219_SendCommand(MAX7219_DIGIT3, minutes % 10); // Units digit of minutes
		MAX7219_SendCommand(MAX7219_DIGIT4, minutes / 10); // Tens digit of minutes
		MAX7219_SendCommand(MAX7219_DIGIT5, 0x0A);         // Decimal point between minutes and hours
		MAX7219_SendCommand(MAX7219_DIGIT6, hours % 10);   // Units digit of hours
		MAX7219_SendCommand(MAX7219_DIGIT7, hours / 10);   // Tens digit of hours
}


// Display day (DD-MM-YYYY)
void MAX7219_DisplayDate(uint8_t day, uint8_t month, uint16_t year) 
{
    MAX7219_SendCommand(MAX7219_DIGIT7, day / 10);   
    MAX7219_SendCommand(MAX7219_DIGIT6, (day % 10)|0x80);     // Add decimal point
    MAX7219_SendCommand(MAX7219_DIGIT5, month / 10); 
    MAX7219_SendCommand(MAX7219_DIGIT4, (month % 10) |0x80);  // Add decimal point
    MAX7219_SendCommand(MAX7219_DIGIT3, (year / 1000) % 10); 
    MAX7219_SendCommand(MAX7219_DIGIT2, (year / 100) % 10);
    MAX7219_SendCommand(MAX7219_DIGIT1, (year / 10) % 10);   
    MAX7219_SendCommand(MAX7219_DIGIT0, year % 10);         
}
// Enable or disable shutdown mode
void MAX7219_Shutdown(uint8_t enable) 
{
    MAX7219_SendCommand(MAX7219_SHUTDOWN, enable ? 0x00 : 0x01);  // Send command to set shutdown mode
}

