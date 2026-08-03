#include "esp_log.h"
#include <hal/gpio_hal.h>


static const char* TAG = "hook";

/* Function used to tell the linker to include this file
 * with all its symbols.
 */
void bootloader_hooks_include(void){
}


void bootloader_before_init(void) {
	ESP_LOGW(TAG, "Setting CALIB_EN output high");

	const gpio_num_t CALIB_EN = GPIO_NUM_19;
	gpio_ll_output_enable(&GPIO, CALIB_EN);
	gpio_ll_set_level(&GPIO, CALIB_EN, 1);
}

void bootloader_after_init(void) {
}