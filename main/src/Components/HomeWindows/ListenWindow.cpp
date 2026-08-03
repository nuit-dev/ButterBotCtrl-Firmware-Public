#include "ListenWindow.h"

#include <LV_Interface/LVGL.h>

static constexpr const char* TAG = "Listen";

static const std::map<ListenData::Phase, const char*> ListenSentences = {
	{ ListenData::Phase::Listening, "Listening... reluctantly" },
	{ ListenData::Phase::NotUnderstand, "Try... human language?" },
	{ ListenData::Phase::Abort, "Silent... but resentful" },
};

ListenWindow::ListenWindow(lv_obj_t* parent, const ListenData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void ListenWindow::onData(const BBData* data){
	const auto newData = *(const ListenData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void ListenWindow::loop(){
	HomeWindow::loop();
}

void ListenWindow::buildUI(const ListenData data){
	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, ListenSentences.at(data.phase));
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);

	switch(data.phase){
		case ListenData::Phase::Listening: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::Listening));
			lv_obj_move_to_index(img, 0);
			lv_obj_set_size(sentenceLabel, 68, LV_SIZE_CONTENT);
			break;
		}
		case ListenData::Phase::NotUnderstand: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::ListeningNotunderstanding));
			lv_obj_move_to_index(img, 0);
			lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);
			break;
		}
		case ListenData::Phase::Abort: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::Mute));
			lv_obj_move_to_index(img, 0);
			lv_obj_set_size(sentenceLabel, 60, LV_SIZE_CONTENT);
			break;
		}
		default:
			ESP_LOGE(TAG, "Listen state not recognised");
			break;
	}

	updateLayout();
}
