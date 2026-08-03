#include "IR_trainWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "IR_trainWindow";

static const std::map<IR_trainData::State, const char*> ThinkingSentences = {
	{ IR_trainData::State::MaxCapacity, "New info denied" },
	{ IR_trainData::State::Scanning, "Laser eyes engaged" },
	{ IR_trainData::State::ScanDone, "IR success, baby!" },
	{ IR_trainData::State::Listening, "" },
	{ IR_trainData::State::SaveDone, "New power unlocked" },
};

IR_trainWindow::IR_trainWindow(lv_obj_t* parent, const IR_trainData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void IR_trainWindow::onData(const BBData* data){
	const auto newData = *(const IR_trainData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void IR_trainWindow::buildUI(const IR_trainData data){
	if(data.missing){ // Module not inserted or disconnected
		lv_obj_t* img = lv_image_create(innerContent);
		lv_image_set_src(img, theme->getAsset(Asset::ModuleUnplugged));
		lv_obj_set_style_pad_ver(img, 8, 0);

		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::IRModuleMissing, data.id).c_str());
		lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

		updateLayout();
		return;
	}

	switch(data.state){
		case IR_trainData::State::Listening: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::Listening));
			break;
		}
		case IR_trainData::State::Scanning: {
			gif = new LVGIF(innerContent, theme->getAsset(Asset::IrScan));
			gif->setLooping(LVGIF::LoopType::On);
			gif->reset();

			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ThinkingSentences.at(IR_trainData::State::Scanning));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 76, LV_SIZE_CONTENT);
			break;
		}
		case IR_trainData::State::ScanDone: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrScanned));

			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ThinkingSentences.at(IR_trainData::State::ScanDone));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 76, LV_SIZE_CONTENT);
			break;
		}
		case IR_trainData::State::MaxCapacity: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrNotSaved));

			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ThinkingSentences.at(IR_trainData::State::MaxCapacity));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 80, LV_SIZE_CONTENT);
			break;
		}
		case IR_trainData::State::SaveDone: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::IrSaved));

			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ThinkingSentences.at(IR_trainData::State::SaveDone));
			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 88, LV_SIZE_CONTENT);
			break;
		}
		default:
			ESP_LOGE(TAG, "IR_train state not recognised");
			break;
	}

	updateLayout();
}

void IR_trainWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
