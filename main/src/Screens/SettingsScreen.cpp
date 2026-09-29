#include "SettingsScreen.h"

#include <Core/Application.h>
#include <Fonts/font.hpp>
#include <LV_Interface/LVGL.h>

#include "HomeScreen.h"

static constexpr const char* TAG = "SettingsScreen";

SettingsScreen::SettingsScreen(){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();
	buttonInput = app->getService<ButtonInput>();
	settings = app->getService<Settings>();

	buttonInput->OnButtonEvent.bind(app->getService<LVGL>(), [this](Enum<int> btn, ButtonInput::Action action) {
		handleButtonEvent((Button)(int)btn, action);
	});

	buildUI();
}

SettingsScreen::~SettingsScreen(){
	const auto app = Application::getApp();
	buttonInput->OnButtonEvent.unbind(app->getService<LVGL>());
}

void SettingsScreen::handleButtonEvent(const Button btn, const ButtonInput::Action action){
	if (btn == Button::Joystick && action == ButtonInput::Action::Release){
		// Custom (NUIT): on DATE / TIME the press edits the robot's clock instead of closing Settings
		if(settingsWindow != nullptr && settingsWindow->onJoystickPress()){
			return;
		}

		// Save current settings
		settings->store();
		// Return to home screen
		transition([]() {
			return std::make_unique<HomeScreen>();
		});
	}
}

void SettingsScreen::loop(){
	topBar->loop();
	if(settingsWindow != nullptr) settingsWindow->loop(); // Custom (NUIT): DATE / TIME
}

template<typename T_Window, typename T_Data>
T_Window* initActionWindow(lv_obj_t* parent, const std::vector<uint8_t>& data){
	if(data.size() != sizeof(T_Data)){
		ESP_LOGE(TAG, "Wrong data size for type of action");
		return nullptr;
	}
	auto params = (T_Data*)data.data();
	return new T_Window(parent, params);
}

void SettingsScreen::switchTheme(const Theme& newTheme){
	if (windowContainer != nullptr){
		theme->setTheme(newTheme);

		SettingsStruct currentSet = settings->get();
		currentSet.currentTheme = newTheme;
		settings->set(currentSet);

		lv_obj_clean(*this);
		buildUI();
	}
}

void SettingsScreen::buildUI(){
	const lv_color_t bgColor = theme->getTertiaryColor();

	topBar = new TopBar(*this);
	windowContainer = lv_obj_create(*this);

	settingsWindow = new SettingsWindow(windowContainer, inputGroup, [this](const Theme &newTheme) {
		switchTheme(newTheme);
	});

	// Settings window container
	lv_obj_set_pos(windowContainer, 0, 8);
	lv_obj_set_style_pad_all(windowContainer, 4, 0);
	lv_obj_set_size(windowContainer, 128, 120);
	lv_obj_set_layout(windowContainer, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(windowContainer, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(windowContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_obj_set_style_bg_color(windowContainer, bgColor, 0);
	lv_obj_set_style_bg_opa(windowContainer, LV_OPA_COVER, 0);
	lv_obj_set_style_bg_image_src(windowContainer, theme->getAsset(Asset::Grid), 0);

	// Custom (NUIT): "Press joystick to return" hint and the FCC ID / TELEC footer removed - all six rows fit
}
