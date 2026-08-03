#include "SignalElement.h"

#include <algorithm>

SignalElement::SignalElement(lv_obj_t* parent, const uint8_t level) : LVObject(parent){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();
	img = lv_image_create(*this);

	setLevel(level);

	lv_obj_set_size(*this, 10, 6);
}

void SignalElement::setLevel(uint8_t level){
	level = std::clamp(level, minLevel, maxLevel);
	const char* assetPath = theme->getAsset(SigLevels[level]);
	lv_image_set_src(img, assetPath);
}
