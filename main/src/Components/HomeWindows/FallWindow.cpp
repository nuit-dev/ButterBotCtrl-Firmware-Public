#include "FallWindow.h"

FallWindow::FallWindow(lv_obj_t* parent, const FallData* params) : HomeWindow(parent, TITLE){
	img = lv_image_create(innerContent);
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void FallWindow::buildUI(const FallData data){
	// Image
	lv_image_set_src(img, theme->getAsset(Asset::Fall));

	// Sentence label
	lv_label_set_text(sentenceLabel, Sentence);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
