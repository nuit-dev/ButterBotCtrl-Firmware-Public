#include "PokeWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "PokeWindow";

PokeWindow::PokeWindow(lv_obj_t* parent, const PokeData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void PokeWindow::buildUI(const PokeData data){
	switch(data.level){
		case PokeData::Level::One:
			lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PokeLvl1, data.id).c_str());
			break;
		case PokeData::Level::Two:
			lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PokeLvl2, data.id).c_str());
			break;
		case PokeData::Level::Three:
			lv_label_set_text(sentenceLabel, Phrases::mapShown(Phrase::PokeLvl3, data.id).c_str());
			break;
		default:
			ESP_LOGE(TAG, "Poke level not recognised");
			break;
	}

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);

	updateLayout();
}
