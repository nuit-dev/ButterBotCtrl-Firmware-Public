#ifndef BUTTERBOTCTRL_FIRMWARE_PERCENTSLIDER_H
#define BUTTERBOTCTRL_FIRMWARE_PERCENTSLIDER_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>

#include "Services/ThemeService.h"

class PercentSlider : public LVObject {
public:
	// Custom (NUIT): BRIGHTNESS-style slider for 10-100 % in steps of 10 (VOLUME, NIGHT VOLUME)
	PercentSlider(lv_obj_t* parent, const char* label, int32_t sliderX, uint8_t initPercent, const std::function<void(uint8_t)>& valCb, const std::function<void(uint32_t)>& keyCb);
	~PercentSlider() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle sliderStyleKnob;
	LVStyle sliderStyleBg;
	LVStyle sliderStyleIndic;
	LVStyle labelDefaultStyle;

	lv_obj_t* slider;
	static constexpr int32_t SliderMax = 10; // x 10 %
	static constexpr int32_t SliderMin = 1;
	static constexpr int32_t SliderBuffer = 1;
	static constexpr int32_t RightEdge = 111; // same right edge as the other rows
	const char* label;
	int32_t sliderX;
	uint8_t currentPercent;
	std::function<void(uint8_t)> valCb;
	std::function<void(uint32_t)> keyCb;

	void buildUI(lv_obj_t* parent);
};

#endif //BUTTERBOTCTRL_FIRMWARE_PERCENTSLIDER_H
