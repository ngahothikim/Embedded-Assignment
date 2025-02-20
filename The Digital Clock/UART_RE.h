#ifndef UART_RE_H
#define UART_RE_H

// NVIC 
#define NVIC_ADDRESS 0xE000E100               // Base address for NVIC (Nested Vectored Interrupt Controller)
#define LPUART_NVIC_ID 33                     // IRQ number for LPUART
#define LPUART_NVIC_BIT 1

//PORT Registers 
#define SCG_BASE_ADDRESS         0x40064000   // Base address for SCG (System Clock Generation)
#define PORT_C_BASE_ADDRESS      0x4004B000   // Base address for PORT C
#define PORT_D_BASE_ADDRESS      0x4004C000   // Base address for PORT D
#define PORT_PCR_OFFSET          0
#define PORT_PCR6_OFFSET         0x18        // Pin Control Register for pin 6
#define PORT_PCR7_OFFSET         0x1C        // Pin Control Register for pin 7
#define LPUART1_BASE_ADDRESS     0x4006B000   // Base address for LPUART1

// GPIO Registers 
#define GPIO_PDDR_OFFSET         0x14         // Data Direction Register offset
#define PIN_15                   15           // Pin number 15
#define PIN_16                   16           // Pin number 15

//LPUART Registers
#define LPUART_BAUD_OFFSET       0x10       // BAUD Register offset
#define LPUART_CTRL_OFFSET       0x18       // CTRL Register offset
#define LPUART_STAT_OFFSET       0x14       // STAT Register offset
#define LPUART_DATA_OFFSET       0x1C       // DATA Register offset

#define MUX_START_BIT            8
#define MUX_BIT_LENGTH           3
#define MUX_ALT_2                0x2

#define PCS_START_BIT            24
#define PCS_BIT_LENGTH           3
#define SIRCDIV2_CLK             0x2
#define FIRCDIV2_CLK             0x3
#define SPLLDIV2_CLK             0x6

#define BAUD_START_SBR           0
#define BAUD_BIT_SBR_LENGTH      13
#define BAUD_START_OSR           24
#define BAUD_BIT_OSR_LENGTH      5

#define BAUD_SBNS                13
#define CTRL_M                   4
#define CTRL_PE                  1
#define CTRL_TE                  19
#define CTRL_RE                  18
#define CTRL_RIE                 21

#define STAT_RDRF                21
#define STAT_TDRE                23

#define GPIOD_BASE_ADDRESS       0x400FF0C0
#define GPIO_PDOR_OFFSET         0x0
#define RGB_RED                  15
#define RGB_BLUE                 16

#define SCG_BASE_ADDRESS         0x40064000
#define SCG_SOSCCSR_OFFSET       0x100
#define SCG_SOSCDIV_OFFSET       0x104
#define SCG_SOSCCFG_OFFSET       0x108

#define SOSCDIV_mode_div1        0x1  
#define HGO_BIT                  3
#define EREFS_BIT                2
#define RANGE                    0x2

#define SOSCEN_BIT               0
#define LK_BIT                   23
#define SOSCVLD_BIT              24

// Function prototypes 
//void SetSeriesOfBit(volatile unsigned int *address, unsigned int startPosition, unsigned int bitlength, unsigned int hexvalue);
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
