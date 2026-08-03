#ifndef BB_CTRL_CONTROLLER_BATTERYINDICATOR_H
#define BB_CTRL_CONTROLLER_BATTERYINDICATOR_H

#include <Entity/AsyncEntity.h>
#include "Battery.h"
#include "Services/LED/LED.h"
#include "Enums.hpp"
#include <cmath>

class BatteryIndicator : public Object {
	GENERATED_BODY(BatteryIndicator, Object, CONSTRUCTOR_PACK(StrongObjectPtr<Battery>, StrongObjectPtr<LED<LEDs, RGB_LEDs>>))
public:
	BatteryIndicator(StrongObjectPtr<Battery> battery = nullptr, StrongObjectPtr<LED<LEDs, RGB_LEDs>> leds = nullptr);

private:
	void batteryChangeOccurred(Battery::Level level);

	StrongObjectPtr<Battery> battery;
	StrongObjectPtr<LED<LEDs, RGB_LEDs>> leds;

	static constexpr float ShortBlinkInterval = 0.5f; //[s]
	static constexpr float BlinkOnDuration = 0.1f; //[s]

	static constexpr float MaxBrightness = 0.02f;
	static constexpr float GreenFactor = 0.25f; //Green LEDs are visually brighter than red at same value;
};

#endif //BB_CTRL_CONTROLLER_BATTERYINDICATOR_H