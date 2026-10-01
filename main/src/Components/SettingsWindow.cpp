#include "SettingsWindow.h"

#include "Fonts/font.hpp"
#include "Services/Com.h"
#include "Services/RobotState.h"
#include <Util/stdafx.h>

SettingsWindow::SettingsWindow(lv_obj_t* parent, lv_group_t* inputGroup, const std::function<void(const Theme& newTheme)>& themeCb)
	: LVObject(parent), inputGroup(inputGroup), themeCb(themeCb){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();
	settings = app->getService<Settings>();
	ledService = app->getService<LED<LEDs, RGB_LEDs>>();

	buildUI(parent);
}

SettingsWindow::~SettingsWindow(){
	// Custom (NUIT): the blink animation runs on a row label
	for(lv_obj_t* label : rowLabels){
		if(label != nullptr) lv_anim_delete(label, nullptr);
	}
}

void SettingsWindow::buildUI(lv_obj_t* parent){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	SettingsStruct initSet = settings->get();

	titleEl = lv_obj_create(*this);
	titleLabel = lv_label_create(titleEl);
	lv_label_set_text(titleLabel, WindowTitle);

	corner = lv_image_create(*this);
	lv_obj_add_flag(corner, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_add_flag(corner, LV_OBJ_FLAG_FLOATING);
	innerContent = lv_obj_create(*this);

	// CONTAINER
	lv_obj_set_size(*this, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_layout(*this, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(*this, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(*this, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_remove_flag(*this, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_margin_top(*this, 2, 0);

	lv_obj_set_style_border_width(innerContent, 1, 0);
	lv_obj_set_style_border_color(innerContent, colorPrim, 0);
	lv_obj_set_style_bg_color(*this, colorTert, 0);
	lv_obj_set_style_bg_image_src(*this, theme->getAsset(Asset::Grid), 0);

	// INNER WINDOW
	lv_obj_set_size(innerContent, WindowWidth, WindowHeight);
	lv_obj_set_style_bg_color(innerContent, colorTert, 0);
	lv_obj_set_style_bg_opa(innerContent, LV_OPA_COVER, 0);
	// Custom (NUIT): 12 rows scroll (by code, see scrollToRow); a 2 px bar at the right edge shows where you are.
	// pad_bottom 1 lets the last row scroll up to 1 px above the bottom border, like the first row below the top.
	lv_obj_add_flag(innerContent, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_scroll_dir(innerContent, LV_DIR_VER);
	lv_obj_set_scrollbar_mode(innerContent, LV_SCROLLBAR_MODE_ON);
	lv_obj_set_style_pad_bottom(innerContent, 1, 0);
	lv_obj_set_style_width(innerContent, 2, LV_PART_SCROLLBAR);
	lv_obj_set_style_radius(innerContent, 0, LV_PART_SCROLLBAR);
	lv_obj_set_style_bg_color(innerContent, colorPrim, LV_PART_SCROLLBAR);
	lv_obj_set_style_bg_opa(innerContent, LV_OPA_COVER, LV_PART_SCROLLBAR);
	lv_obj_set_style_pad_right(innerContent, 2, LV_PART_SCROLLBAR);
	lv_obj_set_style_pad_top(innerContent, 2, LV_PART_SCROLLBAR);
	lv_obj_set_style_pad_bottom(innerContent, 2, LV_PART_SCROLLBAR);

	// TITLE
	lv_obj_set_size(titleEl, LV_SIZE_CONTENT, 6);
	lv_obj_set_layout(titleEl, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(titleEl, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(titleEl, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_obj_set_style_pad_hor(titleEl, 1, 0);
	lv_obj_set_style_text_color(titleEl, colorTert, 0);
	lv_obj_set_style_text_font(titleEl, &lv_font_butter, 0);
	lv_obj_set_style_bg_color(titleEl, colorPrim, 0);
	lv_obj_set_style_bg_opa(titleEl, LV_OPA_COVER, 0);

	lv_obj_set_style_pad_right(titleLabel, 6, 0);
	lv_obj_set_style_pad_left(titleLabel, 1, 0);

	// LABEL DEFAULT STYLE
	lv_style_set_text_font(labelDefaultStyle, &lv_font_butter);
	lv_style_set_text_line_space(labelDefaultStyle, 3);
	lv_style_set_text_color(labelDefaultStyle, colorPrim);
	lv_style_set_size(labelDefaultStyle, 80, LV_SIZE_CONTENT);
	lv_style_set_pad_all(labelDefaultStyle, 2);
	lv_style_set_bg_opa(labelDefaultStyle, LV_OPA_TRANSP);

	// CORNER
	lv_obj_set_size(corner, 6, 6);
	lv_obj_set_style_bg_color(corner, colorTert, 0);
	lv_obj_set_style_bg_opa(corner, LV_OPA_COVER, 0);
	lv_image_set_src(corner, theme->getAsset(Asset::Corner));

	/************************ CONTENT ******************************/
	// Blink animation
	lv_anim_init(&blinkAnim);
	lv_anim_set_duration(&blinkAnim, 300);
	lv_anim_set_playback_duration(&blinkAnim, 300);
	lv_anim_set_repeat_count(&blinkAnim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&blinkAnim, lv_anim_path_ease_in_out);
	lv_anim_set_values(&blinkAnim, LV_OPA_COVER, LV_OPA_0);
	lv_anim_set_exec_cb(&blinkAnim, [](void* var, const int32_t v) {
		lv_obj_set_style_opa((lv_obj_t*)var, v, 0);
	});

	// Widget switch callback
	auto switchCb = [this](const uint32_t key) {
		if(key == LV_KEY_DOWN){
			focusRow((currentFocusIndex + 1) % RowCount);
		} else{
			focusRow((currentFocusIndex + RowCount - 1) % RowCount);
		}
	};

	// Theme selector
	themeSelector = new ThemeSelector(innerContent, switchCb, themeCb);
	lv_obj_set_pos(*themeSelector, 2, RowY[0]); // Custom (NUIT): rows moved up
	lv_group_add_obj(inputGroup, *themeSelector);
	// Set init animation
	lv_anim_set_var(&blinkAnim, themeSelector->widgetLabel);
	lv_anim_start(&blinkAnim);

	// Sleep timeout selector
	auto sleepValCb = [this](const InactivityTimeout timeout) {
		SettingsStruct currentSet = settings->get();
		currentSet.inactivityTimeout = timeout;
		settings->set(currentSet);
	};
	sleepSelector = new SleepSelector(innerContent, initSet.inactivityTimeout, switchCb, sleepValCb);
	lv_obj_set_pos(*sleepSelector, 2, RowY[1]);
	lv_group_add_obj(inputGroup, *sleepSelector);

	// Brightness slider
	auto brightnessValCb = [this](const float value) {
		ledService->on(LEDs::TftBacklight, value);
		SettingsStruct currentSet = settings->get();
		currentSet.screenBrightness = value;
		settings->set(currentSet);
	};
	brightnessSlider = new BrightnessSlider(innerContent, initSet.screenBrightness, brightnessValCb, switchCb);
	lv_obj_set_pos(*brightnessSlider, 2, RowY[2]);
	lv_group_add_obj(inputGroup, *brightnessSlider);

	// Custom (NUIT): proximity sensor filter, sent to the robot right away and on every connect
	auto sensorValCb = [this](const SensorMode mode) {
		settings->setSensorMode(mode);
		if(Com* com = Application::getApp()->getService<Com>()){
			com->setSensorCommand(sensorModeToCommand(mode));
		}
	};
	sensorSelector = new SensorSelector(innerContent, settings->getSensorMode(), switchCb, sensorValCb);
	lv_obj_set_pos(*sensorSelector, 2, RowY[3]);
	lv_group_add_obj(inputGroup, *sensorSelector);

	// Custom (NUIT): ROAMING - OFF keeps the robot in place while idle (part of the robot config)
	roamingSelector = new RoamingSelector(innerContent, settings->getRobotConfig().roaming != 0, switchCb, [this](const bool roaming) {
		RobotConfigData config = settings->getRobotConfig();
		config.roaming = roaming ? 1 : 0;
		sendRobotConfig(config);
	});
	lv_obj_set_pos(*roamingSelector, 2, RowY[4]);
	lv_group_add_obj(inputGroup, *roamingSelector);

	// Custom (NUIT): robot TTS voice preset, sent right away and on every connect
	auto voiceValCb = [this](const VoiceMode mode) {
		settings->setVoiceMode(mode);
		if(Com* com = Application::getApp()->getService<Com>()){
			com->setVoiceCommand(voiceModeToCommand(mode));
		}
	};
	voiceSelector = new VoiceSelector(innerContent, settings->getVoiceMode(), switchCb, voiceValCb);
	lv_obj_set_pos(*voiceSelector, 2, RowY[5]);
	lv_group_add_obj(inputGroup, *voiceSelector);

	// Custom (NUIT): startup speed, read at the next boot
	auto fastStartValCb = [this](const FastStart level) {
		settings->setFastStart(level);
	};
	fastStartSelector = new FastStartSelector(innerContent, settings->getFastStart(), switchCb, fastStartValCb);
	lv_obj_set_pos(*fastStartSelector, 2, RowY[6]);
	lv_group_add_obj(inputGroup, *fastStartSelector);

	// Custom (NUIT): robot volume and night mode, sent right away and on every connect
	const RobotConfigData robotConfig = settings->getRobotConfig();
	volumeSlider = new PercentSlider(innerContent, "VOLUME", 53, robotConfig.volume, [this](const uint8_t percent) {
		RobotConfigData config = settings->getRobotConfig();
		config.volume = percent;
		sendRobotConfig(config);
	}, switchCb);
	lv_obj_set_pos(*volumeSlider, 2, RowY[7]);
	lv_group_add_obj(inputGroup, *volumeSlider);

	nightModeSelector = new NightModeSelector(innerContent, static_cast<NightMode>(robotConfig.nightMode), switchCb, [this](const NightMode mode) {
		RobotConfigData config = settings->getRobotConfig();
		config.nightMode = static_cast<uint8_t>(mode);
		sendRobotConfig(config);
	});
	lv_obj_set_pos(*nightModeSelector, 2, RowY[8]);
	lv_group_add_obj(inputGroup, *nightModeSelector);

	nightVolumeSlider = new PercentSlider(innerContent, "NIGHT VOLUME", 61, robotConfig.nightVolume, [this](const uint8_t percent) {
		RobotConfigData config = settings->getRobotConfig();
		config.nightVolume = percent;
		sendRobotConfig(config);
	}, switchCb);
	lv_obj_set_pos(*nightVolumeSlider, 2, RowY[9]);
	lv_group_add_obj(inputGroup, *nightVolumeSlider);

	// Custom (NUIT): the robot's clock - joystick press edits it (onJoystickPress)
	dateRow = new DateTimeRow(innerContent, DateTimeRow::Kind::Date, switchCb);
	lv_obj_set_pos(*dateRow, 2, RowY[DateRowIndex]);
	lv_group_add_obj(inputGroup, *dateRow);

	timeRow = new DateTimeRow(innerContent, DateTimeRow::Kind::Time, switchCb);
	lv_obj_set_pos(*timeRow, 2, RowY[TimeRowIndex]);
	lv_group_add_obj(inputGroup, *timeRow);

	rowObjs = { *themeSelector, *sleepSelector, *brightnessSlider, *sensorSelector, *roamingSelector, *voiceSelector, *fastStartSelector,
				*volumeSlider, *nightModeSelector, *nightVolumeSlider, *dateRow, *timeRow };
	rowLabels = { themeSelector->widgetLabel, sleepSelector->widgetLabel, brightnessSlider->widgetLabel, sensorSelector->widgetLabel,
				  roamingSelector->widgetLabel, voiceSelector->widgetLabel, fastStartSelector->widgetLabel, volumeSlider->widgetLabel,
				  nightModeSelector->widgetLabel, nightVolumeSlider->widgetLabel, dateRow->widgetLabel, timeRow->widgetLabel };
	refreshClock();

	lv_group_set_editing(inputGroup, true);

	updateLayout();
}

void SettingsWindow::focusRow(const int32_t index){
	for(lv_obj_t* label : rowLabels){
		lv_anim_delete(label, nullptr);
		lv_obj_set_style_opa(label, LV_OPA_COVER, 0);
	}

	currentFocusIndex = index;
	lv_anim_set_var(&blinkAnim, rowLabels[index]);
	lv_anim_start(&blinkAnim);
	lv_group_focus_obj(rowObjs[index]);
	scrollToRow(index);
}

void SettingsWindow::scrollToRow(const int32_t index){
	// Keep the focused row fully visible with 1 px to the border, scrolling as little as possible
	lv_obj_update_layout(innerContent);
	const int32_t top = RowY[index];
	const int32_t bottom = top + lv_obj_get_height(rowObjs[index]);
	const int32_t visible = WindowHeight - 2; // inside the 1 px border

	int32_t y = lv_obj_get_scroll_y(innerContent);
	if(top - y < 1){
		y = top - 1;
	}else if(bottom - y > visible - 1){
		y = bottom - (visible - 1);
	}
	if(y < 0) y = 0;
	lv_obj_scroll_to_y(innerContent, y, LV_ANIM_ON);
}

void SettingsWindow::sendRobotConfig(const RobotConfigData& config){
	settings->setRobotConfig(config);
	if(Com* com = Application::getApp()->getService<Com>()){
		com->setRobotConfig(config);
	}
}

DateTimeRow::Value SettingsWindow::clockNow(bool& known) const{
	DateTimeRow::Value value;
	tm now = {};
	const RobotState* robotState = Application::getApp()->getService<RobotState>();
	known = robotState != nullptr && robotState->getRobotTime(now);
	if(known){
		value.year = now.tm_year + 1900;
		value.month = now.tm_mon + 1;
		value.day = now.tm_mday;
		value.hour = now.tm_hour;
		value.minute = now.tm_min;
	}
	return value;
}

void SettingsWindow::refreshClock(){
	bool known = false;
	const DateTimeRow::Value now = clockNow(known);
	dateRow->show(known, now);
	timeRow->show(known, now);
}

void SettingsWindow::loop(){
	if(millis() - lastClockRefresh < 500) return;
	lastClockRefresh = millis();
	refreshClock();
}

bool SettingsWindow::onJoystickPress(){
	DateTimeRow* row = currentFocusIndex == DateRowIndex ? dateRow : currentFocusIndex == TimeRowIndex ? timeRow : nullptr;
	if(row == nullptr) return false;

	if(!row->isEditing()){
		bool known = false;
		editBase = clockNow(known); // unknown clock: starts from the defaults (01.01.2026 12:00)
		row->startEdit(editBase);
		return true;
	}

	const DateTimeRow::Value edited = row->finishEdit();
	bool known = false;
	DateTimeRow::Value value = clockNow(known);
	if(!known) value = editBase;
	if(row == dateRow){
		value.year = edited.year;
		value.month = edited.month;
		value.day = edited.day;
	}else{
		value.hour = edited.hour;
		value.minute = edited.minute;
	}

	if(Com* com = Application::getApp()->getService<Com>()){
		com->sendSetTime(SetTimeData{ value.year, value.month, value.day, value.hour, value.minute });
	}
	return true;
}

void SettingsWindow::updateLayout(){
	lv_obj_set_size(titleEl, WindowWidth, 6);
	lv_obj_set_width(*this, WindowWidth);
	lv_obj_align_to(corner, *this, LV_ALIGN_TOP_RIGHT, 0, 0);
};
