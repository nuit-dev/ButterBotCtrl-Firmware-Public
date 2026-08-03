#include "ModuleChangeWindow.h"

static constexpr const char* TAG = "ModuleChangeWindow";

static const std::map<ModuleType, const Asset> ModuleAssetMap = {
	{ ModuleType::LED, Asset::ModuleLed },
	{ ModuleType::PerfBoard, Asset::ModulePerf },
	{ ModuleType::Motion, Asset::ModuleIntruder },
	{ ModuleType::CO2, Asset::ModuleGas },
	{ ModuleType::IR, Asset::ModuleIr },
	{ ModuleType::TempHumidity, Asset::ModuleTemp },
	{ ModuleType::Unknown, Asset::ModuleUnplugged },
};

static const std::map<ModuleType, const char*> ModuleSentenceMap = {
	{ ModuleType::LED, "Light powers gained!" },
	{ ModuleType::PerfBoard, "Tinkering mode unlocked!" },
	{ ModuleType::Motion, "Motion paranoia activated!" },
	{ ModuleType::CO2, "Air sniffer attached!" },
	{ ModuleType::IR, "IR powers acquired!" },
	{ ModuleType::TempHumidity, "Weather brain plugged!" },
	{ ModuleType::Unknown, "Unknown module detected!" },
};


ModuleChangeWindow::ModuleChangeWindow(lv_obj_t* parent, const ModuleData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void ModuleChangeWindow::onData(const BBData* data){
	const auto newData = *(const ModuleData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void ModuleChangeWindow::buildUI(const ModuleData data){
	if(!data.inserted){
		// MODULE REMOVED
		lv_obj_t* img = lv_image_create(innerContent);
		lv_image_set_src(img, theme->getAsset(Asset::ModuleUnplugged));
		lv_obj_set_style_pad_ver(img, 8, 0);

		lv_obj_t* removeLabel = lv_label_create(innerContent);
		lv_label_set_text(removeLabel, ModuleRemovedSentence);
		lv_obj_add_style(removeLabel, labelDefaultStyle, 0);
		lv_obj_set_size(removeLabel, 90, LV_SIZE_CONTENT);
		lv_obj_set_style_text_align(removeLabel, LV_TEXT_ALIGN_CENTER, 0);
	} else{
		// MODULE INSERTED
		switch(data.type){
			case ModuleType::TempHumidity:
			case ModuleType::LED:
			case ModuleType::Motion:
			case ModuleType::CO2:
			case ModuleType::IR:
			case ModuleType::PerfBoard:
			case ModuleType::Unknown: {
				lv_obj_t* img = lv_image_create(innerContent);
				lv_image_set_src(img, theme->getAsset(ModuleAssetMap.at(data.type)));
				lv_obj_set_style_pad_ver(img, 8, 0);

				lv_obj_t* insertLabel = lv_label_create(innerContent);
				lv_label_set_text(insertLabel, ModuleSentenceMap.at(data.type));
				lv_obj_add_style(insertLabel, labelDefaultStyle, 0);
				lv_obj_set_size(insertLabel, 90, LV_SIZE_CONTENT);
				lv_obj_set_style_text_align(insertLabel, LV_TEXT_ALIGN_CENTER, 0);

				if(data.type == ModuleType::Unknown) ESP_LOGE(TAG, "Unknown module type");
				break;
			}
			default:
				ESP_LOGE(TAG, "Module type not recognised");
				break;
		}
	}

	updateLayout();
}
