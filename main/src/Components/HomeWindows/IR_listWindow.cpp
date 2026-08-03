#include "IR_listWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "IR_listWindow";

static const std::map<IR_listData::State, const char*> ListSentences = {
	{ IR_listData::State::NoActions, "Blank slate, smooth brain" },
	{ IR_listData::State::ListingActions, "Behold my IR powers" },
};

IR_listWindow::IR_listWindow(lv_obj_t* parent, const IR_listData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void IR_listWindow::onData(const BBData* data){
	const auto newData = *(const IR_listData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void IR_listWindow::buildUI(const IR_listData data){
	switch(data.state){
		case IR_listData::State::NoActions: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrNoActions));
			lv_obj_set_style_pad_all(img, 12, 0);
			break;
		}
		case IR_listData::State::ListingActions: {
			gif = new LVGIF(innerContent, theme->getAsset(Asset::IrList));
			lv_obj_set_style_margin_all(*gif, 8, 0);
			gif->setLooping(LVGIF::LoopType::On);
			gif->reset();
			break;
		}
		default:
			ESP_LOGE(TAG, "IR_list state not recognised");
			break;
	}

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, ListSentences.at(data.state));
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);

	updateLayout();
}

void IR_listWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
