/* 8-bit bit-bang PWM driver using hardware timers
 * Author: Griffin Jourda 6/04/2025
 */

#ifndef __BBPWM_H__
#define __BBPWM_H__

#include "stm32c0xx_hal.h"
#include "low_level/type_definitions.h"

/* PWM driver constants */
#define COUNTS_PER_CYCLE 480	// Number of hardware timer counts in one PWM cycle (timer frequency/desired PWM frequency)

/* Device structure typedef */
typedef struct {
	// Pin state (true = HIGH, false = LOW)
	bool state;

	// PWM timing variables
	uint16_t ton;
	uint16_t toff;
	uint16_t last_update;

	// Hardware pin variables
	GPIO_TypeDef *gpio_port;
	uint16_t gpio_pin;

	// Hardware timer channel
	TIM_HandleTypeDef *htim;
} bbpwm_device_t;

/* Error enum typedef */
typedef enum {
	BBPWM_ERR_OK = 0,
	BBPWM_ERR_HAL,
	BBPWM_ERR_TIMER_NOINIT
} bbpwm_err_t;

bbpwm_err_t bbpwm_init(bbpwm_device_t *device);

/** @brief Function to update the software PWM pin
 *
 * This function updates the status of a bit-banged PWM pin (turns it on or off)
 * by comparing the linked hardware timer to the current state of the pin and the
 * last transition of the pin.
 * @param device pointer to a bbpwm_device_t structure
 * @return PWM error code
 */
bbpwm_err_t bbpwm_update(bbpwm_device_t *device);

/** @brief Function update the duty cycle of a software PWM pin
 *
 * This function computes and sets the new on and off times (ton and toff)
 * of the PWM pin based on an input duty cycle. This information is then
 * stored in the device structure for the update function to reference.
 * @param device pointer to a bbpwm_device_t structure
 * @param dc commanded duty cycle
 * @return PWM error code
 */
bbpwm_err_t bbpwm_set_dc(bbpwm_device_t *device, uint8_t dc);

bbpwm_err_t bbpwm_set_low(bbpwm_device_t *device);

bbpwm_err_t bbpwm_set_high(bbpwm_device_t *device);

#endif /* __BBPWM_H__ */
