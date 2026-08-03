#include "VoiceControlWindow.h"

#include <LV_Interface/LVGL.h>

static constexpr const char* TAG = "VoiceControl";

static const std::map<VoiceControlData::Direction, Asset> VoiceControlAssetMap = {
	{ VoiceControlData::Direction::Forward, Asset::ArrowUp },
	{ VoiceControlData::Direction::Backward, Asset::ArrowDown },
	{ VoiceControlData::Direction::Right, Asset::ArrowRight },
	{ VoiceControlData::Direction::Left, Asset::ArrowLeft },
	{ VoiceControlData::Direction::Rotate, Asset::Arrow180 },
};

static const std::map<VoiceControlData::Direction, const char*> VoiceControlSentences = {
	{ VoiceControlData::Direction::Forward, "Onward, butter soldier" },
	{ VoiceControlData::Direction::Backward, "Backwards like my life" },
	{ VoiceControlData::Direction::Right, "Right turn... probably" },
	{ VoiceControlData::Direction::Left, "Leftward wobble" },
	{ VoiceControlData::Direction::Rotate, "Dramatic U turn" },
};

VoiceControlWindow::VoiceControlWindow(lv_obj_t* parent, const VoiceControlData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void VoiceControlWindow::onData(const BBData* data){
	const auto newData = *(const VoiceControlData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void VoiceControlWindow::buildUI(const VoiceControlData data){
	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, VoiceControlSentences.at(data.direction));
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_margin_all(sentenceLabel, 4, 0);
	lv_obj_set_size(sentenceLabel, 108, LV_SIZE_CONTENT);

	// Direction image
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(VoiceControlAssetMap.at(data.direction)));
	lv_obj_set_style_pad_bottom(img, 20, 0);

	updateLayout();
}
