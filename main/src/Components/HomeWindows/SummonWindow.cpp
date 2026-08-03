#include "SummonWindow.h"

#include <LV_Interface/LVGL.h>
#include <Util/stdafx.h>

static constexpr const char* TAG = "Summon";

SummonWindow::SummonWindow(lv_obj_t* parent, const SummonData* params) : HomeWindow(parent, TITLE){
	lastMillis = millis();

	buildUI(*params);
}

void SummonWindow::loop(){
	HomeWindow::loop();

	if (progressVal > ProgressBarMax) return;

	const uint64_t newMillis = millis();
	progressVal += newMillis - lastMillis;
	lastMillis = newMillis;
}

void SummonWindow::buildUI(const SummonData data){
	// Distance hint label
	lv_obj_t* hintLabel = lv_label_create(innerContent);
	lv_label_set_text(hintLabel, "Keep face 0.5M from camera");
	lv_label_set_long_mode(hintLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(hintLabel, labelDefaultStyle, 0);
	lv_obj_set_size(hintLabel, 116, LV_SIZE_CONTENT);
	lv_obj_move_to_index(hintLabel, 0);

	// GIF
	gif = new LVGIF(innerContent, theme->getAsset(Asset::FaceScan));
	gif->setLooping(LVGIF::LoopType::On);
	gif->reset();

	updateLayout();
}

void SummonWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
