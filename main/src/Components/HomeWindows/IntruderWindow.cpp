#include "IntruderWindow.h"

#include <Phrases.h>

static constexpr const char* TAG = "IntruderWindow";

static constexpr const char* DetectedSentence = "Suspicious lifeform spotted!";
static constexpr const char* NotDetectedSentence = "Zone clear!";

IntruderWindow::IntruderWindow(lv_obj_t* parent, const IntruderData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void IntruderWindow::onData(const BBData* data){
	const auto newData = *(const IntruderData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void IntruderWindow::buildUI(const IntruderData data){
	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, data.detected ? DetectedSentence : NotDetectedSentence);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_pad_all(sentenceLabel, 4, 0);
	lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);

	// Image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(data.detected ? Asset::IntruderYes : Asset::IntruderNo));
	lv_obj_set_style_pad_all(img, 4, 0);

	updateLayout();
}
