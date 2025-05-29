#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Base address for the LPSPI0 (Low Power Serial Peripheral Interface 0) peripheral
#define LPSPI0_BASE        (0x4002C000U)   

// Offset for the LPSPI0 clock control in the Peripheral Clock Control (PCC) register
#define PCC_LPSPI0_OFFSET  (0xB0)            

// Mask used to set the MUX (Multiplexer) configuration in the Port Control Register (PCR)
#define PORT_PCR_MUX_MASK  (0x700U)          

// Define pointer to LPSPI0 register structure
#define LPSPI0  ((LPSPI_Registers *)(LPSPI0_BASE))                     

// Define PCC_LPSPI0 clock control
#define PCC_LPSPI0  (*(volatile uint32_t *)(PCC_BASE_ADDRESS + PCC_LPSPI0_OFFSET))  

// Define the LPSPI register structure
typedef struct 
{
    volatile uint32_t VERID;   // Version ID Register
    volatile uint32_t PARAM;   // Parameter Register
	  volatile uint32_t re1[2];
    volatile uint32_t CR;      // Control Register
    volatile uint32_t SR;      // Status Register
    volatile uint32_t IER;     // Interrupt Enable Register
    volatile uint32_t DER;     // DMA Enable Register
    volatile uint32_t CFGR0;   // Configuration 0 Register
    volatile uint32_t CFGR1;   // Configuration 1 Register
		volatile uint32_t re2[6];
		volatile uint32_t CCR;     // Clock Configuration Register
		volatile uint32_t re3[5];
    volatile uint32_t FCR;     // FIFO Control Register
		volatile uint32_t re4;
		volatile uint32_t TCR;     // Transmit Command Register
		volatile uint32_t TDR; 
		volatile uint32_t re5[2];
    volatile uint32_t RSR;     // Software Reset Register
    volatile uint32_t RDR;     // Receive Data Register   
} LPSPI_Registers;

// Function prototypes
void LPSPI0_SetPins(void);
void LPSPI0_Init(void);
int LPSPI0_Transmit(uint16_t data);
int LPSPI0_Receive(uint16_t* data);

#endif // SPI_H
