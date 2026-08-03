#include "WanderWindow.h"

#include <Phrases.h>

WanderWindow::WanderWindow(lv_obj_t* parent, const WanderData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void WanderWindow::buildUI(const WanderData data){
	lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::Ramble, Phrases::get(Phrase::Ramble)).c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);

	updateLayout();
}
