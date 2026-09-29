#ifndef BUTTERBOTCTRL_FIRMWARE_SETTINGSWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_SETTINGSWINDOW_H

#include <Enums.hpp>
#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>

#include "Services/ThemeService.h"
#include <Services/Settings.h>
#include <Services/LED/LED.h>

#include "Components/SettingsWindow/BrightnessSlider.h"
#include "Components/SettingsWindow/ThemeSelector.h"
#include "Components/SettingsWindow/SleepSelector.h"
#include "Components/SettingsWindow/SensorSelector.h"
#include "Components/SettingsWindow/VoiceSelector.h"
#include "Components/SettingsWindow/FastStartSelector.h"
#include "Components/SettingsWindow/PercentSlider.h"
#include "Components/SettingsWindow/NightModeSelector.h"
#include "Components/SettingsWindow/DateTimeRow.h"
#include <array>

class SettingsWindow : public LVObject {
public:
	/**
	 * UI window that contains settings options
	 * @param parent parent LVGL object
	 * @param inputGroup parent input group
	 */
	SettingsWindow(lv_obj_t* parent, lv_group_t* inputGroup, const std::function<void(const Theme &newTheme)>& themeCb);
	~SettingsWindow() override;

	/* Main update loop, called by the parent. Custom (NUIT): keeps DATE / TIME showing the robot's clock */
	virtual void loop();

	// Custom (NUIT): joystick press on DATE / TIME starts or confirms editing the robot's clock.
	// Returns false when the press isn't used here (it then closes Settings, as before).
	bool onJoystickPress();

private:
	ThemeService* theme;
	Settings* settings;
	LED<LEDs, RGB_LEDs>* ledService;
	lv_group_t* inputGroup;

	ThemeSelector* themeSelector;
	SleepSelector* sleepSelector;
	BrightnessSlider* brightnessSlider;
	SensorSelector* sensorSelector; // Custom (NUIT)
	VoiceSelector* voiceSelector; // Custom (NUIT)
	FastStartSelector* fastStartSelector; // Custom (NUIT)
	PercentSlider* volumeSlider; // Custom (NUIT)
	NightModeSelector* nightModeSelector; // Custom (NUIT)
	PercentSlider* nightVolumeSlider; // Custom (NUIT)
	DateTimeRow* dateRow; // Custom (NUIT)
	DateTimeRow* timeRow; // Custom (NUIT)

	lv_obj_t* innerContent;
	LVStyle labelDefaultStyle;
	LVStyle barStyleBg;
	LVStyle barStyleIndic;
	LVStyle barStyleKnob;

	lv_anim_t blinkAnim;

	lv_obj_t* titleEl;
	lv_obj_t* titleLabel;
	lv_obj_t* corner;

	int32_t currentFocusIndex = 0;

	static constexpr int32_t WindowWidth = 120;
	static constexpr int32_t WindowHeight = 92; // Custom (NUIT): was 60; 6 of the 11 rows visible, footer removed
	static constexpr int32_t RowCount = 11;

	// Custom (NUIT): row positions in the scrolling content (15 px selectors, 7 px sliders, 1-2 px gaps)
	static constexpr std::array<int32_t, RowCount> RowY = { 1, 17, 34, 42, 58, 74, 91, 99, 116, 124, 140 };
	static constexpr int32_t DateRowIndex = 9;
	static constexpr int32_t TimeRowIndex = 10;
	std::array<lv_obj_t*, RowCount> rowObjs{};
	std::array<lv_obj_t*, RowCount> rowLabels{};
	void focusRow(int32_t index);
	void scrollToRow(int32_t index);

	uint64_t lastClockRefresh = 0;
	DateTimeRow::Value editBase;
	DateTimeRow::Value clockNow(bool& known) const;
	void refreshClock();
	void sendRobotConfig(const RobotConfigData& config);
	static constexpr const char* WindowTitle = "Settings";

	const std::function<void(const Theme &newTheme)> themeCb;

	/* Properly scales window top bar with inner content */
	void updateLayout();
	void buildUI(lv_obj_t* parent);
};


#endif //BUTTERBOTCTRL_FIRMWARE_SETTINGSWINDOW_H