#ifndef BUTTERBOTCTRL_FIRMWARE_THEMESELECTOR_H
#define BUTTERBOTCTRL_FIRMWARE_THEMESELECTOR_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>

class ThemeSelector : public LVObject {
public:
	ThemeSelector(lv_obj_t* parent, const std::function<void(uint32_t)>& keyCb, const std::function<void(const Theme &newTheme)>& themeCb);
	~ThemeSelector() override;

	lv_obj_t* widgetLabel;

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;

	lv_obj_t* selectorLabel;

	uint8_t currentThemeIndex;
	static constexpr uint8_t ThemesNum = 4;
	static constexpr std::array<const char*, ThemesNum> ThemeNameMap = {
		"GREEN MACHINE",
		"BOOGIE OOGIE",
		"COZY CORE",
		"THE MEMO DEMO"
	};

	std::function<void(uint32_t)> keyCb;
	const std::function<void(const Theme &newTheme)> themeCb;

	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_THEMESELECTOR_H
