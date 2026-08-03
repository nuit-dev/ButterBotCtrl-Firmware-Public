#ifndef BUTTERBOTCTRL_FIRMWARE_BRIGHTNESSSLIDER_H
#define BUTTERBOTCTRL_FIRMWARE_BRIGHTNESSSLIDER_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>

#include "Services/ThemeService.h"

class BrightnessSlider : public LVObject {
public:
	BrightnessSlider(lv_obj_t* parent, float initVal, const std::function<void(float)>& valCb, const std::function<void(uint32_t)>& keyCb);
	~BrightnessSlider() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle sliderStyleKnob;
	LVStyle sliderStyleBg;
	LVStyle sliderStyleIndic;
	LVStyle labelDefaultStyle;

	lv_obj_t* slider;
	static constexpr int32_t SliderMax = 20;
	static constexpr int32_t SliderMin = 2;
	static constexpr int32_t SliderBuffer = 1;
	float currentVal;
	std::function<void(float)> valCb;
	std::function<void(uint32_t)> keyCb;

	void buildUI(lv_obj_t* parent);
};

#endif //BUTTERBOTCTRL_FIRMWARE_BRIGHTNESSSLIDER_H
