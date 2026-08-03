#ifndef BUTTERBOTCTRL_FIRMWARE_LEDCONTROLLER_H
#define BUTTERBOTCTRL_FIRMWARE_LEDCONTROLLER_H

#include <mutex>
#include <Object/Object.h>
#include <Services/LED/LED.h>
#include "Enums.hpp"

class LEDController : public Object {
	GENERATED_BODY(LEDController, Object, CONSTRUCTOR_PACK(StrongObjectPtr<LED<LEDs, RGB_LEDs>>))
public:
	LEDController(StrongObjectPtr<LED<LEDs, RGB_LEDs>> ledService);

	void powerLedOn();
	void powerLedOff();

	void batteryLowLedOn();
	void batteryLowLedOff();
	void batteryLowLedBreathe();
	void batteryLowLedStrobe();

	void wifiLedOn();
	void wifiLedOff();
	void wifiLedStrobe();

	void bigGreenLedOn();
	void bigGreenLedOff();
	void bigGreenLedStrobe();
	void bigGreenLedStrobeData(uint32_t blinkCount = 0);

	/** Check if specified LED is turned on */
	static bool isLedOn(LEDs led);

private:
	enum class Pattern : uint8_t {
		Off,
		On,
		Strobe,
		Breathe
	};

	StrongObjectPtr<LED<LEDs, RGB_LEDs>> ledService;
	std::mutex stateMutex;

	static std::map<LEDs, Pattern> LedPatternMap;

	static constexpr float LedDefaultBrightness = 0.1f;
	static constexpr float BreatheDefaultPeriod = 1.0f;
	static constexpr float StrobeDefaultPeriod = 0.5f;
	static constexpr float StrobeDefaultOnTime = 0.05f;

	static constexpr float BatteryLowStrobePeriod = 1.0f;
	static constexpr float BatteryLowStrobeTime = 0.05f;

	static constexpr float WifiStrobePeriod = 0.5f;
	static constexpr float WifiStrobeTime = 0.05f;

	static constexpr float BigGreenBrightness = 0.25f;
	static constexpr float BigGreenStrobePeriod = 1.0f;
	static constexpr float BigGreenStrobeTime = 0.05f;
	static constexpr float BigGreenDataStrobePeriod = 0.1f;
	static constexpr float BigGreenDataStrobeTime = 0.05f;

	void turnLedOn(LEDs led, float ledBrightness) const;
	void turnLedOff(LEDs led) const;
	void applyStrobe(LEDs led, float ledBrightness, float strobeDefaultPeriod, float strobeOnTime, uint32_t blinkCount = 0u);
	void applyBreathe(LEDs led, float ledBrightness, float breathePeriod);
};

#endif //BUTTERBOTCTRL_FIRMWARE_LEDCONTROLLER_H
