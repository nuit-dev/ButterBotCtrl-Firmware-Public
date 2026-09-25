#ifndef BUTTERBOT_COMMON_QUOTETEXT_H
#define BUTTERBOT_COMMON_QUOTETEXT_H

// Custom (NUIT): helpers shared by robot and controller for the quote menu items

#include <string>
#include <vector>
#include "BBData.h"
#include "Phrases.h"

namespace QuoteText {
	// Quotes longer than this are spoken and shown one sentence at a time
	inline constexpr size_t SplitThreshold = 140;

	inline Phrase phraseFor(QuoteData::Category category){
		switch(category){
			case QuoteData::Category::Overkloking: return Phrase::Overkloking;
			case QuoteData::Category::OverklokingBest: return Phrase::OverklokingBest;
			case QuoteData::Category::Bender: return Phrase::Bender;
			case QuoteData::Category::Ultron: return Phrase::Ultron;
		}
		return Phrase::None;
	}

	inline const char* titleFor(QuoteData::Category category){
		switch(category){
			case QuoteData::Category::Overkloking:
			case QuoteData::Category::OverklokingBest: return "OVERKLOKING";
			case QuoteData::Category::Bender: return "BENDER";
			case QuoteData::Category::Ultron: return "ULTRON";
		}
		return "";
	}

	// Splits after '.', '!' or '?' followed by a space
	inline std::vector<std::string> splitSentences(const std::string& text){
		std::vector<std::string> out;
		size_t start = 0;
		for(size_t i = 0; i + 1 < text.size(); ++i){
			const char ch = text[i];
			if((ch == '.' || ch == '!' || ch == '?') && text[i + 1] == ' '){
				out.push_back(text.substr(start, i + 1 - start));
				start = i + 2;
			}
		}
		if(start < text.size()){
			out.push_back(text.substr(start));
		}
		return out;
	}
}

#endif //BUTTERBOT_COMMON_QUOTETEXT_H
