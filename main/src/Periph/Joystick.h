#ifndef BB_CTRL_CONTROLLER_JOYSTICK_H
#define BB_CTRL_CONTROLLER_JOYSTICK_H

#include <atomic>
#include <hal/gpio_types.h>
#include <Entity/AsyncEntity.h>
#include <Memory/SmartPtr/StrongObjectPtr.h>
#include <Services/ADCReader.h>

class Joystick : public AsyncEntity {
	GENERATED_BODY(Joystick, AsyncEntity, CONSTRUCTOR_PACK(gpio_num_t, gpio_num_t))

public:
	Joystick(gpio_num_t hPin = GPIO_NUM_NC, gpio_num_t vPin = GPIO_NUM_NC, bool internalStack = true);

	int8_t getHorizontal() const noexcept;
	int8_t getVertical() const noexcept;

protected:
	void tick(float deltaTime) noexcept override;

private:
	static constexpr TickType_t SampleInterval = 20; // ms
	static constexpr float EmaA = 0.3f;
	static constexpr int DeadZone = 25; // ±25 points around center
	static constexpr float MaxMapping = 80.0f; // After which points around center is it considered to be at 100%
	static constexpr int RawCenter = 2048;

	StrongObjectPtr<EMA_ADCFilter> hEMAFilter;
	StrongObjectPtr<EMA_ADCFilter> vEMAFilter;
	StrongObjectPtr<ADCReader> hReader;
	StrongObjectPtr<ADCReader> vReader;

	std::atomic<int8_t> hValue{ 0 };
	std::atomic<int8_t> vValue{ 0 };

	static int8_t mapSample(float raw) noexcept;
};

#endif //BB_CTRL_CONTROLLER_JOYSTICK_H
