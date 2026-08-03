#include "ShakeWindow.h"

static constexpr const char* TAG = "ShakeWindow";

ShakeWindow::ShakeWindow(lv_obj_t* parent, const ShakeData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void ShakeWindow::buildUI(const ShakeData data){
	// GIF control
	gif = new LVGIF(innerContent, theme->getAsset(Asset::Shake));
	gif->setLooping(LVGIF::LoopType::On);
	gif->reset();

	// Sentence label
	sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Sentence);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 76, LV_SIZE_CONTENT);

	updateLayout();
}

void ShakeWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
