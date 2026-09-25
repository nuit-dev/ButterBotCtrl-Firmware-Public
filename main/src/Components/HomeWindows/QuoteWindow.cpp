#include "QuoteWindow.h"

#include <algorithm>
#include <Phrases.h>
#include <QuoteText.h>

QuoteWindow::QuoteWindow(lv_obj_t* parent, const QuoteData* params) : HomeWindow(parent, QuoteText::titleFor(params->category)){
	sentenceLabel = lv_label_create(innerContent);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	setText(*params);
}

void QuoteWindow::onData(const BBData* data){
	setText(*static_cast<const QuoteData*>(data));
}

void QuoteWindow::setText(const QuoteData& data){
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
