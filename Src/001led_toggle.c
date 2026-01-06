/*
 * 001led_toggle.c
 *
 *  Created on: 30 Ara 2025
 *      Author: polat
 */
#include <stm32f407xx.h>
void delay(){
	for(uint32_t i = 0; i<500000 ;i++);
}
int main(void){

	GPIO_Handle_t GpiaoLed;
	GpiaoLed.pGPIOx = GPIOD;
	GpiaoLed.GPIO_PinConfig.GPIO_Number=GPIO_PIN_NO_12;
	GpiaoLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpiaoLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpiaoLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpiaoLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;



	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GpiaoLed);


	while(1){
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_12);

		delay();
	}
	return 0;
}


