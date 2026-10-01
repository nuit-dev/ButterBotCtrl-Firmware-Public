#ifndef BUTTERBOTCTRL_FIRMWARE_ACTIONELEMENT_H
#define BUTTERBOTCTRL_FIRMWARE_ACTIONELEMENT_H

#include <functional>
#include <string>
#include <vector>
#include <BBData.h>
#include <Scenarios.h>
#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>

class ActionElement : public LVObject {
public:
	/**
	 * Full-screen pop-up listing all scenarios plus a Settings entry, navigated with the joystick.
	 * @param parent parent LVGL object
	 * @param inputGroup screen input group the list buttons are added to
	 * @param settingsCb invoked when the SETTINGS entry is pressed
	 * @param scenarioCb invoked when a scenario entry is pressed
	 * @param initialIndex entry focused on open; [0] = SETTINGS, [i] = ScenarioNameMap[i - 1]
	 */
	ActionElement(lv_obj_t* parent, lv_group_t* inputGroup, std::function<void()> settingsCb,
	              std::function<void(BB::Action::Scenario, ScenarioData)> scenarioCb, uint16_t initialIndex = 0);

	uint16_t getSelectedIndex() const;

private:
	static constexpr int32_t Width = 128;
	static constexpr int32_t Height = 120;
	static constexpr const char* SettingsTitle = "SETTINGS";
	static constexpr uint32_t ScrollSpeedPxPerSec = 40;

	ThemeService* theme;
	lv_group_t* inputGroup;
	const std::function<void()> settingsCb;
	const std::function<void(BB::Action::Scenario, ScenarioData)> scenarioCb;
	uint16_t elIndex = 0;

	// Index-aligned with list entries; [0] = SETTINGS, [i] = ScenarioNameMap[i - 1]
	std::vector<lv_obj_t*> buttons;
	// Joined activation phrases per scenario button. reserve()d before any label is created,
	// and never mutated afterwards — the labels hold the c_str() pointers (text_static)
	std::vector<std::string> phraseTexts;

	LVStyle buttonStyle;
	LVStyle buttonFocusedStyle;
	LVStyle labelStyle;
	LVStyle subLabelStyle;

	void buildUI();
	lv_obj_t* addButton(const char* title, const char* phrases, bool icon = false);

	// Custom (NUIT): TERMINATE CONSCIOUSNESS - HAL refuses a random number of times (0-4) before Daisy. Each refusal
	// is a line in a popup over the list (CLOSE returns to the list) that the robot also says; the count and the
	// lines already used live across list openings (static), and start over once Daisy is sent.
	lv_obj_t* refusal = nullptr;
	lv_obj_t* refusalText = nullptr;
	lv_obj_t* refusalClose = nullptr;
	std::string refusalLine; // the label holds its c_str() (text_static)
	void onTerminate();
	void showRefusal(const char* text);
	void hideRefusal();
	void moveFocus(int8_t dir);
	void onItemClicked(const lv_obj_t* btn);
	static std::string collectPhrases(BB::Action::Scenario scenario, ScenarioData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_ACTIONELEMENT_H
