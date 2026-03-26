#include "i2c.h"

uint32_t RCC_GelPLLOutputClock(void){
	return 0;
}

AhbPreScelar[8] = {2,4,8,16,64,128,256,512};
Apb1PreScelar[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void){

	uint32_t pclk1,SystemClk;

	uint8_t clksrc,val,ahb,apb1;

	clksrc = (RCC->CFGR >> 2) & (0x3);

	if(clksrc == 0)//HSI
	{
		SystemClk == 16000000;
	}
	if(clksrc == 1)//HSE
	{
		SystemClk == 16000000;
	}
	if(clksrc == 2)//PLL
	{
		RCC_GelPLLOutputClock();
	}

	val = (RCC->CFGR >> 4) & (0xf);

	if(val<8) {
		ahb = 1;
	}
	else {
		ahb = AhbPreScelar[val-8];
	}

	val = (RCC->CFGR >> 10) & (0x7);

	if(val<4){
		apb1 = 1;
	}
	else{
		apb1 = Apb1PreScelar[val-4];
	}

	pclk1 = (SystemClk/ahb)/apb1;

	return pclk1;

}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx ,uint8_t EnorDi){
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1) I2C1_PCLK_EN();
		if(pI2Cx == I2C2) I2C2_PCLK_EN();
		if(pI2Cx == I2C3) I2C3_PCLK_EN();
	}
	else{
		if(pI2Cx == I2C1) I2C1_PCLK_DI();
		if(pI2Cx == I2C2) I2C2_PCLK_DI();
		if(pI2Cx == I2C3) I2C3_PCLK_DI();
	}
}


void I2C_Init(I2C_Handle_t *pI2CHandle){

	uint32_t tempReg;

	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);

	tempReg |= pI2CHandle->pI2CCfg->I2C_ACKControl << I2C_CR1_ACK;
	pI2CHandle->pI2Cx->CR1 |= tempReg;

	tempReg = 0;
	tempReg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempReg & 0x3F);

	tempReg=0;

	tempReg = pI2CHandle->pI2CCfg->I2C_DeviceAddress << 1;
	tempReg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 |= tempReg;

	uint16_t ccr_value;
	tempReg = 0;

	if(pI2CHandle->pI2CCfg->I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		ccr_value = (RCC_GetPCLK1Value() / (2*pI2CHandle->pI2CCfg->I2C_SCLSpeed));
		tempReg |= (ccr_value & 0xFFF);
	}
	else{
		tempReg |= (1 << 15);
		tempReg |= (pI2CHandle->pI2CCfg->I2C_FMDutyCycle << 14);
		if(pI2CHandle->pI2CCfg->I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / (3*pI2CHandle->pI2CCfg->I2C_SCLSpeed));
		}
		else{
			ccr_value = (RCC_GetPCLK1Value() / (25*pI2CHandle->pI2CCfg->I2C_SCLSpeed));
		}
		tempReg |= (ccr_value & 0xFFF);
	}

	pI2CHandle->pI2Cx->CCR |= tempReg;
	tempReg = 0;

	uint8_t trise;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){

		trise |= (RCC_GelPLLOutputClock() * (1/1000000U)) + 1;
	}
	else{
		trise |= (RCC_GelPLLOutputClock() * (300/1000000000U)) + 1;
	}
	pI2CHandle->pI2Cx->TRISE |= (trise & 0x3F);
}
void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if(pI2Cx == I2C1) I2C1_REG_RESET();
	if(pI2Cx == I2C2) I2C2_REG_RESET();
	if(pI2Cx == I2C3) I2C3_REG_RESET();
}

void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){

	pI2Cx->CR1 |= (1 << I2C_CR1_START);

}
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){

	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);

}
void I2C_ExecuteAddressWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){

	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR |= SlaveAddr;

}
void I2C_ExecuteAddressRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){

	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1;
	pI2Cx->DR |= SlaveAddr;

}
void I2C_ClearADDR(I2C_RegDef_t *pI2Cx){
	uint8_t dummyRead;

	dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr ){


	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)));

	I2C_ExecuteAddressWrite(pI2CHandle->pI2Cx, SlaveAddr);

	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR)));

	I2C_ClearADDR(pI2CHandle->pI2Cx);

	while(Len > 0){
		while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)));
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		Len--;
		pTxBuffer++;
	}
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)));
	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF)));

	if(Sr == I2C_RS_NOSET) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);


}
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr ){

	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)));

	I2C_ExecuteAddressRead(pI2CHandle->pI2Cx, SlaveAddr);

	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR)));

	if(Len == 1){

		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

		I2C_ClearADDR(pI2CHandle->pI2Cx);

		while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE)));

		if(Sr == I2C_RS_NOSET) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	if(Len > 1){

		I2C_ClearADDR(pI2CHandle->pI2Cx);

		for(uint8_t i = Len; i>0;i--){
			while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE)));
			if (i == 2) {

				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}
			*pRxBuffer = pI2CHandle->pI2Cx->DR;
			pRxBuffer++;
		}

	}
	if(Sr == I2C_RS_NOSET) I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi){

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
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority){

	uint8_t iPrx = IRQNumber/4;
	uint8_t iPrx_section = IRQNumber%4;
	uint8_t shift_amount = (8*iPrx_section)+(8-NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iPrx) |= (Priority<<(shift_amount));

}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else{
		pI2Cx->CR1 &= ~(1<< I2C_CR1_PE);
	}
}
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t Flagname){
	if(pI2Cx->SR1 & Flagname) return ENABLE;
	return DISABLE;
}
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE) pI2Cx->CCR |= (1 << I2C_CR1_ACK);
	pI2Cx->CCR &= ~(1 << I2C_CR1_ACK);
}

