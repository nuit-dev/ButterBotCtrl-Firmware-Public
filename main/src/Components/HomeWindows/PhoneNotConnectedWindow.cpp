#include "PhoneNotConnectedWindow.h"

PhoneNotConnectedWindow::PhoneNotConnectedWindow(lv_obj_t* parent, const PhoneNotConnectedData* params)
	: HomeWindow(parent, TITLE){
	buildUI();
}

void PhoneNotConnectedWindow::buildUI(){
	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(Asset::NoPhone));
	lv_obj_set_style_pad_all(img, 4, 0);

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Sentence);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
