#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#define ADC1_BASE          (0x40027000)
#define PCC_ADC0_OFFSET    (0xEC)
#define PCC_ADC1_OFFSET    (0x9C)

#define CGC_BIT_30         30
#define ADC0_BASE_ADDRESS  0x4003B000
#define ADC_CFG1_OFFSET    0x40 
#define ADC_CFG2_OFFSET    0x44
#define ADC_CV1_OFFSET     0X88
#define ADC_CV2_OFFSET     0x8C
#define ADC_SC2_OFFSET     0x90
#define ADC_SC3_OFFSET     0x94
#define ADC_SC1A_OFFSET    0x0
#define ADC_R0_OFFSET      0x48

#define ADC_MODE_START_BIT_2    2
#define ADC_MODE_BIT_LENGTH_2   2
#define BIT_CONVERSION_12       0x1
#define ADICLK_START_BIT_0      0
#define ADICLK_BIT_LENGTH_2     2
#define ALTCLK1                 0x0
#define ADIV_START_BIT_5        5
#define ADIV_BIT_LENGTH_2       2
#define ADIV_DIV1               0x0	
#define ADIV_DIV2               0x1
#define SMPLTS_START_BIT_0      0
#define SMPLTS_BIT_LENGTH_8     8
#define SC2_ACFE_BIT_5          3
#define SC1_COCO_BIT_7          7
#define SC1_ADCH_START_BIT_0    0
#define SC1_ADCH_BIT_LENGTH_5   6
#define SC2_ADTRG_BIT_6         6
#define SC3_ADC0_BIT_3          3
#define ADC_CHANNEL_12          0xC


#define ADC0  ((LPSPI_Registers *)(LPSPI0_BASE))   // Define pointer to LPSPI0 register structure
#define PCC_LPSPI0  (*(volatile uint32_t *)(PCC_BASE_ADDRESS + PCC_LPSPI0_OFFSET)) 
void ADC_Init(void);
unsigned short ADC_Read(void);


#endif // ADC_H
