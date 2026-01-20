/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: 18 Oca 2026
 *      Author: polat
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"



#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68
uint8_t rcv_buf[32];
void delay(void){
	for(uint32_t i= 0;i<250000;i++);
}

I2C_Handle_t I2C1Handle;

//pb6 SCL
//pb9 SDA
void I2C1_GPIOInits(void){


	GPIO_Handle_t I2C1pins;

	I2C1pins.pGPIOx = GPIOB;
	I2C1pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2C1pins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2C1pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	I2C1pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2C1pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	//SCL
	I2C1pins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_6;
	GPIO_Init(&I2C1pins);
	//SDA
	I2C1pins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_9;
	GPIO_Init(&I2C1pins);


}

void I2C1_Inits(void){


	I2C1Handle.pI2Cx = I2C1;

	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);




}

void GPIO_ButtonInit(void){
	GPIO_Handle_t Gpiobtn;

	Gpiobtn.pGPIOx = GPIOA;
	Gpiobtn.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_0;
	Gpiobtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	Gpiobtn.GPIO_PinConfig.GPIO_PinSpeed =GPIO_SPEED_FAST;
	Gpiobtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&Gpiobtn);
}

int main(){

	//ı2c pin inits
	I2C1_GPIOInits();
	//ı2c peripheral config
	I2C1_Inits();
	//gpio button init;
	GPIO_ButtonInit();
	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1,ENABLE);

	//acki enable etmemiz gerekiyor cünkü pe = 0 olduğunda ack donanım tarafından 0lanır ackin enable olamsı icin pe=1 olması gerekiyor
	I2C_ManageAcking(I2C1, ENABLE);
	//send some data the slave
	uint8_t LEN,READ_LEN = 0x51,READ_DATA = 0x52;
	while(1){

		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		I2C_MasterSendData(&I2C1Handle,&READ_LEN,1,SLAVE_ADDR,I2C_ENABLE_SR);

		I2C_MasterReceiveData(&I2C1Handle,&LEN,1, SLAVE_ADDR,I2C_ENABLE_SR);

		if(LEN>0){
			I2C_MasterSendData(&I2C1Handle,&READ_DATA,1,SLAVE_ADDR,I2C_ENABLE_SR);
			I2C_MasterReceiveData(&I2C1Handle,rcv_buf,LEN, SLAVE_ADDR,I2C_DISABLE_SR);

		}
	}


}
