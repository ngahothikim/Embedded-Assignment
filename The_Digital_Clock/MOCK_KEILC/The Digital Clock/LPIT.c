#include "LPIT.h"
#include "GPIO.h"
#include "SPI.h"

#define LPIT0_IRQn 48

// 1. Setting NVIC
// Enable interrupt in NVIC manually
void LPIT_Set_NVIC(void) 
{
		// Define LPIT0 interrupt vector number
		volatile uint32_t *NVIC_ISER = (volatile uint32_t *)(NVIC_ISER_BASE + (LPIT0_IRQn / 32) * 4);
		*NVIC_ISER = (1 << (LPIT0_IRQn % 32));     // Set the bit corresponding to the IRQ number
}


// 2. Setting Clocking
void LPIT_Set_Clock(void) 
{
    // Enable the LPIT clock in the PCC module
    SetSerialMutilBit(&PCC_LPIT, 24, 3, 7);    // LPO128_CLK
    SetSerialMutilBit(&PCC_LPIT, 30, 1, 1);    // Enable LPIT clock by setting bit 30
}

// 3. LPIT Initialization 
void LPIT_Init(void) 
{
    SetSerialMutilBit(&LPIT->MCR, 0, 1, 1);     // M_CEN Enable LPIT by setting MCR[0]
    SetSerialMutilBit(&LPIT->MCR, 3, 1, 1);     // DBG_EN Debug Enable Bit
    SetSerialMutilBit(&LPIT->TCTRL0, 2, 2, 0);  // TCTRL0[MODE]
}

// 4. Enable Channel Timer Interrupt
void LPIT_Enable_Interrupt(uint8_t channel) 
{
    SetSerialMutilBit(&LPIT->MIER, channel, 1, 1);  // Enable interrupt for Channel (0-3) 
}

// 5. Set Timer Value Start
void LPIT_Set_Timer_Value(uint8_t channel, uint32_t value) 
{
    switch (channel) 
		{
        case 0: LPIT->TVAL0 = value; break;
        case 1: LPIT->TVAL1 = value; break;
        case 2: LPIT->TVAL2 = value; break;
        case 3: LPIT->TVAL3 = value; break;
        default: break;
    }
}

// 6. Configuration for Channel
void LPIT_Configure_Channel(uint8_t channel, uint32_t mode) 
{
    switch (channel) 
		{
        case 0: SetSerialMutilBit(&LPIT->TCTRL0, 2, 2, mode); break; // Set the mode(0) in bits [3:2] of TCTRL0
        case 1: SetSerialMutilBit(&LPIT->TCTRL1, 2, 2, mode); break; // Set the mode in bits [3:2] of TCTRL1
        case 2: SetSerialMutilBit(&LPIT->TCTRL2, 2, 2, mode); break; // Set the mode in bits [3:2] of TCTRL2
        case 3: SetSerialMutilBit(&LPIT->TCTRL3, 2, 2, mode); break; // Set the mode in bits [3:2] of TCTRL3
        default: break;
    }
}

// 7. Enable Timer Channel
void LPIT_Enable_Timer_Channel(uint8_t channel) 
{
    switch (channel) 
    {
        case 0: SetSerialMutilBit(&LPIT->TCTRL0, 0, 1, 1); break; // Set the mode(0) in bits [3:2] of TCTRL0
        case 1: SetSerialMutilBit(&LPIT->TCTRL1, 0, 1, 1); break; // Set the mode in bits [3:2] of TCTRL1
        case 2: SetSerialMutilBit(&LPIT->TCTRL2, 0, 1, 1); break; // Set the mode in bits [3:2] of TCTRL2
        case 3: SetSerialMutilBit(&LPIT->TCTRL3, 0, 1, 1); break; // Set the mode in bits [3:2] of TCTRL3
        default: break;
    }
}

// 8. Disable Timer Channel
void LPIT_Disable_Timer_Channel(uint8_t channel) 
{
    switch (channel) 
    {
        case 0: SetSerialMutilBit(&LPIT->TCTRL0, 0, 1, 0); break; // Set the mode(0) in bits [3:2] of TCTRL0
        case 1: SetSerialMutilBit(&LPIT->TCTRL1, 0, 1, 0); break; // Set the mode in bits [3:2] of TCTRL1
        case 2: SetSerialMutilBit(&LPIT->TCTRL2, 0, 1, 0); break; // Set the mode in bits [3:2] of TCTRL2
        case 3: SetSerialMutilBit(&LPIT->TCTRL3, 0, 1, 0); break; // Set the mode in bits [3:2] of TCTRL3
        default: break;
    }
}
