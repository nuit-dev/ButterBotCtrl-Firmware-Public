#include "BatteryIndicator.h"
#include "Services/LED/LEDBlinkFunction.h"
#include <utility>

#include "LEDController.h"

DEFINE_LOG(BatteryIndicator)

BatteryIndicator::BatteryIndicator(StrongObjectPtr<Battery> battery, StrongObjectPtr<LED<LEDs, RGB_LEDs>> leds) : leds(std::move(leds)){
	if(!battery){
		CMF_LOG(BatteryIndicator, LogLevel::Error, "Battery param is nullptr!");
		return;
	}

	//Called for initial setup
	batteryChangeOccurred(battery->getLevel());

	battery->OnLevelChanged.bind(this, &BatteryIndicator::batteryChangeOccurred);
}

void BatteryIndicator::batteryChangeOccurred(const Battery::Level level){
	const auto ledController = getApp()->getService<LEDController>();

	// Strobe LED if battery very low
	if (level <= Battery::Level::VeryLow){
		ledController->batteryLowLedStrobe();
	} else{
		ledController->batteryLowLedOff();
	}
}
