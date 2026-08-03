#include "LEDController.h"
#include <algorithm>
#include <esp_log.h>

#include "Services/LED/LEDBlinkFunction.h"
#include "Services/LED/LEDFadeFunction.h"
#include "Util/BigGreenLEDBlinkFunction.h"

static const char* TAG = "LEDController";

std::map<LEDs, LEDController::Pattern> LEDController::LedPatternMap = {
	{ LEDs::Power, Pattern::Off },
	{ LEDs::BatteryLow, Pattern::Off },
	{ LEDs::BigGreen, Pattern::Off },
	{ LEDs::Wifi, Pattern::Off },
	{ LEDs::TftBacklight, Pattern::Off },
};

LEDController::LEDController(StrongObjectPtr<LED<LEDs, RGB_LEDs>> ledService) : ledService(std::move(ledService)){
}

bool LEDController::isLedOn(const LEDs led){
	const Pattern pattern = LedPatternMap.at(led);
	return pattern != Pattern::Off;
}

void LEDController::powerLedOn(){
	std::lock_guard lock(stateMutex);
	turnLedOn(LEDs::Power, LedDefaultBrightness);
}

void LEDController::powerLedOff(){
	std::lock_guard lock(stateMutex);
	turnLedOff(LEDs::Power);
}

void LEDController::batteryLowLedOn(){
	std::lock_guard lock(stateMutex);
	turnLedOn(LEDs::BatteryLow, LedDefaultBrightness);
}

void LEDController::batteryLowLedOff(){
	std::lock_guard lock(stateMutex);
	turnLedOff(LEDs::BatteryLow);
}

void LEDController::batteryLowLedBreathe(){
	std::lock_guard lock(stateMutex);
	applyBreathe(LEDs::BatteryLow, LedDefaultBrightness, BreatheDefaultPeriod);
}

void LEDController::batteryLowLedStrobe(){
	std::lock_guard lock(stateMutex);
	applyStrobe(LEDs::BatteryLow, LedDefaultBrightness, BatteryLowStrobePeriod, BatteryLowStrobeTime);
}

void LEDController::wifiLedOn(){
	std::lock_guard lock(stateMutex);
	turnLedOn(LEDs::Wifi, LedDefaultBrightness);
}

void LEDController::wifiLedOff(){
	std::lock_guard lock(stateMutex);
	turnLedOff(LEDs::Wifi);
}

void LEDController::wifiLedStrobe(){
	std::lock_guard lock(stateMutex);
	applyStrobe(LEDs::Wifi, LedDefaultBrightness, WifiStrobePeriod, WifiStrobeTime);
}

void LEDController::bigGreenLedOn(){
	std::lock_guard lock(stateMutex);
	turnLedOn(LEDs::BigGreen, BigGreenBrightness);
}

void LEDController::bigGreenLedOff(){
	std::lock_guard lock(stateMutex);
	turnLedOff(LEDs::BigGreen);
}

void LEDController::bigGreenLedStrobe(){
	std::lock_guard lock(stateMutex);

	// Use custom LEDBlinkFunction
	LedPatternMap.at(LEDs::BigGreen) = Pattern::Strobe;
	if(!ledService){
		ESP_LOGW(TAG, "applyStrobe: ledService is null!");
		return;
	}
	auto func = std::make_unique<BigGreenLEDBlinkFunction>();
	ledService->set(LEDs::BigGreen, std::move(func));
}

void LEDController::bigGreenLedStrobeData(const uint32_t blinkCount){
	std::lock_guard lock(stateMutex);
	applyStrobe(LEDs::BigGreen, BigGreenBrightness, BigGreenDataStrobePeriod, BigGreenDataStrobeTime, blinkCount);
}

void LEDController::turnLedOn(const LEDs led, const float ledBrightness) const{
	LedPatternMap.at(led) = Pattern::On;
	if(!ledService){
		ESP_LOGW(TAG, "turnLedOn: ledService is null!");
		return;
	}
	ledService->on(led, ledBrightness);
}

void LEDController::turnLedOff(const LEDs led) const{
	LedPatternMap.at(led) = Pattern::Off;
	if(!ledService){
		ESP_LOGW(TAG, "turnLedOff: ledService is null!");
		return;
	}
	ledService->off(led);
}

void LEDController::applyStrobe(const LEDs led, float ledBrightness, float strobeDefaultPeriod, float strobeOnTime, uint32_t blinkCount){
	LedPatternMap.at(led) = Pattern::Strobe;
	if(!ledService){
		ESP_LOGW(TAG, "applyStrobe: ledService is null!");
		return;
	}
	auto func = std::make_unique<LEDBlinkFunction<LEDs, float>>(ledBrightness, strobeDefaultPeriod, strobeOnTime, blinkCount);
	ledService->set(led, std::move(func));
}

void LEDController::applyBreathe(const LEDs led, float ledBrightness, float breathePeriod){
	LedPatternMap.at(led) = Pattern::Breathe;
	if(!ledService){
		ESP_LOGW(TAG, "applyBreathe: ledService is null!");
		return;
	}
	auto func = std::make_unique<LEDFadeFunction<LEDs, float>>(0.0f, ledBrightness, breathePeriod, 0u);
	ledService->set(led, std::move(func));
}
