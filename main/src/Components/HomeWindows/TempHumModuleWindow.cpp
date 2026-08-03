#include "TempHumModuleWindow.h"

static std::map<TempHumScale, const char*> tempScaleMap = {
	{ TempHumScale::Celsius, "C" },
	{ TempHumScale::Fahrenheit, "F" },
	{ TempHumScale::Kelvin, "K" },
};

TempHumModuleWindow::TempHumModuleWindow(lv_obj_t* parent, const TempHumModuleData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void TempHumModuleWindow::buildUI(const TempHumModuleData data){
	if (data.missing){
		// Sentence label
		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, MissingModuleSentence);
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);
		lv_obj_set_style_margin_ver(sentenceLabel, 12, 0);
		lv_obj_set_style_margin_hor(sentenceLabel, 8, 0);
	} else{
		const char* scale = tempScaleMap.at(data.scale);
		const auto hum = std::format("{}%", data.humidity);
		const auto temp = std::format("{}{}", data.temperature, scale);

		// Humidity container
		lv_obj_t* humContainer = lv_obj_create(innerContent);
		lv_obj_set_size(humContainer, 80, LV_SIZE_CONTENT);
		lv_obj_set_layout(humContainer, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(humContainer, LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_align(humContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_remove_flag(humContainer, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_style_bg_opa(humContainer, LV_OPA_TRANSP, 0);
		lv_obj_set_style_margin_ver(humContainer, 8, 0);
		lv_obj_set_style_margin_hor(humContainer, 8, 0);

		// Temperature container
		lv_obj_t* tempContainer = lv_obj_create(innerContent);
		lv_obj_set_size(tempContainer, 80, LV_SIZE_CONTENT);
		lv_obj_set_layout(tempContainer, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(tempContainer, LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_align(tempContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_remove_flag(tempContainer, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_style_bg_opa(tempContainer, LV_OPA_TRANSP, 0);
		lv_obj_set_style_pad_bottom(tempContainer, 16, 0);

		// Temp Image
		lv_obj_t* tempImg = lv_image_create(tempContainer);
		lv_image_set_src(tempImg, theme->getAsset(Asset::Temperature));
		lv_obj_set_style_pad_right(tempImg, 4, 0);

		// Hum Image
		lv_obj_t* humImg = lv_image_create(humContainer);
		lv_image_set_src(humImg, theme->getAsset(Asset::Humidity));
		lv_obj_set_style_pad_right(humImg, 4, 0);

		// Temp sentence label
		lv_obj_t* sentenceTempLabel = lv_label_create(tempContainer);
		lv_label_set_text(sentenceTempLabel, temp.c_str());
		lv_obj_add_style(sentenceTempLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceTempLabel, 50, LV_SIZE_CONTENT);

		// Hum sentence label
		lv_obj_t* sentenceHumLabel = lv_label_create(humContainer);
		lv_label_set_text(sentenceHumLabel, hum.c_str());
		lv_obj_add_style(sentenceHumLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceHumLabel, 50, LV_SIZE_CONTENT);
	}

	updateLayout();
}
