#include "EightBallWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "EightBallWindow";

EightBallWindow::EightBallWindow(lv_obj_t* parent, const EightBallData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void EightBallWindow::onData(const BBData* data){
	const auto newData = *(const EightBallData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void EightBallWindow::buildUI(const EightBallData data){
	switch(data.phase){
		case EightBallData::Phase::Listening: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::Listening));
			lv_obj_set_size(img, 76, LV_SIZE_CONTENT);
			break;
		}
		case EightBallData::Phase::Thinking: {
			gif = new LVGIF(innerContent, theme->getAsset(Asset::_8BallThinking));
			gif->setLooping(LVGIF::LoopType::On);
			gif->reset();

			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, ThinkingSentence);

			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 76, LV_SIZE_CONTENT);
			break;
		}
		case EightBallData::Phase::Response: {
			lv_obj_t* sentenceLabel = lv_label_create(innerContent);
			lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::EightBall, data.responseId).c_str());

			lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
			lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
			lv_obj_set_size(sentenceLabel, 76, LV_SIZE_CONTENT);
			break;
		}
		default:
			ESP_LOGE(TAG, "EightBall phase not recognised");
			break;
	}

	updateLayout();
}

void EightBallWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
