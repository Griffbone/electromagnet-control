/* SYSTEM IMPORTS*/
#include "falafel/falafel.h"
#include "low_level/memcpy_s.h"
#include "low_level/type_definitions.h"

/* HAL IMPORTS*/
#include "system/init.h"
#include "system/gpio.h"
#include "system/usart.h"
#include "system/i2c.h"
// #include "system/rtc.h"
#include "system/spi.h"
// #include "system/tim.h"

/* DRIVER IMPORTS*/
#include "drivers/ina260/ina260.h"
#include "drivers/lis3mdl/lis3mdl.h"
#include "drivers/pyro_board/pyro_board.h"
#include "drivers/adis16577/adis16577.h"
#include "drivers/icm45686/icm45686.h"
#include "drivers/bmp581/bmp581.h"

/* ==================== On-Board Hardware Declarations ==================== */
// Define INA260 hardware
ina260_device_t ina_3v3_ch1;
ina260_device_t ina_3v3_ch2;
ina260_device_t ina_3v3_ch3;
ina260_device_t ina_5v;

// Define ICM-45686 hardware
icm45686_device_t icm45686;

// Define LIS3MDL magnetometer hardware
lis3mdl_device_t magnetometer;

// Define ADIS16577 IMU hardware
adis16577_device_t adis16577;

// Define bmp581 hardware
bmp581_device_t bmp581;

/* ==================== Off-Board Hardware Declarations ==================== */
// Define pyro board hardware 
pyro_board_device_t pyro_board;

int main(void) {
  init();

  /* ==================== On-Board Hardware Initializations ==================== */
  // INA260 3V3 CH1 (radio module slot)
  ina_3v3_ch1.average_mode = INA260_AVG_1;
  ina_3v3_ch1.curr_conv_time = INA260_CURR_CONV_TIME_140US;
  ina_3v3_ch1.hi2c = &hi2c1;
  ina_3v3_ch1.i2c_addr = 0x46;
  ina_3v3_ch1.power_mode = INA260_MODE_SHUNT_CONTINUOUS;
  ina_3v3_ch1.volt_conv_time = INA260_VOLT_CONV_TIME_140US;
  ina_3v3_ch1.i2c_timeout = 100;

  // INA260 3V3 CH2 (XBee, pyro module slot)
  ina_3v3_ch2.average_mode = INA260_AVG_1;
  ina_3v3_ch2.curr_conv_time = INA260_CURR_CONV_TIME_140US;
  ina_3v3_ch2.hi2c = &hi2c1;
  ina_3v3_ch2.i2c_addr = 0x4A;
  ina_3v3_ch2.power_mode = INA260_MODE_SHUNT_CONTINUOUS;
  ina_3v3_ch2.volt_conv_time = INA260_VOLT_CONV_TIME_140US;
  ina_3v3_ch2.i2c_timeout = 100;

  // INA260 3V3 CH3 (ADCS board [me when I measure the current consumption of
  // this board, thereby increasing the current consumption of the board :)])
  ina_3v3_ch3.average_mode = INA260_AVG_1;
  ina_3v3_ch3.curr_conv_time = INA260_CURR_CONV_TIME_140US;
  ina_3v3_ch3.hi2c = &hi2c1;
  ina_3v3_ch3.i2c_addr = 0x4E;
  ina_3v3_ch3.power_mode = INA260_MODE_SHUNT_CONTINUOUS;
  ina_3v3_ch3.volt_conv_time = INA260_VOLT_CONV_TIME_140US;
  ina_3v3_ch3.i2c_timeout = 100;

  // INA260 5V (carrier board modules, Pi)
  ina_5v.average_mode = INA260_AVG_1;
  ina_5v.curr_conv_time = INA260_CURR_CONV_TIME_140US;
  ina_5v.hi2c = &hi2c1;
  ina_5v.i2c_addr = 0x45;
  ina_5v.power_mode = INA260_MODE_SHUNT_CONTINUOUS;
  ina_5v.volt_conv_time = INA260_VOLT_CONV_TIME_140US;
  ina_5v.i2c_timeout = 100;

  // ICM-45686 (SDM IMU)
  icm45686.gyro_pwr_mode = ICM45686_GYRO_MODE_LN;
  icm45686.accel_pwr_mode = ICM45686_ACCEL_MODE_LN;
  icm45686.gyro_fs = ICM45686_GYRO_FS_4000DPS;
  icm45686.gyro_odr = ICM45686_GYRO_ODR_3200HZ;
  icm45686.accel_fs = ICM45686_ACCEL_FS_32G;
  icm45686.accel_odr = ICM45686_ACCEL_ODR_3200HZ;

  // LIS3MDL Magnetometer
  magnetometer.cs_gpio_pin = SPI1_CS2_Pin;
  magnetometer.cs_gpio_port = SPI1_CS2_GPIO_Port;
  magnetometer.data_rate = LIS3MDL_LP_1000Hz;
  magnetometer.full_scale = LIS3MDL_FS_4Gauss;
  magnetometer.hspi = &hspi1;
  magnetometer.self_test = LIS3MDL_SELF_TEST_DIS;
  magnetometer.spi_timeout = 100;
  magnetometer.temp_enable = LIS3MDL_TEMP_EN;
  magnetometer.z_axis_mode = LIS3MDL_Z_LP;

  // ADIS16577 IMU 
  adis16577.gyro_fir_cascade = 0b100; // linked to FILT_CTRL (bartlett?)
  adis16577.accel_fir_cascade = 0b100; // linked to FILT_CTRL (bartlett?)
  adis16577.gyro_av_dc = 0; // average and decimating filter, linked to DEC_RATE // ODR
  adis16577.accel_av_dc = 0; // average and decimating filter, linked to DEC_RATE

  // BMP581 Barometer
  bmp581.hi2c = &hi2c1;
  bmp581.i2c_addr = 0x76;
  // None of this actually makes a difference since the config is hardcoded in its init functio
  // TODO: make it matter
  /*
  bmp581.mode = BMP581_MODE_ULTRA_PRECISE;
  bmp581.osr_config = ;
  bmp581.odr_config = BMP581_PWR_NORMAL;
  bmp581.power_mode = 
  */


  /* ==================== Off-Board Hardware Initializations ==================== */
  pyro_board.hi2c = &hi2c1;
  pyro_board.i2c_address = 0x70;
  pyro_board.i2c_timeout_ms = 100;

  while (1) {
  }
  return 0;
}