#include "IntroScreen.h"
#include "PairingScreen.h"
#include "HomeScreen.h"
#include "Services/Com.h"
#include "Services/LEDController.h"
#include "Services/ThemeService.h"

IntroScreen::IntroScreen(){
	const auto app = Application::getApp();

	// Load the intro GIF archive
	app->getService<ThemeService>()->activateIntroAssets();

	lv_obj_set_style_bg_color(*this, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(*this, LV_OPA_COVER, 0);

	introGif = new LVGIF(*this, "S:/intro");
	introGif->setLoopCallback([this]() {
		// Stop reading GIF frames before fade transition
		lv_obj_add_flag(*introGif, LV_OBJ_FLAG_HIDDEN);

		// Custom (NUIT): FAST START FAST connects during the intro - if it's done, go straight home
		const auto app = Application::getApp();
		const Com* com = app->getService<Com>();
		if(com != nullptr && com->getStatus() == Com::ConnStatus::Connected){
			if(LEDController* leds = app->getService<LEDController>()){
				leds->wifiLedOn();
				leds->bigGreenLedOff();
			}
			transition([](){ return std::make_unique<HomeScreen>(); }, LV_SCR_LOAD_ANIM_FADE_IN);
			return;
		}

		transition([](){ return std::make_unique<PairingScreen>(); }, LV_SCR_LOAD_ANIM_FADE_IN);
	});
	introGif->setLooping(LVGIF::LoopType::Single);
	introGif->reset();
}
