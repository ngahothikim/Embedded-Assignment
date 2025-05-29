/********************************************************************************************************************************
 * Main Functionality: 
 * 		This program is designed to interface with an S32K144 microcontroller and a MAX7219 7-segment LED module. 
 * 		It allows the user to input time and date strings via UART, which are then parsed and displayed on the 7-segment display.
 * 		The program features real-time clock updates using the LPIT timer, with SW1 toggling between time and date display,
 * 		and SW2 controlling the display on/off status. It also handles invalid input cases and provides feedback via UART. 
 * 		Key functionalities include SPI communication, UART input parsing, real-time clock display, and GPIO-based button control.
 *********************************************************************************************************************************/

#include "GPIO.h"
#include "LPIT.h"
#include "UART.h"
#include "SPI.h"
#include "CLOCK.h"
#include "MAX7219.h"
#include "UART_RE.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define LED_RED_PIN 15
#define MAX_BUFFER_SIZE 50

// Struct to store time and date information
typedef struct 
{
    unsigned char  hours;
    unsigned char  minutes;
    unsigned char  seconds;
    unsigned char  day;
    unsigned short month;
    unsigned short year;
} Time;

//// Time and date variables
static Time current_time = {00, 00, 00, 01, 01, 1971};  // 00:00:00;   01.01.1971
static unsigned char time_mode = 0;  // 0 for time display, 1 for date display
static unsigned char display_on = 1; // 1 for display on, 0 for display off

void NVIC(uint16_t iNVIC_ID); 
void UpdateTime(void);
void UpdateDate(void);
void DisplayTime(void);
void DisplayDate(void);
void LPIT0_Ch0_IRQHandler(void);
void PORTC_IRQHandler(void);
void LPUART1_RxTx_IRQHandler(void);

// UART receive buffer
static char buffer[MAX_BUFFER_SIZE];  // Buffer to store received data
static int index = 0;        // Index for buffer

// UART messages
static char dataToSend[] = "\nEnter the hour or date using the syntax:\r\nTime: t[hh-mm-ss] \r\nDate: d[dd.mm.yyyy]\r\n"; // Transmit "xxx\r\n"
static char error_message[] = "\r\nBuffer overflow. Please enter a valid string:\r\nTime: t[hh-mm-ss] \r\nDate: d[dd.mm.yyyy]\r\n";
static char dataToSend_valid[] = "\r\nSet completed\r\n";


int main (void)
{
    /* SPI initialization */
    LPSPI0_SetPins();
    LPSPI0_Init();  // Initialize the LPSPI0 module for communication

    /* LPIT (Low Power Interrupt Timer) initialization */
    LPIT_Set_NVIC();                      // Set up the NVIC for interrupts
    LPIT_Set_Clock();                     // Set the clock for the LPIT module
    LPIT_Init();                          // Initialize the LPIT module
    LPIT_Enable_Interrupt    (0);         // Enable LPIT channel 0 interrupt
    LPIT_Set_Timer_Value     (0, 128000); //80000000) 1 second delay for time keeping (assuming 80MHz clock)
    LPIT_Configure_Channel   (0, 0);      // Configure channel 0
    LPIT_Enable_Timer_Channel(0);         // Start timer channel 0
   
		
    /* UART1 Receive */
	  LPUART1_Init();                      // Initialize UART1
    SettingNVIC();                       // Set up NVIC for UART interrupts
    LPUART_Pin_Setup();                  // Set up UART pins
    InitializeDesiredSOSC();             // Initialize system oscillator for proper clock source
    Configure_SCG_SOSCCFG();             // Configure system oscillator clock settings
    Configure_SCG_SOSCCSR();             // Enable system oscillator
    LPUART_Clock_Setup();                // Set up the clock for UART
    LPUART_Set_BaudRate (9600, 8000000); // Set baud rate for UART communication
    LPUART_Set_Frame();                  // Set up frame format for UART (8 data bits, 1 stop bit)
    LPUART_Interrupt();                  // Enable UART receive interrupt
    LPUART_Enable();                     // Enable UART module
    LPUART1_Transmit(dataToSend);        // Transmit prompt message to UART
		
    /* GPIO Initialization for Buttons (SW1 and SW2) */
    Gpio_Init(PCC_PORTC_OFFSET);                                 // Initialize GPIO port C
    Gpio_SetMode((PORT_Type *) PORT_C_OFFSET, GPIO_C, 12, 0, 1); // Set SW1 pin (PTC12) as input
    Gpio_SetMode((PORT_Type *) PORT_C_OFFSET, GPIO_C, 13, 0, 1); // Set SW2 pin (PTC13) as input
    NVIC(61);                                                    // Enable GPIO interrupt
    
    /* LED Initialization */
    Gpio_Init(PCC_PORTD_OFFSET);                                            // Initialize GPIO port D for LED control
    //Gpio_SetMode((PORT_Type *) PORT_D_OFFSET, GPIO_D, LED_RED_PIN, 1, 0); // Configure the red LED (optional)
    
		/* Initialize Module LED MAX7219 */
    MAX7219_Init();	                                                        // Initialize the MAX7219 module for 7-segment display
	
	
		/* Main loop */
    while (1)
    {
		
		}
 }


//---------------------------------------------------//
//        Enable an interrupt in NVIC function       //
//---------------------------------------------------//
void NVIC(uint16_t iNVIC_ID)
{
		SetBit((uint32_t *)(0xE000E100u + 4*(iNVIC_ID / 32)), iNVIC_ID % 32, 1);  // Enable the specific interrupt in NVIC
}


//---------------------------------------------------//
//             Update the current date               //
//---------------------------------------------------//
void UpdateDate(void) 
{
    // Check if the day exceeds the number of days in the month
    unsigned char days_in_month = 31;

    if (current_time.month == 4 || current_time.month == 6 || current_time.month == 9 || current_time.month == 11) 
				{
						days_in_month = 30;  // April, June, September, November have 30 days
				} else if (current_time.month == 2) 
				{
						if ((current_time.year % 4 == 0 && current_time.year % 100 != 0) || (current_time.year % 400 == 0)) 
						{
								days_in_month = 29;  // February in leap year
						} else 
						{
								days_in_month = 28;  // February in non-leap year
						}
				}
    if (current_time.day > days_in_month) 
			{
        current_time.day = 1;
        current_time.month++;
        if (current_time.month > 12) 
				{
            current_time.month = 1;
            current_time.year++;
        }
    }
}

//---------------------------------------------------//
//  LPIT Timer Interrupt Handler (1 second interval) //
//---------------------------------------------------//
void LPIT0_Ch0_IRQHandler(void) 
{
	  LPIT->MSR |= (1u << 0);    //clear flag
    current_time.seconds++;
    if (current_time.seconds == 60) 
		{
        current_time.seconds = 0;
        current_time.minutes++;
        if (current_time.minutes == 60) 
				{
            current_time.minutes = 0;
            current_time.hours++;
            if (current_time.hours == 24) 
						{
                current_time.hours = 0;
                current_time.day++;
							  UpdateDate(); 
            }
        }
    }
		update_display(); 
}

//---------------------------------------------------//
//        GPIO Interrupt Handler for Buttons         //
//---------------------------------------------------//
void PORTC_IRQHandler(void) 
{
    if (PORT_C->PCR[12] & (1u << 24)) {  // SW1 dc nhan
        time_mode = !time_mode;          // chuyen doi che do gio & ngay
        SetBit(&PORT_C->PCR[12], 24, 1); // clear flag
    }
    
    if (PORT_C->PCR[13] & (1u << 24)) {  // SW2 dc nhan
        display_on = !display_on;        // Bat tat hien thi
        SetBit(&PORT_C->PCR[13], 24, 1); // clear flag
    }
}


//---------------------------------------------------//
//        Update the 7-segment display output        //
//---------------------------------------------------//
void update_display(void) 
{
    if (display_on) 
		{
        if (time_mode == 0) 
				{
            // (HH-MM-SS)
            MAX7219_DisplayTime(current_time.hours, current_time.minutes, current_time.seconds);
        } else 
				{
            // (DD-MM-YYYY)
            MAX7219_DisplayDate(current_time.day,(uint8_t)current_time.month, current_time.year);
        }
    } else
		{
        MAX7219_ClearDisplay();  // turn off
    }
}


//---------------------------------------------------//
//        Parse the received UART string             //
//---------------------------------------------------//
/*
 * Description: Parses a received UART string to extract time and date information.
 * Parameters:
 *   - buffer: Pointer to the buffer containing the received string.
 *************************************************************************************/
void ParseUARTStringToTimeDate(char* inbuffer) 
{
     // Check if input is for time (t[hh-mm-ss]) and length is correct
    if (strncmp(inbuffer, "t[", 2) == 0 && index == 11) 
		{
				if (inbuffer[4] != '-' || inbuffer[7] != '-') 
        {
            char syntaxError[] = "\r\nSyntax error. Please enter the time as: t[hh-mm-ss]\r\n";
            LPUART1_Transmit(syntaxError);
					  index = 0;    
				    memset(inbuffer, 0, MAX_BUFFER_SIZE);  // Clear buffer content
        } 
			    	
        else 
        {
						uint8_t hours_Temp   = (inbuffer[2]  - '0') * 10 + (inbuffer[3]  - '0');
						uint8_t minutes_Temp = (inbuffer[5]  - '0') * 10 + (inbuffer[6]  - '0');
						uint8_t seconds_Temp = (inbuffer[8]  - '0') * 10 + (inbuffer[9]  - '0');

						if (hours_Temp > 23 || minutes_Temp > 59 || seconds_Temp > 59) 
							{
								char dataToSend_invalid1[] = "\r\nInvalid time.\r\n";
								LPUART1_Transmit(dataToSend_invalid1);
					 		} 
							else 
							{
								current_time.hours = hours_Temp;
								current_time.minutes = minutes_Temp;
								current_time.seconds = seconds_Temp;

								// Send the time to MAX7219
								MAX7219_DisplayTime(current_time.hours, current_time.minutes, current_time.seconds);
								LPUART1_Transmit(dataToSend_valid);
							}
				}
					index = 0;    
					memset(inbuffer, 0, MAX_BUFFER_SIZE);  // Clear buffer content
      } 

   // Check if input is for date (d[dd.mm.yyyy]) and length is correct
	 if (strncmp(inbuffer, "d[", 2) == 0 && index == 13) 
   {
				if (inbuffer[4] != '.' || inbuffer[7] != '.') 
        {
            char syntaxError[] = "\r\nSyntax error. Please enter the date as: d[dd.mm.yyyy]\r\n";
            LPUART1_Transmit(syntaxError);
					  index = 0;    
			     	memset(inbuffer, 0, MAX_BUFFER_SIZE);  // Clear buffer content
        } 
        else 
        {
						uint8_t day_Temp   = (inbuffer[2]  - '0') * 10 + (inbuffer[3]  - '0');
						uint8_t month_Temp = (inbuffer[5]  - '0') * 10 + (inbuffer[6]  - '0');
						uint16_t year_Temp = (inbuffer[8]  - '0') * 1000 +(inbuffer[9]  - '0') * 100 + 
																 (inbuffer[10] - '0') * 10 + (inbuffer[11] - '0');
						// Check for valid date
						if (month_Temp > 12 || month_Temp < 1 || day_Temp < 1 || year_Temp < 1800) 
						{
								char invalidDate[] = "\r\nInvalid date.\r\n";
								LPUART1_Transmit(invalidDate);
						} 
						else 
						{
								// Check for February (Leap year consideration)
								if (month_Temp == 2) 
								{
										if ((year_Temp % 4 == 0 && year_Temp % 100 != 0) || (year_Temp % 400 == 0)) 
										{
												// Leap year: February has 29 days
												if (day_Temp > 29) 
												{
														char invalidFeb[] = "\r\nInvalid date: February in a leap year has only 29 days.\r\n";
														LPUART1_Transmit(invalidFeb);
												}
										} 
										else 
										{
												// Non-leap year: February has 28 days
												if (day_Temp > 28) 
												{
														char invalidFeb[] = "\r\nInvalid date: February has only 28 days.\r\n";
														LPUART1_Transmit(invalidFeb);
												}
										}
								} 
								else 
								{
										// Check for months with 30 days
										if ((month_Temp == 4 || month_Temp == 6 || month_Temp == 9 || month_Temp == 11) && day_Temp > 30) 
										{
												char invalid30[] = "\r\nInvalid date: This month has only 30 days.\r\n";
												LPUART1_Transmit(invalid30);
										}
										// Check for months with 31 days
										else if (day_Temp > 31) 
										{
												char invalid31[] = "\r\nInvalid date: This month has only 31 days.\r\n";
												LPUART1_Transmit(invalid31);	
						        }
										else 
										{
										    // Valid date, update and display
												current_time.day   = day_Temp;
												current_time.month = month_Temp;
												current_time.year  = year_Temp;
											
											  // Send the date to MAX7219
												MAX7219_DisplayDate(current_time.day,(uint8_t)current_time.month, current_time.year);
												LPUART1_Transmit(dataToSend_valid);
										}
				        }
			      }
				index = 0;    
				memset(inbuffer, 0, MAX_BUFFER_SIZE);  // Clear buffer content
				}
		}
}

// Interrupt handler function for LPUART1 to Recevie
void LPUART1_RxTx_IRQHandler(void)                            
{   
	    // Check if there is data ready to be read from UART
	    if ((CheckBit((volatile unsigned int *) (LPUART1_BASE_ADDRESS + LPUART_STAT_OFFSET),STAT_RDRF) == 1))      
			{
			    // If data is already in the DATA register, read the data into the buffer
					char received_char = (char)((*(unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_DATA_OFFSET)) & 0xFF);
			    
					// Check if buffer is full
					if (index < (int) sizeof(buffer) - 1) // Ensure no buffer overflow
          {
							// Store the character in the buffer and increment the index
							buffer[index++] = received_char;
          }
					buffer[index] = '\0';				
					ParseUARTStringToTimeDate(buffer);  // Process the received string
			}
		  else 
		  {
					// Handle buffer overflow
					LPUART1_Transmit(error_message);
				  index = 0;
		  }
}




