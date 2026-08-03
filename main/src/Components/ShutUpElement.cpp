#include "ShutUpElement.h"

ShutUpElement::ShutUpElement(lv_obj_t* parent, const bool muted) : LVObject(parent), isMuted(muted){
	theme = Application::getApp()->getService<ThemeService>();
	img = lv_image_create(*this);
	update();
}

void ShutUpElement::setMutedStatus(const bool muted){
	isMuted = muted;
	update();
}

void ShutUpElement::update(){
	const char* assetPath = theme->getAsset(Asset::ShutupMute);
	lv_image_set_src(img, assetPath);
	lv_obj_set_size(*this, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

	lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, !isMuted);
}
