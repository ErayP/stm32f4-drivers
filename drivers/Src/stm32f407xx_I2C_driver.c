/*
 * stm32f407xx_I2C_driver.c
 *
 *  Created on: 16 Oca 2026
 *      Author: polat
 */

#include "stm32f407xx_I2C_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = {2,4,8,16};
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAdddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAdddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1<<I2C_CR1_START);
}
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAdddr){

	SlaveAdddr = SlaveAdddr << 1;
	SlaveAdddr &= ~(1);
	pI2Cx->DR = SlaveAdddr;
}
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAdddr){

	SlaveAdddr = SlaveAdddr << 1;
	SlaveAdddr |= 1;
	pI2Cx->DR = SlaveAdddr;
}
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx){
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
}

/*
 * Peripheral Clock setup
 */

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx ,uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		if(pI2Cx == I2C1){
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2){
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3){
			I2C3_PCLK_EN();
		}
	}
	else{
		if(pI2Cx == I2C1){
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2){
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3){
			I2C3_PCLK_DI();
		}
	}

}

/*
 * Init and De-init
 */
uint32_t RCC_GelPLLOutputClock(void){
	return 0;
}


uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1,SystemClk;

	uint8_t clksrc,temp,ahbp,apb1;

	clksrc = (RCC->CFGR >> 2) & (0x3);

	if(clksrc == 0){//HSI
		SystemClk = 16000000;
	}
	else if(clksrc == 1){//HSE
		SystemClk = 8000000;
	}
	else if(clksrc == 2){//PLL

		SystemClk = RCC_GelPLLOutputClock();
	}

	temp = ((RCC->CFGR >> 4) & 0xF);

	if(temp < 8){
		ahbp = 1;
	}
	else{
		ahbp = AHB_PreScaler[temp-8];
	}

	temp = ((RCC->CFGR >> 10) & 0x7);

	if(temp < 4){
		apb1 = 1;
	}
	else{
		apb1 = APB1_PreScaler[temp-4];
	}


	pclk1 = (SystemClk/ahbp) /apb1;

	return pclk1;
}

void I2C_Init(I2C_Handle_t *pI2CHandle){
	/* ***************************************************************************************************************
	 * 											SCL'UN HIZINI AYARLAMAK ICIN
	 * ***************************************************************************************************************
	 * I2C CR2 DA FREQ ALANINA I2C NİN BAĞLI OLDUĞU APBx İN HIZINI YAZMAMIZ LAZIM BU BİZDE 16MHZ'DİR YANİ 16 YAZICAZ 01000 YANİ
	 *
	 * ARDINDAN I2C DEKİ CCR DA BULUNAN F/S DEKİ 		F(FAST MODE)(1)				  S(STANDART MODE)(0)
	 *
	 *seçmen gerekiyor.
	 *
	 *I2C DEKİ CCR DA BULUNAN DUTY SECENEGİ VAR BURASI SADECE FAST MODE ICIN KULLANILIYOR 0 FASTMODE2    1 = FASTMODE 16/9
	 *
	 *ARDINDAN I2C CCR DA BULUNAN CCR REGİSTERLARI VAR 0-11BİT
	 *
	 *EGER STANDART MODE SECİLİ İSE
	 *====================
	 *Thigh = CCR * TPCLK1
	 *Tlow = CCR * TPCLK1
	 *====================
	 *THIGH = |''''| CLOCKUN HIGH KISMI
	 *TLOW = _____ CLOCKUN LOW KISMI
	 *
	 *******************************
	 *BURADA CCRI BULUP REGISTERA BASICAN ISTE
	 *******************************
	 *FAST MODE SECİLİ İSE
	 *DUTY = 0:
	 *Thigh = CCR * TPCLK1
	 *Tlow = 2 * CCR * TPCLK1
	 *******************************
	 *If DUTY = 1:
	 *Thigh = 9 * CCR * TPCLK1
	 *Tlow = 16 * CCR * TPCLK1
	 *******************************
	 *
	 *BASIT BİR ORNEK YAPALIM
	 *
	 *FAST MODE SECİLİ
	 *DUTY = 0
	 *
	 *
	 * PCLK1 = 16MHZ = 16x10^6
	 * TPLCK1 = 62,5 nS
	 *TH+TL 200khz istiyoruz = 5mikro saniyedir
	 *
	 *Th+Tl = 3 * ccr *TPCLK1
	 *TH+TL = 3 * CCR * 62,5nS
	 *5x10^-6/(3*62.5x10^-9) = CCR
	 *ccr = 2,66 ~3 olur 3 yazarsın
	 *
	 *
	 */

	uint32_t tempreg = 0;
	//enable the clok

	I2C_PeriClockControl(pI2CHandle->pI2Cx,ENABLE);


	//ack control bit
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
	pI2CHandle->pI2Cx->CR1 = tempreg;


	//configure the FREQ field of cr2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value()/1000000U;//16mhz değil bize 16 kısmı lazım
	pI2CHandle->pI2Cx->CR2 = (tempreg&0x3f);

	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14); // Bit 14 Should always be kept at 1 by software
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//standart mode
		ccr_value = (RCC_GetPCLK1Value() /(2*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);
	}
	else
	{
		//fast mode
		tempreg |= (1<<15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() /(3*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			ccr_value = (RCC_GetPCLK1Value() /(25*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//TRISE Configuration

	uint8_t trise;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//standart mode

		trise = (RCC_GetPCLK1Value()* 1/1000000U) + 1;//1000nano saniye = 1 mikro saniye t = 1/f = 1/1x10^6
	}
	else{
		//fast mode
		trise = (RCC_GetPCLK1Value() * 300 /1000000000U) + 1; //300 nano saniye  300/1x10^9
	}
	pI2CHandle->pI2Cx->TRISE = (trise & 0x3F);//ilk 5 bite yazıcaz o yüzden ilk 5 biti 1 ledik diğerleri 0 andledik sadece ilk 5 biti yazmış olduk.

}



void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if(pI2Cx == I2C1){
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2){
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3){
		I2C3_REG_RESET();
	}
}



/*
 * DATA SEND AND RECEIVE
 */


void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr ){
	//1.generate the start condition

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2 Confirm the start generation is completed by checking the SB flag in the SR1
	// NOTE. Until SB is Cleared SCL will be stretched (pulled to low)
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)));

	//3.Send the address of the slave with r/nw bit set to w(0) (total 8 bits)

	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	//4. Confirm that address phase is completed by checking ADDR flag in the SR1

	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)));

	//5. clear the ADDR flag according to its software sequance
	//Note: Until ADDR is cleared SCL will be streched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);
	//6. Send the data until Len Becomes 0
	while(Len>0){
		while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)));
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		Len--;
		pTxBuffer++;
	}
	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	// Note: TXE = 1, BTF = 1 means that both SR and DR are empty and next transmission shoul begin
	//when BTF=1 SCL will be stretched (pulled to LOW)
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)));
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF)));

	//8. Generate STOP condition and master need not to wait for the completion of stop condition
	//Note : generatin STOP, automatically clears the BTF.
	if(Sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}


void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *rTxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr )
{

	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB)));

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits )
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in the SR1
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)));



	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);


		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//wait until  RXNE becomes 1
		while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE)));

		//generate STOP condition
		if(Sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data in to buffer
		*rTxBuffer = pI2CHandle->pI2Cx->DR;
	}


    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//read the data until Len becomes zero
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			//wait until RXNE becomes 1
			while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE)));

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

				//generate STOP condition
				if(Sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}


			//read the data from data register in to buffer
			*rTxBuffer = pI2CHandle->pI2Cx->DR;
			//increment the buffer address
			rTxBuffer++;

		}

	}

	//re-enable ACKing
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_ENABLE);
	}
}



/*
 * IRQ and ISR handling
 */

void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority)
{
	uint8_t iPrx = IRQNumber/4;
	uint8_t iPrx_section = IRQNumber%4;
	uint8_t shift_amount = (8*iPrx_section)+(8-NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iPrx) |= (Priority<<(shift_amount));
}


/*
 * other peripheral control apis
 */

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		pI2Cx->CR1 |= (1<<I2C_CR1_PE);
	}
	else{
		pI2Cx->CR1 &= ~(1<<I2C_CR1_PE);
	}
}
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t Flagname){
	if((pI2Cx->SR1) & (Flagname)){
		return FLAG_SET;
	}
	return FLAG_RESET;
}
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if(EnorDi == I2C_ACK_ENABLE){
		pI2Cx->CR1 |= (1<<I2C_CR1_ACK);
	}
	else{
		pI2Cx->CR1 &= ~(1<<I2C_CR1_ACK);
	}
}
/*
 * application call back
 */
void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle,uint8_t AppEv);
