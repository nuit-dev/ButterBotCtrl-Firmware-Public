#include "IR_forgetWindow.h"

#include <Phrases.h>
#include <LV_Interface/LVGL.h>
#include <LV_Interface/LVGIF.h>

static constexpr const char* TAG = "IR_forgetWindow";

static const std::map<IR_forgetData::State, const char*> ForgetSentences = {
	{ IR_forgetData::State::Listening, "" },
	{ IR_forgetData::State::NotRecognised, "IR confusion" },
	{ IR_forgetData::State::Done, "Memory purged" },
	{ IR_forgetData::State::Empty, "Nothing to forget" },
};

IR_forgetWindow::IR_forgetWindow(lv_obj_t* parent, const IR_forgetData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void IR_forgetWindow::onData(const BBData* data){
	const auto newData = *(const IR_forgetData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void IR_forgetWindow::buildUI(const IR_forgetData data){
	switch(data.state){
		case IR_forgetData::State::Listening: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::Listening));
			lv_obj_set_style_pad_all(img, 12, 0);
			break;
		}
		case IR_forgetData::State::NotRecognised: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrUnknown));
			lv_obj_set_style_pad_all(img, 8, 0);

			// Sentence label
			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ForgetSentences.at(data.state));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
			break;
		}
		case IR_forgetData::State::Empty: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrNoActions));
			lv_obj_set_style_pad_all(img, 8, 0);

			// Sentence label
			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ForgetSentences.at(data.state));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
			break;
		}
		case IR_forgetData::State::Done: {
			gif = new LVGIF(innerContent, theme->getAsset(Asset::IrDelete));
			lv_obj_set_style_margin_all(*gif, 8, 0);
			gif->setLooping(LVGIF::LoopType::On);
			gif->reset();

			// Sentence label
			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ForgetSentences.at(data.state));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
			break;
		}
		default:
			ESP_LOGE(TAG, "IR_forget state not recognised");
			break;
	}

	updateLayout();
}

void IR_forgetWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
