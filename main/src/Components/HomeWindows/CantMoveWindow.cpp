#include "CantMoveWindow.h"

CantMoveWindow::CantMoveWindow(lv_obj_t* parent, const CantMoveData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void CantMoveWindow::buildUI(const CantMoveData data){
	const bool charging = data.reason == CantMoveData::Reason::Charging;

	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(charging ? Asset::BatteryCharging : Asset::Fall));
	lv_obj_set_style_pad_all(img, 4, 0);

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text_static(sentenceLabel, charging ? SentenceCharging : SentenceUnstable);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
