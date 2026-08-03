#ifndef BUTTERBOTCTRL_FIRMWARE_SLEEPSELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_SLEEPSELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>
#include <Services/Settings.h>

class SleepSelector : public LVObject {
public:
	SleepSelector(lv_obj_t* parent, InactivityTimeout initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(InactivityTimeout)>& valCb);
	~SleepSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	static constexpr uint8_t OptionsNum = 5;
	static constexpr std::array<InactivityTimeout, OptionsNum> OptionValueMap = {
		InactivityTimeout::Off,
		InactivityTimeout::Min2,
		InactivityTimeout::Min5,
		InactivityTimeout::Min10,
		InactivityTimeout::Min30
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"OFF",
		"2 MIN",
		"5 MIN",
		"10 MIN",
		"30 MIN"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(InactivityTimeout)> valCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_SLEEPSELECTOR_H
