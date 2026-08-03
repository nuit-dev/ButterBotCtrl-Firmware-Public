#include "TopBar.h"
#include <LV_Interface/LVGL.h>
#include "BBData.h"

TopBar::TopBar(lv_obj_t* parent) : LVObject(parent){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();
	battery = app->getService<Battery>();
	robotState = app->getService<RobotState>();

	buildUI();

	battery->OnLevelChanged.bind(app->getService<LVGL>(), [this](Battery::Level val) {
		ctrlBattery->setLevel((uint8_t)val);
	});

	com = app->getService<Com>();

	com->onData.bind(app->getService<LVGL>(), [this](const BB::State state, const BB::Action action, const std::vector<uint8_t>& data) {
		if(state == BB::State::Idle && action.idle == BB::Action::Idle::BatteryLevel){
			updateBotBattery(data);
		} else if(state == BB::State::Idle && action.idle == BB::Action::Idle::ShutUp){
			updateShutUpEl();
		}
	});
}

TopBar::~TopBar(){
	battery->OnLevelChanged.unbind(Application::getApp()->getService<LVGL>());

	//This will unbind every other UI-related registration of Com's event, since LVGL is the only Object in the UI system.
	//However, TopBar will not be dynamically constructed. It always exists with it's parent screen.
	com->onData.unbind(Application::getApp()->getService<LVGL>());
}

void TopBar::updateBotBattery(const std::vector<uint8_t>& data) const{
	if(data.size() != sizeof(BatteryLevelData)){
		return;
	}

	const auto levelData = (BatteryLevelData*)data.data();

	botBattery->setLevel(levelData->level);
	botBattery->setChargeStatus(levelData->charge);
}

void TopBar::updateShutUpEl() const{
	shutupEl->setMutedStatus(robotState->isMuted());
}

void TopBar::loop(){
	// Charging animation runs on its own (200ms) cadence, independent of the blink gate.
	botBattery->loop();
	ctrlBattery->loop();

	const uint64_t currentMillis = millis();
	if (currentMillis - lastLoopMillis < 500) return;
	lastLoopMillis = currentMillis;

	const uint8_t currentCtrlBat = (uint8_t)battery->getLevel();
	const uint8_t currentBotBat = robotState->getBotBatteryLevel();

	// Critical battery blinking (skip the bot while charging - the charge animation owns its image)
	if (currentBotBat <= 1 && robotState->getChargeState() == ChargeStatus::Unplugged){
		botBattery->blink();
	}
	if (currentCtrlBat <= 1){
		ctrlBattery->blink();
	}
}

void TopBar::buildUI(){
	const lv_color_t color = theme->getPrimaryColor();

	ctrlBattery = new BatteryElement(*this, (uint8_t)battery->getLevel());
	botBattery = new BatteryElement(*this, robotState->getBotBatteryLevel());
	botBattery->setChargeStatus(robotState->getChargeState());
	signalEl = new SignalElement(*this, 2);
	shutupEl = new ShutUpElement(*this, robotState->isMuted());

	ctrlIcon = lv_image_create(*this);
	botIcon = lv_image_create(*this);
	lv_image_set_src(ctrlIcon, theme->getAsset(Asset::BatteryController));
	lv_image_set_src(botIcon, theme->getAsset(Asset::BatteryButter));

	lv_obj_set_pos(*botBattery, 2, 1);
	lv_obj_set_pos(botIcon, 15, 1);
	lv_obj_set_pos(*shutupEl, 24, 1);
	lv_obj_set_pos(*signalEl, 59, 1);
	lv_obj_set_pos(*ctrlBattery, 109, 1);
	lv_obj_set_pos(ctrlIcon, 122, 1);

	lv_obj_set_size(*this, lv_pct(100), 8);

	lv_obj_set_style_bg_color(*this, color, 0);
	lv_obj_set_style_bg_color(*botBattery, color, 0);
	lv_obj_set_style_bg_color(*ctrlBattery, color, 0);
	lv_obj_set_style_bg_color(*signalEl, color, 0);
	lv_obj_set_style_bg_color(*shutupEl, color, 0);
}
