#ifndef BUTTERBOTCTRL_FIRMWARE_BIGGREENLEDBLINKFUNCTION_H
#define BUTTERBOTCTRL_FIRMWARE_BIGGREENLEDBLINKFUNCTION_H

// Triple blink function for Big Green LED
class BigGreenLEDBlinkFunction : public LEDFunction<LEDs, float> {
	using Super = LEDFunction<LEDs, float>;

public:
	BigGreenLEDBlinkFunction(){}

	float getValue() const noexcept override{
		if(blinkState) return value;

		return float{};
	}

	bool isDone() const noexcept override{
		return false;
	}

	TickType_t getInterval() const noexcept override{
		if(blinkState){
			return static_cast<TickType_t>((blinkPeriod - blinkOnTime) * 1000 / portTICK_PERIOD_MS);
		}

		if(onState){
			return static_cast<TickType_t>((period - onTime) * 1000 / portTICK_PERIOD_MS);
		}

		return static_cast<TickType_t>(onTime * 1000 / portTICK_PERIOD_MS);
	}

private:
	void tick(const float deltaTime) noexcept override{
		// Track elapsed time via an absolute timestamp, instead of accumulated deltaTime
		// Reason: the service sleeps while idle, so the first deltaTime after waking would jump past intended state
		if(!started){
			started = true;
			startTime = millis();
		}

		timer = (millis() - startTime) / 1000.0f;

		onState = false;
		if(fmodf(timer, period) <= onTime){
			onState = true;
		}

		blinkState = false;
		if(onState && fmodf(timer, blinkPeriod) <= blinkOnTime){
			blinkState = true;
		}
	}

	// BLINK PARAMS
	const float value = 0.25f; // ON value for blinking
	const float period = 1.0f; // main blink period in seconds
	const float onTime = 0.25f; // overall blinks duration in seconds

	const float blinkOnTime = 0.05f; // blink duration
	const float blinkPeriod = 0.1f; // period between blinks

	uint64_t startTime = 0;
	bool started = false;

	float timer = 0;
	bool onState = false;
	bool blinkState = false;
};

#endif //BUTTERBOTCTRL_FIRMWARE_BIGGREENLEDBLINKFUNCTION_H
