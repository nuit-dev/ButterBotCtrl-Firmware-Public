#include "ObserveWindow.h"

#include <Phrases.h>

ObserveWindow::ObserveWindow(lv_obj_t* parent, const ObserveData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void ObserveWindow::buildUI(const ObserveData data){
	lv_label_set_text(sentenceLabel, Phrases::mapShown(data.phrase, data.id).c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
