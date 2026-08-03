#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"

#include <soc/efuse_reg.h>
#include <esp_efuse.h>
#include <iostream>
#include <esp_mac.h>
#include <driver/ledc.h>

#include "Util/EfuseMeta.h"

JigHWTest::JigHWTest(){
	const lgfx::Bus_SPI::config_t displayBusConfig = {
		.freq_write = 40000000,
		.freq_read = 40000000,
		.pin_sclk = TFT_SCK,
		.pin_miso = -1,
		.pin_mosi = TFT_SDA,
		.pin_dc = TFT_DC,
		.spi_mode = 0,
		.spi_3wire = false,
		.use_lock = false,
		.dma_channel = LGFX_ESP32_SPI_DMA_CH,
		.spi_host = SPI2_HOST
	};
	const lgfx::Panel_Device::config_t displayPanelConfig = {
		.pin_cs = -1,
		.pin_rst = TFT_RST,
		.pin_busy = -1,
		.memory_width = 128,
		.memory_height = 128,
		.panel_width = 128,
		.panel_height = 128,
		.offset_x = 0,
		.offset_y = 0,
		.offset_rotation = 0,
		.readable = false,
		.invert = false,
		.rgb_order = false,
		.dlen_16bit = false,
		.bus_shared = false
	};

	display = newObject<Display>(nullptr, displayBusConfig, displayPanelConfig, [](Sprite& canvas) {
		canvas.setColorDepth(lgfx::rgb565_2Byte);
		canvas.createSprite(128, 128);
	});
	display->getLGFX().setSwapBytes(true);
	canvas = &display->getCanvas();

	test = this;

	tests.push_back({
		JigHWTest::VoltReferenceCheck, "Voltage ref", []() {
			gpio_set_level((gpio_num_t)PIN_VREF, 0);
		}
	});
	tests.push_back({ JigHWTest::BatteryCheck, "Battery check", []() {} });
	tests.push_back({ JigHWTest::SPIFFSTest, "SPIFFS check", []() {} });
	tests.push_back({
		JigHWTest::HWVersion, "HW version", []() {
			esp_efuse_batch_write_cancel();
		}
	});
}

bool JigHWTest::checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	int c;
	while(millis() - start < CheckTimeout){
		vTaskDelay(1);
		c = getchar();
		if(c == EOF) continue;
		buf[wp] = (char)c;
		wp = (wp + 1) % 7;

		for(int i = 0; i < 7; i++){
			int match = 0;
			static const char* target = "JIGTEST";

			for(int j = 0; j < 7; j++){
				match += buf[(i + j) % 7] == target[j];
			}

			if(match == 7) return true;
		}
	}

	return false;
}


void JigHWTest::start(){
	uint64_t _chipmacid = 0LL;
	esp_efuse_mac_get_default((uint8_t*)(&_chipmacid));
	printf("\nTEST:begin:%llx\n", _chipmacid);

	gpio_config_t cfg = {
		.pin_bit_mask = ((uint64_t)1) << TFT_BL,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE
	};
	gpio_config(&cfg);
	gpio_set_level((gpio_num_t)TFT_BL, 0);

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);
	canvas->pushSprite(0, 0);

	canvas->setTextFont(0);
	canvas->setTextSize(1);

	canvas->drawString("ButterBot Controller", canvas->width() / 2, 6);
	canvas->drawString("Hardware Test", canvas->width() / 2, 16);
	canvas->println();

	canvas->pushSprite(0, 0);

	canvas->setCursor(0, 36);

	bool pass = true;
	for(const Test& test : tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		canvas->pushSprite(0, 0);


		printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		canvas->setTextColor(result ? TFT_SILVER : TFT_ORANGE);
		canvas->printf("%s\n", result ? "PASS" : "FAIL");
		canvas->pushSprite(0, 0);

		printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}

	if(pass){
		printf("TEST:passall\n");
	} else{
		printf("TEST:fail:%s\n", currentTest);
	}


	//------------------------------------------------------
	canvas->print("\n");
	canvas->setTextColor(pass ? TFT_BLUE : TFT_ORANGE);
	canvas->drawCentreString(pass ? "All OK!" : "FAIL!", canvas->width() / 2, canvas->getCursorY());
	canvas->pushSprite(0, 0);

	bool painted = false;
	const int color = pass ? TFT_GREEN : TFT_RED;
	uint64_t flashTime = 0;

	// LED PWM setup
	if(pass){
		for(int i = 0; i < std::size(ledPins); ++i){
			const gpio_num_t led_pin = ledPins[i];
			const auto led_channel = (ledc_channel_t)i;

			gpio_reset_pin(led_pin);
			gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);

			const ledc_timer_config_t ledc_timer = {
				.speed_mode = getSpeedMode(led_channel),
				.duty_resolution = DutyResDefault,
				.timer_num = getTimer(led_channel),
				.freq_hz = DefaultFreq,
				.clk_cfg = LEDC_AUTO_CLK
			};
			ledc_timer_config(&ledc_timer);

			const ledc_channel_config_t ledc_channel = {
				.gpio_num = led_pin,
				.speed_mode = getSpeedMode(led_channel),
				.channel = getChannel(led_channel),
				.intr_type = LEDC_INTR_DISABLE,
				.timer_sel = getTimer(led_channel),
				.duty = 0,
				.hpoint = 0,
				.flags = { .output_invert = false }
			};
			ledc_channel_config(&ledc_channel);
		}
	}

	for(;;){
		if(millis() - flashTime >= 500){
			for(int x = 0; x < canvas->width(); x++){
				for(int y = 0; y < canvas->height(); y++){
					const uint16_t previousPixel = canvas->readPixel(x, y);
					if(!painted && previousPixel == TFT_BLACK){
						canvas->drawPixel(x, y, color);
					} else if(painted && previousPixel == color){
						canvas->drawPixel(x, y, TFT_BLACK);
					}
				}
			}
			if(pass){
				for(int i = 0; i < std::size(ledPins); ++i){
					const auto led_channel = (ledc_channel_t)i;

					const uint32_t duty = FullDuty * 0.1 * !painted;
					const ledc_mode_t group = getSpeedMode(led_channel);
					const ledc_channel_t chan = getChannel(led_channel);

					ledc_set_duty(group, chan, duty);
					ledc_update_duty(group, chan);
				}
			}

			flashTime = millis();
			painted = !painted;
			canvas->pushSprite(0, 0);
		}

		delayMillis(10);
	}
}

void JigHWTest::rgb(){
	static const char* names[] = { "RED", "GREEN", "BLUE" };
	static constexpr uint16_t colors[] = { TFT_RED, TFT_GREEN, TFT_BLUE };
	for(int i = 0; i < 3; i++){
		canvas->clear(colors[i]);
		canvas->setCursor(20, 40);
		canvas->setTextFont(0);
		canvas->setTextSize(2);
		canvas->print(names[i]);
		canvas->pushSprite(0, 0);
		vTaskDelay(350);
	}
}

void JigHWTest::log(const char* property, const char* value){
	printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, int32_t value){
	printf("%s:%s:%ld\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const std::string& value){
	printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

void JigHWTest::instr(const char* msg){
	canvas->setTextColor(TFT_GOLD);
	canvas->print(msg);
	canvas->print(" ");
}


bool JigHWTest::BatteryCheck(){
	ADC adc(ADC_UNIT_1);

	const auto config = [&adc](int pin, adc_cali_handle_t& cali, std::unique_ptr<ADCReader>& reader) {
		adc_unit_t unit;
		adc_channel_t chan;
		ESP_ERROR_CHECK(adc_oneshot_io_to_channel(pin, &unit, &chan));
		assert(unit == adc.getUnit());

		adc.config(chan, {
			           .atten = ADC_ATTEN_DB_2_5,
			           .bitwidth = ADC_BITWIDTH_12
		           });

		const adc_cali_curve_fitting_config_t curveCfg = {
			.unit_id = unit,
			.chan = chan,
			.atten = ADC_ATTEN_DB_2_5,
			.bitwidth = ADC_BITWIDTH_12
		};

		ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&curveCfg, &cali));

		reader = std::make_unique<ADCReader>(adc, chan, cali, Offset, Factor);
	};

	adc_cali_handle_t cali;
	std::unique_ptr<ADCReader> reader;
	config(PIN_BATT, cali, reader);

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 10;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += reader->sample();
		vTaskDelay(readDelay / portTICK_PERIOD_MS);
	}
	reading /= numReadings;

	test->log("reading", reading);

	if(reading < BatVoltageMinimum){
		return false;
	}

	return true;
}

bool JigHWTest::VoltReferenceCheck(){
	constexpr auto RefSwitch = (gpio_num_t)PIN_VREF;
	gpio_set_direction(RefSwitch, GPIO_MODE_OUTPUT);
	gpio_set_level(RefSwitch, 1);

	delayMillis(1000);

	ADC adc(ADC_UNIT_1);

	const auto config = [&adc](int pin, adc_cali_handle_t& cali, std::unique_ptr<ADCReader>& reader) {
		adc_unit_t unit;
		adc_channel_t chan;
		ESP_ERROR_CHECK(adc_oneshot_io_to_channel(pin, &unit, &chan));
		assert(unit == adc.getUnit());

		adc.config(chan, {
			           .atten = ADC_ATTEN_DB_2_5,
			           .bitwidth = ADC_BITWIDTH_12
		           });

		const adc_cali_curve_fitting_config_t curveCfg = {
			.unit_id = unit,
			.chan = chan,
			.atten = ADC_ATTEN_DB_2_5,
			.bitwidth = ADC_BITWIDTH_12
		};

		ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&curveCfg, &cali));

		reader = std::make_unique<ADCReader>(adc, chan, cali, Offset, Factor);
	};

	adc_cali_handle_t cali;
	std::unique_ptr<ADCReader> reader;
	config(PIN_BATT, cali, reader);

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 10;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += reader->sample();
		vTaskDelay(readDelay / portTICK_PERIOD_MS);
	}
	reading /= numReadings;

	test->log("reading", reading);

	if(reading < VoltReference - VoltReferenceTolerance || reading > VoltReference + VoltReferenceTolerance){
		return false;
	}

	gpio_set_level(RefSwitch, 0);
	delayMillis(1000);

	return true;
}

bool JigHWTest::SPIFFSTest(){
	esp_err_t ret = esp_vfs_spiffs_register(&spiffsConfig);
	if(ret != ESP_OK){
		test->log("spiffs", false);
		return false;
	}

	for(const auto& f : SPIFFSFiles){
		FILE* file = fopen(f.name, "rb");
		if(file == nullptr){
			test->log("missing", f.name);
			return false;
		}

		fseek(file, 0, SEEK_END);
		const long size = ftell(file);
		fclose(file);

		if(size < 0 || (size_t) size != f.size){
			test->log("size mismatch", f.name);
			test->log("expected", (uint32_t) f.size);
			test->log("got", (int32_t) size);
			return false;
		}
	}

	return true;
}

bool JigHWTest::HWVersion(){
	uint16_t pid = 1;
	bool result = EfuseMeta::readPID(pid);

	if(!result){
		test->log("HW version", "couldn't read from efuse");
		return false;
	}

	if(pid != 0){
		test->log("Existing HW version", (uint32_t)pid);
		if(pid == EfuseMeta::getHardcodedPID()){
			test->log("Already fused.", (uint32_t)pid);
			return true;
		}
		test->log("Wrong binary already fused!", (uint32_t)pid);
		return false;
	}

	return EfuseMeta::write();
}

/*
 * Basic ADC abstraction class
 */
JigHWTest::ADC::ADC(adc_unit_t unit) : unit(unit){
	const adc_oneshot_unit_init_cfg_t config = {
		.unit_id = unit,
		.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
		.ulp_mode = ADC_ULP_MODE_DISABLE
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&config, &hndl));
}

JigHWTest::ADC::~ADC(){
	ESP_ERROR_CHECK(adc_oneshot_del_unit(hndl));
}

void JigHWTest::ADC::config(adc_channel_t chan, const adc_oneshot_chan_cfg_t& cfg){
	ESP_ERROR_CHECK(adc_oneshot_config_channel(hndl, chan, &cfg));
}

adc_unit_t JigHWTest::ADC::getUnit() const{
	return unit;
}

esp_err_t JigHWTest::ADC::read(adc_channel_t chan, int& valueOut, const adc_cali_handle_t cali) const{
	if(cali != nullptr){
		return adc_oneshot_get_calibrated_result(hndl, cali, chan, &valueOut);
	}
	return adc_oneshot_read(hndl, chan, &valueOut);
}

void JigHWTest::ADC::reinit(){
	ESP_ERROR_CHECK(adc_oneshot_del_unit(hndl));
	const adc_oneshot_unit_init_cfg_t config = {
		.unit_id = unit,
		.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
		.ulp_mode = ADC_ULP_MODE_DISABLE
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&config, &hndl));
}

JigHWTest::ADCReader::ADCReader(ADC& adc, adc_channel_t chan, adc_cali_handle_t cali, float offset, float factor, float emaA, float min, float max)
	: adc(adc), chan(chan), cali(cali), offset(offset), factor(factor), emaA(emaA), min(min), max(max){
}

float JigHWTest::ADCReader::sample(){
	int raw = 0;
	if(adc.read(chan, raw, cali) != ESP_OK){
		return getValue();
	}

	if(value == -1 || emaA == 1){
		value = raw;
	} else{
		value = value * (1.0f - emaA) + emaA * (float)raw;
	}

	return getValue();
}

float JigHWTest::ADCReader::getValue() const{
	const float adjusted = value * factor + offset + moreOffset;

	if(max == 0 && min == 0){
		return adjusted;
	}

	float minimum = min;
	float maximum = max;

	if(min > max){
		std::swap(minimum, maximum);
	}

	float val = std::clamp(adjusted, minimum, maximum);
	val = (val - minimum) / (maximum - minimum);
	val = std::clamp(val * 100.0f, 0.0f, 100.0f);

	if(min > max){
		val = 100.0f - val;
	}

	return val;
}

void JigHWTest::ADCReader::resetEma(){
	value = -1;
	sample();
}

void JigHWTest::ADCReader::setMoreOffset(float offset){
	moreOffset = offset;
}

void JigHWTest::ADCReader::setEMAFactor(float factor){
	emaA = factor;
	resetEma();
}
