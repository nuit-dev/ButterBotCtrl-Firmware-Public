#include "DiceRollWindow.h"

static constexpr const char* TAG = "DiceRollWindow";

typedef DiceRollData::DiceType DiceType;

static constexpr std::array<Asset, 4> D4Results = {
	{
		Asset::D41,
		Asset::D42,
		Asset::D42,
		Asset::D42,
	}
};

static constexpr std::array<Asset, 6> D6Results = {
	{
		Asset::D61,
		Asset::D62,
		Asset::D63,
		Asset::D64,
		Asset::D65,
		Asset::D66,
	}
};

static constexpr std::array<Asset, 8> D8Results = {
	{
		Asset::D81,
		Asset::D82,
		Asset::D83,
		Asset::D84,
		Asset::D85,
		Asset::D86,
		Asset::D87,
		Asset::D88,
	}
};

static constexpr std::array<Asset, 10> D10Results = {
	{
		Asset::D1001,
		Asset::D1002,
		Asset::D1003,
		Asset::D1004,
		Asset::D1005,
		Asset::D1006,
		Asset::D1007,
		Asset::D1008,
		Asset::D1009,
		Asset::D1010,
	}
};

static constexpr std::array<Asset, 12> D12Results = {
	{
		Asset::D1201,
		Asset::D1202,
		Asset::D1203,
		Asset::D1204,
		Asset::D1205,
		Asset::D1206,
		Asset::D1207,
		Asset::D1208,
		Asset::D1209,
		Asset::D1210,
		Asset::D1211,
		Asset::D1212,
	}
};

static constexpr std::array<Asset, 20> D20Results = {
	{
		Asset::D2001,
		Asset::D2002,
		Asset::D2003,
		Asset::D2004,
		Asset::D2005,
		Asset::D2006,
		Asset::D2007,
		Asset::D2008,
		Asset::D2009,
		Asset::D2010,
		Asset::D2011,
		Asset::D2012,
		Asset::D2013,
		Asset::D2014,
		Asset::D2015,
		Asset::D2016,
		Asset::D2017,
		Asset::D2018,
		Asset::D2019,
		Asset::D2020,
	}
};

static const std::map<DiceType, std::span<const Asset>> DiceAssetMap = {
	{ DiceType::D4,  D4Results },
	{ DiceType::D6,  D6Results },
	{ DiceType::D8,  D8Results },
	{ DiceType::D10, D10Results },
	{ DiceType::D12, D12Results },
	{ DiceType::D20, D20Results },
};

static const std::map<DiceType, const char*> DiceTypeMap = {
	{ DiceType::D4, "d4" },
	{ DiceType::D6, "d6" },
	{ DiceType::D8, "d8" },
	{ DiceType::D10, "d10" },
	{ DiceType::D12, "d12" },
	{ DiceType::D20, "d20" },
	{ DiceType::D100, "d100" },
};

static const std::map<DiceRollData::Phase, const char*> DiceRollSentences = {
	{ DiceRollData::Phase::RollCount, "Roll count, please" },
	{ DiceRollData::Phase::DiceKind, "Destiny format, please" },
	{ DiceRollData::Phase::RollAnim, "Destiny in progress" },
	{ DiceRollData::Phase::Result, "The dice have spoken" },
};

DiceRollWindow::DiceRollWindow(lv_obj_t* parent, const DiceRollData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void DiceRollWindow::onData(const BBData* data){
	const auto newData = *(const DiceRollData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void DiceRollWindow::buildUI(const DiceRollData data){
	// Sentence label
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, DiceRollSentences.at(data.phase));
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);

	switch(data.phase){
		case DiceRollData::Phase::RollCount: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::DiceNumber));
			lv_obj_set_style_pad_bottom(img, 20, 0);

			lv_obj_set_size(sentenceLabel, 84, LV_SIZE_CONTENT);
			break;
		}
		case DiceRollData::Phase::DiceKind: {
			lv_obj_t* img = lv_image_create(innerContent);
			lv_image_set_src(img, theme->getAsset(Asset::DiceAll));

			lv_obj_set_size(sentenceLabel, 112, LV_SIZE_CONTENT);
			break;
		}
		case DiceRollData::Phase::RollAnim: {
			gif = new LVGIF(innerContent, theme->getAsset(Asset::Roll));
			gif->setLooping(LVGIF::LoopType::On);
			gif->reset();
			lv_obj_set_style_margin_all(*gif, 16, 0);

			lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
			break;
		}
		case DiceRollData::Phase::Result: {
			// Show result image based on dice type and value (only if dice count is 1 and not D100)
			if(data.diceCount == 1 && data.diceType != DiceType::D100){
				lv_obj_t* img = lv_image_create(innerContent);
				const Asset resultAsset = DiceAssetMap.at(data.diceType)[data.diceResult - 1];
				lv_image_set_src(img, theme->getAsset(resultAsset));
				lv_obj_set_style_margin_ver(img, 8, 0);
			} else{
				lv_obj_t* resultContainer = lv_obj_create(innerContent);
				lv_obj_set_layout(resultContainer, LV_LAYOUT_FLEX);
				lv_obj_set_flex_flow(resultContainer, LV_FLEX_FLOW_COLUMN);
				lv_obj_set_flex_align(resultContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
				lv_obj_remove_flag(resultContainer, LV_OBJ_FLAG_SCROLLABLE);
				lv_obj_set_style_bg_opa(resultContainer, LV_OPA_TRANSP, 0);
				lv_obj_set_size(resultContainer, 68, 42);

				// Dice format label
				std::string formatStr = std::format("{} x {}", data.diceCount, DiceTypeMap.at(data.diceType));
				lv_obj_t* diceFormatLabel = lv_label_create(resultContainer);
				lv_label_set_text(diceFormatLabel, formatStr.c_str());
				lv_obj_set_style_text_align(diceFormatLabel, LV_TEXT_ALIGN_CENTER, 0);
				lv_obj_add_style(diceFormatLabel, labelDefaultStyle, 0);

				// Result label
				lv_obj_t* resultLabel = lv_label_create(resultContainer);
				lv_label_set_text(resultLabel, std::to_string(data.diceResult).c_str());
				lv_obj_set_style_text_align(resultLabel, LV_TEXT_ALIGN_CENTER, 0);
				lv_obj_add_style(resultLabel, labelDefaultStyle, 0);
			}
			lv_obj_set_size(sentenceLabel, 68, LV_SIZE_CONTENT);
			break;
		}
		default:
			ESP_LOGE(TAG, "Roll phase not recognised");
			break;
	}

	updateLayout();
}

void DiceRollWindow::onDismiss(){
	ESP_LOGI(TAG, "onDismiss");
	if(gif){
		gif->stop();
	}
}
