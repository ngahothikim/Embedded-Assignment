#include "GPIO.h"
#include "ADC.h"
#include "UART.h"
#include "UART_RE.h"

/************************************* FUNCTION PROTOTYPE IRQHandler *************************************/
void ADC_Init(void);
unsigned short ADC_Read(void);
void MAX7219_SetBrightness(unsigned char brightness); 

static unsigned short ADC_Data = 0;

void ADC_Init(void)
{
	// Enable clock for ADC0
	SetBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + PCC_ADC0_OFFSET), CGC_BIT_30, 1);
	
	// Select clock option 1
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + PCC_ADC0_OFFSET), 24, 3, SOSCDIV_mode_div1);																	
	
	// ADC0_SE12 
	//SetSeriesOfBit(&(PORTC->PORT_PCR14), PORT_PCR_MUX_START_BIT_8, PORT_PCR_MUX_BIT_LENGTH_3, ALT0);																							
	
	// Input Clock Select
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_CFG1_OFFSET), ADICLK_START_BIT_0, ADICLK_BIT_LENGTH_2, ALTCLK1);
	
	// Clock Divide Select
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_CFG1_OFFSET), ADIV_START_BIT_5, ADIV_BIT_LENGTH_2, ADIV_DIV1);
	
	// Conversion mode selection
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_CFG1_OFFSET), ADC_MODE_START_BIT_2, ADC_MODE_BIT_LENGTH_2, BIT_CONVERSION_12);
	
	// Sample Time Select
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_CFG2_OFFSET),  SMPLTS_START_BIT_0, SMPLTS_BIT_LENGTH_8, 0xF);
	
	// Set trigger mode
	SetBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_SC2_OFFSET), SC2_ADTRG_BIT_6, 1);						// Trigger by hardware
	
	// Set conversion mode
	SetBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_SC3_OFFSET), SC3_ADC0_BIT_3, 0);						// One shot mode
	
	// Configures channel + Start conversion
	SetSerialMutilBit((volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_SC1A_OFFSET), SC1_ADCH_START_BIT_0, SC1_ADCH_BIT_LENGTH_5, ADC_CHANNEL_12);
}

unsigned short ADC_Read(void)
{
    // Wait for conversion to complete
    while (CheckBit((volatile unsigned int *)(ADC0_BASE_ADDRESS +ADC_SC1A_OFFSET), SC1_COCO_BIT_7) != 1)
		{
      
    }
    ADC_Data = (*(volatile unsigned int *)(ADC0_BASE_ADDRESS + ADC_R0_OFFSET) & (1 << 12));
		
    // Read the result
    return ADC_Data; 
}


