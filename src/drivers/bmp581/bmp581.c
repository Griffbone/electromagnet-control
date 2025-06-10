#include "bmp581.h"
#include "stm32h7xx_hal.h"

/* Private Constants */
#define PRESSURE_TO_ALTITUDE_CONST 44330.0f  // Barometric pressure to altitude constant (from barometric formula)
#define PRESSURE_EXPONENT 0.1903f            // Exponent for altitude calculation (Also taken from barometric formula)
#define TEMP_DATA_RESOLUTION_FACTOR 65536.0f   // 65526 = 2^16 which will convert raw data to degress celsius when dividing
#define PRESS_DATA_RESOLUTION_FACTOR 64.0f     // Converts raw sensor reading to units of pascal (1/64)
#define VELOCITY_CONVERSION_FACTOR 0.12f     // Converts changes in pressure Pa/s into velocity m/s

bmp581_err_t bmp581_init(bmp581_device_t *device) {

    // Recommended power-up procedure from page 18 of the datasheet
    uint8_t buf;

    // Wait t_powerup
    bmp581_delay(2);

    // Soft reset
    bmp581_soft_reset(device);

    // Check chip ID is not zero
    bmp581_read_byte(device, BMP581_REG_CHIP_ID, &buf);

    if (buf == 0) {
        return BMP581_ERR_GENERAL;
    }
    
    // Check status register
    bmp581_read_byte(device, BMP581_REG_CHIP_STATUS, &buf);
    uint8_t status_nvm_ready = (buf >> 1) && 0x01;
    uint8_t status_nvm_err = (buf >> 2) && 0x01;
    
    if (status_nvm_ready == 0 || status_nvm_err == 1) {
        return BMP581_ERR_GENERAL;
    }

    // Read interrupt status register
    bmp581_read_byte(device, BMP581_REG_INT_STATUS, &buf);
    uint8_t int_status_por = (buf >> 4) && 0x01;

    if (int_status_por != 1) {
        return buf;
    }

    // Enter standby mode 
    bmp581_write_byte(device, BMP581_REG_ODR_CONFIG, BMP581_PWR_STANDBY);
    bmp581_delay(3);

    // Set measurement configuration
    // Enable pressure measurement
    bmp581_write_byte(device, BMP581_REG_OSR_CONFIG, 1 << 6);
    // TODO: SET ODR CONFIGURATION HERE

    // Enter normal mode 
    bmp581_write_byte(device, BMP581_REG_ODR_CONFIG, BMP581_PWR_NORMAL);

    return BMP581_ERR_OK;    
}

bmp581_err_t bmp581_soft_reset(bmp581_device_t *device) {
    // TODO: add error handling
    bmp581_write_byte(device, BMP581_REG_CMD, BMP581_COMMAND_SOFT_REST);
    bmp581_delay(2);

    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_read_temp(bmp581_device_t *device, double *temp) {
    uint8_t buf[3]; 
    int32_t temp_raw; 

    bmp581_read_block(device, BMP581_REG_TEMP_XLSB, 3, buf);

    temp_raw = (uint32_t)((buf[2] << 16) | (buf[1] << 8) | buf[0]);
    *temp = temp_raw/65536.0f;
    
    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_read_press(bmp581_device_t *device, double *press) {
    uint8_t buf[3];
    int32_t press_raw;

    bmp581_read_block(device, BMP581_REG_PRESS_XLSB, 3, buf);

    press_raw = ((buf[2] << 16) | (buf[1] << 8) | buf[0]);
    *press = press_raw/64.0f;

    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_write_byte(bmp581_device_t *device, uint8_t reg, uint8_t data) {
    uint8_t ret;
    uint8_t tx[2];
    tx[0] = reg;
    tx[1] = data;

    ret = HAL_I2C_Master_Transmit(device->hi2c, device->i2c_addr << 1, tx, 2, HAL_MAX_DELAY);

    if (ret != HAL_OK) {
        return BMP581_ERR_HAL;
    }

    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_read_byte(bmp581_device_t *device, uint8_t reg, uint8_t *data) {
    uint8_t rx;
    uint8_t ret;

    ret = HAL_I2C_Master_Transmit(device->hi2c, device->i2c_addr << 1, &reg, 1, HAL_MAX_DELAY);

    if (ret != HAL_OK) {
      return BMP581_ERR_HAL;
    }

    ret = HAL_I2C_Master_Receive(device->hi2c, device->i2c_addr << 1, &rx, 1, HAL_MAX_DELAY);
  
    if (ret != HAL_OK) {
        return BMP581_ERR_HAL;
    }

    *data = rx;

    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_read_block(bmp581_device_t *device, uint8_t start_reg, uint8_t bytes, uint8_t *data) {
    uint8_t ret;

    ret = HAL_I2C_Mem_Read(device->hi2c, device->i2c_addr << 1, start_reg, 1, data, bytes, HAL_MAX_DELAY);

    if (ret != HAL_OK) {
        return BMP581_ERR_HAL;
    }

    return BMP581_ERR_OK;
}

bmp581_err_t bmp581_delay(uint32_t ms) {
    HAL_Delay(ms);

    return BMP581_ERR_OK;
}
/** 
bmp581_err_t bmp581_read_telemetry(bmp581_device_t *dev, bmp581_telemetry_t *tel) {
    
    // Reading in raw data
    if (bmp581_read_press(dev, &tel->pressure) != BMP581_ERR_OK ||
        bmp581_read_temp(dev, &tel->temperature) != BMP581_ERR_OK) {
        return BMP581_ERR_HAL;
    }

    // Calculating altidude using barometric formula and constants above
    tel->altitude = 44330.0f * (1.0f - powf(tel->pressure / 101325.0f, 0.1903f));

    // Calculating vertical velocty from changes in pressure
    uint32_t now = HAL_GetTick();
    if (last_timestamp > 0) {  // Skip first sample
        float dt = (now - last_timestamp) / 1000.0f;
        tel->vertical_vel = 0.12f * ((tel->pressure - last_pressure) / dt);
    }
    last_pressure = tel->pressure;
    last_timestamp = now;

    // Checking for errors with sensor
    return BMP581_ERR_OK;
}

bool bmp581_data_is_plausible(const bmp581_telemetry_t *tel) {
    // Checking pressure readings make sence (essentially checking the altitude as well)
    if (tel->pressure < BMP581_MIN_PRESSURE || 
        tel->pressure > BMP581_MAX_PRESSURE) {
        return false;
    }

    // Temperature range from barometer documentation file (-40°C to +85°C)
    if (tel->temperature < -40.0f || tel->temperature > 85.0f) {
        return false;
    }

    return true;
}

**/