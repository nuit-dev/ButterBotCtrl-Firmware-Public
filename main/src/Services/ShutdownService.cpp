#include "ShutdownService.h"
#include "Pins.hpp"
#include "Enums.hpp"
#include "Drivers/Output/OutputGPIO.h"
#include "Services/LED/LED.h"

SemaphoreHandle_t ShutdownService::shutdownSem = xSemaphoreCreateBinary();

ShutdownService::ShutdownService(bool internalStack) : Super(CheckInterval, 3 * 1024, 10, -1, internalStack){
	inactivitySem = xSemaphoreCreateBinary();
	xSemaphoreGive(inactivitySem);

	xSemaphoreGive(shutdownSem);

	settings = getApp()->getService<Settings>();

	const auto buttonInput = getApp()->getService<ButtonInput>();
	buttonInput->OnButtonEvent.bind(this, &ShutdownService::inputEvent);
}

void ShutdownService::shutdown(ShutdownReason reason){
	xSemaphoreTake(shutdownSem, portMAX_DELAY);
	const auto app = getApp();

	const auto leds = app->getService<LED<LEDs, RGB_LEDs>>();

	//Fade out - LEDFadeFunction couldn't be used since it provides periodic breathing effect
	for(int8_t i = 100; i >= 0; i--){
		leds->on(LEDs::TftBacklight, (float) i / 100.0f);
		delayMillis(1);
	}

	for(int i = 0; i < (uint8_t) LEDs::COUNT; i++){
		leds->off((LEDs) i);
	}

	if(GPIOPeriph* gpio = app->getPeriphery<GPIOPeriph>()) {
		// Turn off backlight GPIO
		gpio->setMode(static_cast<gpio_num_t>(TFT_BL), GPIOMode::Output);
		gpio->write(static_cast<gpio_num_t>(TFT_BL), true);
		gpio_hold_en(static_cast<gpio_num_t>(TFT_BL));

		// Turn off all LED GPIO
		for(uint8_t led : {LED_WIFI, LED_POWER, LED_BATTLOW, LED_BIG}){
			gpio->setMode(static_cast<gpio_num_t>(led), GPIOMode::Output);
			gpio->write(static_cast<gpio_num_t>(led), false);
			gpio_hold_en(static_cast<gpio_num_t>(led));
		}
	}

	gpio_deep_sleep_hold_en();

	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RC_FAST, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_CPU, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL));
	esp_deep_sleep_start();
}

void ShutdownService::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(xSemaphoreTake(inactivitySem, 0) == pdTRUE){
		//Semaphore given by the inputEvent callback since the last tick.
		inactivityTime = 0.0f;
	}else{
		inactivityTime += deltaTime;
	}

	if(settings == nullptr) return;

	const InactivityTimeout timeout = settings->get().inactivityTimeout;
	if(timeout == InactivityTimeout::Off) return;

	if(inactivityTime >= (float) ((uint32_t) timeout * 60)){
		shutdown(ShutdownReason::Inactivity);
	}
}

void ShutdownService::inputEvent(Enum<int> btn, ButtonInput::Action action){
	xSemaphoreGive(inactivitySem);
}