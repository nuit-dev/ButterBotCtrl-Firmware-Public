#include "PairingScreen.h"

#include <LV_Interface/LVGL.h>
#include <LV_Interface/LVGIF.h>

#include "HomeScreen.h"
#include "Fonts/font.hpp"
#include "Services/Settings.h"
#include "Services/ThemeService.h"

PairingScreen::PairingScreen(){
	const auto app = Application::getApp();
	gap = app->getSingleton<BLE::GAP>();
	com = app->getService<Com>();
	ledController = app->getService<LEDController>();

	// Blink Wifi LED
	ledController->wifiLedStrobe();
	ledController->bigGreenLedStrobe();

	ThemeService* theme = app->getService<ThemeService>();
	const Settings* settings = app->getService<Settings>();
	if(settings != nullptr && settings->getFastStart() == FastStart::Overkloking){
		// Custom (NUIT): FAST START OVERKLOKING - no pairing animation. Load the theme now, while BLE connects,
		// so the home screen doesn't have to.
		theme->activateThemeAssets();

		lv_obj_set_style_bg_color(*this, theme->getTertiaryColor(), 0);
		lv_obj_set_style_bg_opa(*this, LV_OPA_COVER, 0);

		lv_obj_t* label = lv_label_create(*this);
		lv_label_set_text_static(label, "CONNECTING...");
		lv_obj_set_style_text_font(label, &lv_font_butter, 0);
		lv_obj_set_style_text_color(label, theme->getPrimaryColor(), 0);
		lv_obj_center(label);
	}else{
		// Load the pairing GIF archive
		theme->activatePairingAssets();

		// Init pairing GIF
		LVGIF* pairingGif = new LVGIF(*this, "S:/pairing");
		pairingGif->setLooping(LVGIF::LoopType::On);
		pairingGif->reset();
	}

	gap->onConnEvent.bind(app->getService<LVGL>(), [this](const BLE::GAP::ConnEvent event) {
		// If connection failed, try again
		if(event == BLE::GAP::ConnEvent::Failed){
			gap->connect();
		}
	});

	com->onConnStatus.bind(app->getService<LVGL>(), [this](const Com::ConnStatus status) {
		// If connection successful, turn on LED and transition to home screen
		if(status == Com::ConnStatus::Connected){
			goHome();
		}
	});

	if(!gap->isConnected() && !gap->isConnecting()) gap->connect();
}

PairingScreen::~PairingScreen(){
	LVGL* lvgl = Application::getApp()->getService<LVGL>();

	gap->onConnEvent.unbind(lvgl);
	com->onConnStatus.unbind(lvgl);
}

void PairingScreen::goHome(){
	if(leaving) return;
	leaving = true;

	ledController->wifiLedOn();
	ledController->bigGreenLedOff();
	transition([]() {
		return std::make_unique<HomeScreen>();
	});
}

void PairingScreen::loop(){
	// Custom (NUIT): already connected (FAST START connected during the intro, before this screen bound its event)
	if(com->getStatus() == Com::ConnStatus::Connected){
		goHome();
		return;
	}

	// Custom (NUIT): (re)connect whenever BLE is idle - after a link that dropped while it was being set up (the
	// robot still held the previous one, e.g. the controller was restarted while connected), or while a connect()
	// waits for BLE privacy (goes ahead by itself after at most 2 s)
	if(!gap->isConnected() && !gap->isConnecting()){
		gap->connect();
	}
}
