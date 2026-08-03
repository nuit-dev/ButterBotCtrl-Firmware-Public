#include "Joystick.h"
#include <algorithm>
#include "Memory/ObjectMemory.h"

Joystick::Joystick(gpio_num_t hPin, gpio_num_t vPin, bool internalStack) :
		Super(SampleInterval / portTICK_PERIOD_MS, 3 * 1024, 5, -1, internalStack){
	adc_oneshot_chan_cfg_t cfg = {
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_12
	};

	hEMAFilter = newObject<EMA_ADCFilter>(this, EmaA);
	vEMAFilter = newObject<EMA_ADCFilter>(this, EmaA);

	hReader = newObject<ADCReader>(this, hPin, cfg, false, StrongObjectPtr<ADCFilter>(hEMAFilter));
	vReader = newObject<ADCReader>(this, vPin, cfg, false, StrongObjectPtr<ADCFilter>(vEMAFilter));
}

int8_t Joystick::getHorizontal() const noexcept{
	return hValue.load();
}

int8_t Joystick::getVertical() const noexcept{
	return vValue.load();
}

int8_t Joystick::mapSample(float raw) noexcept{
	const float centered = raw - static_cast<float>(RawCenter);
	int mapped = std::clamp(static_cast<int>(centered / static_cast<float>(RawCenter) * 100.0f), -80, 80);
	if(mapped > -DeadZone && mapped < DeadZone){
		return 0;
	}

	mapped = static_cast<int>(100.0f * mapped / MaxMapping); // NOLINT(*-narrowing-conversions)
	return static_cast<int8_t>(mapped);
}

void Joystick::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	hValue.store(-mapSample(hReader->sample()));
	vValue.store(mapSample(vReader->sample()));
}
