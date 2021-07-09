/*
 * Sensor_Core.h
 *
 *  Created on: 09-May-2021
 *      Author: abhay
 */

#ifndef SENSOR_CORE_H_
#define SENSOR_CORE_H_



#endif /* SENSOR_CORE_H_ */

 #include <ti/drivers/I2C.h>
 #include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>

#define HDC_2080_ADDRESS 0x41
#define OPT_3001_ADDRESS 0x44
void uart_start();
void HDC_2080_SET_REG_VALUES();
void I2C_Sensor_Initialization_HDC2080();
static void hdc2080LimitInterruptFxn(uint_least8_t index);

void I2C_Sensor_Initialization_OPT3001();
void OPT_3001_SET_REG_VALUES();
static void opt3001LimitInterruptFxn(uint_least8_t index);
void sensor_i2c_close();
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

uint8_t data;






