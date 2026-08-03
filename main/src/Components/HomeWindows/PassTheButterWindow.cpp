#include "PassTheButterWindow.h"

#include <Phrases.h>

PassTheButterWindow::PassTheButterWindow(lv_obj_t* parent, const PassTheButterData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void PassTheButterWindow::buildUI(const PassTheButterData data){
	lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PassTheButter, data.id).c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
