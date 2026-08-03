#include "RambleWindow.h"

#include <Phrases.h>

RambleWindow::RambleWindow(lv_obj_t* parent, const RambleData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void RambleWindow::buildUI(const RambleData data){
	lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::Ramble, data.id).c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);

	updateLayout();
}
