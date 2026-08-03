#include "UpsideDownWindow.h"

UpsideDownWindow::UpsideDownWindow(lv_obj_t* parent, const UpsideDownData* params) : HomeWindow(parent, TITLE){
	img = lv_image_create(innerContent);
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void UpsideDownWindow::buildUI(const UpsideDownData data){
	// Image
	lv_image_set_src(img, theme->getAsset(Asset::UpsideDown));

	// Sentence label
	lv_label_set_text(sentenceLabel, Sentence);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 42, LV_SIZE_CONTENT);

	updateLayout();
}
