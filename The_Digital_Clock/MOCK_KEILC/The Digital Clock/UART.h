#ifndef UART_H
#define UART_H

#include <stdint.h>

// Macros for registers
#define SCG_BASE_ADDRESS        0x40064000    // Base address for SCG (System Clock Generation)
#define PORT_C_BASE_ADDRESS     0x4004B000    // Base address for PORT C
#define PORT_D_BASE_ADDRESS     0x4004C000    // Base address for PORT D
#define LPUART1_BASE_ADDRESS    0x4006B000    // Base address for LPUART1

// NVIC 
#define NVIC_ADDRESS            0xE000E100    // Base address for NVIC (Nested Vectored Interrupt Controller)
#define LPUART_NVIC_ID          33            // IRQ number for LPUART
#define LPUART_NVIC_BIT         1             // Bit position for LPUART in NVIC

//PORT Registers 
#define PORT_PCR_OFFSET          0           // Offset for Port Control Register
#define PORT_PCR6_OFFSET         0x18        // Pin Control Register for pin 6
#define PORT_PCR7_OFFSET         0x1C        // Pin Control Register for pin 7

// GPIO Registers 
#define GPIO_PDDR_OFFSET         0x14         // Data Direction Register offset
#define PIN_15                   15           // Pin number 15
#define PIN_16                   16           // Pin number 15

// Offsets for various registers
#define PCC_LPUART1_OFFSET      0x1AC         // Offset for LPUART1 clock gate control

#define LPUART_BAUD_OFFSET      0x10          // Offset for LPUART baud rate register
#define LPUART_CTRL_OFFSET      0x18          // Offset for LPUART control register
#define LPUART_STAT_OFFSET      0x14          // Offset for LPUART status register
#define LPUART_DATA_OFFSET      0x1C          // Offset for LPUART data register

#define SCG_SOSCCSR_OFFSET      0x100         // Offset for SOSC Control Status Register
#define SCG_SOSCDIV_OFFSET      0x104         // Offset for SOSC Divider Register
#define SCG_SOSCCFG_OFFSET      0x108         // Offset for SOSC Configuration Register

// MUX configuration for LPUART
#define MUX_START_BIT            8             // Start bit for MUX configuration
#define MUX_BIT_LENGTH           3             // Length of MUX bit field
#define MUX_ALT_2                0x2           // Alternate function for LPUART

// SOSC configuration settings
#define SOSCDIV_mode_div1        0x1            // SOSC division mode
#define HGO_BIT                  3              // High Gain Oscillator bit position
#define EREFS_BIT                2              // External Reference Select bit position
#define RANGE                    0x2            // Range setting for SOSC

#define SOSCEN_BIT               0               // SOSC Enable bit position
#define LK_BIT                   23              // Lock bit position
#define SOSCVLD_BIT              24              // SOSC Valid bit position
#define SOSC_VALID_CHECK_MASK    (1 << SOSCVLD_BIT) // Mask to check if SOSC is valid

// PCS configuration
#define PCS_START_BIT            24              // Start bit for PCS configuration
#define PCS_BIT_LENGTH           3               // Length of PCS bit field

// Divider settings for PLL
#define SIRCDIV2_CLK             0x2            // SIRC division setting
#define FIRCDIV2_CLK             0x3            // FIRC division setting
#define SPLLDIV2_CLK             0x6            // SPLL division setting

// Baud rate configuration
#define BAUD_START_SBR           0               // Start bit for SBR in Baud rate register
#define BAUD_BIT_SBR_LENGTH      13              // Length of SBR bit field
#define BAUD_START_OSR           24              // Start bit for OSR in Baud rate register
#define BAUD_BIT_OSR_LENGTH      5               // Length of OSR bit field
#define BAUD_SBNS                13              // Stop Bit Number Select setting

// Control register bits
#define CTRL_M                   4               // M bit position in control register
#define CTRL_PE                  1               // Parity Enable bit position
#define CTRL_TE                  19              // Transmit Enable bit position
#define CTRL_RE                  18              // Receive Enable bit position
#define CTRL_RIE                 21              // Receive Interrupt Enable bit position

/********************* STAT REGISTER *************************/
/*
 * Description: Definitions for the LPUART status register.
 * STAT_TDRE: Transmit Data Register Empty flag
 * RDRF_BIT_21: Receive Data Register Full flag
 ************************************************************/
#define STAT_TDRE                23              // Transmit Data Register Empty flag position
#define RDRF_BIT_21              2               // Receive Data Register Full flag position

// Bit position definitions
#define POSITION_8               8               // Bit position 8
#define POSITION_10              10              // Bit position 10
#define AL2                      2               // Alternate function 2 bit position
#define POSITION_24              24              // Bit position 24
#define POSITION_26              26              // Bit position 26
#define SOSCDIV2_CLK             1               // SOSC Divider 2 clock bit position
#define POSITION_30              30              // Bit position 30
#define POSITION_31              31              // Bit position 31
#define ENABLE                   1               // Enable value for control
#define BAUD_RATE_DIVISOR        52              // Assuming 8 MHz clock source for 9600 baud rate
#define OVERSAMPLING_RATIO       16              // Oversampling ratio for LPUART
				
// Functions                          
void LPUART1_Init(void);             
void LPUART1_Transmit(char *data);
void InitializeDesiredSOSC(void);
void ParseUARTStringToTimeDate(char* buffer);

// Function prototypes 
unsigned char CheckBit(volatile unsigned int *address, unsigned char BitPosition);
void SettingNVIC(void);
void LPUART_Pin_Setup(void);
void LPUART_Clock_Setup(void);
void LPUART_Set_BaudRate(unsigned int baud_rate, unsigned int src_clock);
void LPUART_Set_Frame(void);
void LPUART_Interrupt(void);
void LPUART_Enable(void);
void Configure_SCG_SOSCCFG(void);
void Configure_SCG_SOSCCSR(void);

#endif 
