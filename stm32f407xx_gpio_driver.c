/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: 28 Ara 2025
 *      Author: polat
 */

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx.h"

/***************************************************************************
 * @fn									GPIO_PeriClockControl
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
 * @Note : Verilen GPIO portunun clock’unu açar veya kapatır,
 *
 * STM32’de clock kapalıysa peripheral ölüdür
 *
 *	Register’a yazsan bile hiçbir şey olmaz
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx ,uint8_t EnorDi){

	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
	}


}

/*
 * Inıt and De-Inıt
 */

/***************************************************************************
 * @fn									GPIO_Init
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
 * @Note : Bir GPIO pinini baştan sona konfigüre eder
	Bu fonksiyon:

	Mode

	Speed

	Pull-up / pull-down

	Output type

	Alternate function

	Interrupt ayarları

	hepsini tek merkezde yapar.
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	//1. configure the mode of gpio pin
	uint32_t temp = 0;

	GPIO_PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);



	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_Number));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_Number);//clearing
		pGPIOHandle->pGPIOx->MODER |= temp;// setting

	}
	else
	{
		/*
		 * ınterrupt mode
		 */
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//1. configure the falling trigger selected register.
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
			//2.CLEAR THE CORRESPONDİNG RTSR BIT
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);


		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1. configure the rtsr.
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
			//2.CLEAR THE CORRESPONDİNG FTSR BIT
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//1. configure the rtsr and ftsr.
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
			//2.CLEAR THE CORRESPONDİNG FTSR BIT
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
		}
		//2. configure the gpio port selection syscfg_exticr
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_Number/4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_Number%4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] |= (portcode<< 4*temp2);


		//3. enable the exti interrupt delivery using IMR
		//exti->imr ile exti hattını cpuya açıyorsun yani kesme geldi mi gelmedimi bilgisini cpuya veriyorsun
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_Number);
	}
	temp = 0;

	//2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_Number));
	pGPIOHandle->pGPIOx->OSPEEDR &=~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_Number);//clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;//setting
	temp = 0;
	//3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_Number));
	pGPIOHandle->pGPIOx->PUPDR &=~(0x3<<pGPIOHandle->GPIO_PinConfig.GPIO_Number);//clearing
	pGPIOHandle->pGPIOx->PUPDR |=temp;//setting
	temp = 0;

	// 4.configure the optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_Number));
	pGPIOHandle->pGPIOx->OTYPER &=~(0x1<<pGPIOHandle->GPIO_PinConfig.GPIO_Number);//clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp;//setting
	temp = 0;
	//5. configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		// configure the alternate funciton register
		uint8_t temp1,temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_Number / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_Number % 8;
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] &=~(0xF<< (4 * temp2));//clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= temp;
	}
	temp = 0;
}

/***************************************************************************
 * @fn									GPIO_DeInit
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
 * @Note :GPIO portunu resetler

	Tüm register’lar default haline döner

	Debug sırasında çok faydalıdır
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){

	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
}

/*
 * Data read and write
 */

/***************************************************************************
 * @fn									GPIO_ReadFromInputPin
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
 * @Note :Tek bir pinin lojik seviyesini okur (0 / 1)
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){
	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	return value;
}

/***************************************************************************
 * @fn									GPIO_ReadFromInputPort
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
 * @Note :Tüm portu 16-bit olarak okur
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;

	value = (uint16_t)(pGPIOx->IDR);

	return value;

}
/***************************************************************************
 * @fn									GPIO_WriteToOutputPin
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
 * @Note :Tek bir pinin çıkışını 0 veya 1 yapar
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t Value){

	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

/***************************************************************************
 * @fn									GPIO_WriteToOutputPort
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
 * @Note : Porttaki tüm pinleri tek seferde yazar
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

/***************************************************************************
 * @fn									GPIO_ToggleOutputPin
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
 * @Note :Pin durumunu tersine çevirir (toggle)
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber){
	pGPIOx->ODR ^= (1<<PinNumber);
}

/*
 * IRQ Configuration and ISR handling.
 */

/***************************************************************************
 * @fn									GPIO_IRQInterruptConfig
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
 * @Note :/
 * NVIC üzerinden hangi interrupt aktif olacak belirler

	ISER → enable

	ICER → disable
 *
 */



void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi){

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


/*
 * burdada o kesmenin önceliğini belirliyoruz 16 seviye öncelik var en düşük 0 en yüksek 15
 * mesela exti15_9 hattının önceliği en önemli olsun yani kesme geldiği zaman herşeyi bırak bunu yap olsun
 * exti15_9 hattının ırq numarası kaç 40 önceliği de 0000 yani 0 olcak
 *iprx = 40 /4 = 0
 *iprx_section 40%4 = 0
 *bu registerda low bitleri işe yaramaz ve 8 bit var 4 bit high 4 bit low.
 *uint8_t shift_amount = (8*iPrx_section)+(8-NO_PR_BITS_IMPLEMENTED); burası yukarıda anlattığımın kod hali
 *
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority){

	uint8_t iPrx = IRQNumber/4;
	uint8_t iPrx_section = IRQNumber%4;
	uint8_t shift_amount = (8*iPrx_section)+(8-NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iPrx) |= (Priority<<(shift_amount));

}

/*
 * donanım kesme olduğunda 1 yazıyor eğer bir olduysa biz gerekli işlemlerden sonra 1 yazarak resetliyoruz
 *
 * Interrupt geldikten sonra pending bit’i temizler

Temizlemezsen:

Interrupt tekrar tekrar gelir

Sonsuz döngü olur
 */

void GPIO_IRQHandling(uint8_t PinNumber){
	//clear the exti pc register corresponding to the pin number
	if(EXTI->PR & (1<<PinNumber))
	{
		EXTI->PR |= (1<<PinNumber);//clear.
	}
}
