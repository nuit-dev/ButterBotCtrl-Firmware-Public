#include "GasConfigStartWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "GasConfigStartWindow";

GasConfigStartWindow::GasConfigStartWindow(lv_obj_t* parent, const GasConfigureStartData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void GasConfigStartWindow::buildUI(const GasConfigureStartData data){
	if (data.id < 0){
		ESP_LOGE(TAG, "Phrase ID not recognised");
		return;
	}

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::GasModuleFirstInsert, data.id).c_str());
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);

	updateLayout();
}
