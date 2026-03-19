/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: 4 Oca 2026
 *      Author: polat
 */
#include "stm32f407xx_spi_driver.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);
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

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    uint32_t temp = 0;

    SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

    // MASTER / SLAVE
    if(pSPIHandle->SPI_PinConfig.SPI_DeviceMode == SPI_DEVICE_MOD_MASTER)
    {
        temp |= (1 << SPI_CR1_MSTR);
    }

    // BUS CONFIG
    if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        temp &= ~(1 << SPI_CR1_BIDI_MODE);
    }
    else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        temp |= (1 << SPI_CR1_BIDI_MODE);
    }
    else if(pSPIHandle->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        temp &= ~(1 << SPI_CR1_BIDI_MODE);
        temp |= (1 << SPI_CR1_RX_ONLY);
    }

    // BAUDRATE
    temp |= pSPIHandle->SPI_PinConfig.SPI_SclkSpeed << SPI_CR1_BR;

    // DFF
    temp |= pSPIHandle->SPI_PinConfig.SPI_DFF << SPI_CR1_DFF;

    // CPOL / CPHA
    temp |= pSPIHandle->SPI_PinConfig.SPI_CPOL << SPI_CR1_CPOL;
    temp |= pSPIHandle->SPI_PinConfig.SPI_CPHA << SPI_CR1_CPHA;

    //  OLMAZSA OLMAZ (MASTER + SOFTWARE NSS) // SSM yani köleyi yazılım ile kontrol etme 1 olursa ssı kısmınıda 1 yapmamız lazım bu stm'nin bir kuralı eğer disable ise dokunmamıza gerek yok.
    if(pSPIHandle->SPI_PinConfig.SPI_SSM == SPI_SSM_EN)
    {
        temp |= (1 << SPI_CR1_SSM);
        temp |= (1 << SPI_CR1_SSI);
    }

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
			Len-=2;
			pTxBuffer+=2;
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


// while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG == FLAG_RESET)); RX buffer doluysa veri geldi boşsa veri gelmedi veri gelince veriyi alıcağımız için böyle
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){

	while(Len>0){

		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG == FLAG_RESET));
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
			//16 bit
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;
			Len-=2;
			pRxBuffer+=2;
		}
		else
		{
			*pRxBuffer = pSPIx->DR ;
			Len--;
			pRxBuffer++;
		}


	}
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


	if(EnorDi == ENABLE){
			if(IRQNumber <= 31){
				//program ISER0 register
				*NVIC_ISER0 |= (1<<IRQNumber);
			}
			else if(IRQNumber>31 && IRQNumber < 64){
				//program ISER1 register
				*NVIC_ISER1 |= (1<<IRQNumber%32);
			}
			else if(IRQNumber>=64 && IRQNumber<96){
				//program ISER2 register
				*NVIC_ISER2 |= (1<<IRQNumber%64);
			}
		}
		else
		{
			if(IRQNumber <= 31){
				//program ICER0 register
				*NVIC_ICER0 |= (1<<IRQNumber);
			}
			else if(IRQNumber>31 && IRQNumber < 64){
				//program ICER1 register
				*NVIC_ICER1 |= (1<<IRQNumber%32);
			}
			else if(IRQNumber>=64 && IRQNumber<96){
				//program ICER2 register
				*NVIC_ICER2 |= (1<<IRQNumber%64);
			}
		}

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

	uint8_t iPrx = IRQNumber/4;
	uint8_t iPrx_section = IRQNumber%4;
	uint8_t shift_amount = (8*iPrx_section)+(8-NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iPrx) |= (Priority<<(shift_amount));

}
/***************************************************************************
 * @fn									SPI_SendDataIT
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

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){


	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX)
	{
		//1. Global değişkenlerde tx bufferın adresini ve uzunluğunu tutucaz
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;
		//2.SPI şu an veri gönderiyor diye işaretle; gönderim bitene kadar başkası araya girip SPI’yi kullanmasın.
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3.SR (status register) içindeki TXE flag set edildiğinde kesme (interrupt) alabilmek için TXEIE kontrol bitini aktif et.

		pSPIHandle->pSPIx->CR2 |=(1 << SPI_CR2_TXEIE);

		//4.Veri iletimi ISR (Interrupt Service Routine) kodu tarafından yapılacak (daha sonra yazılacak).
	}
	return state;
}
/***************************************************************************
 * @fn									SPI_ReceiveDataIT
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
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX)
	{
		//1. Global değişkenlerde tx bufferın adresini ve uzunluğunu tutucaz
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		//2.SPI şu an veri gönderiyor diye işaretle; gönderim bitene kadar başkası araya girip SPI’yi kullanmasın.
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3.SR (status register) içindeki TXE flag set edildiğinde kesme (interrupt) alabilmek için TXEIE kontrol bitini aktif et.

		pSPIHandle->pSPIx->CR2 |=(1 << SPI_CR2_RXNEIE);

		//4.Veri iletimi ISR (Interrupt Service Routine) kodu tarafından yapılacak (daha sonra yazılacak).
	}
	return state;
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
//some helper functions implementations
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){

	if(pSPIHandle->pSPIx->CR1 & (SPI_CR1_DFF))
	{
		//16 bit DFF
		//1.load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen-=2;
		(pSPIHandle->pTxBuffer)+=2;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen-=2;
		pSPIHandle->pTxBuffer++;
	}
	if(pSPIHandle->TxLen == 0){
		//txlen 0 olunca iletişimi kapatmamız gerekiyor.
		//ilk oolarak daha fazla kesme olmaması için txeie bayrağını sıfırlamamız lazım
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallBack(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}

}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
	if(pSPIHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF))
	{
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR ;
		pSPIHandle->RxLen-=2;
		pSPIHandle->pRxBuffer+=2;
	}
	else
	{
		*pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->DR ;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}
	if(pSPIHandle->RxLen == 0)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallBack(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    // 1. OVR flag clear (DR then SR read)
    if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
    {
        (void)pSPIHandle->pSPIx->DR;
        (void)pSPIHandle->pSPIx->SR;
    }

    // 2. Notify application
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);
}



void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
    (void)pSPIx->DR;
    (void)pSPIx->SR;
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

__weak void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle,uint8_t AppEv){
	//bu bir zayıf uygulamadır uygulama bunu gecersiz sayabilir.
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){

	/*
	 * KESMENİN NERDEN GELDİĞİNİ KONTROL ETMEMİZ GEREKİYOR O YÜZDEN TEKER TEKER BAKICAZ
	 */
	uint8_t temp1 , temp2;
	//ilk olarak TXE'ye bakalım

	temp1 = pSPIHandle->pSPIx->SR & (1<<SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1<<SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		spi_txe_interrupt_handle(pSPIHandle);
	}
	//ikinci olarak RXNE'ye bakalım

	temp1 = pSPIHandle->pSPIx->SR & (1<<SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1<<SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		spi_rxne_interrupt_handle(pSPIHandle);
	}

	//SON olarak OVR bakalım

	temp1 = pSPIHandle->pSPIx->SR & (1<<SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1<<SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		spi_ovr_err_interrupt_handle(pSPIHandle);
	}

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



/***************************************************************************
 * @fn									SPI_SSOEConfig
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
 * ŞİMDİ BİR MASTER (STM32) VE BİR SLAVE (ARDUINO) VAR.
 * MASTER'DA NSS, SLAVE'DE SS PİNİ VAR VE BİRBİRLERİNE BAĞLI.
 *
 * NSS = 0 → SLAVE “BEN SENİ DİNLİYORUM”
 * NSS = 1 → SLAVE “BEN SENİ DİNLEMİYORUM”
 *
 * STM32 TARAFINDA SPE (SPI ENABLE) BİTİ VAR.
 * EĞER DONANIMSAL NSS KULLANILIYORSA (SSM = 0, SSOE = 1):
 *
 * SPE = 1 → DONANIM NSS = 0 YAPAR → SLAVE AKTİF
 * SPE = 0 → DONANIM NSS = 1 YAPAR → SLAVE PASİF
 *
 * SSOE = 1 OLMAZSA, DONANIM NSS PININI KONTROL ETMEZ,
 * BU DURUMDA NSS PINI YAZILIM (GPIO) İLE YÖNETİLİR.
 *
 * BİRDEN FAZLA SLAVE VARSA, NSS GENELDE GPIO İLE
 * YAZILIMSAL OLARAK YÖNETİLİR.
 *
 * DETAYLAR İÇİN DATASHEET / REFERENCE MANUAL BAKILABİLİR.
 */

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->CR2 |= (1<<SPI_CR2_SSOE);
	}
	else{
		pSPIx->CR2 &= ~(1<<SPI_CR2_SSOE);
	}
}

/***************************************************************************
 * @fn									SPI_GetFlagStatus
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
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t Flagname){
	if((pSPIx->SR) & Flagname){
		return FLAG_SET;
	}
	return FLAG_RESET;
}
