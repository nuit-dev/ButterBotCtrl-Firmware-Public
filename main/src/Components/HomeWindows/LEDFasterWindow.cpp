#include "LEDFasterWindow.h"

static constexpr const char* TAG = "LEDFasterWindow";

LEDFasterWindow::LEDFasterWindow(lv_obj_t* parent, const LEDFasterData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void LEDFasterWindow::buildUI(const LEDFasterData data){
	if(data.missing){
		// Image
		lv_obj_t* img = lv_image_create(innerContent);
		lv_image_set_src(img, theme->getAsset(Asset::ModuleUnplugged));
		lv_obj_set_style_pad_all(img, 4, 0);

		// Sentence label
		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::LEDModuleMissingPhrases, data.id).c_str());
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
	} else{
		// GIF
		gif = new LVGIF(innerContent, theme->getAsset(Asset::LedFaster));
		gif->setLooping(LVGIF::LoopType::On);
		gif->reset();
		lv_obj_set_style_margin_ver(*gif, 8, 0);

		// Sentence label
		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, Sentence);
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
	}

	updateLayout();
}

void LEDFasterWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
