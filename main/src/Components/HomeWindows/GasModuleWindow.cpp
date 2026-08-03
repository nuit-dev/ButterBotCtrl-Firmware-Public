#include "GasModuleWindow.h"
#include <Phrases.h>
#include "Fonts/font.hpp"

GasModuleWindow::GasModuleWindow(lv_obj_t* parent, const GasData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void GasModuleWindow::buildUI(const GasData data){
	Asset asset;
	Phrase phrase;

	if(data.missing){ // Module not inserted or disconnected
		asset = Asset::ModuleUnplugged;
		phrase = Phrase::GasModuleMissing;
	}else if(data.ok){
		asset = Asset::AirGood;
		phrase = Phrase::AirQualityOK;
	}else{
		asset = Asset::AirBad;
		phrase = Phrase::AirQualityBad;
	}

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Phrases::mapShown(phrase, data.id).c_str());
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_pad_all(sentenceLabel, 4, 0);
	lv_obj_set_size(sentenceLabel, 110, LV_SIZE_CONTENT);

	// Cap the sentence at MaxSentenceLines
	const int32_t lineH = lv_font_get_line_height(&lv_font_butter);
	const int32_t lineSpace = lv_obj_get_style_text_line_space(sentenceLabel, 0);
	const int32_t padVer = lv_obj_get_style_pad_top(sentenceLabel, 0) + lv_obj_get_style_pad_bottom(sentenceLabel, 0);
	const int32_t maxH = MaxSentenceLines * lineH + (MaxSentenceLines - 1) * lineSpace + padVer;

	lv_obj_update_layout(sentenceLabel);
	if(lv_obj_get_height(sentenceLabel) > maxH){
		lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_DOT);
		lv_obj_set_height(sentenceLabel, maxH);
	}

	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(asset));
	lv_obj_set_style_pad_all(img, 4, 0);

	updateLayout();
}

void GasModuleWindow::onData(const BBData* data){
	lv_obj_clean(innerContent);
	buildUI(*(const GasData*)data);
}
