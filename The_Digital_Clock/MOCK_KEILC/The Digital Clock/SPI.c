/*********************************************************************************************************
 * SPI.c
 * Description: 
 * This file contains functions to configure and use the SPI (Serial Peripheral Interface) protocol on 
 * the S32K144 microcontroller through the LPSPI0 module. It includes functions for setting up pins, 
 * initializing LPSPI0, and transmitting data via SPI.
 *
 * Key functions include:
 * - LPSPI0_SetPins: Configures the necessary GPIO pins for LPSPI0.
 * - LPSPI0_Init: Initializes LPSPI0, including selecting the clock source, setting transmission 
 *   parameters, and configuring the operating mode.
 * - LPSPI0_Transmit: Transmits 16-bit data via LPSPI0.
 *******************************************************************************************************/

#include "SPI.h"
#include "GPIO.h"
#include "CLOCK.h"

#define LPSPI_SR_TDF_MASK  1 << 0
#define LPSPI_SR_RDF_MASK  1 << 1

/*-------------------------------------------------------------*/
/*             Select clock source and enable LPSPI0           */
/*-------------------------------------------------------------*/
void LPSPI0_SetPins(void) 
{
		// Initialize GPIO clocks for PORTE
		Gpio_Init(PCC_PORTE_OFFSET);
		Gpio_Init(PCC_PORTB_OFFSET);

		// Set PTE0 (SCK), PTB0 (PCS), PTE1 (SOUT), PTE2 (SIN) as LPSPI0 pins (MUX)
		SetSerialMutilBit((volatile unsigned int *)(PORT_B_OFFSET + 0x8),  8, 3, 0x3);   // PTB2: SCK,  MUX 2
		SetSerialMutilBit((volatile unsigned int *)(PORT_B_OFFSET + 0x0),  8, 3, 0x3);   // PTB0: PCS,  MUX 3
		SetSerialMutilBit((volatile unsigned int *)(PORT_B_OFFSET + 0x4),  8, 3, 0x3);   // PTB1: SOUT, MUX 2
 }

/*-------------------------------------------------------------*/
/*            Select clock source and enable LPSPI0            */
/*-------------------------------------------------------------*/
void LPSPI0_Init(void) 
{
		CLOCK_InitSIRC();

		//Select source LPSPI clock
		SetSerialMutilBit(&PCC_LPSPI0, 24, 3, 2);          // Select SIRCDIV2 as clock source

		// Enable clock for LPSPI0 (assumed to be in PCC)
		//Gpio_Init(PCC_LPSPI0_OFFSET);                    // PCC_LPSPI0_OFFSET
		PCC_LPSPI0 |= (1 << 30); 

		//Setting the clock prescaler and SCK divider
		SetSerialMutilBit(&(LPSPI0->TCR), 27, 3, 0);       // Prescaler 
		SetSerialMutilBit(&(LPSPI0->CCR), 0, 8, 0x2);      // SCK divider

		//Configure clock phase and polarity
		SetSerialMutilBit(&(LPSPI0->TCR), 30, 1, 0x0);     // CPOL = 0
		SetSerialMutilBit(&(LPSPI0->TCR), 31, 1, 0x0);     // CPHA = 0

		//Set frame size (8 bits) and data transfer type (MSB first)
		SetSerialMutilBit(&(LPSPI0->TCR), 0, 12, 0x00F);   // FRAME SIZE = 16 bits
		SetSerialMutilBit(&(LPSPI0->TCR), 23, 1, 0x0);     // MSB (LSPF)

		//Configure peripheral chip select
		SetSerialMutilBit(&(LPSPI0->TCR), 24, 2, 0x0);     // PCS2 = 2

		//Set FIFO watermarks
		//SetSerialMutilBit(&(LPSPI0->FCR), 16, 3, 0x3);   // RXWATER
		SetSerialMutilBit(&(LPSPI0->FCR), 0, 2, 0x3);      // TXWATER

		//Configure LPSPI0 mode
		SetSerialMutilBit(&(LPSPI0->CFGR1), 3, 1, 1);      // NOSTALL
		SetSerialMutilBit(&(LPSPI0->CFGR1), 0, 1, 1);      // Master mode

		SetSerialMutilBit(&(LPSPI0->CR), 3, 1, 1);         // DBGEN

		//Enable LPSPI0
    SetSerialMutilBit(&(LPSPI0->CR), 0, 1, 1);         // MEN (Enable LPSPI0) 
}

int LPSPI0_Transmit(uint16_t data) 
{
		while (!(LPSPI0->SR & (1 << 0)))                   // Check TDF flag in Status Register
		{
			
		}
		LPSPI0->TDR = data;                                // Write data to Transmit Data Register (TDR)
    return 0;		
}


