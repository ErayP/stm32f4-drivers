/*
 * i2c.h
 *
 *  Created on: 20 Mar 2026
 *      Author: Eray
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f407xx.h"

typedef struct{

	uint32_t I2C_SCLSpeed;						//@I2C_SCLSPEED
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_ACKControl;					//@I2C_ACKCONTROL
	uint16_t I2C_FMDutyCycle;					//I2C_FMDUTYCYCLE

}I2C_Config_t;

typedef struct{

	I2C_Config_t *pI2CCfg;
	I2C_RegDef_t *pI2Cx;

}I2C_Handle_t;

#define I2C_SCL_SPEED_SM				100000
#define I2C_SCL_SPEED_FM4K				400000

#define I2C_ACK_ENABLE					1
#define I2C_ACK_DISABLE					0

#define I2C_FM_DUTY_2					0
#define I2C_FM_DUTY_16_9				1

#define I2C_RS_SET						0
#define I2C_RS_NOSET					1

#define I2C_FLAG_SB									(1<<I2C_SR1_SB)
#define I2C_FLAG_ADDR								(1<<I2C_SR1_ADDR)
#define I2C_FLAG_BTF								(1<<I2C_SR1_BTF)
#define I2C_FLAG_ADD10								(1<<I2C_SR1_ADD10)
#define I2C_FLAG_STOPF								(1<<I2C_SR1_STOPF)
#define I2C_FLAG_RXNE								(1<<I2C_SR1_RXNE)
#define I2C_FLAG_TXE								(1<<I2C_SR1_TXE)
#define I2C_FLAG_BERR								(1<<I2C_SR1_BERR)
#define I2C_FLAG_ARLO								(1<<I2C_SR1_ARLO)
#define I2C_FLAG_AF									(1<<I2C_SR1_AF)
#define I2C_FLAG_OVR								(1<<I2C_SR1_OVR)
#define I2C_FLAG_PEC_ERR							(1<<I2C_SR1_PEC_ERR)
#define I2C_FLAG_TIME_OUT							(1<<I2C_SR1_TIME_OUT)
#define I2C_FLAG_SMB_ALERT							(1<<I2C_SR1_SMB_ALERT)



void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx ,uint8_t EnorDi);


void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr );
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *rTxBuffer , uint32_t Len, uint8_t SlaveAddr,uint8_t Sr );

void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t Priority);

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t Flagname);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);


#endif /* INC_I2C_H_ */
