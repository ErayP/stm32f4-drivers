/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: 4 Oca 2026
 *      Author: polat
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_
#include "stm32f407xx.h"

typedef struct{

	uint8_t SPI_DeviceMode;		//@SPI_DEVICE_MOD
	uint8_t SPI_BusConfig;		//@SPI_BUS_CONFIG
	uint8_t SPI_SclkSpeed;		//@SPI_SCLK_SPEED
	uint8_t SPI_DFF;			//@SPI_DFF
	uint8_t SPI_CPHA;			//@SPI_CPHA
	uint8_t SPI_CPOL;			//@SPI_CPOL
	uint8_t SPI_SSM;			//@SPI_SSM

}SPI_PinConfig_t;

typedef struct{

	SPI_RegDef_t *pSPIx;
	SPI_PinConfig_t SPI_PinConfig;

}SPI_Handle_t;


//@SPI_DEVICE_MOD

#define SPI_DEVICE_MOD_MASTER 					1
#define SPI_DEVICE_MOD_SLAVE  					0

//@SPI_BUS_CONFIG

#define SPI_BUS_CONFIG_FD						1
#define SPI_BUS_CONFIG_HD						2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY			3

//@SPI_SCLK_SPEED

#define SPI_SCLK_SPEED_DIV2						0
#define SPI_SCLK_SPEED_DIV4						1
#define SPI_SCLK_SPEED_DIV8						2
#define SPI_SCLK_SPEED_DIV16					3
#define SPI_SCLK_SPEED_DIV32					4
#define SPI_SCLK_SPEED_DIV64					5
#define SPI_SCLK_SPEED_DIV128					6
#define SPI_SCLK_SPEED_DIV256					7


//@SPI_DFF

#define SPI_DFF8								0
#define SPI_DFF16								1

//@SPI_CPHA

#define SPI_CPHA_LOW							0
#define SPI_CPHA_HIGH							1


//@SPI_CPOL
#define SPI_CPOL_LOW							0
#define SPI_CPOL_HIGH							1


//@SPI_SSM

#define SPI_SSM_DI								0
#define SPI_SSM_EN								1

//SPI RELATED STATUS FLAG DEF

#define SPI_RXNE_FLAG 							(1<<SPI_SR_RXNE)
#define SPI_TXE_FLAG 							(1<<SPI_SR_TXE)
#define SPI_CHSIDE_FLAG 						(1<<SPI_SR_CHSIDE)
#define SPI_UDR_FLAG 							(1<<SPI_SR_UDR)
#define SPI_CRC_ERR_FLAG 						(1<<SPI_SR_CRC_ERR)
#define SPI_MODF_FLAG 							(1<<SPI_SR_MODF)
#define SPI_OBR_FLAG 							(1<<SPI_SR_OVR)
#define SPI_BUSY_FLAG 							(1<<SPI_SR_BSY)
#define SPI_FRE_FLAG 							(1<<SPI_SR_FRE)


/****************************************************************************************************
 * 							APIs supported this by driver
 * 		For more information about the APIs check the function definitions,
 ****************************************************************************************************
 */

/*
 * Peripheral Clock setup
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx ,uint8_t EnorDi);

/*
 * Init and De-init
 */


void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);



/*
 * DATA SEND AND RECEIVE
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);


/*
 * IRQ and ISR handling
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/*
 * other peripheral control apis
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);























#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
