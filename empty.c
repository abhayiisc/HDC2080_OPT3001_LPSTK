/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

//* For usleep() */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
 #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
 #include <ti/drivers/UART.h>
/* Driver configuration */
#include "ti_drivers_config.h"
#include "Sensor_Core.h"
#include "OPT3001_PARAMETERS.h"
#include "HDC2080_PARAMETERS.h"
/*
 *  ======== mainThread ========
 */

UART_Handle uart;
UART_Params uartParams;
void uart_start();

void *mainThread(void *arg0)
{float temp_reading ;
float hum_reading ;
    int lux_reading ;
    uart_start();

    I2C_Sensor_Initialization_HDC2080();
    HDC_2080_SET_REG_VALUES();
    HDC2080_triggerMeasurement();
    temp_reading =    HDC2080_readTemp();
    hum_reading =  HDC2080_readHumidity();
    sensor_i2c_close();
    I2C_Sensor_Initialization_OPT3001();
    OPT_3001_SET_REG_VALUES();
    sensor_i2c_close();

    char cmd_status[10];
    memset(cmd_status,0,10);


    while(1)
    {
                I2C_Sensor_Initialization_HDC2080();

                HDC2080_triggerMeasurement();


                temp_reading =    HDC2080_readTemp();
                hum_reading =  HDC2080_readHumidity();

                UART_write(uart,"Temperature of device:",sizeof("Temperature of device:"));
                UART_write(uart,"\t",sizeof("\t"));

                sprintf(cmd_status,"%f",temp_reading);
                UART_write(uart,cmd_status,sizeof(cmd_status));
                memset(cmd_status,0,10);
                UART_write(uart,"\r\n",2);

                UART_write(uart,"Humidity of device:",sizeof("Humidity of device:"));
                UART_write(uart,"\t",sizeof("\t"));

                sprintf(cmd_status,"%f",hum_reading);
                UART_write(uart,cmd_status,sizeof(cmd_status));
                memset(cmd_status,0,10);
                UART_write(uart,"\r\n",2);
                sensor_i2c_close();
       // sleep(2);


                I2C_Sensor_Initialization_OPT3001();


                lux_reading =     light_sensor_Value();


                UART_write(uart,"Light Intensity:",sizeof("Light Intensity:"));
                UART_write(uart,"\t",sizeof("\t"));

                sprintf(cmd_status,"%d",lux_reading);
                UART_write(uart,cmd_status,sizeof(cmd_status));
                memset(cmd_status,0,10);
                UART_write(uart,"\r\n",2);
                sensor_i2c_close();
                //    sleep(1);


         }



}


void uart_start()
{

    UART_init();


    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    if (uart == NULL) {

        while (1);
    }


}

