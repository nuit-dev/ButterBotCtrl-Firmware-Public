#include "WhatsThisWindow.h"

#include <array>
#include <string>
#include <Phrases.h>
#include <ObjDetClass.h>

// Display names for detected object classes
constexpr std::array<const char*, static_cast<size_t>(ObjClass::COUNT)> ObjClassNames = {
	"backpack",   // Backpack
	"bottle",     // Bottle
	"controller", // Controller
	"keyboard",   // Keyboard
	"lamp",       // Lamp
	"laptop",     // Laptop
	"mug",        // Mug
	"notebook",   // Notebook
	"phone",      // Phone
	"plant",      // Plant
};

const char* objClassName(const uint8_t objClass){
	if(objClass >= static_cast<uint8_t>(ObjClass::COUNT)){
		return "something";
	}
	return ObjClassNames[objClass];
}

// Replaces the first "%s" in text with 'replacement'. Returns true if a replacement was made.
bool replaceFirstPlaceholder(std::string& text, const char* replacement){
	const size_t pos = text.find("%s");
	if(pos == std::string::npos){
		return false;
	}
	text.replace(pos, 2, replacement);
	return true;
}

WhatsThisWindow::WhatsThisWindow(lv_obj_t* parent, const WhatsThisData* params) : HomeWindow(parent, TITLE){
	sentenceLabel = lv_label_create(innerContent);

	buildUI(*params);
}

void WhatsThisWindow::buildUI(WhatsThisData data){
	// Select the phrase set by the number of detected objects.
	Phrase phrase;
	switch(data.count){
		case 0:
			phrase = Phrase::WhatsThisNone;
			break;
		case 1:
			phrase = Phrase::WhatsThisOne;
			break;
		default:
			phrase = Phrase::WhatsThisTwo;
			break;
	}

	std::string sentence = Phrases::mapShown(phrase, data.id);

	// Fill the %s placeholders with the detected object class names. With one object, every
	// placeholder refers to it; with two, the first is class1 and the second is class2.
	if(data.count == 1){
		while(replaceFirstPlaceholder(sentence, objClassName(data.class1))){}
	} else if(data.count >= 2){
		replaceFirstPlaceholder(sentence, objClassName(data.class1));
		replaceFirstPlaceholder(sentence, objClassName(data.class2));
	}

	lv_label_set_text(sentenceLabel, sentence.c_str());

	// Sentence label
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, 90, LV_SIZE_CONTENT);

	updateLayout();
}
