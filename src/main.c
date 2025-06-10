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

int main(void) {
  init();

  // Initialize timer
  HAL_TIM_Base_Start(&htim1);

  /* ==================== initialize up BBPWM structs ==================== */

  /* ==================== infinite while loop structs ==================== */
  while (1) {
    uint16_t count = htim1.Instance->CNT;

    printf("%d\r\n", count);

    HAL_Delay(100);
  }
  return 0;
}

int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}