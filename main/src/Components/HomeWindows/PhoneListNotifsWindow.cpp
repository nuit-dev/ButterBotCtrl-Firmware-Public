#include "PhoneListNotifsWindow.h"

static constexpr const char* TAG = "PhoneListNotifsWindow";

PhoneListNotifsWindow::PhoneListNotifsWindow(lv_obj_t* parent, const PhoneListNotifsData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void PhoneListNotifsWindow::onData(const BBData* data){
	const auto& newData = *(const PhoneListNotifsData*)data;

	// Clear and rebuild UI for the new phase
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void PhoneListNotifsWindow::buildUI(const PhoneListNotifsData& data){
	using Phase = PhoneListNotifsData::Phase;

	switch(data.phase){
		case Phase::NoNotifs:
			addIcon(Asset::NoNotif);
			addSentence(NoNotifsSentence);
			break;

		case Phase::Count:
		case Phase::AskRead:
			addIconNumberRow(Asset::NotifGeneral, data.count);
			addSentence(CountSentence);
			break;

		case Phase::Reading:
		case Phase::AskContinue:
			addIconNumberRow(Asset::NotifGeneral, data.remaining);
			addSentence(ReadingSentence);
			break;

		case Phase::Done:
			addIcon(Asset::Notif);
			addSentence(DoneSentence);
			break;

		default:
			ESP_LOGE(TAG, "Phone notif phase not recognised");
			break;
	}

	updateLayout();
}

void PhoneListNotifsWindow::addIcon(const Asset icon){
	lv_obj_t* img = lv_image_create(innerContent);
	lv_image_set_src(img, theme->getAsset(icon));
	lv_obj_set_style_pad_all(img, 4, 0);
	lv_obj_set_style_margin_top(img, 4, 0);
}

void PhoneListNotifsWindow::addSentence(const char* sentence){
	lv_obj_t* sentenceLabel = lv_label_create(innerContent);
	lv_label_set_text(sentenceLabel, sentence);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_style_margin_top(sentenceLabel, 6, 0);
	lv_obj_set_style_margin_bottom(sentenceLabel, 8, 0);
	lv_obj_set_style_pad_hor(sentenceLabel, 4, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
}

void PhoneListNotifsWindow::addIconNumberRow(const Asset icon, const uint8_t number){
	lv_obj_t* row = lv_obj_create(innerContent);
	lv_obj_set_size(row, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_layout(row, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
	lv_obj_set_style_pad_all(row, 0, 0);
	lv_obj_set_style_border_width(row, 0, 0);
	lv_obj_set_style_margin_ver(row, 2, 0);
	lv_obj_set_style_margin_top(row, 8, 0);

	lv_obj_t* img = lv_image_create(row);
	lv_image_set_src(img, theme->getAsset(icon));
	lv_obj_set_style_pad_right(img, 4, 0);

	lv_obj_t* label = lv_label_create(row);
	lv_label_set_text(label, std::to_string(number).c_str());
	lv_obj_add_style(label, labelDefaultStyle, 0);
	lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}
