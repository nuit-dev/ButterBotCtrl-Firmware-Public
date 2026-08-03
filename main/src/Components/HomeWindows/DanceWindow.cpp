#include "DanceWindow.h"

static constexpr const char* TAG = "Dance";

DanceWindow::DanceWindow(lv_obj_t* parent, const DanceData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void DanceWindow::buildUI(const DanceData data){
	// GIF
	gif = new LVGIF(innerContent, theme->getAsset(Asset::Dance));
	gif->setLooping(LVGIF::LoopType::On);
	gif->reset();

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, "Rhythm.exe unleashed");
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_margin_top(sentenceLabel, 6, 0);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);

	updateLayout();
}

void DanceWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
