#include "lis3mdl.h"

lis3mdl_err_t lis3mdl_initialize(lis3mdl_device_t *device) {
  lis3mdl_write_byte(device, LIS3MDL_REG_CTRL3, LIS3MDL_CONTINUOUS_CONVERSION);

  uint8_t ctrl_reg_1 =
      device->temp_enable | device->data_rate | device->self_test;
  lis3mdl_write_byte(device, LIS3MDL_REG_CTRL1, ctrl_reg_1);

  uint8_t ctrl_reg_2 = device->full_scale;
  lis3mdl_write_byte(device, LIS3MDL_REG_CTRL2, ctrl_reg_2);

  return LIS3MDL_ERR_OK;
}

lis3mdl_err_t lis3mdl_get_sensitivity(lis3mdl_device_t *device,
                                      double *sensitivity) {
  switch (device->full_scale) {
  case LIS3MDL_FS_4Gauss:
    *sensitivity = 6842.0f;
    break;
  case LIS3MDL_FS_8Gauss:
    *sensitivity = 3421.0f;
    break;
  case LIS3MDL_FS_12Gauss:
    *sensitivity = 2281.0f;
    break;
  case LIS3MDL_FS_16Gauss:
    *sensitivity = 1711.0f;
    break;
  }

  return LIS3MDL_ERR_OK;
}

lis3mdl_err_t lis3mdl_read_mag(lis3mdl_device_t *device, double *mag_reading) {
  uint8_t mag_read_buf[6];
  double sensitivity = 0;

  lis3mdl_read_block(device, LIS3MDL_REG_OUT_X_L, 6, mag_read_buf);

  int16_t x_reading = (mag_read_buf[1] << 8) | mag_read_buf[0];
  int16_t y_reading = (mag_read_buf[3] << 8) | mag_read_buf[2];
  int16_t z_reading = (mag_read_buf[5] << 8) | mag_read_buf[4];

  lis3mdl_get_sensitivity(device, &sensitivity);

  mag_reading[0] = (double)x_reading / sensitivity;
  mag_reading[1] = (double)y_reading / sensitivity;
  mag_reading[2] = (double)z_reading / sensitivity;

  return LIS3MDL_ERR_OK;
}

lis3mdl_err_t lis3mdl_read_temp(lis3mdl_device_t *device, double *temp) {
  uint8_t temp_read_buf[2];

  lis3mdl_read_block(device, LIS3MDL_REG_TEMP_OUT_L, 2, temp_read_buf);

  int16_t temp_reading = (temp_read_buf[1] << 8) | temp_read_buf[0];
  *temp = (double)temp_reading / 8.0f + 25.0f;

  return LIS3MDL_ERR_OK;
}

__attribute__((weak)) lis3mdl_err_t lis3mdl_write_byte(lis3mdl_device_t *device,
                                                       uint8_t reg,
                                                       uint8_t data) {
  return LIS3MDL_ERR_OK;
}

__attribute__((weak)) lis3mdl_err_t lis3mdl_read_byte(lis3mdl_device_t *device,
                                                      uint8_t reg,
                                                      uint8_t *data) {
  return LIS3MDL_ERR_OK;
}

__attribute__((weak)) lis3mdl_err_t lis3mdl_write_block(
    lis3mdl_device_t *device, uint8_t start_reg, uint8_t bytes, uint8_t *data) {
  return LIS3MDL_ERR_OK;
}

__attribute__((weak)) lis3mdl_err_t lis3mdl_read_block(lis3mdl_device_t *device,
                                                       uint8_t start_reg,
                                                       uint8_t bytes,
                                                       uint8_t *data) {
  return LIS3MDL_ERR_OK;
}