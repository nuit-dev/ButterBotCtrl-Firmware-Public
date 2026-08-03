#include "TempHumScaleWindow.h"

static std::map<TempHumScale, Asset> tempScaleOffMap = {
	{ TempHumScale::Celsius, Asset::TempC0 },
	{ TempHumScale::Fahrenheit, Asset::TempF0 },
	{ TempHumScale::Kelvin, Asset::TempK0 },
};

static std::map<TempHumScale, Asset> tempScaleOnMap = {
	{ TempHumScale::Celsius, Asset::TempC1 },
	{ TempHumScale::Fahrenheit, Asset::TempF1 },
	{ TempHumScale::Kelvin, Asset::TempK1 },
};

static Asset getScaleAsset(const TempHumScale dataScale, const TempHumScale imgScale){
	return dataScale == imgScale
		       ? tempScaleOnMap.at(imgScale)
		       : tempScaleOffMap.at(imgScale);
}

TempHumScaleWindow::TempHumScaleWindow(lv_obj_t* parent, const TempHumScaleData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void TempHumScaleWindow::buildUI(const TempHumScaleData data){
	// Image container
	lv_obj_t* imageContainer = lv_obj_create(innerContent);
	lv_obj_set_size(imageContainer, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_layout(imageContainer, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(imageContainer, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(imageContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_bg_opa(imageContainer, LV_OPA_TRANSP, 0);

	// Celsius image
	lv_obj_t* celsiusImg = lv_image_create(imageContainer);
	lv_image_set_src(celsiusImg, theme->getAsset(getScaleAsset(data.scale, TempHumScale::Celsius)));

	// Fahrenheit image
	lv_obj_t* fahrenheitImg = lv_image_create(imageContainer);
	lv_image_set_src(fahrenheitImg, theme->getAsset(getScaleAsset(data.scale, TempHumScale::Fahrenheit)));

	// Kelvin image
	lv_obj_t* kelvinImg = lv_image_create(imageContainer);
	lv_image_set_src(kelvinImg, theme->getAsset(getScaleAsset(data.scale, TempHumScale::Kelvin)));

	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, Sentence);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
