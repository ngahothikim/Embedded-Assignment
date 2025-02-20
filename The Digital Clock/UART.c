//--------------------------------------------------------------------------
// File: UART.c
// Description: This file contains functions to initialize and configure 
// the Low Power UART (LPUART1) on the S32K144 microcontroller. 
// It includes functions for setting up pins, configuring 
// clock sources, and setting baud rates for communication.
//--------------------------------------------------------------------------

#include "UART.h"
#include "GPIO.h"
#include <stdint.h>

//--------------------------------------------------------------
// Function: Initializes the desired SOSC settings.
//--------------------------------------------------------------
void InitializeDesiredSOSC(void) 
{
    SetSerialMutilBit((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCDIV_OFFSET), 8, 3, SOSCDIV_mode_div1);
    SetSerialMutilBit((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), 4, 2, RANGE);
    SetBit((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), HGO_BIT, 0);
    SetBit((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), EREFS_BIT, 1);
    SetBit((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCSR_OFFSET), SOSCEN_BIT, 1);
    while (!(*((volatile unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCSR_OFFSET)) & SOSC_VALID_CHECK_MASK));
}

//---------------------------------------------------------------
// Description: Transmits a string of characters via LPUART1.
// Parameters:
//   - data: Pointer to the string to transmit.
//---------------------------------------------------------------
void LPUART1_Init(void) 
{
    // Enable clock for PORT C (LPUART1 Tx and Rx pins)
    *(volatile uint32_t *)(PCC_BASE_ADDRESS + 0x12C) |= (1 << 30); // Enable clock for PORTC

    // Configure PORT C pin 6 (Rx) and pin 7 (Tx) for LPUART1
    *(volatile uint32_t *)(PORT_C_BASE_ADDRESS + PORT_PCR6_OFFSET) = (AL2 << 8); // ALT2 function
    *(volatile uint32_t *)(PORT_C_BASE_ADDRESS + PORT_PCR7_OFFSET) = (AL2 << 8); // ALT2 function

    // Enable clock for LPUART1
    *(volatile uint32_t *)(PCC_BASE_ADDRESS + PCC_LPUART1_OFFSET) = (SOSCDIV2_CLK << POSITION_24) | (ENABLE << POSITION_30);

    // Configure baud rate (9600), assuming 8 MHz source clock
    *(volatile uint32_t *)(LPUART1_BASE_ADDRESS + LPUART_BAUD_OFFSET) = (BAUD_RATE_DIVISOR & 0x1FFF) | (OVERSAMPLING_RATIO << 24);

    // Configure frame format: 1 stop bit, 8-bit data, no parity
    *(volatile uint32_t *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET) = (ENABLE << 19) | (ENABLE << 18); // Enable Tx and Rx
}


void LPUART1_Transmit(char *data) 
{
    while (*data != '\0') 
			{
        // Wait until transmit data register is empty
        while (!(*(volatile uint32_t *)(LPUART1_BASE_ADDRESS + LPUART_STAT_OFFSET) & (1 << 23))) {}   // Transmit Data Register Empty Flag

        // Transmit data
        *(volatile uint32_t *)(LPUART1_BASE_ADDRESS + LPUART_DATA_OFFSET) = *data++;

        // Wait for transmission complete
        while (!(*(volatile uint32_t *)(LPUART1_BASE_ADDRESS + LPUART_STAT_OFFSET) & (1 << 22))) {}   // Transmit Data Register Empty Flag
    }
}



/*--------------------------------------------------------------------------------------------*/
/*                                      Function: Check Bit                                   */
/*--------------------------------------------------------------------------------------------*/
unsigned char CheckBit(volatile unsigned int *address, unsigned char BitPosition)
{
    unsigned int itest = 1;
    itest <<= BitPosition;
    if ((*address & itest) == 0)
    {
      return 0;
    }
    else
    {
      return 1;
    }
}


/*--------------------------------------------------------------------------------------------*/
/*                                       Function: set NVIC                                   */
/*--------------------------------------------------------------------------------------------*/
void SettingNVIC(void)
{
		SetBit((unsigned int *)(NVIC_ADDRESS + 4 * 1), LPUART_NVIC_BIT, 1);
}



/*--------------------------------------------------------------------------------------------*/
/*                                    Function: setup pin LPUART                              */
/*--------------------------------------------------------------------------------------------*/
void LPUART_Pin_Setup(void) 
{
    // Enable clock for PORTC 
    SetBit((unsigned int *)(PCC_BASE_ADDRESS + PCC_PORTC_OFFSET), 30, 1);
		   
    // Set MUX to alternate function for Tx (PTC7 as LPUART0 Tx)
    SetSerialMutilBit((unsigned int *)(PORT_C_BASE_ADDRESS + PORT_PCR7_OFFSET), MUX_START_BIT, MUX_BIT_LENGTH, MUX_ALT_2); 

		// Set MUX to alternate function for Rx (PTC6 as LPUART0 Rx)
    SetSerialMutilBit((unsigned int *)(PORT_C_BASE_ADDRESS + PORT_PCR6_OFFSET), MUX_START_BIT, MUX_BIT_LENGTH, MUX_ALT_2); 
}


/*--------------------------------------------------------------------------------------------*/
/*                                    Function: config SOSCCFG                                */
/*--------------------------------------------------------------------------------------------*/
void Configure_SCG_SOSCCFG(void)
{
	SetSerialMutilBit((unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), 4, 2, RANGE);	
	
	SetBit((unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), HGO_BIT, 0);//SET
	
	SetBit((unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCFG_OFFSET), EREFS_BIT, 1);
}


/*--------------------------------------------------------------------------------------------*/
/*                                       Function: config SOSCCSR                              */
/*--------------------------------------------------------------------------------------------*/
void Configure_SCG_SOSCCSR(void)
{
	SetBit((unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCSR_OFFSET), SOSCEN_BIT, 1);
	
	while ((CheckBit((unsigned int *)(SCG_BASE_ADDRESS + SCG_SOSCCSR_OFFSET), SOSCVLD_BIT) != 1))
	{
	
	}
}


/*--------------------------------------------------------------------------------------------*/
/*                                 Function: setup LPUART Clock                               */
/*--------------------------------------------------------------------------------------------*/
void LPUART_Clock_Setup(void) 
{
    // Select clock source for LPUART (SPLL_CLK as PCS)
    SetSerialMutilBit((unsigned int *)(PCC_BASE_ADDRESS + PCC_LPUART1_OFFSET), PCS_START_BIT, PCS_BIT_LENGTH, SOSCDIV2_CLK);
		
	  // Enable clock for LPUART1
    SetBit((unsigned int *)(PCC_BASE_ADDRESS + PCC_LPUART1_OFFSET), 30, 1); 
}


void LPUART_Set_BaudRate(unsigned int baud_rate, unsigned int src_clock) 
{
    unsigned int sbr, osr = 16;  
    
    sbr = (src_clock) / ((osr + 1) * baud_rate);
    
    // Set SBR
    SetSerialMutilBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_BAUD_OFFSET), BAUD_START_SBR, BAUD_BIT_SBR_LENGTH, sbr);
    
    // Set OSR
    SetSerialMutilBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_BAUD_OFFSET), BAUD_START_OSR, BAUD_BIT_OSR_LENGTH, osr);
}


void LPUART_Set_Frame(void) 
{
    // Set 1 stop bit
    SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_BAUD_OFFSET), BAUD_SBNS, 0);
    
    // Set 8 data bits
    SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET), CTRL_M, 0);
    
    // Disable parity
    SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET), CTRL_PE, 0);

}



/*--------------------------------------------------------------------------------------------*/
/*                                 Function: setup LPUART Inerrupt                            */
/*--------------------------------------------------------------------------------------------*/
void LPUART_Interrupt(void)
{
	  // Enable Reciever Interrupt
	  SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET), CTRL_RIE, 1);
}

/*--------------------------------------------------------------------------------------------*/
/*                                     Function: Enable LPUART                                */
/*--------------------------------------------------------------------------------------------*/
void LPUART_Enable(void) 
{
    // Enable transmitter
    SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET), CTRL_TE, 1);
    
    // Enable receiver
    SetBit((unsigned int *)(LPUART1_BASE_ADDRESS + LPUART_CTRL_OFFSET), CTRL_RE, 1);
}



