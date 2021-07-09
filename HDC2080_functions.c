

#include "HDC2080_PARAMETERS.h"

I2C_Handle i2cHandle;
I2C_Transaction i2cTransaction_hdc2080;


uint8_t i2cAddress;
uint8_t txBuffer[3];
uint8_t txCount;
uint8_t rxBuffer[2];
uint8_t rxCount;


bool HDC2080_transfer()
{
    bool ret;

    /* Before performing any i2c transfer, verify we called init() first*/
    if (i2cHandle == NULL)
    {
        return (false);
    }

    i2cTransaction_hdc2080.writeBuf = txBuffer;
    i2cTransaction_hdc2080.writeCount = txCount;
    i2cTransaction_hdc2080.readBuf = rxBuffer;
    i2cTransaction_hdc2080.readCount = rxCount;

    ret = I2C_transfer(i2cHandle, &i2cTransaction_hdc2080);



    return (ret);
}


uint8_t readReg(uint8_t reg)
{
    txBuffer[0] = reg;
    txCount = 1;
    rxCount = 1;

    /* if transfer fails, return -1*/
    if (!HDC2080_transfer())
    {
        return (0);
    }
    else
    {
        return ( (rxBuffer[0]) );
    }
}

bool writeReg(uint8_t reg, uint8_t data)
{
    txBuffer[0] = reg;
    txBuffer[1] = data;

    txCount = 2;
    rxCount = 0;

    return (HDC2080_transfer());
}




bool HDC2080_init(I2C_Handle i2cHandle1,  uint8_t i2cAddress1)
{

i2cHandle = i2cHandle1;
i2cAddress = i2cAddress1;
i2cTransaction_hdc2080.slaveAddress = i2cAddress;


    txCount = 0;
    rxCount = 0;

    /* if getConfiguration returns -1, then we failed to
     * communicate with the HDC2080 sensor. Return false
     * to indicate that initialization failed. */


        return (true);

}

float HDC2080_readTemp(void)
{
    uint8_t byte[2];
    uint16_t temp;
    byte[0] = readReg(TEMP_LOW);
    byte[1] = readReg(TEMP_HIGH);

    temp = (unsigned int)byte[1] << 8 | byte[0];

    return (float)(temp) * 165 / 65536 - 40;

}

float HDC2080_readHumidity(void)
{
    uint8_t byte[2];
    uint16_t humidity;
    byte[0] = readReg(HUMID_LOW);
    byte[1] = readReg(HUMID_HIGH);

    humidity = (unsigned int)byte[1] << 8 | byte[0];

    return (float)(humidity)/( 65536 )* 100;

}



void HDC2080_setLowTemp(float temp)
{
    uint8_t temp_thresh_low;

    // Verify user is not trying to set value outside bounds
    if (temp < -40)
    {
        temp = -40;
    }
    else if (temp > 125)
    {
        temp = 125;
    }

    // Calculate value to load into register
    temp_thresh_low = (uint8_t)(256 * (temp + 40)/165);

    writeReg(TEMP_THR_L, temp_thresh_low);

}

void HDC2080_setHighTemp(float temp)
{
    uint8_t temp_thresh_high;

    // Verify user is not trying to set value outside bounds
    if (temp < -40)
    {
        temp = -40;
    }
    else if (temp > 125)
    {
        temp = 125;
    }

    // Calculate value to load into register
    temp_thresh_high = (uint8_t)(256 * (temp + 40)/165);

    writeReg(TEMP_THR_H, temp_thresh_high);

}

void HDC2080_setHighHumidity(float humid)
{
    uint8_t humid_thresh;

    // Verify user is not trying to set value outside bounds
    if (humid < 0)
    {
        humid = 0;
    }
    else if (humid > 100)
    {
        humid = 100;
    }

    // Calculate value to load into register
    humid_thresh = (uint8_t)(256 * (humid)/100);

    writeReg(HUMID_THR_H, humid_thresh);

}

void HDC2080_setLowHumidity(float humid)
{
    uint8_t humid_thresh;

    // Verify user is not trying to set value outside bounds
    if (humid < 0)
    {
        humid = 0;
    }
    else if (humid > 100)
    {
        humid = 100;
    }

    // Calculate value to load into register
    humid_thresh = (uint8_t)(256 * (humid)/100);

    writeReg(HUMID_THR_L, humid_thresh);

}

//  Return humidity from the low threshold register
float HDC2080_readLowHumidityThreshold(void)
{
    uint8_t regContents;

    regContents = readReg(HUMID_THR_L);

    return (float)regContents * 100/256;

}

//  Return humidity from the high threshold register
float HDC2080_readHighHumidityThreshold(void)
{
    uint8_t regContents;

    regContents = readReg(HUMID_THR_H);

    return (float)regContents * 100/256;

}

//  Return temperature from the low threshold register
float HDC2080_readLowTempThreshold(void)
{
    uint8_t regContents;

    regContents = readReg(TEMP_THR_L);

    return (float)regContents * 165/256 - 40;

}

//  Return temperature from the high threshold register
float HDC2080_readHighTempThreshold(void)
{
    uint8_t regContents;

    regContents = readReg(TEMP_THR_H);

    return (float)regContents * 165/256 - 40;

}


/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void HDC2080_setTempRes(int resolution)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    switch(resolution)
    {
        case FOURTEEN_BIT:
            configContents = (configContents & 0x3F);
            break;

        case ELEVEN_BIT:
            configContents = (configContents & 0x7F);
            configContents = (configContents | 0x40);
            break;

        case NINE_BIT:
            configContents = (configContents & 0xBF);
            configContents = (configContents | 0x80);
            break;

        default:
            configContents = (configContents & 0x3F);
    }

    writeReg(MEASUREMENT_CONFIG, configContents);

}
/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void HDC2080_setHumidRes(int resolution)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    switch(resolution)
    {
        case FOURTEEN_BIT:
            configContents = (configContents & 0xCF);
            break;

        case ELEVEN_BIT:
            configContents = (configContents & 0xDF);
            configContents = (configContents | 0x10);
            break;

        case NINE_BIT:
            configContents = (configContents & 0xEF);
            configContents = (configContents | 0x20);
            break;

        default:
            configContents = (configContents & 0xCF);
    }

    writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bits 2 and 1 of the MEASUREMENT_CONFIG register controls
    the measurement mode  */
void HDC2080_setMeasurementMode(int mode)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    switch(mode)
    {
        case TEMP_AND_HUMID:
            configContents = (configContents & 0xF9);
            break;

        case TEMP_ONLY:
            configContents = (configContents & 0xFC);
            configContents = (configContents | 0x02);
            break;

        case HUMID_ONLY:
            configContents = (configContents & 0xFD);
            configContents = (configContents | 0x04);
            break;

        default:
            configContents = (configContents & 0xF9);
    }

    writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void HDC2080_triggerMeasurement(void)
{
    uint8_t configContents;
    configContents = readReg(MEASUREMENT_CONFIG);

    configContents = (configContents | 0x01);
    writeReg(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 7 of the CONFIG register can be used to trigger a
    soft reset  */
void HDC2080_reset(void)
{
    uint8_t configContents;
    configContents = readReg(CONFIG);

    configContents = (configContents | 0x80);
    writeReg(CONFIG, configContents);
  //  sleep(1);
}

/*  Bits 6-4  of the CONFIG register controls the measurement
    rate  */
void HDC2080_setRate(int rate)
{
    uint8_t configContents;
    configContents = readReg(CONFIG);

    switch(rate)
    {
        case MANUAL:
            configContents = (configContents & 0x8F);
            break;

        case TWO_MINS:
            configContents = (configContents & 0x9F);
            configContents = (configContents | 0x10);
            break;

        case ONE_MINS:
            configContents = (configContents & 0xAF);
            configContents = (configContents | 0x20);
            break;

        case TEN_SECONDS:
            configContents = (configContents & 0xBF);
            configContents = (configContents | 0x30);
            break;

        case FIVE_SECONDS:
            configContents = (configContents & 0xCF);
            configContents = (configContents | 0x40);
            break;

        case ONE_HZ:
            configContents = (configContents & 0xDF);
            configContents = (configContents | 0x50);
            break;

        case TWO_HZ:
            configContents = (configContents & 0xEF);
            configContents = (configContents | 0x60);
            break;

        case FIVE_HZ:
            configContents = (configContents | 0x70);
            break;

        default:
            configContents = (configContents & 0x8F);
    }

    writeReg(CONFIG, configContents);
}



/*  Bit 0 of the CONFIG register can be used to control the
    the interrupt pin's mode */
