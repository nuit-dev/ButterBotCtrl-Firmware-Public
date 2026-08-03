#include "FaceForgetWindow.h"

static constexpr const char* TAG = "FaceForgetWindow";

static const std::map<FaceForgetData::State, Asset> FaceForgetAssetImgMap = {
	{ FaceForgetData::State::Done, Asset::FaceIzbrisano },
	{ FaceForgetData::State::NoDataExist, Asset::FaceNemaspremljeno },
};

static const std::map<FaceForgetData::State, const char*> FaceForgetSentences = {
	{ FaceForgetData::State::Done, "Void where love was" },
	{ FaceForgetData::State::NoDataExist, "Forget... What?" },
};

FaceForgetWindow::FaceForgetWindow(lv_obj_t* parent, const FaceForgetData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void FaceForgetWindow::onData(const BBData* data){
	const auto newData = *(const FaceForgetData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void FaceForgetWindow::buildUI(const FaceForgetData data){
	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, FaceForgetSentences.at(data.state));
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 116, LV_SIZE_CONTENT);

	switch(data.state){
		case FaceForgetData::State::Done:
		case FaceForgetData::State::NoDataExist: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(FaceForgetAssetImgMap.at(data.state)));
			lv_obj_set_style_pad_bottom(img, 20, 0);
			break;
		}
		default:
			ESP_LOGE(TAG, "Face forget state not recognised");
			break;
	}

	updateLayout();
}
