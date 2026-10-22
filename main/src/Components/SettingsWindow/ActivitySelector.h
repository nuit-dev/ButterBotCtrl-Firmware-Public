#ifndef BUTTERBOTCTRL_FIRMWARE_ACTIVITYSELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_ACTIVITYSELECTOR_H

#include <array>
#include <functional>
#include <CtrlData.h>
#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>

class ActivitySelector : public LVObject {
public:
	ActivitySelector(lv_obj_t* parent, ActivityMode initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(ActivityMode)>& valCb);
	~ActivitySelector() override;

	void setValue(ActivityMode value);
	// Custom (NUIT): with the TALKIE TOASTER voice the highest level (HYPER) is called TOASTER
	void setToaster(bool on);

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	bool toaster = false;
	const char* nameAt(uint8_t index) const;
	static constexpr uint8_t OptionsNum = 4;
	static constexpr std::array<ActivityMode, OptionsNum> OptionValueMap = {
		ActivityMode::Off,
		ActivityMode::Chill,
		ActivityMode::Active,
		ActivityMode::Hyper
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"OFF",
		"CHILL",
		"ACTIVE",
		"HYPER"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(ActivityMode)> valCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_ACTIVITYSELECTOR_H