#ifndef BUTTERBOTCTRL_FIRMWARE_VOICESELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_VOICESELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>
#include <Services/Settings.h>

// Custom (NUIT): Settings row that picks the robot's TTS voice preset
class VoiceSelector : public LVObject {
public:
	VoiceSelector(lv_obj_t* parent, VoiceMode initVal, const std::function<void(uint32_t)>& keyCb, const std::function<void(VoiceMode)>& valCb);
	~VoiceSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentIndex;
	static constexpr uint8_t OptionsNum = 4;
	static constexpr std::array<VoiceMode, OptionsNum> OptionValueMap = {
		VoiceMode::Normal,
		VoiceMode::Hawking,
		VoiceMode::Vader,
		VoiceMode::Hal
	};
	static constexpr std::array<const char*, OptionsNum> OptionNameMap = {
		"NORMAL",
		"HAWKING",
		"VADER",
		"HAL 9000"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(VoiceMode)> valCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_VOICESELECTOR_H
