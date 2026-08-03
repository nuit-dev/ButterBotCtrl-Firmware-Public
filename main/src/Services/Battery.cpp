#include "Battery.h"
#include "Pins.hpp"

#include <Util/stdafx.h>
#include <Memory/ObjectMemory.h>

DEFINE_LOG(Battery)

Battery::Battery() : hysteresis({ 0, 4, 15, 30, 50, 70, 90, 100 }, 3){
	adc_oneshot_chan_cfg_t cfg = {
		.atten = ADC_ATTEN_DB_2_5,
		.bitwidth = ADC_BITWIDTH_12
	};

	readerBattoffsetFilter = newObject<FactorOffset_ADCFilter>(this, Factor, Offset);
	readerBattEMAFilter = newObject<EMA_ADCFilter>(this, EmaA);
	std::vector<StrongObjectPtr<ADCFilter>> filters = {
		StrongObjectPtr<ADCFilter>{ readerBattEMAFilter },
		StrongObjectPtr<ADCFilter>{ readerBattoffsetFilter },
		StrongObjectPtr<ADCFilter>{ newObject<Remap_ADCFilter>(this, VoltEmpty, VoltFull) }
	};

	readerBatt = newObject<ADCReader>(this, (gpio_num_t)PIN_BATT, cfg, true, newObject<Composite_ADCFilter>(this, filters).get());

	// Ref voltage reader has no offset
	readerRef = newObject<ADCReader>(this, (gpio_num_t)PIN_BATT, cfg, true, newObject<FactorOffset_ADCFilter>(this, Factor, 0).get());

	calibrate();

	//Sampling twice to provide a somewhat steady reading at startup. Used to determine if immediate shutdown is needed.
	sample(true);
	sample(true);

	batThread = std::make_unique<Threaded>([this](){ tick(); }, "Battery", MeasureIntverval, 3 * 1024, 3, 1, false);
}

uint8_t Battery::getPerc() const{
	return readerBatt->getValue();
}

Battery::Level Battery::getLevel() const{
	return (Level)hysteresis.get();
}

bool Battery::isShutdown() const{
	return shutdown;
}

void Battery::calibrate(){
	//Note: Voltage reference (calibration) pin is enabled in bootloader, so we can immediately start calibration without delay.

	for(int i = 0; i < CalReads; i++){
		readerRef->sample();
		delayMillis(10);
	}

	float total = 0;
	for(int i = 0; i < CalReads; i++){
		total += readerRef->sample();
		delayMillis(10);
	}

	const float reading = total / (float)CalReads;
	const float offset = CalExpected - reading;
	readerBattoffsetFilter->setOffset(readerBattoffsetFilter->getOffset() + offset);

	gpio_config_t config = {
		1 << PIN_VREF, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE
	};
	gpio_config(&config);
	gpio_set_level((gpio_num_t)PIN_VREF, false);
	delayMillis(CalDelay);

	CMF_LOG(Battery, LogLevel::Info, "Calibration: Read %.02f mV, expected %.02f mV. Applying %.02f mV offset.", reading, CalExpected, offset);
}

void Battery::sample(const bool fresh){
	if(shutdown) return;

	const Level oldLevel = getLevel();
	if(fresh){
		readerBattEMAFilter->reset();
		hysteresis.reset(readerBatt->sample());
	} else{
		hysteresis.update(readerBatt->sample());
	}

	ESP_LOGD("battery", "sample value: %.2f", readerBatt->getValue());

	if(oldLevel != getLevel() || fresh){
		OnLevelChanged.broadcast(getLevel());
	}

	if(getLevel() == Level::Critical){
		shutdown = true;
	}
}

void Battery::tick() noexcept{
	if(shutdown) return;
	sample();
}

void Battery::begin(){
	batThread->start();
}
