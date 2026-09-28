#ifndef BUTTERBOTCTRL_FIRMWARE_SETTINGSSCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_SETTINGSSCREEN_H

#include <LV_Interface/LVScreen.h>
#include <Services/ButtonInput.h>

#include "Enums.hpp"
#include "Components/TopBar.h"
#include "Components/SettingsWindow.h"

class SettingsScreen : public LVScreen {
public:
	SettingsScreen();
	~SettingsScreen() override;

private:
	ThemeService* theme;
	Settings* settings;
	ButtonInput* buttonInput;

	lv_obj_t* windowContainer = nullptr;
	TopBar* topBar = nullptr;
	SettingsWindow* settingsWindow = nullptr;


	static constexpr const char* JoystickText = "Press joystick to return";

	void loop() override;

	void handleButtonEvent(Button btn, ButtonInput::Action action);
	void buildUI();
	void switchTheme(const Theme &newTheme);
};

#endif //BUTTERBOTCTRL_FIRMWARE_SETTINGSSCREEN_H