/*
 * 003button_led2.c
 *
 *  Created on: 30 Ara 2025
 *      Author: polat
 */


/*
 * 002button_led.c
 *
 *  Created on: 30 Ara 2025
 *      Author: polat
 */

#include <stm32f407xx.h>


#define HIGH 				1
#define LOW 				0
#define BTN_PRESSED			LOW


void delay()
{
	for(uint32_t i = 0; i<500000/2;i++);
}

int main(){

	GPIO_Handle_t Gpiobtn,GpioLed;

	Gpiobtn.pGPIOx = GPIOB;
	Gpiobtn.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_12;
	Gpiobtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	Gpiobtn.GPIO_PinConfig.GPIO_PinSpeed =GPIO_SPEED_FAST;
	Gpiobtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GpioLed.pGPIOx = GPIOE;
	GpioLed.GPIO_PinConfig.GPIO_Number=GPIO_PIN_NO_8;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


	GPIO_PeriClockControl(GPIOE,ENABLE);
	GPIO_PeriClockControl(GPIOB,ENABLE);
	GPIO_Init(&Gpiobtn);
	GPIO_Init(&GpioLed);

	while(1){
		if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_12) == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_8);
		}
	}
	return 0;
}


