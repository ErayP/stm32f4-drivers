
/*
 * This application receives and prints the user message received from the Arduino peripheral in SPI interrupt mode
 * User sends the message through Arduino IDE's serial monitor tool
 * Monitor the message received in the SWV itm data console
 */
/*
 * Note : Follow the instructions to test this code
 * 1. Download this code on to STM32 board , acts as Master
 * 2. Download Slave code (003SPISlaveUartReadOverSPI.ino) on to Arduino board (Slave)
 * 3. Reset both the boards
 * 4. Enable SWV ITM data console to see the message
 * 5. Open Arduino IDE serial monitor tool
 * 6. Type anything and send the message (Make sure that in the serial monitor tool line ending set to carriage return)
 */
#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"


SPI_Handle_t SPI2handle;

#define MAX_LEN 500

char RcvBuff[MAX_LEN];

volatile char ReadByte;


volatile uint8_t rcvStop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
    SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);


	//NSS
	SPIPins.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);


}

void SPI2_Inits(void)
{
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MOD_MASTER;
	SPI2handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPI_PinConfig.SPI_DFF = SPI_DFF8;
	SPI2handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPI_PinConfig.SPI_SSM = SPI_SSM_DI; //Hardware slave management enabled for NSS pin

	SPI_Init(&SPI2handle);
}


/*This function configures the gpio pin over which SPI peripheral issues data available interrupt */
void Slave_GPIO_InterruptPinInit(void)
{
	GPIO_Handle_t spiIntPin;
	memset(&spiIntPin,0,sizeof(spiIntPin));

	//this is led gpio configuration
	spiIntPin.pGPIOx = GPIOD;
	spiIntPin.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_6;
	spiIntPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	spiIntPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	spiIntPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_Init(&spiIntPin);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRIORITY_15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);

}

int main(){


	uint8_t dummyByte = 0xff;

	SPI2_GPIOInits();

	SPI2_Inits();

	Slave_GPIO_InterruptPinInit();

	SPI_SSOEConfig(SPI2,ENABLE);

	SPI_IRQInterruptConfig(IRQ_NO_SPI2,ENABLE);

	while(1)
	{

		rcvStop = 0; //data bitti mi hayır başlamadı bile o yüzden 0
		while(!dataAvailable);

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,DISABLE);
		SPI_PeripheralControl(SPI2,ENABLE);//spı peripheral clock açmamışsın

		while(!rcvStop)
		{
			while(SPI_SendDataIT(&SPI2handle, &dummyByte, 1) == SPI_BUSY_IN_TX);
			while(SPI_ReceiveDataIT(&SPI2handle, &ReadByte, 1) == SPI_BUSY_IN_RX);
		}

		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );
		SPI_PeriClockControl(SPI2,DISABLE);

		dataAvailable = 0;//bittikten sonra data yok bunu unutmuşsun
		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);

	}
	return 0;
}

void EXTI9_5_IRQHandler (void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_6);
	dataAvailable = 1;
}

void SPI2_IRQHandler (void)
{
	SPI_IRQHandling(&SPI2handle);
}

void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle,uint8_t AppEv){

	static uint32_t i = 0;

	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		RcvBuff[i++] = ReadByte;
		if(ReadByte == '\0' || (i == MAX_LEN))
		{
			rcvStop = 1;
			RcvBuff[i-1] = '\0';
			i=0;// i yi sıfırlamayı unutmuşsun veriler aktarıldıktan sonra 0lanması gerekir
		}
	}
}
