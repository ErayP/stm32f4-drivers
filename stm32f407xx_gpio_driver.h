/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: 28 Ara 2025
 *      Author: polat
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{						//burası gpio registerları içindir.

	uint8_t GPIO_Number;			// REFERANCE	@GPIO_PIN_NUMBLER
	uint8_t GPIO_PinMode;			// REFERANCE 	@GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;			// REFERANCE  	@GPIO_PIN_SPEED
	uint8_t GPIO_PinPuPdControl;	// REFERANCE 	@GPIO_PIN_PUPD_TYPE
	uint8_t GPIO_PinOPType;			// REFERANCE 	@GPIO_PIN_OUTPUT_TYPE
	uint8_t GPIO_PinAltFunMode;


}GPIO_PinConfig_t;
/*
 * This is a handle stucture for a gpio pin
 */

typedef struct{

	//pointer the hold base address of the GPIO peripheral.
	GPIO_RegDef_t *pGPIOx; //The holds the base address of the GPIO port to which the pin belongs.
	GPIO_PinConfig_t GPIO_PinConfig;//bu structa hangi gpio olduğunu seçilir yani port pin config ile register ayarları yapılır.
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBLER
 * GPIO PIN NUMBERS
 */
#define GPIO_PIN_NO_0				0
#define GPIO_PIN_NO_1				1
#define GPIO_PIN_NO_2				2
#define GPIO_PIN_NO_3				3
#define GPIO_PIN_NO_4				4
#define GPIO_PIN_NO_5				5
#define GPIO_PIN_NO_6				6
#define GPIO_PIN_NO_7				7
#define GPIO_PIN_NO_8				8
#define GPIO_PIN_NO_9				9
#define GPIO_PIN_NO_10				10
#define GPIO_PIN_NO_11				11
#define GPIO_PIN_NO_12				12
#define GPIO_PIN_NO_13				13
#define GPIO_PIN_NO_14				14
#define GPIO_PIN_NO_15				15


/*
 * @GPIO_PIN_MODES
 * GPIO PIN POSSIBLE MODES
 */

#define GPIO_MODE_IN 				0
#define GPIO_MODE_OUT 				1
#define GPIO_MODE_ALTFN				2
#define GPIO_MODE_ANALOG 			3

/*
 * bunlar kesme modları bunları reference manuelde 12.3 baslıkta bulabilirsin.
 */

#define GPIO_MODE_IT_FT				4 //dusen kenar
#define GPIO_MODE_IT_RT				5 //yukselen kenar
#define GPIO_MODE_IT_RFT			6 //hem yukselen hem dusen kenar


/*
 *  @GPIO_PIN_OUTPUT_TYPE
 * gpıo pin possible output type
 */

#define GPIO_OP_TYPE_PP				0
#define GPIO_OP_TYPE_OD				1

/*
 * @GPIO_PIN_SPEED
 * gpıo pin SPEED MODE
 */

#define GPIO_SPEED_LOW				0
#define GPIO_SPEED_MEDIUM			1
#define GPIO_SPEED_FAST				2
#define GPIO_SPEED_HIGH				3


/*
 * @GPIO_PIN_PUPD_TYPE
 * gpıo pull up pull down type
 */

#define GPIO_NO_PUPD				0
#define GPIO_PIN_PU					1
#define GPIO_PIN_PD					2





/*
 * Peripheral clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx ,uint8_t EnorDi);

/*
 * Inıt and De-Inıt
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data read and write
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling.
 */

void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
