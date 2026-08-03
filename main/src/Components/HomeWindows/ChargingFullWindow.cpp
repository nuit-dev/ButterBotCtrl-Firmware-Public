#include "ChargingFullWindow.h"

#include <Phrases.h>

ChargingFullWindow::ChargingFullWindow(lv_obj_t* parent, const BatteryData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void ChargingFullWindow::buildUI(const BatteryData data){
	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(Asset::BatteryFull));
	lv_obj_set_style_pad_ver(img, 8, 0);

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Phrases::map(Phrase::BatteryChargingFull, data.id).c_str());
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
	lv_obj_set_style_margin_bottom(sentenceLabel, 6, 0);

	updateLayout();
}
