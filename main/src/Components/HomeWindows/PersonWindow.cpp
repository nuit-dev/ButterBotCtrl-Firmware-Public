#include "PersonWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "PersonWindow";

PersonWindow::PersonWindow(lv_obj_t* parent, const PersonData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void PersonWindow::buildUI(const PersonData data){
	lv_obj_t* img = lv_image_create(innerContent);
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);

	if(data.greetKnown){
		lv_image_set_src(img, theme->getAsset(Asset::FacePrepoznavanje));
		lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PersonOwnerGreeting, data.id).c_str());
	} else{
		lv_image_set_src(img, theme->getAsset(Asset::FaceNeprepoznavanje));
		lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PersonStrangerGreeting, data.id).c_str());
	}

	lv_obj_set_style_pad_ver(img, 6, 0);

	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_pad_bottom(sentenceLabel, 10, 0);

	updateLayout();
}
