#include "YouPassButterWindow.h"

#include <Phrases.h>

YouPassButterWindow::YouPassButterWindow(lv_obj_t* parent, const YouPassButterData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void YouPassButterWindow::buildUI(YouPassButterData data){
	lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::YouPassButter, data.id).c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
