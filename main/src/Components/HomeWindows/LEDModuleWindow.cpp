#include "LEDModuleWindow.h"

LEDModuleWindow::LEDModuleWindow(lv_obj_t* parent, const LEDModuleData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void LEDModuleWindow::buildUI(const LEDModuleData data){
	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(data.missing ? Asset::ModuleUnplugged : Asset::ModuleLed));
	lv_obj_set_style_pad_ver(img, 8, 0);

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, data.missing ? Phrases::mapShown(Phrase::LEDModuleMissingPhrases, data.id).c_str() : Sentence);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
