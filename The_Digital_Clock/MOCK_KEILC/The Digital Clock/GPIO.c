#include "GPIO.h"

/*-------------------------------------------------------------*/
/*                      Function: set a bit                    */
/*-------------------------------------------------------------*/
/* Description: This function sets or clears a specific bit at the given address.
 * Parameters:
 *   - address: Pointer to the register address.
 *   - bit: The bit position to set or clear.
 *   - value: Determines whether to set (SETBIT) or clear (CLEARBIT) the bit.
 *-------------------------------------------------------------------------------*/
void SetBit(volatile unsigned int *address, int bit, int value) 
{
    if (value == SETBIT) 
		{
        *address |= (1 << bit);
    } 
		else if (value == CLEARBIT) 
		{
        *address &= ~(1 << bit);
    }
}

/*-------------------------------------------------------------*/
/*                Function: set multiple bits                  */
/*-------------------------------------------------------------*/
/*
 * Description: This function sets multiple consecutive bits in a register.
 * Parameters:
 *   - address: Pointer to the register address.
 *   - start_bit: The starting bit position.
 *   - length: The number of consecutive bits to modify.
 *   - value: The value to write to the bits.
 *--------------------------------------------------------------------------*/
void SetSerialMutilBit(volatile uint32_t *address, unsigned int start_bit, unsigned int length, unsigned int value)
{	
		unsigned int bit_value;
		unsigned int bitmask;
		unsigned int value_shifted;

		bit_value = *address;
	
		/**** Creat a bit mask ***/
		bitmask = ((1U << length)-1) << start_bit;
		value_shifted = (value & ((1U << length) - 1)) << start_bit;

		/**** Clear bit at request position ****/
		bit_value &= ~bitmask;
		bit_value |= value_shifted;

		/**** Set new series of bit ****/
		*address = bit_value;
}

//------------------------------------------//
//    Function to initialize a GPIO port    //
//------------------------------------------//
/*
 * Description: Initializes the specified GPIO port by enabling the clock for it.
 * Parameters:
 *   - pcc_offset: Offset of the PCC register for the GPIO port.
 *---------------------------------------------------------------------------------*/
void Gpio_Init(uint32_t pcc_offset) 
{
		volatile uint32_t *pccPort = (volatile uint32_t *)(PCC_BASE_ADDRESS + pcc_offset);

		// Enable clock for the selected port
		SET_BIT(*pccPort, PCC_PCCn_CGC_SHIFT);
}

//------------------------------------------//
//       Function to set GPIO mode          //
//------------------------------------------//
/*
 * Description: Configures the mode, direction, and interrupt for a GPIO pin.
 * Parameters:
 *   - portBase: Pointer to the PORT control register.
 *   - gpioBase: Pointer to the GPIO control register.
 *   - pin: Pin number to configure.
 *   - direction: Direction of the pin (1 = output, 0 = input).
 *   - irq: Enable (1) or disable (0) interrupt for the pin.
 *-------------------------------------------------------------------------------*/
void Gpio_SetMode(PORT_Type* portBase, GPIO_Type* gpioBase, uint8_t pin, uint8_t direction, uint8_t irq) 
{
		// Set the pin to GPIO mode (MUX = 1)
		portBase->PCR[pin] = (portBase->PCR[pin] & ~(uint32_t)(0x700)) | (PORT_PCR_MUX_GPIO << 8);

		// Enable interrupt if requested
		if (irq) 
		{
        SetBit(&portBase->PCR[pin], 0, 0);                  //disable PE
        SetBit(&portBase->PCR[pin], 1, 0);                  //disable PS
        SetSerialMutilBit(&portBase->PCR[pin], 16, 4, 9);   // Configure interrupt for the pin (implementation depends on specific interrupt configuration)

    }
    
     // Set the direction
    if (direction == 1) 
		{
        SetBit(&gpioBase->PDDR, pin, 1);     // Set pin as output
    } else 
		{
        SetBit(&gpioBase->PDDR, pin, 0);     // Set pin as input

    }
}

//------------------------------------------//
//    Function to write to a GPIO channel   //
//------------------------------------------//
/*
 * Description: Writes a value (high or low) to a GPIO pin.
 * Parameters:
 *   - gpioBase: Pointer to the GPIO control register.
 *   - pin: Pin number to write to.
 *   - level: Level to write (1 = high, 0 = low).
 *---------------------------------------------------------------------*/
void Gpio_WriteChannel(GPIO_Type*gpioBase, uint8_t pin, uint8_t level) 
{
    // Validate configuration: ensure the pin is configured as output
    if (READ_BIT(gpioBase->PDDR, pin) == 0) 
		{
        return; // Pin is not configured as output
    }

    // Set or clear the pin level
    if (level == 1) 
		{
        SetBit(&GPIO_D->PSOR, pin, 1); // Set pin
    } else 
		{
        SetBit(&GPIO_D->PCOR, pin, 1); // Clear pin
    }
}


//---------------------------------------------//
//     Function to read from a GPIO channel    //
//---------------------------------------------//
/*
 * Description: Reads the value (high or low) of a GPIO pin.
 * Parameters:
 *   - gpioBase: Pointer to the GPIO control register.
 *   - pin: Pin number to read from.
 * Returns: 1 if pin is high, 0 if low, 0xFF if the pin is not configured as input.
 *---------------------------------------------------------------------------------*/
uint8_t Gpio_ReadChannel(GPIO_Type* gpioBase, uint8_t pin) 
{
    // Validate configuration: ensure the pin is configured as input

    if (READ_BIT(gpioBase->PDDR, pin) != 0) 
		{
        return 0xFF; // Pin is not configured as input
    }

    // Read and return the pin level
    return READ_BIT(gpioBase->PDIR, pin) ? 1 : 0;
}



