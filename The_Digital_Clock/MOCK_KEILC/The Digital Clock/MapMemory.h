/********************************************************************************
*
*   DEFINE
*
********************************************************************************/

/* NULL */
#define NULL (unsigned int*) 0
    
/* PCC address */
#define PCC_BASE_ADDRESS    0x40065000u


/* CGC location bit */
#define PCC_CGC_BIT 30u
#define PCC_PCS_BIT_BEGIN 24u
#define PCC_PCS_BIT_END   26u

/* PORT base address */
#define PORT_A_BASE_ADDRESS 0x40049000u
#define PORT_B_BASE_ADDRESS 0x4004A000u
#define PORT_C_BASE_ADDRESS 0x4004B000u
#define PORT_D_BASE_ADDRESS 0x4004C000u
#define PORT_E_BASE_ADDRESS 0x4004D000u


/* Location bit in PCR register */
#define PORT_PCR_PS_BIT             0u
#define PORT_PCR_PE_BIT             1u
#define PORT_PCR_MUX_BIT_BEGIN      8u
#define PORT_PCR_MUX_BIT_END        10u
#define PORT_PCR_IRQC_BIT_BEGIN     16u
#define PORT_PCR_IRQC_BIT_END       19u
#define PORT_PCR_ISF_BIT            24u

/* GPIO address */
#define GPIO_A_BASE_ADDRESS     0x400FF000
#define GPIO_B_BASE_ADDRESS     0x400FF040
#define GPIO_C_BASE_ADDRESS     0x400FF080
#define GPIO_D_BASE_ADDRESS     0x400FF0C0
#define GPIO_E_BASE_ADDRESS     0x400FF100


/* LPIT address map */
#define LPIT_BASE_ADDRESS 0x40037000

/** bit of LPIT MCR */
#define LPIT_MCR_M_CEN_BIT  0u
#define LPIT_MCR_DBG_EN_BIT 3u

/* bit of LPIT TCRL */
#define  LPIT_TCTRL_T_EN_BIT        0u
#define  LPIT_TCTRL_MODE_BIT_BEGIN  2u
#define  LPIT_TCTRL_MODE_BIT_END    3u





/* NVIC address */
#define NVIC_ISER_BASE_ADDRESS  0xE000E100u
#define NVIC_IPR_BASE_ADDRESS   0xE000E400u

/* SCG base address */
#define SCG_BASE_ADDRESS 0x40064000u

/* UART base address */
#define LPUART_0_BASE_ADDRESS   0x4006A000u
#define LPUART_1_BASE_ADDRESS   0x4006B000u
#define LPUART_2_BASE_ADDRESS   0x4006C000u

/** SPI Base Address */
#define LPSPI0_BASE_ADDRESS 0x4002C000u
#define LPSPI1_BASE_ADDRESS 0x4002D000u
#define LPSPI2_BASE_ADDRESS 0x4002E000u





/********************************************************************************
*
*   STRUCT MEMORY MAP
*
********************************************************************************/
/** PCC Memory Map */
typedef struct _PCCMemoryMap
{
    unsigned int Rev0[32];

    unsigned int FTFC;
    unsigned int DMAMUX;
    unsigned int Rev1[2];   

    unsigned int FlexCAN0;
    unsigned int FlexCAN1;
    unsigned int FTM3;    
    unsigned int ADC1;    
    unsigned int Rev2[3];

    unsigned int FlexCAN2;
    unsigned int LPSPI0;
    unsigned int LPSPI1;
    unsigned int LPSPI2;    
    unsigned int Rev3[2]; 

    unsigned int PDB1;
    unsigned int CRC;    
    unsigned int Rev4[3];  

    unsigned int PDB0;
    unsigned int LPIT;
    unsigned int FTM0;
    unsigned int FTM1;
    unsigned int FTM2;
    unsigned int ADC0;
    unsigned int Rev5[1];

    unsigned int RTC;    
    unsigned int Rev6[2];

    unsigned int LPTMR0;
    unsigned int Rev7[8];

    unsigned int PORTA;
    unsigned int PORTB;
    unsigned int PORTC;
    unsigned int PORTD;
    unsigned int PORTE;
    unsigned int Rev8[6];

    unsigned int SAI0;
    unsigned int SAI1;
    unsigned int Rev9[4];

    unsigned int FlexIO;    
    unsigned int Rev10[6];

    unsigned int EWM;
    unsigned int Rev11[4];  

    unsigned int LPI2C0;
    unsigned int LPI2C1;
    unsigned int Rev12[2];  

    unsigned int LPUART0;
    unsigned int LPUART1;
    unsigned int LPUART2;
    unsigned int Rev13[1];

    unsigned int FTM4;
    unsigned int FTM5;
    unsigned int FTM6;
    unsigned int FTM7;
    unsigned int Rev14[1];

    unsigned int CMP0;
    unsigned int Rev15[2];

    unsigned int QSPI;
    unsigned int Rev16[2];

    unsigned int ENET;
}PCCMemoryMap;

/** define PCC module */
#define PCC_MODULE ((PCCMemoryMap*)(PCC_BASE_ADDRESS))


/** PORT Memory Map */
typedef struct _PORTMemory
{
    unsigned int PCR[32];
    /*
    unsigned int PCR0;
    unsigned int PCR1;
    unsigned int PCR2;
    unsigned int PCR3;
    unsigned int PCR4;
    unsigned int PCR5;
    unsigned int PCR6;
    unsigned int PCR7;
    unsigned int PCR8;
    unsigned int PCR9;
    unsigned int PCR10;
    unsigned int PCR11;
    unsigned int PCR12;
    unsigned int PCR13;
    unsigned int PCR14;
    unsigned int PCR15;
    unsigned int PCR16;
    unsigned int PCR17;
    unsigned int PCR18;
    unsigned int PCR19;
    unsigned int PCR20;
    unsigned int PCR21;
    unsigned int PCR22;
    unsigned int PCR23;
    unsigned int PCR24;
    unsigned int PCR25;
    unsigned int PCR26;
    unsigned int PCR27;
    unsigned int PCR28;
    unsigned int PCR29;
    unsigned int PCR30;
    unsigned int PCR31;
    */
    unsigned int GPCLR;
    unsigned int GPCHR;
    unsigned int GICHR;
    unsigned int Rev0[4];

    unsigned int ISFR;
    unsigned int Rev1[7];

    unsigned int DFER;
    unsigned int DFCR;
    unsigned int DFWR;

}PORTMemoryMap;

/** define port module */
#define PORT_A_MODULE ((PORTMemoryMap*)(PORT_A_BASE_ADDRESS))
#define PORT_B_MODULE ((PORTMemoryMap*)(PORT_B_BASE_ADDRESS))
#define PORT_C_MODULE ((PORTMemoryMap*)(PORT_C_BASE_ADDRESS))
#define PORT_D_MODULE ((PORTMemoryMap*)(PORT_D_BASE_ADDRESS))
#define PORT_E_MODULE ((PORTMemoryMap*)(PORT_E_BASE_ADDRESS))


/** UART Memory Map */
typedef struct _UARTMemoryMap
{
    unsigned int VERID;
    unsigned int PARAM;
    unsigned int GLOBAL;
    unsigned int PINCFG;
    unsigned int BAUD;
    unsigned int STAT;
    unsigned int CTRL;
    unsigned int DATA;
    unsigned int MATCH;
    unsigned int MODIR;
    unsigned int FIFO;
    unsigned int WATER;

}UARTMemoryMap;

/** SCG Memory Map */
typedef struct _pSCGMemoryMap
{
    unsigned int VERID;
    unsigned int PARAM;
    unsigned int Rev1[2];
    unsigned int CSR;
    unsigned int RCCR;
    unsigned int VCCR;
    unsigned int HCCR;
    unsigned int CLKOUTCNFG;
    
    unsigned int Rev2[55];
    unsigned int SOSCCSR;
    unsigned int SOSCDIV;
    unsigned int SOSCCFG;
    
    unsigned int Rev3[61];
    unsigned int SIRCCSR;
    unsigned int SIRCDIV;
    unsigned int SIRCCFG;
    
    unsigned int Rev4[61];
    unsigned int FIRCCSR;
    unsigned int FIRCDIV;
    unsigned int FIRCCFG;
    
    unsigned int Rev5[189];
    unsigned int SPLLCSR;
    unsigned int SPLLDIV;
    unsigned int SPLLCFG;
}SCGMemoryMap;

/** define SCG module */
#define SCG_MODULE ((SCGMemoryMap*)(SCG_BASE_ADDRESS))


/** SPI Memory Map */
typedef struct _SPIMemoryMap
{
    unsigned int VERID;
    unsigned int PARAM;
    unsigned int Rev1[2];

    unsigned int CR;
    unsigned int SR;
    unsigned int IER;
    unsigned int DER;
    unsigned int CFGR0;
    unsigned int CFGR1;
    unsigned int Rev2[2];

    unsigned int DMR0;
    unsigned int DMR1;
    unsigned int Rev3[2];

    unsigned int CCR;
    unsigned int Rev4[5];

    unsigned int FCR;
    unsigned int FSR;
    unsigned int TCR;
    unsigned int TDR;
    unsigned int Rev5[2];

    unsigned int RSR;
    unsigned int RDR;

}SPIMemoryMap;



typedef struct _GPIOMemoryMap
{
    unsigned int PDOR;
    unsigned int PSOR;
    unsigned int PCOR;
    unsigned int PTOR;
    unsigned int PDIR;
    unsigned int PDDR;
    unsigned int PIDR;      
}GPIOMemoryMap;

/** define GPIO module */
#define GPIO_A_MODULE ((GPIOMemoryMap*)(GPIO_A_BASE_ADDRESS))
#define GPIO_B_MODULE ((GPIOMemoryMap*)(GPIO_B_BASE_ADDRESS))
#define GPIO_C_MODULE ((GPIOMemoryMap*)(GPIO_C_BASE_ADDRESS))
#define GPIO_D_MODULE ((GPIOMemoryMap*)(GPIO_D_BASE_ADDRESS))
#define GPIO_E_MODULE ((GPIOMemoryMap*)(GPIO_E_BASE_ADDRESS))


typedef struct _LPITMemoryMap
{
    unsigned int VERID;
    unsigned int PARAM;
    unsigned int MCR;
    unsigned int MSR;
    unsigned int MIER;
    unsigned int SETTEN;
    unsigned int CLRTEN;
    unsigned int Rev1[1];
    unsigned int TVAL0;
    unsigned int CVAL0;
    unsigned int TCTRL0;
    unsigned int Rev2[1];
    unsigned int TVAL1;
    unsigned int CVAL1;
    unsigned int TCTRL1;
    unsigned int Rev3[1];
    unsigned int TVAL2;
    unsigned int CVAL2;
    unsigned int TCTRL2;
    unsigned int Rev4[1];
    unsigned int TVAL3;
    unsigned int CVAL3;
    unsigned int TCTRL3;
}LPITMemoryMap;

#define LPIT_MODULE ((LPITMemoryMap *)(LPIT_BASE_ADDRESS))










