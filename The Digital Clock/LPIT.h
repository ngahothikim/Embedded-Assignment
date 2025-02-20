#ifndef LPIT_H
#define LPIT_H

#include <stdint.h>

// Base address for LPIT and PCC
#define LPIT_BASE                  (0x40037000)
#define PCC_LPIT_OFFSET            (0xDC)      

// Define base address of NVIC (for Cortex-M cores)
#define NVIC_ISER_BASE             (0xE000E100) 

// Define the LPIT register structure
typedef struct 
{
    volatile uint32_t VERID;    // Version ID Register (0x0)
    volatile uint32_t PARAM;    // Parameter Register (0x4)
    volatile uint32_t MCR;      // Module Control Register (0x8)
    volatile uint32_t MSR;      // Module Status Register (0xC)
    volatile uint32_t MIER;     // Module Interrupt Enable Register (0x10)
    volatile uint32_t SETTEN;   // Set Timer Enable Register (0x14)
    volatile uint32_t CLRTEN;   // Clear Timer Enable Register (0x18)
    volatile uint32_t re0;
    volatile uint32_t TVAL0;    // Timer Value Register 
    volatile uint32_t CVAL0;    // Current Timer Value
    volatile uint32_t TCTRL0;   // Timer Control Register
    volatile uint32_t re1;
    volatile uint32_t TVAL1;    // Timer Value Register 
    volatile uint32_t CVAL1;    // Current Timer Value
    volatile uint32_t TCTRL1;   // Timer Control Register
    volatile uint32_t re2; 
    volatile uint32_t TVAL2;    // Timer Value Register 
    volatile uint32_t CVAL2;    // Current Timer Value
    volatile uint32_t TCTRL2;   // Timer Control Register
    volatile uint32_t re3;
    volatile uint32_t TVAL3;    // Timer Value Register 
    volatile uint32_t CVAL3;    // Current Timer Value
    volatile uint32_t TCTRL3;   // Timer Control Register
} LPIT_Registers;

// Define pointer to LPIT register structure
#define LPIT  ((LPIT_Registers *)(LPIT_BASE))

// Define PCC_LPIT clock control
#define PCC_LPIT (*(volatile uint32_t *)(PCC_BASE_ADDRESS + PCC_LPIT_OFFSET))

// Function prototypes
void LPIT_Set_NVIC(void);
void LPIT_Set_Clock(void); 
void LPIT_Init(void);
void LPIT_Enable_Interrupt(uint8_t channel);
void LPIT_Set_Timer_Value(uint8_t channel, uint32_t value);
void LPIT_Configure_Channel(uint8_t channel, uint32_t mode);
void LPIT_Enable_Timer_Channel(uint8_t channel); 
void LPIT_Disable_Timer_Channel(uint8_t channel);

#endif // LPIT_H
