/********************************************************************************************************
 * File: CLOCK.c
 * Description: This file contains functions to initialize and configure various clock sources 
 * for the S32K144 microcontroller, including the System Oscillator (SOSC), Slow Internal 
 * Reference Clock (SIRC), Fast Internal Reference Clock (FIRC), and System Phase-Locked Loop (SPLL).
 ***********************************************************************************************************/

#include "CLOCK.h"
#include "GPIO.h"

void CLOCK_InitSOSC(void) 
{   
    // Disable SOSC before configuration
    SetSerialMutilBit(&(SCG->SOSCCSR), 0, 1, 0);      // Clear SOSCEN to disable SOSC
    SetSerialMutilBit(&(SCG->SOSCCSR), 23, 1, 0);     // Clear LK bit to unlock

    // Configure SOSCCFG
    // EREFS=1 (External crystal), HGO=0 (Low power), RANGE=2 (8MHz-32MHz)
    SetSerialMutilBit(&(SCG->SOSCCFG), 2, 1, 1);      // EREFS
    SetSerialMutilBit(&(SCG->SOSCCFG), 3, 1, 0);      // HGO
    SetSerialMutilBit(&(SCG->SOSCCFG), 4, 2, 2);      // RANGE - Medium frequency

    // Enable SOSC
    SetSerialMutilBit(&(SCG->SOSCCSR), 0, 1, 1);      // SOSCEN

    // Wait until SOSC is valid
    while (!(SCG->SOSCCSR & (1U << 24)));             // Wait for SOSCVLD SET 1

    // Lock SOSCCSR
    //SetSerialMutilBit(&(SCG->SOSCCSR), 23, 1, 1);   // LK
}

void CLOCK_InitSIRC(void) 
{
		// Disable SIRC before configuration
		SetSerialMutilBit(&(SCG->SIRCCSR), 0, 1, 0);   // Clear SIRCEN to disable SIRC
		SetSerialMutilBit(&(SCG->SIRCCSR), 23, 1, 0);  // Clear LK bit to unlock

		// SICDIV 2-0
		SetSerialMutilBit(&(SCG->SIRCDIV), 8, 3, 2);

		// Configure SIRCCFG
		// RANGE=1 (8MHz slow IRC)
		SetSerialMutilBit(&(SCG->SIRCCFG), 0, 1, 1);   // RANGE

		// Enable SIRC
		SetSerialMutilBit(&(SCG->SIRCCSR), 0, 1, 1);   // SIRCEN

		// Wait until SIRC is valid
		while (!(SCG->SIRCCSR & (1U << 24)));  // Wait for SIRCVLD

		// Lock SIRCCSR
		//SetSerialMutilBit(&(SCG->SIRCCSR), 23, 1, 1);  // LK
}

void CLOCK_InitFIRC(void) 
{
    // Disable FIRC before configuration
    SetSerialMutilBit(&(SCG->FIRCCSR), 0, 1, 0);   // Clear FIRCEN to disable FIRC
    SetSerialMutilBit(&(SCG->FIRCCSR), 23, 1, 0);  // Clear LK bit to unlock

    // Configure FIRCCFG
    // RANGE=0 (48MHz FIRC)
    SetSerialMutilBit(&(SCG->FIRCCFG), 0, 2, 0);   // RANGE

    // Enable FIRC
    SetSerialMutilBit(&(SCG->FIRCCSR), 0, 1, 1);   // FIRCEN

    // Wait until FIRC is valid
    while (!(SCG->FIRCCSR & (1U << 24)));  // Wait for FIRCVLD

    // Lock FIRCCSR
    SetSerialMutilBit(&(SCG->FIRCCSR), 23, 1, 1);  // LK
}


void CLOCK_InitSPLL(void) 
{
    // Disable SPLL before configuration
    SetSerialMutilBit(&(SCG->SPLLCSR), 0, 1, 0);   // Clear SPLLEN to disable SPLL
    SetSerialMutilBit(&(SCG->SPLLCSR), 23, 1, 0);  // Clear LK bit to unlock

    // Configure SPLLCFG
    // PREDIV=0 (Divide by 1), MULT=4 (Multiply by 20)
    SetSerialMutilBit(&(SCG->SPLLCFG), 16, 5, 0);  // PREDIV
    SetSerialMutilBit(&(SCG->SPLLCFG), 24, 7, 4);  // MULT

    // Enable SPLL
    SetSerialMutilBit(&(SCG->SPLLCSR), 0, 1, 1);   // SPLLEN

    // Wait until SPLL is valid
    while (!(SCG->SPLLCSR & (1U << 24)));  // Wait for SPLLVLD

    // Lock SPLLCSR
    SetSerialMutilBit(&(SCG->SPLLCSR), 23, 1, 1);  // LK
}

void CLOCK_SetupRunMode(void) 
{
    // Unlock RCCR
    SetSerialMutilBit(&(SCG->RCCR), 23, 1, 0);  // Clear LK

    // Configure RCCR
    // SCS=6 (SPLL as system clock)
    // DIVCORE=1 (Divide by 2), DIVBUS=1 (Divide by 2), DIVSLOW=2 (Divide by 3)
    SetSerialMutilBit(&(SCG->RCCR), 24, 4, 6);  // SCS
    SetSerialMutilBit(&(SCG->RCCR), 16, 4, 1);  // DIVCORE
    SetSerialMutilBit(&(SCG->RCCR), 12, 4, 1);  // DIVBUS
    SetSerialMutilBit(&(SCG->RCCR), 8, 4, 2);   // DIVSLOW

    // Wait until system clock source is SPLL
    while (((SCG->CSR >> 24) & 0xF) != 6);  // Check SCS in CSR

    // Lock RCCR
    //SetSerialMutilBit(&(SCG->RCCR), 23, 1, 1);  // LK
}
