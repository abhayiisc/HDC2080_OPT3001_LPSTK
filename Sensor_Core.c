/*
 * I2C_functions.c
 *
 *  Created on: 09-May-2021
 *      Author: abhay
 */

#include "Sensor_Core.h"
#include "OPT3001_PARAMETERS.h"
#include "HDC2080_PARAMETERS.h"
/* For usleep() */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
 #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
 #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
//#include <ti/display/Display.h>
/* Driver configuration */
#include "ti_drivers_config.h"

void I2C_Sensor_Initialization_HDC2080()
{

    GPIO_init();
    I2C_init();



    GPIO_setConfig(CONFIG_GPIO_HDC2080_POWER,GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
                   //  sleep(1);
    GPIO_setCallback(CONFIG_GPIO_HDC2080_INTERRUPT, hdc2080LimitInterruptFxn);

      I2C_Params_init(&i2cParams);
      i2cParams.bitRate = I2C_400kHz;
      i2c = I2C_open(CONFIG_I2C_SENSORS, &i2cParams);
      if (i2c == NULL) {

          while (1);
      }
     i2cTransaction.writeBuf = &data;
     i2cTransaction.writeCount = 1;
     i2cTransaction.readBuf = &data;
     i2cTransaction.readCount = 0;
     data = 0;

     i2cTransaction.slaveAddress = HDC_2080_ADDRESS ;

     if(!HDC2080_init(i2c, 0b1000001))
     {

         while (1);
     }
    GPIO_enableInt(CONFIG_GPIO_HDC2080_INTERRUPT);

}


void HDC_2080_SET_REG_VALUES()
{
// Set up the comfort zone
    HDC2080_reset();
    HDC2080_setHighTemp(28);         // High temperature of 28C
    HDC2080_setLowTemp(22);          // Low temperature of 22C
    HDC2080_setHighHumidity(55);     // High humidity of 55%
    HDC2080_setLowHumidity(40);      // Low humidity of 40%

    // Configure Measurements
    HDC2080_setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
    HDC2080_setRate(MANUAL);                     // Set measurement frequency to 1 Hz
    HDC2080_setTempRes(FOURTEEN_BIT);
    HDC2080_setHumidRes(FOURTEEN_BIT);

}



static void hdc2080LimitInterruptFxn(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_GPIO_LED_0);
    /* Post the alarm semaphore to unblock the alarmThread*/

}



void I2C_Sensor_Initialization_OPT3001()
{

    GPIO_init();
    I2C_init();



    GPIO_setConfig(CONFIG_GPIO_OPT3001_POWER,GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
                  //   sleep(1);
    GPIO_setCallback(CONFIG_GPIO_OPT3001_INTERRUPT, opt3001LimitInterruptFxn);

      I2C_Params_init(&i2cParams);
      i2cParams.bitRate = I2C_400kHz;
      i2c = I2C_open(CONFIG_I2C_SENSORS, &i2cParams);
      if (i2c == NULL) {

          while (1);
      }
     i2cTransaction.writeBuf = &data;
     i2cTransaction.writeCount = 1;
     i2cTransaction.readBuf = &data;
     i2cTransaction.readCount = 0;
     data = 0;

     i2cTransaction.slaveAddress = OPT_3001_ADDRESS ;

     if(!OPT3001_init(i2c, ADDRPIN_GND))
     {
         while (1);
     }
    GPIO_enableInt(CONFIG_GPIO_OPT3001_INTERRUPT);

}

void OPT_3001_SET_REG_VALUES()
{
    OPT3001_resetConfiguration();
    OPT3001_setLowLimit(TEMPORARY_LOWLIMIT);
    OPT3001_setHighLimit(TEMPORARY_HIGHLIMIT);
    OPT3001_setConversionMode(CONTINUOUS_CONVERSIONS);


}



static void opt3001LimitInterruptFxn(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */

    /* Post the alarm semaphore to unblock the alarmThread*/

}


void sensor_i2c_close()
{

            I2C_close(i2c);
}

