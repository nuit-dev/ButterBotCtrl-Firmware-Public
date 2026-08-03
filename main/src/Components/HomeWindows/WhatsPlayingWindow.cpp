#include "WhatsPlayingWindow.h"

WhatsPlayingWindow::WhatsPlayingWindow(lv_obj_t* parent, const WhatsPlayingData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void WhatsPlayingWindow::onData(const BBData* data){
	const auto& newData = *(const WhatsPlayingData*)data;

	// Clear and rebuild UI
	lv_obj_clean(innerContent);
	buildUI(newData);
}

void WhatsPlayingWindow::buildUI(const WhatsPlayingData& data){
	if(data.title.empty()){
		// Nothing playing
		lv_obj_t* img = lv_image_create(innerContent);
		lv_image_set_src(img, theme->getAsset(Asset::MusicNo));
		lv_obj_set_style_pad_all(img, 4, 0);

		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, NothingSentence);
		lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);
	} else{
		// Track info header
		lv_obj_t* sentenceLabel = lv_label_create(innerContent);
		lv_label_set_text(sentenceLabel, PlayingSentence);
		lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_align(sentenceLabel, LV_TEXT_ALIGN_CENTER, 0);
		lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
		lv_obj_set_size(sentenceLabel, 100, LV_SIZE_CONTENT);
		lv_obj_set_style_margin_bottom(sentenceLabel, 4, 0);

		addField(Asset::MusicSong, data.title);
		if(!data.artist.empty()) addField(Asset::MusicArtist, data.artist);
		if(!data.album.empty()) addField(Asset::MusicAlbum, data.album);
	}

	updateLayout();
}

void WhatsPlayingWindow::addField(const Asset icon, const std::string& value){
	lv_obj_t* row = lv_obj_create(innerContent);
	lv_obj_set_size(row, 100, LV_SIZE_CONTENT);
	lv_obj_set_layout(row, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
	lv_obj_set_style_pad_all(row, 0, 0);
	lv_obj_set_style_border_width(row, 0, 0);
	lv_obj_set_style_margin_ver(row, 2, 0);

	lv_obj_t* img = lv_image_create(row);
	lv_image_set_src(img, theme->getAsset(icon));
	lv_obj_set_style_pad_right(img, 4, 0);

	lv_obj_t* label = lv_label_create(row);
	lv_label_set_text(label, value.c_str());
	// Song fields can be up to 64 chars; marquee-scroll so long values don't blow out the display.
	lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_add_style(label, labelDefaultStyle, 0);
	lv_obj_set_size(label, 70, LV_SIZE_CONTENT);
}
