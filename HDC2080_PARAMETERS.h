/*
 * HDC2080_PARAMETERS.h
 *
 *  Created on: 10-May-2021
 *      Author: abhay
 */

#ifndef HDC2080_PARAMETERS_H_
#define HDC2080_PARAMETERS_H_



#endif /* HDC2080_PARAMETERS_H_ */


#include <ti/drivers/I2C.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>



//Define Register Map
    #define TEMP_LOW 0x00
    #define TEMP_HIGH 0x01
    #define HUMID_LOW 0x02
    #define HUMID_HIGH 0x03
    #define INTERRUPT_DRDY 0x04
    #define TEMP_MAX 0x05
    #define HUMID_MAX 0x06
    #define INTERRUPT_CONFIG 0x07
    #define TEMP_OFFSET_ADJUST 0x08
    #define HUM_OFFSET_ADJUST 0x09
    #define TEMP_THR_L 0x0A
    #define TEMP_THR_H 0x0B
    #define HUMID_THR_L 0x0C
    #define HUMID_THR_H 0x0D
    #define CONFIG 0x0E
    #define MEASUREMENT_CONFIG 0x0F
    #define MID_L 0xFC
    #define MID_H 0xFD
    #define DEVICE_ID_L 0xFE
    #define DEVICE_ID_H 0xFF


#define FOURTEEN_BIT 0
#define ELEVEN_BIT 1
#define NINE_BIT 2

#define TEMP_AND_HUMID 0
#define TEMP_ONLY 1
#define HUMID_ONLY 2

#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1

#define LEVEL_MODE 0
#define COMPARATOR_MODE 1

#define MANUAL 0
#define TWO_MINS 1
#define ONE_MINS 2
#define TEN_SECONDS 3
#define FIVE_SECONDS 4
#define ONE_HZ 5
#define TWO_HZ 6
#define FIVE_HZ 7


bool HDC2080_transfer();
uint8_t readReg(uint8_t reg);
bool writeReg(uint8_t reg, uint8_t data);
bool HDC2080_init(I2C_Handle i2cHandle1,  uint8_t i2cAddress1);
float HDC2080_readTemp(void);
float HDC2080_readHumidity(void);
void HDC2080_setLowTemp(float temp);
void HDC2080_setHighTemp(float temp);
void HDC2080_setHighHumidity(float humid);
void HDC2080_setLowHumidity(float humid);
float HDC2080_readLowHumidityThreshold(void);
float HDC2080_readHighHumidityThreshold(void);
float HDC2080_readLowTempThreshold(void);
float HDC2080_readHighTempThreshold(void);
void HDC2080_setTempRes(int resolution);
void HDC2080_setHumidRes(int resolution);
void HDC2080_setMeasurementMode(int mode);
void HDC2080_triggerMeasurement(void);
void HDC2080_reset(void);
void HDC2080_setRate(int rate);
//void HDC2080_i2c_close();

/*
 *



*/
