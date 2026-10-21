#ifndef BUTTERBOTCTRL_FIRMWARE_CHATTERSELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_CHATTERSELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>
#include <Services/Settings.h>

// Custom (NUIT): Settings row CHATTER - how often the robot speaks idle comments (OFF / RARE / NORMAL / OFTEN / TOASTER)
class ChatterSelector : public LVObject {
public:
	ChatterSelector(lv_obj_t* parent, ChatterMode initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(ChatterMode)>& valCb);
	~ChatterSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	static constexpr uint8_t OptionsNum = 5;
	static constexpr std::array<ChatterMode, OptionsNum> OptionValueMap = {
		ChatterMode::Off,
		ChatterMode::Rare,
		ChatterMode::Normal,
		ChatterMode::Frequent,
		ChatterMode::Toaster
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"OFF",
		"RARE",
		"NORMAL",
		"OFTEN",
		"TOASTER"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(ChatterMode)> valCb;

	void buildUI(lv_obj_t* parent);
};

#endif //BUTTERBOTCTRL_FIRMWARE_CHATTERSELECTOR_H
