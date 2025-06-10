#include "bbpwm.h"

bbpwm_err_t bbpwm_update(bbpwm_device_t *device) {
	// Get timer counter
	uint16_t count = device->htim->Instance->CNT;

	// If we are on and past ton, turn off
	if (device->state && ((count - device->last_update) > device->ton)) {
		HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, GPIO_PIN_RESET);
		device->last_update = count;
	}
	// If we are off and past toff, turn on
	else if (!(device->state) && ((count - device->last_update) > device->toff)) {
		HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, GPIO_PIN_SET);
		device->last_update = count;
	}

	return BBPWM_ERR_OK;
}

bbpwm_err_t bbpwm_set_dc(bbpwm_device_t *device, uint8_t dc) {
	// Calculate ton and toff
	uint16_t ton = 0;
	uint16_t toff = 0;

	// TODO: figure out the logic for how to calculate this
	// Need to take in actual frequency (from clock speed and prescaler) and convert to a
	// delta-count (amount of ticks on and amount of ticks off)

	// Set ton and toff into device structure
	device->ton = ton;
	device->toff = toff;

	// Return error code
	return BBPWM_ERR_OK;
}
