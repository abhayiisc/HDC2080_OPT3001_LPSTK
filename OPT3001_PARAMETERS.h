/*
 * OPT3001_PARAMETERS.h
 *
 *  Created on: 11-May-2021
 *      Author: abhay
 */

#ifndef OPT3001_PARAMETERS_H_
#define OPT3001_PARAMETERS_H_



#endif /* OPT3001_PARAMETERS_H_ */



#include <ti/drivers/I2C.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>



#define TEMPORARY_LOWLIMIT   30
#define TEMPORARY_HIGHLIMIT  4000

/* Number of samples to read from the sensor */
#define NUM_SAMPLES 1




#define DEFAULT_CONFIGURATION    0xC810

#define FLAG_MASK    0x60

/*
 * A mask of the 16-bit result register where
 * there are only 1's on the exponent bits.
 * The exponent bits are the 4 most significant bits.
 */
#define EXP_MASK    0xF000

/*
 * A mask of the 16-bit result register where
 * there are only 1's on the mantissa bits.
 * The mantissa bits are the 12 least significant bits
 */
#define MANTISSA_MASK    0x0FFF

/*
 * A mask of the 16-bit configuration register where
 * there are only 1's on the bits explaining the conversion mode.
 * The mantissa bits are the 12 least significant bits. (bits 10:9)
 */
#define CONVERSION_MODE_MASK    0x0600

/*
 * A mask of the 16-bit configuration register where
 * there are only 1's on the bits explaining the conversion mode.
 * The mantissa bits are the 12 least significant bits. (bits 10:9)
 */
#define FLAG_LOW_VALUE    0x1

/*
 * A mask of the 16-bit configuration register where
 * there are only 1's on the bits explaining the conversion mode.
 * The mantissa bits are the 12 least significant bits. (bits 10:9)
 */
#define FLAG_HIGH_VALUE    0x2

/* Min/max values of the limit registers as seen in datasheet */
#define MIN_LIMIT    0x0000
#define MAX_LIMIT    0xBFFF

/* Lowest LSB Size in milliLux per LSB */
#define LUX_BASE_RES    1

/* Lowest full-scale range in milliLux */
#define RANGE_BASE    4095

typedef enum
    {
        /*!
         *  Slave Address when the ADDR pin is connected to the GND pin
         *  Hex: 0x44
         */
        ADDRPIN_GND = 0b1000100,

        /*!
         *  Slave Address when the ADDR pin is connected to the VDD pin
         *  Hex: 0x45
         */
        ADDRPIN_VDD = 0b1000101,

        /*!
         *  Slave Address when the ADDR pin is connected to the SDA pin
         *  Hex: 0x46
         */
        ADDRPIN_SDA = 0b1000110,

        /*!
         *  Slave Address when the ADDR pin is connected to the SCL pin
         *  Hex: 0x47
         */
        ADDRPIN_SCL = 0b1000111
    }SlaveAddress ;

    /*!
     * @brief Conversion Modes
     *
     * The values stored in this enums are associated
     * with bits 10:9 in the configuration register.
     */
 typedef enum
    {
        /*! Default mode */
        SHUTDOWN = 0x0,
        /*! Reads one value before returning to shutdown mode */
        SINGLESHOT = 0x1,
        /*! Reads values to the result register continuously */
        CONTINUOUS_CONVERSIONS = 0x2
    }ConversionMode;

    /*!
     * @brief Interrupt Flag Values
     *
     * Used by getFlag() to check if an interrupt was low or high.
     */
    typedef enum
    {
        /*! If InterruptFlag is NONE, then the value was within the limits */
        NONE = -1,
        /*! If InterruptFlag is LOW, then the value was below the low limit */
        LOW = 0,
        /*! If InterruptFlag is HIGH, then the value was above the high limit */
        HIGH = 1
    }InterruptFlag;

    /*!
     * @brief The address of all accessible registers
     *        associated with the OPT3001
     */
    typedef enum
    {
        /*!
         *  RESULT stores result of the most recent light to digital conversion
         *  conversion mode cannot be in "Shutdown (default)" to get data)
         */
        RESULT = 0x00,

        /*!
         *  Controls the major operational modes of the device and the three modes
         *  of conversions: Shutdown, Single-shot, and Continuous conversions
         */
        CONFIGURATION = 0x01,

        /*! The lower comparison limit for the interrupt reporting mechanism */
        LOW_LIMIT = 0x02,

        /*! The upper comparison limit for the interrupt reporting mechanisms */
        HIGH_LIMIT = 0x03,

        /*! Intended to uniquely identify the device (hex: 5549h, ASCII: TI) */
        MANUFACTURER_ID = 0x7E,

        /*! Intended to uniquely identify the device (its value hex: 3001h) */
        DEVICE_ID = 0x7F
    }Register ;

    /*!
     *  @brief    OPT3001 Result Register Range
     *
     *  The range field selects the full-scale lux range of the device.
     *  The result register may contain a value up to that specified
     *  by the range. Greater range constitutes lower resolution while
     *  less range constitutes higher resolution. Range is set as the
     *  first 4 bits of the configuration register
     */
    typedef enum
    {
        RANGE0 = 0x0000,  /*!< Range = 00040.95, lux per LSB = 00.01  */
        RANGE1 = 0x1000,  /*!< Range = 00081.90, lux per LSB = 00.02  */
        RANGE2 = 0x2000,  /*!< Range = 00163.80, lux per LSB = 00.04  */
        RANGE3 = 0x3000,  /*!< Range = 00327.60, lux per LSB = 00.08  */
        RANGE4 = 0x4000,  /*!< Range = 00665.20, lux per LSB = 00.16  */
        RANGE5 = 0x5000,  /*!< Range = 01310.40, lux per LSB = 00.32  */
        RANGE6 = 0x6000,  /*!< Range = 02620.80, lux per LSB = 00.64  */
        RANGE7 = 0x7000,  /*!< Range = 05241.60, lux per LSB = 01.28  */
        RANGE8 = 0x8000,  /*!< Range = 10483.20, lux per LSB = 02.56  */
        RANGE9 = 0x9000,  /*!< Range = 20966.40, lux per LSB = 05.12  */
        RANGE10 = 0xA000, /*!< Range = 41932.80, lux per LSB = 10.24  */
        RANGE11 = 0xB000, /*!< Range = 83865.60, lux per LSB = 20.48  */
        AUTO = 0xC000 /*!< Automatic Scaling */
    }FullRange;

    bool OPT3001_transfer();
    int32_t OPT3001_getRawRegisterValue(Register reg);
    int32_t OPT3001_getConfiguration();
    bool OPT3001_init(I2C_Handle i2cHandle1,  SlaveAddress i2cAddress1);
    int16_t OPT3001_getDeviceID();
    bool OPT3001_setRawRegisterValue(Register reg, uint16_t data);
    bool OPT3001_setConfiguration(uint16_t data);
    bool OPT3001_resetConfiguration();
    int32_t OPT3001_calculateLux(int32_t reg_value);
    int32_t OPT3001_getHighLimit();
    int32_t OPT3001_getLowLimit();
    int16_t OPT3001_getManufacturerID();
    uint16_t OPT3001_parseLux(uint32_t lux);
    bool OPT3001_setHighLimit(uint32_t lux);
    bool OPT3001_setLowLimit(uint32_t lux);
    bool OPT3001_setConversionMode(ConversionMode cmode);
    int32_t OPT3001_getResult();
    bool OPT3001_resetHighLimit();
    bool OPT3001_resetLowLimit();
    int light_sensor_Value();





