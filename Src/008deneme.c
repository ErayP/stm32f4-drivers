/*
 * 008spi_cmd_handling.c
 *
 *  Created on: 10 Oca 2026
 *      Author: polat
 */
#include "stm32f407xx.h"
#include <string.h>




#define COMMAND_LED_CTRL					0x50
#define COMMAND_SENSOR_READ					0x51
#define COMMAND_LED_READ					0x52
#define COMMAND_PRINT						0x53
#define COMMAND_ID_READ						0x54



#define LED_ON								1
#define LED_OFF								0

// arduino analogs pins

#define ANALOG_PIN0							0
#define ANALOG_PIN1							1
#define ANALOG_PIN2							2
#define ANALOG_PIN3							3
#define ANALOG_PIN4							4

#define ACK									1
#define NACK								0


// ARDUİNO LED

#define LED_PIN								9
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
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t response){
	if(response == 0xF5) return ACK;
	return NACK;

}
int main(){



	GPIO_ButtonInit();

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1){

		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		SPI_PeripheralControl(SPI2,ENABLE);

		//1. CMD_LED_CTRL 	<PİN NO(1)>		<VALUE>
		uint8_t commndcode = COMMAND_LED_CTRL;

		SPI_SendData(SPI2, &commndcode,1);

		//send some dummy bits (1byte) spı iletişiminde slave clock üretemediği için veri göndermeden slave mastare ack/nack sını gönderemez
		//eğer 16 bit iletişim kullanıyorsan 2 byte göndermelisin.
		uint8_t dummy_write = 0xff;
		uint8_t dummy_read;

		//şimdi her bir veri göndermeden sonra boş bir geri dönüş alırız rxne yi boşaltmamız gerekir bu yüzden boş bir okuma yapıyoruz
		SPI_ReceiveData(SPI2, &dummy_read,1);

		//ack mesajını alabilmemiz için clock üretmemiz lazum bu yüzden clock üretiroyuz
		SPI_SendData(SPI2,&dummy_write,1);

		//clock ürettikten sonra dönüş alıcaz o yüzden dönüşü alıyoruz
		uint8_t ackbyte;
		SPI_ReceiveData(SPI2,&ackbyte,1);

		//şimdi gelen mesajı karşılaştıralım.

		if(SPI_VerifyResponse(ackbyte) == ACK)
		{
			//send arg
			uint8_t args[2];
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, sizeof(args));
			SPI_ReceiveData(SPI2, args,2);
		}

		//2. CMD_SENSOR_READ <ANALOG PİN NUMBER>

		while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

		delay();

		commndcode = COMMAND_SENSOR_READ;

		SPI_SendData(SPI2, &commndcode, sizeof(commndcode));

		SPI_ReceiveData(SPI2, &dummy_read,1);

		SPI_SendData(SPI2,&dummy_write, 1);

		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte) == ACK)
		{
			uint8_t arg;
			arg = ANALOG_PIN0;
			SPI_SendData(SPI2,&arg,1);
			// sensor read olduğu için bir geri dönüş olacak bu dönüşü alıyoruz.

			SPI_ReceiveData(SPI2, &dummy_read,1);

			// burda ANALOG 0 PİNİNİN DEĞERİNE BAKICAK VE BİRAZ ZAMAN GEÇİRİCEK O YÜZDEN DELAY İLE O
			//ZAMANI VERİYORUZ VERMEZSEK HER ZAMAN 0 OKUR OKUMA YAPMADAN ÖNCEKİ DEĞERİ 0DIR
			delay();

			SPI_SendData(SPI2,&dummy_write, 1);

			uint8_t analogread;

			SPI_ReceiveData(SPI2, &analogread,1);
			//============================
			if(analogread > 0){
				while(!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

				delay();

				commndcode = COMMAND_LED_CTRL;

				SPI_SendData(SPI2, &commndcode, sizeof(commndcode));

				SPI_ReceiveData(SPI2, &dummy_read,1);

				SPI_SendData(SPI2,&dummy_write, 1);

				SPI_ReceiveData(SPI2, &ackbyte, 1);

				if(SPI_VerifyResponse(ackbyte) == ACK)
				{
					//send arg
					uint8_t args[2];
					args[0] = LED_PIN;
					args[1] = LED_OFF;
					SPI_SendData(SPI2, args, sizeof(args));
					SPI_ReceiveData(SPI2, args,2);
				}
			}

			//============================
		}

		while(SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI2,DISABLE);
	}

	return 0;
}

