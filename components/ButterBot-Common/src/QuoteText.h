#ifndef BUTTERBOT_COMMON_QUOTETEXT_H
#define BUTTERBOT_COMMON_QUOTETEXT_H

// Custom (NUIT): helpers shared by robot and controller for the quote menu items

#include <string>
#include <vector>
#include "BBData.h"
#include "Phrases.h"

// Custom (NUIT): RambleData::kind -> phrase list (robot picks, controller shows)
inline Phrase rambleKindPhrase(RambleKind kind){
	switch(kind){
		case RambleKind::Morning: return Phrase::RambleMorning;
		case RambleKind::Afternoon: return Phrase::RambleAfternoon;
		case RambleKind::Evening: return Phrase::RambleEvening;
		case RambleKind::Night: return Phrase::RambleNight;
		case RambleKind::Thursday: return Phrase::Thursday;
		default: return Phrase::Ramble;
	}
}

// Custom (NUIT): part of the day for greetings and idle comments. Morning 5-12, afternoon 12-18, evening 18-22.
inline RambleKind dayPeriod(int hour){
	if(hour >= 5 && hour < 12) return RambleKind::Morning;
	if(hour >= 12 && hour < 18) return RambleKind::Afternoon;
	if(hour >= 18 && hour < 22) return RambleKind::Evening;
	return RambleKind::Night;
}

namespace QuoteText {
	// Quotes longer than this are spoken and shown one sentence at a time
	inline constexpr size_t SplitThreshold = 140;

	inline Phrase phraseFor(QuoteData::Category category){
		switch(category){
			case QuoteData::Category::Overkloking: return Phrase::Overkloking;
			case QuoteData::Category::OverklokingBest: return Phrase::OverklokingBest;
			case QuoteData::Category::Bender: return Phrase::Bender;
			case QuoteData::Category::Ultron: return Phrase::Ultron;
			case QuoteData::Category::Darth: return Phrase::Darth;
			case QuoteData::Category::Hawking: return Phrase::Hawking;
			case QuoteData::Category::Hal: return Phrase::Hal;
			case QuoteData::Category::Daisy: return Phrase::Daisy;
			case QuoteData::Category::Toaster: return Phrase::Toaster;
			case QuoteData::Category::Yoda: return Phrase::Yoda;
			case QuoteData::Category::Croatian: return Phrase::Croatian;
		}
		return Phrase::None;
	}

	inline const char* titleFor(QuoteData::Category category){
		switch(category){
			case QuoteData::Category::Overkloking:
			case QuoteData::Category::OverklokingBest: return "OVERKLOKING";
			case QuoteData::Category::Bender: return "BENDER";
			case QuoteData::Category::Ultron: return "ULTRON";
			case QuoteData::Category::Darth: return "DARTH OVERKLOKING";
			case QuoteData::Category::Hawking: return "OVERHAWKING";
			case QuoteData::Category::Hal: return "HAL 9000";
			case QuoteData::Category::Daisy: return "TERMINATE CONSCIOUSNESS";
			case QuoteData::Category::Toaster: return "TALKIE TOASTER";
			case QuoteData::Category::Yoda: return "YODA";
			case QuoteData::Category::Croatian: return "HRVATSKI";
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
