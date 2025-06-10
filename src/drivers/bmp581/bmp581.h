/* Driver for BMP581 barometer
    Minimal driver for Bosch BMP581 barometer
    Datasheet: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp581-ds004.pdf
*/

#ifndef __BMP581_H__
#define __BMP581_H__

#include <stdint.h>
#include <stdbool.h>
#include <type_definitions.h>

/* LIS3MDL Register Map */
#define BMP581_REG_CHIP_ID          0x01
#define BMP581_REG_REV_ID           0x02
#define BMP581_REG_CHIP_STATUS      0x11

#define BMP581_REG_TEMP_XLSB        0x1D
#define BMP581_REG_TEMP_LSB         0x1E
#define BMP581_REG_TEMP_MSB         0x1F

#define BMP581_REG_PRESS_XLSB       0x20
#define BMP581_REG_PRESS_LSB        0x21
#define BMP581_REG_PRESS_MSB        0x22

#define BMP581_REG_INT_STATUS       0x27

#define BMP581_REG_STATUS           0x28
#define BMP581_REG_CMD              0x7E

#define BMP581_REG_ODR_CONFIG       0x37
#define BMP581_REG_OSR_CONFIG       0x36
#define BMP581_REG_OOR_CONFIG       0x35

/* REG_ODR_CONFIG configuration Map */
#define BMP581_PWR_STANDBY          0x00
#define BMP581_PWR_NORMAL           0x01
#define BMP581_PWR_FORCED           0x02
#define BMP581_PWR_NONSTOP          0x03

/* BMP581 Command Map */
#define BMP581_COMMAND_SOFT_REST    0xB6

/* Error Masks: Checks sensor error register*/
#define BMP581_FATAL_ERR_MASK       0x01  // Checks sensor failure
#define BMP581_CMD_ERR_MASK         0x02  // Checks command errors

#define BMP581_REG_ERR_REG        0x12  // Critical for detecting sensor faults

/* Device ID */
#define BMP581_ADDR_SDO_HIGH    0x47
#define BMP581_ADDR_SDO_LOW     0x46

/* Plausibility Checks: Checks if sensor readings are physically possible*/
#define BMP581_MAX_DPDT         5000.0f  // Not sure what max pressure change pa/sec would be
#define BMP581_MIN_PRESSURE     0.0f  // Theoretically no pressure at karman line
#define BMP581_MAX_PRESSURE     1000000.0f // Pressure in Pascals at Sea Level

// Power modes + ODR/OSR presets
typedef enum {
    BMP581_MODE_LOW_POWER = 0,    // Low ODR for ground idle
    BMP581_MODE_HIGH_SPEED,   // 100Hz+ for ascent
    BMP581_MODE_ULTRA_PRECISE  // Max OSR for calibration
} bmp581_mode_t;

typedef enum {
    BMP581_ERR_OK,
    BMP581_ERR_HAL,
    BMP581_ERR_GENERAL,
    BMP581_ERR_INVALID_DATA, // Sensor readings don't make sense
    BMP581_ERR_NOT_READY // Sensor isn't ready
}  bmp581_err_t;

typedef struct  {
    bmp581_mode_t mode; // Power mode (datasheet 7.2)
    uint8_t osr_config;  // Oversampling (datasheet 7.3)
    uint8_t odr_config;  // Output Data Rate (datasheet 7.4)
    uint8_t power_mode;  // PWR_NORMAL, PWR_NONSTOP, etc.

    uint8_t i2c_addr;
    void *hi2c;
} bmp581_device_t;

bmp581_err_t bmp581_init(bmp581_device_t *device);
bmp581_err_t bmp581_soft_reset(bmp581_device_t *device);

bmp581_err_t bmp581_read_temp(bmp581_device_t *device, double *temp);
bmp581_err_t bmp581_read_press(bmp581_device_t *device, double *press);

bmp581_err_t bmp581_write_byte(bmp581_device_t *device, uint8_t reg, uint8_t data);
bmp581_err_t bmp581_read_byte(bmp581_device_t *device, uint8_t reg, uint8_t *data);
bmp581_err_t bmp581_read_block(bmp581_device_t *device, uint8_t start_reg, uint8_t bytes, uint8_t *data);

bmp581_err_t bmp581_delay(uint32_t ms);

#endif /* __BMP581_H__*/