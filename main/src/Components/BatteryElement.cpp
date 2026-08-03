#include "BatteryElement.h"

#include <algorithm>
#include <Util/stdafx.h>

BatteryElement::BatteryElement(lv_obj_t* parent, const uint8_t level) : LVObject(parent){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();
	img = lv_image_create(*this);

	setLevel(level);

	lv_obj_set_size(*this, 12, 6);
}

void BatteryElement::setLevel(uint8_t level){
	level = std::clamp(level, MinLevel, MaxLevel);
	currentLevel = level;

	// While charging the animation owns the image; Full stays pinned to the highest level.
	if(charge != ChargeStatus::Unplugged){
		return;
	}

	const char* assetPath = theme->getAsset(BatLevels[level]);
	lv_image_set_src(img, assetPath);

	lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, false);
	isHidden = false;
}

void BatteryElement::setChargeStatus(ChargeStatus status){
	const ChargeStatus prev = charge;
	charge = status;

	switch(status){
		case ChargeStatus::Charging:
			// Restart the animation only when entering the charging state.
			if(prev != ChargeStatus::Charging){
				chargeFrame = 0;
				lastChargeMillis = 0;
			}
			break;
		case ChargeStatus::Full:
			// Pin to the highest level (bypasses setLevel's charging guard).
			lv_image_set_src(img, theme->getAsset(BatLevels[MaxLevel]));
			lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, false);
			isHidden = false;
			break;
		case ChargeStatus::Unplugged:
		default:
			setLevel(currentLevel);
			break;
	}
}

void BatteryElement::blink(){
	isHidden = !isHidden;
	lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, isHidden);
}

void BatteryElement::loop(){
	if(charge != ChargeStatus::Charging){
		return;
	}

	const uint64_t currentMillis = millis();
	if(currentMillis - lastChargeMillis < ChargeFrameMs){
		return;
	}
	lastChargeMillis = currentMillis;

	chargeFrame = (chargeFrame + 1) % (sizeof(ChargeFrames) / sizeof(ChargeFrames[0]));
	lv_image_set_src(img, theme->getAsset(ChargeFrames[chargeFrame]));

	lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, false);
	isHidden = false;
}
