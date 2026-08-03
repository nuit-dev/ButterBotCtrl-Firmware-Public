#ifndef BUTTERBOTCTRL_FIRMWARE_JIGHWTEST_H
#define BUTTERBOTCTRL_FIRMWARE_JIGHWTEST_H

#include <vector>
#include <esp_efuse.h>
#include <esp_spiffs.h>
#include <esp_adc/adc_oneshot.h>

#include <Util/stdafx.h>
#include <Devices/Display.h>
#include <Periphery/GPIOPeriph.h>
#include <Drivers/Input/InputGPIO.h>
#include <Services/ButtonInput.h>

#include "HardwareConfiguration.hpp"
#include "Pins.hpp"

struct Test {
	bool (*test)();
	const char* name;
	void (*onFail)();
};

class JigHWTest {
public:
	JigHWTest();
	static bool checkJig();
	void start();

private:
	inline static StrongObjectPtr<Display> display = nullptr;
	inline static LGFX_Sprite* canvas = nullptr;
	inline static JigHWTest* test = nullptr;
	std::vector<Test> tests;
	const char* currentTest;

	void log(const char* property, const char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char* property, uint32_t value);
	void log(const char* property, int32_t value);
	void log(const char* property, const std::string& value);

	void instr(const char* msg);

	static bool BatteryCheck();
	static bool VoltReferenceCheck();
	static bool SPIFFSTest();
	static bool HWVersion();

	static void rgb();

	static constexpr int16_t BatVoltageMinimum = 3300;
	static constexpr float VoltReference = 2500;
	static constexpr float VoltReferenceTolerance = 100;
	static constexpr float Factor = 4.0f;
	static constexpr float Offset = 0;

	static constexpr uint32_t CheckTimeout = 500;

	static constexpr esp_vfs_spiffs_conf_t spiffsConfig = {
		.base_path = "/spiffs_image",
		.partition_label = "storage",
		.max_files = 8,
		.format_if_mount_failed = false
	};

	// LED SETUP
	static constexpr gpio_num_t ledPins[] = { (gpio_num_t)LED_WIFI, (gpio_num_t)LED_POWER, (gpio_num_t)LED_BATTLOW, (gpio_num_t)LED_BIG };

	static constexpr uint32_t DefaultFreq = 5000; // Placeholder, usually changed before attaching to a channel or pin
	static constexpr ledc_timer_bit_t DRAM_ATTR DutyResDefault = LEDC_TIMER_10_BIT;
	static constexpr uint32_t DRAM_ATTR FullDuty = 1 << DutyResDefault;

	static constexpr ledc_mode_t getSpeedMode(int port){
		return (ledc_mode_t)(port / (int)LEDC_CHANNEL_MAX);
	}

	static constexpr ledc_timer_t getTimer(int port){
		return (ledc_timer_t)(port % LEDC_TIMER_MAX);
	}

	static constexpr ledc_channel_t getChannel(int port){
		return (ledc_channel_t)(port % LEDC_CHANNEL_MAX);
	}


	//Note - using a more basic ADC abstraction here to have a more stable hardware-testing environment
	class ADC {
	public:
		explicit ADC(adc_unit_t unit);
		virtual ~ADC();

		adc_unit_t getUnit() const;

		void config(adc_channel_t chan, const adc_oneshot_chan_cfg_t& cfg);

		esp_err_t read(adc_channel_t chan, int& valueOut, adc_cali_handle_t cali = nullptr) const;

		void reinit();

	private:
		adc_oneshot_unit_handle_t hndl;
		const adc_unit_t unit;
	};

	class ADCReader {
	public:
		/**
		 * Reads an ADC channel, applies calibration, EMA, factor, offset, and mapping.
		 * First, calibration data is applied, then EMA.
		 * Smoothed value is multiplied by the factor, then offset is applied.
		 * Lastly, the value is mapped from [min, max] to [0, 100].
		 * @param adc ADC unit
		 * @param chan ADC channel
		 * @param cali Calibration data
		 * @param offset Offset
		 * @param factor Multiplication factor
		 * @param emaA EMA factor
		 * @param min Mapping lower bound
		 * @param max Mapping upper bound
		 */
		ADCReader(ADC& adc, adc_channel_t chan, adc_cali_handle_t cali = nullptr, float offset = 0, float factor = 1, float emaA = 1, float min = 0, float max = 0);

		/** Sample and return new value. */
		float sample();

		/** Return current value without sampling. */
		float getValue() const;

		/** Reset EMA and take a fresh sample. */
		void resetEma();

		/** Set additional offset. This offset is applied after initial factor and offset. */
		void setMoreOffset(float offset);

		/**
		 * Set EMA factor. Filter be refreshed afterwards.
		 * @param factor EMA factor
		 */
		void setEMAFactor(float factor);

	private:
		ADC& adc;
		const adc_channel_t chan;
		const adc_cali_handle_t cali;

		const float offset;
		const float factor;
		float emaA;
		const float min;
		const float max;

		float value = -1.0f;

		float moreOffset = 0;
	};
};


#endif //BUTTERBOTCTRL_FIRMWARE_JIGHWTEST_H
