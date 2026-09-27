#include "QuoteWindow.h"

#include <algorithm>
#include <Phrases.h>
#include <QuoteText.h>

QuoteWindow::QuoteWindow(lv_obj_t* parent, const QuoteData* params) : HomeWindow(parent, QuoteText::titleFor(params->category)){
	const std::string text = Phrases::mapShown(QuoteText::phraseFor(params->category), params->id);

	lv_obj_t* labelParent = innerContent;
	if(text.size() > QuoteText::SplitThreshold){
		// Whole text in a fixed box, scrolled to the sentence the robot is saying
		viewport = lv_obj_create(innerContent);
		lv_obj_set_size(viewport, ViewportWidth, ViewportHeight);
		lv_obj_set_style_bg_opa(viewport, LV_OPA_TRANSP, 0);
		lv_obj_set_style_border_width(viewport, 0, 0);
		lv_obj_set_style_pad_all(viewport, 0, 0);
		lv_obj_set_scroll_dir(viewport, LV_DIR_VER);
		lv_obj_set_scrollbar_mode(viewport, LV_SCROLLBAR_MODE_OFF);
		labelParent = viewport;

		size_t pos = 0;
		for(const std::string& part : QuoteText::splitSentences(text)){
			pos = text.find(part, pos);
			partStarts.push_back(pos == std::string::npos ? 0 : (uint32_t)pos);
			if(pos == std::string::npos) pos = 0;
		}
	}

	sentenceLabel = lv_label_create(labelParent);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, viewport ? ViewportWidth - 2 : 90, LV_SIZE_CONTENT);
	if(viewport){
		lv_label_set_text(sentenceLabel, text.c_str());
		updateLayout();
	}

	setText(*params);
}

void QuoteWindow::onData(const BBData* data){
	setText(*static_cast<const QuoteData*>(data));
}

void QuoteWindow::setText(const QuoteData& data){
	if(viewport){
		if(data.part == QuoteData::WholeQuote || partStarts.empty()) return;
		const uint32_t letter = partStarts[std::min<size_t>(data.part, partStarts.size() - 1)];
		lv_obj_update_layout(viewport);
		lv_point_t pos;
		lv_label_get_letter_pos(sentenceLabel, letter, &pos);
		lv_obj_scroll_to_y(viewport, pos.y, LV_ANIM_ON);
		return;
	}

	std::string text = Phrases::mapShown(QuoteText::phraseFor(data.category), data.id);

	if(data.part != QuoteData::WholeQuote){
		const std::vector<std::string> parts = QuoteText::splitSentences(text);
		if(!parts.empty()){
			text = parts[std::min<size_t>(data.part, parts.size() - 1)];
		}
	}

	lv_label_set_text(sentenceLabel, text.c_str());
	updateLayout();
}
