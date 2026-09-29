#ifndef BUTTERBOTCTRL_FIRMWARE_NIGHTMODESELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_NIGHTMODESELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>
#include <Services/Settings.h>

// Custom (NUIT): Settings row NIGHT MODE - robot night hours (quieter, no idle comments), every range ends at 07:00
class NightModeSelector : public LVObject {
public:
	NightModeSelector(lv_obj_t* parent, NightMode initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(NightMode)>& valCb);
	~NightModeSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	static constexpr uint8_t OptionsNum = 4;
	static constexpr std::array<NightMode, OptionsNum> OptionValueMap = {
		NightMode::Off,
		NightMode::From22,
		NightMode::From23,
		NightMode::From00
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"OFF",
		"22-07",
		"23-07",
		"00-07"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(NightMode)> valCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_NIGHTMODESELECTOR_H
