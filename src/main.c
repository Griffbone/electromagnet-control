/* SYSTEM IMPORTS*/
#include "low_level/type_definitions.h"

/* HAL IMPORTS*/
#include "system/init.h"
#include "system/gpio.h"
#include "system/tim.h"
#include "system/usart.h"

/* DRIVER IMPORTS*/
#include "drivers/bbpwm/bbpwm.h"

/* ==================== declare up BBPWM structs ==================== */
bbpwm_device_t led_pwm;
bbpwm_device_t pwm2;

int main(void) {
  init();

  // Initialize timer
  HAL_TIM_Base_Start(&htim1);

  /* ==================== Initialize PWM ====================*/
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  TIM1->CCR1 = 30;

  /* ==================== Initialize up BBPWM structs ==================== */
  led_pwm.gpio_pin = LEDPIN_Pin;
  led_pwm.gpio_port = LEDPIN_GPIO_Port;
  led_pwm.htim = &htim1;
  bbpwm_init(&led_pwm);
  bbpwm_set_dc(&led_pwm, 128);

  pwm2.gpio_pin = TESTPIN_Pin;
  pwm2.gpio_port = TESTPIN_GPIO_Port;
  pwm2.htim = &htim1;
  bbpwm_init(&pwm2);
  bbpwm_set_dc(&pwm2, 64);



  /* ==================== infinite while loop structs ==================== */

  while (1) {
    // GPIOA->ODR = (1 << 5);
    // GPIOA->ODR = (0 << 5);
    // GPIOA->ODR = (1 << 21);
    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    // *GPIOA = 0b0000000000000000;
    bbpwm_update(&led_pwm);
    bbpwm_update(&pwm2);
    // printf("%d %d %d\r\n", led_pwm.toff, led_pwm.ton, led_pwm.ton + led_pwm.toff);
    
    // printf("%i %i\r\n", led_pwm.last_update, led_pwm.state);


  }
  return 0;
}

int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}