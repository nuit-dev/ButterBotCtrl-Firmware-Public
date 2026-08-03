#include "IR_actionWindow.h"

#include <LV_Interface/LVGL.h>

static constexpr const char* TAG = "IR_actionWindow";

IR_actionWindow::IR_actionWindow(lv_obj_t* parent, const IR_actionData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void IR_actionWindow::onData(const BBData* data){
	const auto newData = *(const IR_actionData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void IR_actionWindow::buildUI(const IR_actionData data){
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(Asset::IrPlay));
	lv_obj_set_style_pad_all(img, 8, 0);

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, "Light magic activated");
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_pad_bottom(sentenceLabel, 8, 0);

	updateLayout();
}
