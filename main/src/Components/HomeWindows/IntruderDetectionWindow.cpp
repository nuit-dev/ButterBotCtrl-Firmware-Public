#include "IntruderDetectionWindow.h"

static constexpr const char* ActivatedSentence = "Intruder detection activated";
static constexpr const char* DeactivatedSentence = "Intruder detection deactivated";
static constexpr const char* MissingSentence = "Intruder detection module missing";

IntruderDetectionWindow::IntruderDetectionWindow(lv_obj_t* parent, const PIRModuleData* params, bool activated)
	: HomeWindow(parent, TITLE){
	buildUI(*params, activated);
}

void IntruderDetectionWindow::buildUI(const PIRModuleData data, const bool activated){
	const char* sentence;
	Asset asset;

	if(data.missing){
		sentence = MissingSentence;
		asset = Asset::ModuleUnplugged;
	}else if(activated){
		sentence = ActivatedSentence;
		asset = Asset::IntruderYes;
	}else{
		sentence = DeactivatedSentence;
		asset = Asset::IntruderNo;
	}

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, sentence);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_pad_all(sentenceLabel, 4, 0);
	lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);

	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(asset));
	lv_obj_set_style_pad_all(img, 4, 0);

	updateLayout();
}
