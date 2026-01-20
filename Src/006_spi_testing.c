/*
 * 006_spi_testing.c
 *
 *  Created on: 4 Oca 2026
 *      Author: polat
 */

#include "stm32f407xx.h"
#include <string.h>
/*
 *
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 -> NSS
 * ALT FUNCTİON MODE : 5
 *
 *
 *
 *
 */


void SPI2_GPIOInits(void){

	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	//SPI2_MISO
	//SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//SPI2_MOSI
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//SPI2_SCLK
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//SPI2_NSS
	//SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){


	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MOD_MASTER;
	SPI2Handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256; //generates sclk of 8 mhz
	SPI2Handle.SPI_PinConfig.SPI_DFF = SPI_DFF8;
	SPI2Handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPI_PinConfig.SPI_SSM = SPI_SSM_EN;// software slave management enabled for enable;

	SPI_Init(&SPI2Handle);

}
int main(){

	char user_data[] = "selamin aleykum hayirli isler antep kizlari nazli cilveler.";

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_PeripheralControl(SPI2,ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	while(SPI_GetFlagStatus(SP2,SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2,DISABLE);

	while(1);

	return 0;
}
