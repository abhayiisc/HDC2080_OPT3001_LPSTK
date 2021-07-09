/*
 * OPT3001_functions.c
 *
 *  Created on: 11-May-2021
 *      Author: abhay
 */

#include "OPT3001_PARAMETERS.h"


I2C_Handle i2cHandle;
I2C_Transaction i2cTransaction_opt3001;


uint8_t i2cAddress;
uint8_t txBuffer[3];
uint8_t txCount;
uint8_t rxBuffer[2];
uint8_t rxCount;




bool OPT3001_transfer()
{
    bool ret;

    /* Before performing any i2c transfer, verify we called init() first*/
    if (i2cHandle == NULL)
    {
        return (false);
    }

    i2cTransaction_opt3001.writeBuf = txBuffer;
    i2cTransaction_opt3001.writeCount = txCount;
    i2cTransaction_opt3001.readBuf = rxBuffer;
    i2cTransaction_opt3001.readCount = rxCount;

    ret = I2C_transfer(i2cHandle, &i2cTransaction_opt3001);



    return (ret);
}


int32_t OPT3001_getRawRegisterValue(Register reg)
{
    txBuffer[0] = reg;
    txCount = 1;
    rxCount = 2;

    /* if transfer fails, return -1*/
    if (!OPT3001_transfer())
    {
        return (-1);
    }
    else
    {
        return ( (rxBuffer[0] << 8) | (rxBuffer[1]) );
    }
}


int32_t OPT3001_getConfiguration()
{
    return (OPT3001_getRawRegisterValue(CONFIGURATION));
}

bool OPT3001_init(I2C_Handle i2cHandle1,  SlaveAddress i2cAddress1)
{

i2cHandle = i2cHandle1;
i2cAddress = i2cAddress1;
i2cTransaction_opt3001.slaveAddress = i2cAddress;


    txCount = 0;
    rxCount = 0;

    /* if getConfiguration returns -1, then we failed to
     * communicate with the OPT3001 sensor. Return false
     * to indicate that initialization failed. */
    if(OPT3001_getConfiguration() == -1)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}





int16_t OPT3001_getDeviceID()
{
    return (OPT3001_getRawRegisterValue(DEVICE_ID));
}



bool OPT3001_setRawRegisterValue(Register reg, uint16_t data)
{
    txBuffer[0] = reg;
    txBuffer[1] = (uint8_t)((data & 0xFF00) >> 8);
    txBuffer[2] = (uint8_t)(data & 0x00FF);
    txCount = 3;
    rxCount = 0;

    return (OPT3001_transfer());
}

bool OPT3001_setConfiguration(uint16_t data)
{
    return (OPT3001_setRawRegisterValue(CONFIGURATION, data));
}



bool OPT3001_resetConfiguration()
{
    return (OPT3001_setConfiguration(DEFAULT_CONFIGURATION));
}

int32_t OPT3001_calculateLux(int32_t reg_value)
{
    /* Return -1 if I2C transfer failed */
    if(reg_value == -1)
    {
        return (-1);
    }

    /* Calculate lux = (2^exp) * mantissa / 100 */
    uint32_t exp = ((reg_value & EXP_MASK) >> 12);
    uint32_t mantissa = (reg_value & MANTISSA_MASK);

    uint32_t twoToTheExp = (1 << exp);

    return ((twoToTheExp * mantissa) / 100);
}





int32_t OPT3001_getHighLimit()
{
    return (OPT3001_calculateLux(OPT3001_getRawRegisterValue(HIGH_LIMIT)));
}

/*
 *  ======== getLowLimit ========
 */
int32_t OPT3001_getLowLimit()
{
    return (OPT3001_calculateLux(OPT3001_getRawRegisterValue(LOW_LIMIT)));
}

/*
 *  ======== getManufacturerID ========
 */
int16_t OPT3001_getManufacturerID()
{
    return (OPT3001_getRawRegisterValue(MANUFACTURER_ID));
}

uint16_t OPT3001_parseLux(uint32_t lux)
{
    /* convert lux to centi-lux to avoid floating point math */
    lux = lux * 100;

    uint16_t mantissa;  /* Bits[11:00] */
    uint16_t exp;       /* Bits[15:12] */

    for (exp = 0; exp <= (RANGE11 >> 12); exp++)
    {
        if (lux < (unsigned)(RANGE_BASE * (2 << exp)) / 2)
        {
            break;
        }
    }

    mantissa = (int) (lux / ((LUX_BASE_RES * (2 << exp)) / 2));
    exp = exp << 12;

    return (exp | mantissa);
}


bool OPT3001_setHighLimit(uint32_t lux)
{
    uint16_t limit = OPT3001_parseLux(lux);

    /* Verify the value is within min/max range*/
    if(limit <= MIN_LIMIT)
    {
        limit = MIN_LIMIT;
    }
    else if (limit >= MAX_LIMIT)
    {
        limit = MAX_LIMIT;
    }

    return (OPT3001_setRawRegisterValue(HIGH_LIMIT, limit));
}

/*
 *  ======== setLowLimit ========
 */
bool OPT3001_setLowLimit(uint32_t lux)
{

    uint16_t limit = OPT3001_parseLux(lux);

    /* Verify the value is within min/max range*/
    if(limit <= MIN_LIMIT)
    {
        limit = MIN_LIMIT;
    }
    else if (limit >= MAX_LIMIT)
    {
        limit = MAX_LIMIT;
    }

    return (OPT3001_setRawRegisterValue(LOW_LIMIT, limit));
}


bool OPT3001_setConversionMode(ConversionMode cmode)
{
    /* Write conversion mode to configuration register without
     * modifying other bits */
    uint16_t output;

    /* Clear conversion mode bits */
    output = OPT3001_getConfiguration() & ~CONVERSION_MODE_MASK;

    /* Write new conversion mode bits */
    output |= (cmode << 9);

    return (OPT3001_setConfiguration(output));
}

int32_t OPT3001_getResult()
{
    return (OPT3001_calculateLux(OPT3001_getRawRegisterValue(RESULT)));
}


bool OPT3001_resetHighLimit()
{
    return (OPT3001_setRawRegisterValue(HIGH_LIMIT, MAX_LIMIT));
}

/*
 *  ======== resetLowLimit ========
 */
bool OPT3001_resetLowLimit()
{
    return (OPT3001_setRawRegisterValue(LOW_LIMIT, MIN_LIMIT));
}


int light_sensor_Value()
{
    uint16_t sample;
    int32_t luxValue;
       for (sample = 1; sample <= NUM_SAMPLES; sample++) {


           sleep(1);
           luxValue = OPT3001_getResult();


           if(luxValue != -1)
           {

           }
       }

     //  OPT3001_setConversionMode(SHUTDOWN);
     ///  OPT3001_resetLowLimit();
     //  OPT3001_resetHighLimit();


           //  I2C_close(i2c);

             return(luxValue);
}




