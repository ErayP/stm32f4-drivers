#include <stm32f407xx.h>
#include <string.h>

#define HIGH 				1
#define LOW 				0
#define BTN_PRESSED			LOW


void delay()
{
	for(uint32_t i = 0; i<500000/2;i++);
}

int main(){

	GPIO_Handle_t Gpiobtn,GpioLed;


	memset(&GpioLed,0,sizeof(GpioLed));//Tüm değerleri sıfırlar. #include <string.h>
	memset(&Gpiobtn,0,sizeof(Gpiobtn));


	Gpiobtn.pGPIOx = GPIOD;
	Gpiobtn.GPIO_PinConfig.GPIO_Number = GPIO_PIN_NO_5;
	Gpiobtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	Gpiobtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	Gpiobtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&Gpiobtn);


	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_Number=GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GpioLed);



	//IRQ CONFİG
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRIORITY_15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);
	while(1){

	}

	return 0;
}

void EXTI9_5_IRQHandler (void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}

