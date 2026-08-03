#include "HoldPopupWindow.h"
#include <Util/stdafx.h>

HoldPopupWindow::HoldPopupWindow(lv_obj_t* parent, const char* title, const uint64_t fillDurationMs) :
	HomeWindow(parent, title), fillDurationMs(fillDurationMs){
	buildUI();
	createdTime = millis();
}

void HoldPopupWindow::loop(){
	if(loadBar == nullptr){
		return;
	}

	uint64_t elapsed = millis() - createdTime;
	if(elapsed > fillDurationMs){
		elapsed = fillDurationMs;
	}

	lv_bar_set_value(loadBar, (int32_t)(elapsed * 100 / fillDurationMs), LV_ANIM_OFF);
	lv_obj_update_flag(*this, LV_OBJ_FLAG_HIDDEN, elapsed >= fillDurationMs);
}

bool HoldPopupWindow::isBarFull() const{
	return loadBar != nullptr && millis() - createdTime >= fillDurationMs;
}

void HoldPopupWindow::showHint(const char* text){
	// Freeze the inner content at its current size so the pop-up doesn't resize to the hint
	lv_obj_update_layout(*this);
	lv_obj_set_size(innerContent, lv_obj_get_width(innerContent), lv_obj_get_height(innerContent));

	if(loadBar != nullptr){
		lv_obj_delete(loadBar);
		loadBar = nullptr;
	}

	lv_obj_remove_flag(*this, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t* hintLabel = lv_label_create(innerContent);
	lv_label_set_text(hintLabel, text);
	lv_obj_add_style(hintLabel, labelDefaultStyle, 0);
	lv_obj_set_width(hintLabel, LV_SIZE_CONTENT);
}

void HoldPopupWindow::buildUI(){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	loadBar = lv_bar_create(innerContent);
	lv_bar_set_range(loadBar, 0, 100);
	lv_bar_set_value(loadBar, 0, LV_ANIM_OFF);

	lv_obj_set_size(loadBar, 80, 8);
	lv_obj_set_style_margin_hor(loadBar, 1, LV_PART_MAIN);
	lv_obj_set_style_margin_ver(loadBar, 3, LV_PART_MAIN);
	lv_obj_set_style_pad_all(loadBar, 1, LV_PART_MAIN);
	lv_obj_set_style_border_width(loadBar, 1, LV_PART_MAIN);
	lv_obj_set_style_border_color(loadBar, colorPrim, LV_PART_MAIN);
	lv_obj_set_style_bg_color(loadBar, colorTert, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(loadBar, LV_OPA_COVER, LV_PART_MAIN);
	lv_obj_set_style_bg_color(loadBar, colorPrim, LV_PART_INDICATOR);
	lv_obj_set_style_bg_opa(loadBar, LV_OPA_COVER, LV_PART_INDICATOR);

	updateLayout();
}
