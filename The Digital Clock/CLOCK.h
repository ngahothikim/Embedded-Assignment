#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

// Base addresses for Clock registers
#define SCG_BASE    (0x40064000U)  // System Clock Generation module base

// SCG Register Structure
typedef struct 
{
    volatile uint32_t VERID;       // Version ID Register (0x000)
    volatile uint32_t PARAM;       // Parameter Register (0x004)
    uint32_t RESERVED0[2];         // Reserved (0x008 - 0x00F)
    volatile uint32_t CSR;         // Clock Status Register (0x010)
    volatile uint32_t RCCR;        // Run Clock Control Register (0x014)
    volatile uint32_t VCCR;        // VLPR Clock Control Register (0x018)
    volatile uint32_t HCCR;        // High-speed Run Clock Control Register (0x01C)
    volatile uint32_t CLKOUTCNFG;  // SCG CLKOUT Configuration Register (0x020)
    uint32_t RESERVED1[55];        // Reserved (0x024 - 0x0FF)
	
    // System OSC Registers
    volatile uint32_t SOSCCSR;     // SOSC Control Status Register (0x100)
    volatile uint32_t SOSCDIV;     // SOSC Divide Register (0x104)
    volatile uint32_t SOSCCFG;     // SOSC Configuration Register (0x108)
    uint32_t RESERVED2[61];        // Reserved (0x10C - 0x1FF)
    
	  // Slow IRC Registers
    volatile uint32_t SIRCCSR;     // SIRC Control Status Register (0x200)
    volatile uint32_t SIRCDIV;     // SIRC Divide Register (0x204)
    volatile uint32_t SIRCCFG;     // SIRC Configuration Register (0x208)
    uint32_t RESERVED3[61];        // Reserved (0x20C - 0x2FF)
    
		// Fast IRC Registers
    volatile uint32_t FIRCCSR;     // FIRC Control Status Register (0x300)
    volatile uint32_t FIRCDIV;     // FIRC Divide Register (0x304)
    volatile uint32_t FIRCCFG;     // FIRC Configuration Register (0x308)
    uint32_t RESERVED4[189];       // Reserved (0x30C - 0x3FF)
    
		// System PLL Registers
    volatile uint32_t SPLLCSR;     // SPLL Control Status Register (0x600)
    volatile uint32_t SPLLDIV;     // SPLL Divide Register (0x604)
    volatile uint32_t SPLLCFG;     // SPLL Configuration Register (0x608)
}   SCG_Registers;

// Define pointer to SCG register structure
#define SCG ((SCG_Registers *)(SCG_BASE))

// Clock configuration functions
void CLOCK_InitSOSC(void);
void CLOCK_InitSIRC(void);
void CLOCK_InitFIRC(void);
void CLOCK_InitSPLL(void);
void CLOCK_SetupRunMode(void);

#endif // CLOCK_H
