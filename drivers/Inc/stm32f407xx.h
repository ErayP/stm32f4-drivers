/*
 * stm32f407xx.h
 *
 *  Created on: 27 Ara 2025
 *      Author: polat
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>



#define __vo volatile
#define __weak __attribute__((weak))

/**********************************************START:Processor Spesific Details********************************************************
 *
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */


#define NVIC_ISER0					((__vo uint32_t* )0xE000E100)
#define NVIC_ISER1					((__vo uint32_t* )0xE000E104)
#define NVIC_ISER2					((__vo uint32_t* )0xE000E108)
#define NVIC_ISER3					((__vo uint32_t* )0xE000E10C)

 /*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */

#define NVIC_ICER0					((__vo uint32_t* )0XE000E180)
#define NVIC_ICER1					((__vo uint32_t* )0XE000E184)
#define NVIC_ICER2					((__vo uint32_t* )0XE000E188)
#define NVIC_ICER3					((__vo uint32_t* )0XE000E18C)

 /*
 * ARM Cortex Mx Processor NVIC IPRx register Addresses
 */


#define NVIC_PR_BASE_ADDR			((__vo uint32_t* )0xE000E400)

 /*
 * ARM Cortex Mx Processor number of priority bits implemented in priority register.
 */


#define NO_PR_BITS_IMPLEMENTED		4



#define FLASH_BASEADDR				0X08000000U //flash temel adresi
#define	SRAM1_BASEADDR				0X20000000U //sram1 base adress 112kb
#define SRAM2_BASEADDR				0x2001C000U	//sram1 + 112 = sram2 address
#define ROM							0X1FFF0000U //system memory ile rom aynı
#define SRAM 						SRAM1_BASEADDR


//AHBx AND APHx BUS PERIPHERAL BASE ADDRESSES
#define	PERIPH_BASEADDR				0x40000000U
#define	APB1PERIPH_BASEADDR			PERIPH_BASEADDR
#define	APB2PERIPH_BASEADDR			0x40010000U
#define	AHB1PERIPH_BASEADDR			0x40020000U
#define	AHB2PERIPH_BASEADDR			0x50000000U


//BASE ADDRESSES OF PERIPHERALS WHİCH ARE HANGİNG ON AHB1 BUS

#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR +0x0000U)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR +0x0400U)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR +0x0800U)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR +0x0C00U)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR +0x1000U)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASEADDR +0x1400U)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASEADDR +0x1800U)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR +0x1C00U)
#define GPIOI_BASEADDR				(AHB1PERIPH_BASEADDR +0x2000U)
#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR +0x3800U)

//BASE ADDRESSES OF PERIPHERALS WHİCH ARE HANGİNG ON APB1 BUS

#define I2C1_BASEADDR				(APB1PERIPH_BASEADDR +0x5400U)
#define I2C2_BASEADDR				(APB1PERIPH_BASEADDR +0x5800U)
#define I2C3_BASEADDR				(APB1PERIPH_BASEADDR +0x5C00U)

#define SPI2_BASEADDR				(APB1PERIPH_BASEADDR +0x3800U)
#define SPI3_BASEADDR				(APB1PERIPH_BASEADDR +0x3C00U)

#define USART2_BASEADDR				(APB1PERIPH_BASEADDR +0x4400U)
#define USART3_BASEADDR				(APB1PERIPH_BASEADDR +0x4800U)
#define UART4_BASEADDR				(APB1PERIPH_BASEADDR +0x4C00U)
#define UART5_BASEADDR				(APB1PERIPH_BASEADDR +0x5000U)

//BASE ADDRESSES OF PERIPHERALS WHİCH ARE HANGİNG ON APB2 BUS

#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR +0x3C00U)
#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR +0x3000U)
#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR +0X3800U)
#define USART1_BASEADDR				(APB2PERIPH_BASEADDR +0x1000U)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR +0x1400U)

//peripheral register definition structures
//BU GPIOx ICIN REGISTER KAYDIDIR.
typedef struct
{

	__vo uint32_t MODER; 	//GPIO port mode register					0X00
	__vo uint32_t OTYPER;	//GPIO port output type register			0X04
	__vo uint32_t OSPEEDR;	//GPIO port speed register					0X08
	__vo uint32_t PUPDR;	//GPIO port pull-up/pull-down register		0x0C
	__vo uint32_t IDR;		//GPIO port input data register				0x10
	__vo uint32_t ODR;		//GPIO port output data register			0x14
	__vo uint32_t BSRR;		//GPIO port bit set/reset register			0x18
	__vo uint32_t LCKR;		//GPIO port configuration lock register		0x1C
	__vo uint32_t AFR[2];	//GPIO alternate function low-high register 0X20-0X24

}GPIO_RegDef_t;


typedef struct
{

	__vo uint32_t CR; 			//clock control register 								0x00
	__vo uint32_t PLLCFGR;		//PLL configuration register							0x04
	__vo uint32_t CFGR;			//clock configuration register							0x08
	__vo uint32_t CIR;			//clock interrupt register								0x0C
	__vo uint32_t AHB1RSTR;		//peripheral reset register AHBx						0x10
	__vo uint32_t AHB2RSTR;		//peripheral reset register AHBx						0x14
	__vo uint32_t AHB3RSTR;		//peripheral reset register AHBx						0x18
		 uint32_t RESERVED0;	//RESERVED												0X1C
	__vo uint32_t APB1RSTR;		//peripheral reset register	APBx						0x20
	__vo uint32_t APB2RSTR;		//peripheral reset register	APBx						0x24
		 uint32_t RESERVED1[2];	//RESERVED												0X28 0X2C
	__vo uint32_t AHB1ENR;		//peripheral clock register								0x30
	__vo uint32_t AHB2ENR;		//peripheral clock register								0x34
	__vo uint32_t AHB3ENR;		//peripheral clock register								0x38
		 uint32_t RESERVED2;	//RESERVED												0X3C
	__vo uint32_t APB1ENR;		//peripheral clock enable register  					0x40
	__vo uint32_t APB2ENR;		//peripheral clock enable register  					0x44
	 	 uint32_t RESERVED3[2];	//RESERVED												0X48 0X4C
	__vo uint32_t AHB1LPENR;	//peripheral clock enable in low power mode register	0x50
	__vo uint32_t AHB2LPENR;	//peripheral clock enable in low power mode register	0x54
	__vo uint32_t AHB3LPENR;	//peripheral clock enable in low power mode register	0x58
		 uint32_t RESERVED4;	//RESERVED												0X5C
	__vo uint32_t APB1LPENR;	//peripheral clock enable in low power mode register	0x60
	__vo uint32_t APB2LPENR;	//peripheral clock enable in low power mode register	0x64
		 uint32_t RESERVED5[2];	//RESERVED												0X68 0X6C
	__vo uint32_t BDCR;			//Backup domain control register						0x70
	__vo uint32_t CSR;			//clock control & status register						0x74
	 	 uint32_t RESERVED6[2];	//RESERVED												0X78 0X7C
	__vo uint32_t SSCGR;		//spread spectrum clock generation register				0x80
	__vo uint32_t PLLI2SCFGR;	//PLLI2S configuration register							0x84


}RCC_RegDef_t;


//peripheral register definition structures FOR EXTI
//BU EXTI ICIN REGISTER KAYDIDIR.

typedef struct {

	__vo uint32_t IMR; 			//INTERRUPT MASK REGISTER 								0x00
	__vo uint32_t EMR; 			//EVENT MASK REGISTER									0x04
	__vo uint32_t RTSR; 		//RISING TRIGGER SELECTION REGISTER 					0x08
	__vo uint32_t FTSR; 		//Falling trigger selection register					0x0c
	__vo uint32_t SWIER; 		//Software interrupt event register						0x10
	__vo uint32_t PR; 			//Pending register										0x14

}EXTI_RegDef_t;


//peripheral register definition structures FOR SYSCFG
//BU SYSCFG ICIN REGISTER KAYDIDIR.

typedef struct {

	__vo uint32_t MEMRMP; 		//INTERRUPT MASK REGISTER 								0x00
	__vo uint32_t PMC; 			//EVENT MASK REGISTER									0x04
	__vo uint32_t EXTICR[4]; 	//RISING TRIGGER SELECTION REGISTER 					0x08-0x0C-0X10-0X14
		 uint32_t RESERVED0[2];	//reserved												0x18-0x1C
	__vo uint32_t CMPCR; 		//Pending register										0x20

}SYSCFG_RegDef_t;


//peripheral register definition structures FOR SPI


typedef struct {

	__vo uint32_t CR1; 			//SPI control register 1								0x00
	__vo uint32_t CR2; 			//SPI control register 2								0x00
	__vo uint32_t SR; 			//SPI status register									0x08
	__vo uint32_t DR;	 		//SPI data register										0x0C
	__vo uint32_t CRCPR; 		//CRC polynomial register 								0x10
	__vo uint32_t RXCRCR; 		//SPI RX CRC register	 								0x14
	__vo uint32_t TXCRCR; 		//SPI TX CRC register	 								0x18
	__vo uint32_t I2SCFGR; 		//SPI_I2S configuration register						0x1C
	__vo uint32_t I2SPR; 		//SPI_I2S prescaler register 							0x20

}SPI_RegDef_t;

typedef struct {

	__vo uint32_t CR1; 			//I2C control register 1								0x00
	__vo uint32_t CR2; 			//I2C control register 2								0x04
	__vo uint32_t OAR1; 		//I2C Own address register-1							0x08
	__vo uint32_t OAR2;	 		//I2C Own address register-2							0x0C
	__vo uint32_t DR; 			//I2C Data register		 								0x10
	__vo uint32_t SR1; 			//I2C Status register-1		 							0x14
	__vo uint32_t SR2; 			//I2C Status register-2		 							0x18
	__vo uint32_t CCR; 			//I2C Clock control register							0x1C
	__vo uint32_t TRISE; 		//I2C TRISE register		 							0x20
	__vo uint32_t FLTR; 		//I2C FLTR register										0x24

}I2C_RegDef_t;
/*
 * peripheral definitions (Peripheral base addresses typecast to xxx_RegDef_t)
 */

#define GPIOA					((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB					((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC					((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD					((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE					((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOF					((GPIO_RegDef_t*) GPIOF_BASEADDR)
#define GPIOG					((GPIO_RegDef_t*) GPIOG_BASEADDR)
#define GPIOH					((GPIO_RegDef_t*) GPIOH_BASEADDR)
#define GPIOI					((GPIO_RegDef_t*) GPIOI_BASEADDR)

#define RCC						((RCC_RegDef_t*) RCC_BASEADDR)
#define EXTI					((EXTI_RegDef_t*) EXTI_BASEADDR)
#define SYSCFG					((SYSCFG_RegDef_t*) SYSCFG_BASEADDR)
#define SPI1					((SPI_RegDef_t*) SPI1_BASEADDR)
#define SPI2					((SPI_RegDef_t*) SPI2_BASEADDR)
#define SPI3					((SPI_RegDef_t*) SPI3_BASEADDR)

#define I2C1					((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2					((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3					((I2C_RegDef_t*)I2C3_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<0 ))
#define GPIOB_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<1 ))
#define GPIOC_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<2 ))
#define GPIOD_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<3 ))
#define GPIOE_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<4 ))
#define GPIOF_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<5 ))
#define GPIOG_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<6 ))
#define GPIOH_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<7 ))
#define GPIOI_PCLK_EN()				(RCC->AHB1ENR |= ( 1<<8 ))

/*
 * Clock Enable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()				(RCC->APB1ENR |= ( 1<<21 ))
#define I2C2_PCLK_EN()				(RCC->APB1ENR |= ( 1<<22 ))
#define I2C3_PCLK_EN()				(RCC->APB1ENR |= ( 1<<23 ))

/*
 * Clock Enable Macros for SPIx peripherals
 */

#define SPI1_PCLK_EN()				(RCC->APB2ENR |= ( 1<<12 ))
#define SPI2_PCLK_EN()				(RCC->APB1ENR |= ( 1<<14 ))
#define SPI3_PCLK_EN()				(RCC->APB1ENR |= ( 1<<15 ))

/*
 * Clock Enable Macros for USARTx peripherals
 */

#define USART1_PCLK_EN()				(RCC->APB2ENR |= ( 1<<4  ))
#define USART2_PCLK_EN()				(RCC->APB1ENR |= ( 1<<17 ))
#define USART3_PCLK_EN()				(RCC->APB1ENR |= ( 1<<18 ))
#define USART6_PCLK_EN()				(RCC->APB2ENR |= ( 1<<5  ))

/*
 * Clock Enable Macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_EN()				(RCC->APB2ENR |= ( 1<<14 ))

/*
 * Clock Disable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<0 ))
#define GPIOB_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<1 ))
#define GPIOC_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<2 ))
#define GPIOD_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<3 ))
#define GPIOE_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<4 ))
#define GPIOF_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<5 ))
#define GPIOG_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<6 ))
#define GPIOH_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<7 ))
#define GPIOI_PCLK_DI()				(RCC->AHB1ENR &= ~( 1<<8 ))

/*
 * Clock Disable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<21 ))
#define I2C2_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<22 ))
#define I2C3_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<23 ))

/*
 * Clock Disable Macros for SPIx peripherals
 */

#define SPI1_PCLK_DI()				(RCC->APB2ENR &= ~( 1<<12 ))
#define SPI2_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<14 ))
#define SPI3_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<15 ))

/*
 * Clock Disable Macros for USARTx peripherals
 */

#define USART1_PCLK_DI()				(RCC->APB2ENR &= ~( 1<<4  ))
#define USART2_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<17 ))
#define USART3_PCLK_DI()				(RCC->APB1ENR &= ~( 1<<18 ))
#define USART6_PCLK_DI()				(RCC->APB2ENR &= ~( 1<<5  ))

/*
 * Clock Disable Macros for SYSCFG peripherals
 */

#define SYSCFG_PCLK_DI()				(RCC->APB2ENR &= ~( 1<<14 ))



/*
 * some generic macros
 */

#define ENABLE 					1
#define DISABLE 				0
#define SET 					ENABLE
#define RESET 					DISABLE
#define GPIO_PIN_SET 			ENABLE
#define GPIO_PIN_RESET 			DISABLE
#define FLAG_RESET				RESET
#define FLAG_SET				SET


/*
 * MARCOS TO RESET GPIOX PERIPHERALS
 */
#define GPIOA_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 0)); 	(RCC->AHB1RSTR &= ~( 1<<0 )); }while(0)
#define GPIOB_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 1)); 	(RCC->AHB1RSTR &= ~( 1<<1 )); }while(0)
#define GPIOC_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 2)); 	(RCC->AHB1RSTR &= ~( 1<<2 )); }while(0)
#define GPIOD_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 3)); 	(RCC->AHB1RSTR &= ~( 1<<3 )); }while(0)
#define GPIOE_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 4)); 	(RCC->AHB1RSTR &= ~( 1<<4 )); }while(0)
#define GPIOF_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 5)); 	(RCC->AHB1RSTR &= ~( 1<<5 )); }while(0)
#define GPIOG_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 6)); 	(RCC->AHB1RSTR &= ~( 1<<6 )); }while(0)
#define GPIOH_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 7)); 	(RCC->AHB1RSTR &= ~( 1<<7 )); }while(0)
#define GPIOI_REG_RESET()		do{(RCC->AHB1RSTR |= (1 << 8)); 	(RCC->AHB1RSTR &= ~( 1<<8 )); }while(0)


/*
 * MARCOS TO RESET SPIx PERIPHERALS
 */
#define SPI1_REG_RESET()		do{(RCC->APB2RSTR |= (1 << 12)); 	(RCC->APB2RSTR &= ~( 1<<12 )); }while(0)
#define SPI2_REG_RESET()		do{(RCC->APB1RSTR |= (1 << 14)); 	(RCC->APB1RSTR &= ~( 1<<14 )); }while(0)
#define SPI3_REG_RESET()		do{(RCC->APB1RSTR |= (1 << 15)); 	(RCC->APB1RSTR &= ~( 1<<15 )); }while(0)

/*
 * MARCOS TO RESET I2Cx PERIPHERALS
 */
#define I2C1_REG_RESET()		do{(RCC->APB1RSTR |= (1 << 21)); 	(RCC->APB1RSTR &= ~( 1<<21 )); }while(0)
#define I2C2_REG_RESET()		do{(RCC->APB1RSTR |= (1 << 22)); 	(RCC->APB1RSTR &= ~( 1<<22 )); }while(0)
#define I2C3_REG_RESET()		do{(RCC->APB1RSTR |= (1 << 23)); 	(RCC->APB1RSTR &= ~( 1<<23 )); }while(0)



/*
 * MARCOS TO RESET SPI_CR1 PERIPHERALS
 */

#define SPI_CR1_CPHA									0
#define SPI_CR1_CPOL									1
#define SPI_CR1_MSTR									2
#define SPI_CR1_BR										3
#define SPI_CR1_SPE										6
#define SPI_CR1_LSB_FIRST								7
#define SPI_CR1_SSI										8
#define SPI_CR1_SSM										9
#define SPI_CR1_RX_ONLY									10
#define SPI_CR1_DFF										11
#define SPI_CR1_CRC_NEXT								12
#define SPI_CR1_CRC_EN									13
#define SPI_CR1_BIDI_OE									14
#define SPI_CR1_BIDI_MODE								15



/*
 * MARCOS TO RESET SPI_CR2 PERIPHERALS
 */

#define SPI_CR2_RXMAEN									0
#define SPI_CR2_TDXMAEN									1
#define SPI_CR2_SSOE									2
#define SPI_CR2_FRF										4
#define SPI_CR2_ERRIE									5
#define SPI_CR2_RXNEIE									6
#define SPI_CR2_TXEIE									7



/*
 * MARCOS TO RESET SPI_SR PERIPHERALS
 */

#define SPI_SR_RXNE										0
#define SPI_SR_TXE										1
#define SPI_SR_CHSIDE									2
#define SPI_SR_UDR										3
#define SPI_SR_CRC_ERR									4
#define SPI_SR_MODF										5
#define SPI_SR_OVR										6
#define SPI_SR_BSY										7
#define SPI_SR_FRE										8


/*
 * THIS MACRO RETURN THE BETWEEN(0-7) FOR A GIVEN GPIO BASE ADDR(X)
 */

/*************************************************************************
 * 					I2C BIT POSİTİON
 *************************************************************************/
//I2C_CR1
#define I2C_CR1_PE										0
#define I2C_CR1_SMBUS									1
#define I2C_CR1_SMB_TYPE								3
#define I2C_CR1_ENARP									4
#define I2C_CR1_ENPEC									5
#define I2C_CR1_ENGC									6
#define I2C_CR1_NO_STRETCH								7
#define I2C_CR1_START									8
#define I2C_CR1_STOP									9
#define I2C_CR1_ACK										10
#define I2C_CR1_POS										11
#define I2C_CR1_PEC										12
#define I2C_CR1_ALERT									13
#define I2C_CR1_SWRST									15


//I2C_CR2

#define I2C_CR2_FREQ									0
#define I2C_CR2_ITERREN									1
#define I2C_CR2_ITEVTEN									3
#define I2C_CR2_ITBUFEN									4
#define I2C_CR2_DMAEN									5
#define I2C_CR2_LAST									6


//I2C_SR1

#define I2C_SR1_SB										0
#define I2C_SR1_ADDR									1
#define I2C_SR1_BTF										2
#define I2C_SR1_ADD10									3
#define I2C_SR1_STOPF									4
#define I2C_SR1_RXNE									6
#define I2C_SR1_TXE										7
#define I2C_SR1_BERR									8
#define I2C_SR1_ARLO									9
#define I2C_SR1_AF										10
#define I2C_SR1_OVR										11
#define I2C_SR1_PEC_ERR									12
#define I2C_SR1_TIME_OUT								14
#define I2C_SR1_SMB_ALERT								15

//I2C_SR2

#define I2C_SR2_MSL										0
#define I2C_SR2_BUSY									1
#define I2C_SR2_TRA										2
#define I2C_SR2_GEN_CALL								4
#define I2C_SR2_SMBDE_FAULT								5
#define I2C_SR2_SMB_HOST								6
#define I2C_SR2_DUALF									7



#define GPIO_BASEADDR_TO_CODE(x)						   ((x == GPIOA) ? 0:\
															(x == GPIOB) ? 1:\
															(x == GPIOC) ? 2:\
															(x == GPIOD) ? 3:\
															(x == GPIOE) ? 4:\
															(x == GPIOF) ? 5:\
															(x == GPIOG) ? 6:\
															(x == GPIOH) ? 7:\
															(x == GPIOI) ? 8:0)

/*
 * GPIOx IRQ NUMBERS FOR EXTI
 */

#define IRQ_NO_EXTI0										6
#define IRQ_NO_EXTI1										7
#define IRQ_NO_EXTI2										8
#define IRQ_NO_EXTI3										9
#define IRQ_NO_EXTI4										10
#define IRQ_NO_EXTI9_5										23
#define IRQ_NO_EXTI15_10									40



/*
 * SPIx IRQ Numbers
 */

#define IRQ_NO_SPI1											42
#define IRQ_NO_SPI2											43
#define IRQ_NO_SPI3											51





/*
 * macros all the priority level
 */
#define NVIC_IRQ_PRIORITY_15								15
#define NVIC_IRQ_PRIORITY_14								14
#define NVIC_IRQ_PRIORITY_13								13
#define NVIC_IRQ_PRIORITY_12								12
#define NVIC_IRQ_PRIORITY_11								11
#define NVIC_IRQ_PRIORITY_10								10
#define NVIC_IRQ_PRIORITY_9									9
#define NVIC_IRQ_PRIORITY_8									8
#define NVIC_IRQ_PRIORITY_7									7
#define NVIC_IRQ_PRIORITY_6									6
#define NVIC_IRQ_PRIORITY_5									5
#define NVIC_IRQ_PRIORITY_4									4
#define NVIC_IRQ_PRIORITY_3									3
#define NVIC_IRQ_PRIORITY_2									2
#define NVIC_IRQ_PRIORITY_1									1
#define NVIC_IRQ_PRIORITY_0									0



#include <stm32f407xx_gpio_driver.h>
#include <stm32f407xx_spi_driver.h>
#include <stm32f407xx_I2C_driver.h>

#endif /* INC_STM32F407XX_H_ */
