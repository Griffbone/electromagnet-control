#include "bbpwm.h"

bbpwm_err_t bbpwm_init(bbpwm_device_t *device) {
	device->last_update = device->htim->Instance->CNT;
	device->state = false;

	return BBPWM_ERR_OK;
}

bbpwm_err_t bbpwm_update(bbpwm_device_t *device) {
	// Get timer counter
	uint16_t count = device->htim->Instance->CNT;
	
	// If we are at 100% or 0% duty cycle, don't do anything unless the current state is wrong
	if (device->toff == 0 || device->ton == 0) {
		// If we are off and should be on, turn on
		if (!device->state && device->toff == 0) {
			bbpwm_set_high(device);
			device->state = false;
		}
		// If we are on and should be off, turn off
		else if (device->state && device->ton == 0) {
			bbpwm_set_high(device);
			device->state = true;
		}
	}
	// If we are on and past ton, turn off
	if (device->state && ((uint16_t)(count - device->last_update) >= device->ton)) {
		bbpwm_set_low(device);
		device->state = !device->state;
		device->last_update = count;
	}
	// If we are off and past toff, turn on
	else if (!(device->state) && ((uint16_t)(count - device->last_update) >= device->toff)) {
		bbpwm_set_high(device);
		device->state = !device->state;
		device->last_update = count;
	}

	return BBPWM_ERR_OK;																				
}

bbpwm_err_t bbpwm_set_dc(bbpwm_device_t *device, uint8_t dc) {
	// Calculate ton and toff
	uint16_t ton = 0;
	uint16_t toff = 0;																																											

	ton = ((uint16_t)dc*COUNTS_PER_CYCLE)/255;
	toff = (uint16_t)(COUNTS_PER_CYCLE - ton);
	
	// Set ton and toff into device structure
	device->ton = ton;
	device->toff = toff;

	// Return error code
	return BBPWM_ERR_OK;
}

bbpwm_err_t bbpwm_set_low(bbpwm_device_t *device) {
	device->gpio_port->ODR ^= device->gpio_pin;
	// HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, GPIO_PIN_RESET);

	return BBPWM_ERR_OK;
}

bbpwm_err_t bbpwm_set_high(bbpwm_device_t *device) {
	device->gpio_port->ODR |= device->gpio_pin;
	// HAL_GPIO_WritePin(device->gpio_port, device->gpio_pin, GPIO_PIN_SET);
	
	return BBPWM_ERR_OK;
}