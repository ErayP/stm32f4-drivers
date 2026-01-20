/*
 * 007spi_txonly_arduino.c
 *
 *  Created on: 7 Oca 2026
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
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){


	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MOD_MASTER;
	SPI2Handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; //2mhz clock istediğimiz için 8'e bölüyoruz
	SPI2Handle.SPI_PinConfig.SPI_DFF = SPI_DFF8;
	SPI2Handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPI_PinConfig.SPI_SSM = SPI_SSM_DI;// bir başka cihaza bağlıcamız için yazılım yönetimini dısable ediyoruz

	SPI_Init(&SPI2Handle);

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

void delay(void){
	for(uint32_t i= 0;i<250000;i++);
}
int main(){

	char user_data[] = "Hello selamin aleykum hayirli isler antep kizlari nazli cilveler.";

	GPIO_ButtonInit();

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1){

		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		SPI_PeripheralControl(SPI2,ENABLE);

		//============================================
		uint8_t dataLen = strlen(user_data);

		SPI_SendData(SPI2, &dataLen,1);
		//============================================
		/*
		 * 	Uzunluk göndermek EN MANTIKLISI.
		 *	Göndermezsen çalışır gibi görünür ama zamanla sorun çıkarır.
		 */

		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		//bu altaki kod olamsaydı iletişimi direk kesicekti buda probleme sorun açabilir
		//örneğin veri tam aktarılmadan kesilmesi gibi bu yüzden busy bayrağını kullanıyoruz
		//bu bayrak 0 ise meşgul değil demek yani sıkıntı yok
		//bu bayrak 1 ise meşgul demek yani sıkıntı var bu yüzden 0 olmasını beklemeliyiz.
		while(SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI2,DISABLE);
	}

	return 0;
}

