#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
 
#define SETBIT    1
#define CLEARBIT  0

// Base addresses for peripheral registers 
#define PCC_BASE_ADDRESS         (0x40065000)  // Base address for PCC (Peripheral Clock Control), used to enable and configure clocks for peripherals
#define PORT_BASE_ADDRESS        (0x40049000)  // Base address for PORT registers, used for configuring GPIO pin functions and attributes
#define GPIO_A_BASE_ADDRESS      (0x400FF000)  // Base address for GPIO Port A, used for accessing GPIO functionalities for Port A
#define GPIO_B_BASE_ADDRESS      (0x400FF040)  // Base address for GPIO Port B, used for accessing GPIO functionalities for Port B
#define GPIO_C_BASE_ADDRESS      (0x400FF080)  // Base address for GPIO Port C, used for accessing GPIO functionalities for Port C
#define GPIO_D_BASE_ADDRESS      (0x400FF0C0)  // Base address for GPIO Port D, used for accessing GPIO functionalities for Port D
#define GPIO_E_BASE_ADDRESS      (0x400FF100)  // Base address for GPIO Port E, used for accessing GPIO functionalities for Port E

// Corrected PCC offsets for each port
#define PCC_PORTA_OFFSET         (0x124)   // PCC PORTA offset
#define PCC_PORTB_OFFSET         (0x128)   // PCC PORTB offset
#define PCC_PORTC_OFFSET         (0x12C)   // PCC PORTC offset
#define PCC_PORTD_OFFSET         (0x130)   // PCC PORTD offset
#define PCC_PORTE_OFFSET         (0x134)   // PCC PORTE offset
#define PCC_PCCn_CGC_SHIFT       (30)

// PORT base addresses
#define PORT_A_OFFSET            (0x40049000U)  // PORT A base address
#define PORT_B_OFFSET            (0x4004A000U)  // PORT B base address
#define PORT_C_OFFSET            (0x4004B000U)  // PORT C base address
#define PORT_D_OFFSET            (0x4004C000U)  // PORT D base address
#define PORT_E_OFFSET            (0x4004D000U)  // PORT E base address

// Define PORT PCR MUX values for GPIO mode
#define PORT_PCR_MUX_GPIO        1U
#define PORT_PCR_PE_GPIO         0U
#define PORT_PCR_IRQC_GPIO       0x9

// Macros for bit manipulation on registers
#define SET_BIT(REG, BIT)     ((REG) |=  (1U << (BIT)))    // Set a specific BIT in REG to 1 (enable)
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))    // Clear a specific BIT in REG to 0 (disable)
#define READ_BIT(REG, BIT)    ((REG) &   (1U << (BIT)))    // Read the value of a specific BIT in REG (returns non-zero if BIT is set)

// Define structures for PORT and GPIO registers
typedef struct 
{
    volatile uint32_t PCR[32];   // PCR registers for each pin

}PORT_Type;

#define PORT_A ((PORT_Type*)(PORT_A_OFFSET))
#define PORT_B ((PORT_Type*)(PORT_B_OFFSET))
#define PORT_C ((PORT_Type*)(PORT_C_OFFSET))
#define PORT_D ((PORT_Type*)(PORT_D_OFFSET))
#define PORT_E ((PORT_Type*)(PORT_E_OFFSET))

// Define structures for GPIO type registers
typedef struct 
{
    volatile uint32_t PDOR; // Port Data Output Register
    volatile uint32_t PSOR; // Port Set Output Register
    volatile uint32_t PCOR; // Port Clear Output Register
    volatile uint32_t PTOR; // Port Toggle Output Register
    volatile uint32_t PDIR; // Port Data Input Register
    volatile uint32_t PDDR; // Port Data Direction Register
} GPIO_Type;

// Macros for accessing GPIO register structures for each GPIO port
#define GPIO_A ((GPIO_Type *) GPIO_A_BASE_ADDRESS)  // Pointer to GPIO port A registers
#define GPIO_B ((GPIO_Type *) GPIO_B_BASE_ADDRESS)  // Pointer to GPIO port B registers
#define GPIO_C ((GPIO_Type *) GPIO_C_BASE_ADDRESS)  // Pointer to GPIO port C registers
#define GPIO_D ((GPIO_Type *) GPIO_D_BASE_ADDRESS)  // Pointer to GPIO port D registers
#define GPIO_E ((GPIO_Type *) GPIO_E_BASE_ADDRESS)  // Pointer to GPIO port E registers

// Function declarations
void SetBit(volatile unsigned int *address, int bit, int value);
void SetSerialMutilBit(volatile unsigned int *address,unsigned int start_bit,unsigned int length,  unsigned int value);
void Gpio_Init(uint32_t pcc_offset);
void Gpio_SetMode(PORT_Type* portBase, GPIO_Type* gpioBase, uint8_t pin, uint8_t direction, uint8_t irq);
void Gpio_WriteChannel(GPIO_Type* gpioBase, uint8_t pin, uint8_t level);
uint8_t Gpio_ReadChannel(GPIO_Type* gpioBase, uint8_t pin);

#endif // GPIO_H
