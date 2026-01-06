/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: 4 Oca 2026
 *      Author: polat
 */
#include "stm32f407xx_spi_driver.h"

/***************************************************************************
 * @fn									SPI_PeriClockControl
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx ,uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1){
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2){
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
	}
	else{
		if(pSPIx == SPI1){
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2){
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3){
			SPI3_PCLK_DI();
		}
	}

}

/***************************************************************************
 * @fn									SPI_Init
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */

void SPI_Init(SPI_Handle_t *pSPIHandle){

//FIRST LETS CONFİGURE THE SPI_CR1 REG


	uint32_t temp = 0;

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
	//1.configure the device mode
	temp |= pSPIHandle->SPI_PinConfig.SPI_DeviceMode << SPI_CR1_BIDI_MODE;
	//2.CONFİGURE THE BUS CONFİG
	if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		//BIDI MODE SHOULD BE CLEARED
		temp &= ~(1<<SPI_CR1_BIDI_MODE);
	}
	else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		//BIDI MODE SHOULD BE SET
		temp |= (1<<SPI_CR1_BIDI_MODE);
	}
	else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		//BIDI MODE SHOULD BE CLEARED AND RXONLY BIT MUST BE SET
		temp &= ~(1<<SPI_CR1_BIDI_MODE);
		temp |= (1<<SPI_CR1_RX_ONLY);
	}
	//3.CONFİGURE THE SPI SERIAL CLOCK SPEED
	temp|= pSPIHandle->SPI_PinConfig.SPI_SclkSpeed << SPI_CR1_BR;
	//4.CONFİGURE THE DFF
	temp|= pSPIHandle->SPI_PinConfig.SPI_DFF << SPI_CR1_DFF;
	//5.CONFİGURE THE BUS CPOL
	temp|= pSPIHandle->SPI_PinConfig.SPI_CPOL << SPI_CR1_CPOL;
	//6.CONFİGURE THE BUS CPHA
	temp|= pSPIHandle->SPI_PinConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = temp;

}

/***************************************************************************
 * @fn									SPI_DeInit
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */

void SPI_DeInit(SPI_RegDef_t *pSPIx){

	if(pSPIx == SPI1){
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2){
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3){
		SPI3_REG_RESET();
	}

}
/***************************************************************************
 * @fn									SPI_SendData
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note		This is blocking call
 */


uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName){
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){

	while(Len>0)
	{
		//1.wait until txe is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);
		//2.check the dff bit in cr1
		if(pSPIx->CR1 & (SPI_CR1_DFF))
		{
			//16 bit DFF
			//1.load the data in to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}
/***************************************************************************
 * @fn									SPI_ReceiveData
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){

}

/***************************************************************************
 * @fn									SPI_IRQInterruptConfig
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi){

}
/***************************************************************************
 * @fn									SPI_IRQPriorityConfig
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority){

}
/***************************************************************************
 * @fn									SPI_IRQHandling
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){

}
/***************************************************************************
 * @fn									SPI_PeripheralControl
 *
 * @brief
 *
 *
 *
 *
 * @param[in]
 * @param[in]
 * @param[in]
 *
 *
 * @return
 *
 * @Note
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR1 |= (1<<SPI_CR1_SPE);
	}
	else{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);
	}
}
