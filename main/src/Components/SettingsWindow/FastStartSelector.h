#ifndef BUTTERBOTCTRL_FIRMWARE_FASTSTARTSELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_FASTSTARTSELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>
#include <Services/Settings.h>

// Custom (NUIT): Settings row STARTUP - how much of the controller's startup is skipped (next boot)
class FastStartSelector : public LVObject {
public:
	FastStartSelector(lv_obj_t* parent, FastStart initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(FastStart)>& valCb);
	~FastStartSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	static constexpr uint8_t OptionsNum = 4;
	static constexpr std::array<FastStart, OptionsNum> OptionValueMap = {
		FastStart::Off,
		FastStart::Fast,
		FastStart::Boost,
		FastStart::Overkloking
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"OFF",
		"FAST",
		"BOOST",
		"OVERKLOKING"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(FastStart)> valCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_FASTSTARTSELECTOR_H
