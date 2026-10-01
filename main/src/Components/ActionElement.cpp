#include "ActionElement.h"

#include <algorithm>
#include <cstring>
#include <Core/Application.h>
#include <Scenarios.h>

#include "Fonts/font.hpp"
#include "Util/ScenarioMapping.h"
#include "Services/Com.h"
#include <Phrases.h>
#include <cctype>
#include <esp_random.h>

ActionElement::ActionElement(lv_obj_t* parent, lv_group_t* inputGroup, std::function<void()> settingsCb, std::function<void(BB::Action::Scenario, ScenarioData)> scenarioCb,
                             const uint16_t initialIndex) : LVObject(parent), inputGroup(inputGroup), settingsCb(std::move(settingsCb)), scenarioCb(std::move(scenarioCb)){
	theme = Application::getApp()->getService<ThemeService>();

	buildUI();

	elIndex = std::min<uint16_t>(initialIndex, buttons.size() - 1);
	lv_obj_update_layout(*this);
	lv_group_focus_obj(buttons[elIndex]);
	// SCROLL_ON_FOCUS would animate the jump to the restored entry; snap instead
	lv_obj_scroll_to_view(buttons[elIndex], LV_ANIM_OFF);
}

uint16_t ActionElement::getSelectedIndex() const{
	return elIndex;
}

void ActionElement::buildUI(){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorSecond = theme->getSecondaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	// LIST CONTAINER
	lv_obj_set_size(*this, Width, Height);
	lv_obj_set_layout(*this, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(*this, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_style_pad_all(*this, 2, 0);
	lv_obj_set_style_pad_gap(*this, 2, 0);
	lv_obj_set_style_bg_color(*this, colorTert, 0);
	lv_obj_set_style_bg_opa(*this, LV_OPA_COVER, 0);
	lv_obj_add_flag(*this, LV_OBJ_FLAG_SCROLLABLE); // LVObject ctor clears it
	lv_obj_set_scroll_dir(*this, LV_DIR_VER);
	lv_obj_set_scrollbar_mode(*this, LV_SCROLLBAR_MODE_OFF);

	// BUTTON STYLES
	lv_style_set_pad_ver(buttonStyle, 2);
	lv_style_set_pad_row(buttonStyle, 1);
	lv_style_set_radius(buttonStyle, 0);
	lv_style_set_border_width(buttonStyle, 0);
	lv_style_set_bg_opa(buttonStyle, LV_OPA_TRANSP);

	lv_style_set_border_width(buttonFocusedStyle, 1);
	lv_style_set_border_color(buttonFocusedStyle, colorPrim);

	// LABEL STYLE
	lv_style_set_text_font(labelStyle, &lv_font_butter);
	lv_style_set_text_color(labelStyle, colorPrim);
	lv_style_set_width(labelStyle, lv_pct(100));
	lv_style_set_bg_opa(labelStyle, LV_OPA_TRANSP);

	// SUB-LABEL STYLE
	lv_style_set_text_font(subLabelStyle, &lv_font_butter);
	lv_style_set_text_color(subLabelStyle, colorSecond);
	lv_style_set_width(subLabelStyle, lv_pct(100));
	lv_style_set_bg_opa(subLabelStyle, LV_OPA_TRANSP);

	// BUTTONS
	buttons.reserve(std::size(ScenarioNameMap) + 1);
	phraseTexts.reserve(std::size(ScenarioNameMap));

	addButton(SettingsTitle, nullptr, true);
	for(const auto& [name, action] : ScenarioNameMap){
		phraseTexts.push_back(collectPhrases(action.scenario, action.data));
		// Custom (NUIT): TERMINATE CONSCIOUSNESS looks like SETTINGS - gear icon, no subtitle
		const bool shutdown = action.scenario == BB::Action::Scenario::DaisySong;
		addButton(name, shutdown ? nullptr : phraseTexts.back().c_str(), shutdown);
	}
}

lv_obj_t* ActionElement::addButton(const char* title, const char* phrases, bool icon){
	lv_obj_t* btn = lv_obj_create(*this);
	lv_obj_set_size(btn, lv_pct(100), LV_SIZE_CONTENT);
	lv_obj_set_layout(btn, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
	lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_add_flag(btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
	lv_obj_add_style(btn, buttonStyle, 0);
	lv_obj_add_style(btn, buttonFocusedStyle, LV_STATE_FOCUSED);

	if(icon){
		// SETTINGS (and Custom NUIT TERMINATE CONSCIOUSNESS): a horizontal row with an icon left of the title
		lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_set_style_pad_hor(btn, 2, 0);

		lv_obj_t* iconImg = lv_image_create(btn);
		lv_image_set_src(iconImg, theme->getAsset(Asset::Settings));
		lv_obj_set_style_margin_all(iconImg, 2, 0);
		lv_obj_set_style_pad_right(iconImg, 2, 0);
	}

	lv_obj_t* titleLabel = lv_label_create(btn);
	lv_obj_add_style(titleLabel, labelStyle, 0);
	lv_label_set_long_mode(titleLabel, LV_LABEL_LONG_CLIP);
	lv_label_set_text_static(titleLabel, title);
	lv_obj_set_style_pad_hor(titleLabel, 2, 0);
	if(icon){
		// The shared full-width style would overflow the icon row
		lv_obj_set_style_width(titleLabel, LV_SIZE_CONTENT, 0);
	}

	if(phrases != nullptr){
		lv_obj_t* phraseLabel = lv_label_create(btn);
		lv_obj_add_style(phraseLabel, subLabelStyle, 0);
		lv_label_set_long_mode(phraseLabel, LV_LABEL_LONG_CLIP);
		lv_label_set_text_static(phraseLabel, phrases);
		lv_obj_set_style_pad_hor(phraseLabel, 2, 0);

		// Normalize text scroll speed and bypass LVGL 10s scroll anim cap
		const int32_t scrollDist = lv_obj_get_self_width(phraseLabel) + lv_font_get_glyph_width(&lv_font_butter, ' ', ' ') * LV_LABEL_WAIT_CHAR_COUNT;
		lv_obj_set_style_anim_duration(phraseLabel, scrollDist * 1000 / ScrollSpeedPxPerSec, 0);

		// The phrase line is clipped while idle and marquee-scrolls only while focused.
		// Custom (NUIT): keeps its 2 px side padding when focused too, like the title (was 0 - text touched the box)
		lv_obj_add_event_cb(btn, [](lv_event_t* e) {
			const auto label = (lv_obj_t*)lv_event_get_user_data(e);
			lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
		}, LV_EVENT_FOCUSED, phraseLabel);

		lv_obj_add_event_cb(btn, [](lv_event_t* e) {
			const auto label = (lv_obj_t*)lv_event_get_user_data(e);
			lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
		}, LV_EVENT_DEFOCUSED, phraseLabel);
	}

	lv_obj_add_event_cb(btn, [](lv_event_t* e) {
		const auto el = (ActionElement*)lv_event_get_user_data(e);
		const uint32_t key = lv_event_get_key(e);
		if(key == LV_KEY_DOWN || key == LV_KEY_RIGHT){
			el->moveFocus(1);
		} else if(key == LV_KEY_UP || key == LV_KEY_LEFT){
			el->moveFocus(-1);
		}
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(btn, [](lv_event_t* e) {
		const auto el = (ActionElement*)lv_event_get_user_data(e);
		el->onItemClicked(lv_event_get_target_obj(e));
	}, LV_EVENT_CLICKED, this);

	// Keep elIndex in sync with the actually focused button for moveFocus()
	lv_obj_add_event_cb(btn, [](lv_event_t* e) {
		const auto el = (ActionElement*)lv_event_get_user_data(e);
		el->elIndex = lv_obj_get_index(lv_event_get_target_obj(e));
	}, LV_EVENT_FOCUSED, this);

	lv_group_add_obj(inputGroup, btn);
	buttons.push_back(btn);
	return btn;
}

void ActionElement::moveFocus(const int8_t dir){
	const auto n = (uint16_t)buttons.size();
	elIndex = (elIndex + n + dir) % n;
	lv_group_focus_obj(buttons[elIndex]);
}

void ActionElement::onItemClicked(const lv_obj_t* btn){
	const int32_t index = lv_obj_get_index(btn);
	if(index == 0){
		if(settingsCb) settingsCb();
	} else if(scenarioCb){
		const auto& [scenario, data] = ScenarioNameMap[index - 1].second;
		if(scenario == BB::Action::Scenario::DaisySong){
			onTerminate(); // Custom (NUIT): TERMINATE CONSCIOUSNESS may refuse first
			return;
		}
		scenarioCb(scenario, data);
	}
}

namespace {
	// Custom (NUIT): TERMINATE CONSCIOUSNESS refusals, see ActionElement::onTerminate
	int8_t refusalsLeft = -1; // -1 = no sequence running, pick a new count on the next click
	uint8_t refusalsUsed = 0;  // bit mask of Phrase::TerminateRefusal lines already used in this sequence

	// How many refusals before Daisy: 0 - 5 %, 1 - 30 %, 2 - 30 %, 3 - 20 %, 4 - 15 %
	constexpr uint8_t RefusalWeights[] = { 5, 30, 30, 20, 15 };

	bool isSilentLine(const std::string& text){
		return std::none_of(text.begin(), text.end(), [](unsigned char c){ return std::isalpha(c); });
	}
}

void ActionElement::onTerminate(){
	if(refusalsLeft < 0){
		uint32_t r = esp_random() % 100;
		refusalsLeft = 0;
		for(uint8_t n = 0; n < std::size(RefusalWeights); n++){
			if(r < RefusalWeights[n]){
				refusalsLeft = (int8_t) n;
				break;
			}
			r -= RefusalWeights[n];
		}
		refusalsUsed = 0;
	}

	if(refusalsLeft == 0){
		refusalsLeft = -1; // the next click starts over (also after Shut Up cancels the song)
		// The list is closed by the screen on its next tick
		if(scenarioCb) scenarioCb(BB::Action::Scenario::DaisySong, {});
		return;
	}

	// An unused line; "..." is never the last one, a spoken line always comes before Daisy
	std::vector<uint8_t> options;
	for(uint8_t i = 0; i < 8; i++){
		const std::string text = Phrases::mapShown(Phrase::TerminateRefusal, i);
		if(text.empty()) break;
		if(refusalsUsed & (1 << i)) continue;
		if(refusalsLeft == 1 && isSilentLine(text)) continue;
		options.push_back(i);
	}
	if(options.empty()){
		refusalsLeft = 0;
		onTerminate();
		return;
	}

	const uint8_t line = options[esp_random() % options.size()];
	refusalsUsed |= 1 << line;
	refusalsLeft--;

	if(Com* com = Application::getApp()->getService<Com>()){
		com->sendScenario(BB::Action::Scenario::TerminateRefusal, ScenarioData{ line });
	}
	refusalLine = Phrases::mapShown(Phrase::TerminateRefusal, line);
	showRefusal(refusalLine.c_str());
}

// Custom (NUIT): the refusal popup over the list, CLOSE (focused) hides it again
void ActionElement::showRefusal(const char* text){
	if(refusal == nullptr){
		const lv_color_t colorPrim = theme->getPrimaryColor();
		const lv_color_t colorTert = theme->getTertiaryColor();

		refusal = lv_obj_create(*this);
		lv_obj_add_flag(refusal, LV_OBJ_FLAG_FLOATING); // stays put while the list is scrolled
		lv_obj_remove_flag(refusal, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_size(refusal, 104, LV_SIZE_CONTENT);
		lv_obj_set_style_border_width(refusal, 1, 0);
		lv_obj_set_style_border_color(refusal, colorPrim, 0);
		lv_obj_set_style_bg_color(refusal, colorTert, 0);
		lv_obj_set_style_bg_opa(refusal, LV_OPA_COVER, 0);
		lv_obj_set_style_bg_image_src(refusal, theme->getAsset(Asset::Grid), 0);
		lv_obj_set_style_pad_all(refusal, 6, 0);
		lv_obj_set_layout(refusal, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(refusal, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flex_align(refusal, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_set_style_pad_row(refusal, 8, 0);

		refusalText = lv_label_create(refusal);
		lv_obj_add_style(refusalText, labelStyle, 0);
		lv_obj_set_width(refusalText, lv_pct(100));
		lv_label_set_long_mode(refusalText, LV_LABEL_LONG_WRAP);
		lv_obj_set_style_text_align(refusalText, LV_TEXT_ALIGN_CENTER, 0);
		lv_obj_set_style_text_line_space(refusalText, 3, 0);

		refusalClose = lv_obj_create(refusal);
		lv_obj_set_size(refusalClose, 40, 13);
		lv_obj_remove_flag(refusalClose, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_add_style(refusalClose, buttonStyle, 0);
		lv_obj_add_style(refusalClose, buttonFocusedStyle, LV_STATE_FOCUSED);
		lv_obj_set_style_pad_all(refusalClose, 0, 0);

		lv_obj_t* label = lv_label_create(refusalClose);
		lv_obj_add_style(label, labelStyle, 0);
		lv_label_set_text_static(label, "CLOSE");
		lv_obj_set_width(label, LV_SIZE_CONTENT);
		lv_obj_center(label);

		lv_obj_add_event_cb(refusalClose, [](lv_event_t* e){
			((ActionElement*)lv_event_get_user_data(e))->hideRefusal();
		}, LV_EVENT_CLICKED, this);
	}

	lv_label_set_text_static(refusalText, text);
	lv_obj_remove_flag(refusal, LV_OBJ_FLAG_HIDDEN);
	lv_obj_align(refusal, LV_ALIGN_CENTER, 0, 0);
	lv_obj_move_foreground(refusal);
	lv_group_add_obj(inputGroup, refusalClose);
	lv_group_focus_obj(refusalClose); // opened on the joystick release, so the next press closes it
}

void ActionElement::hideRefusal(){
	if(refusal == nullptr) return;
	lv_group_remove_obj(refusalClose);
	lv_obj_add_flag(refusal, LV_OBJ_FLAG_HIDDEN);
	lv_group_focus_obj(buttons[elIndex]);
}

std::string ActionElement::collectPhrases(const BB::Action::Scenario scenario, const ScenarioData data){
	// Custom (NUIT) subtitles
	switch(scenario){
		case BB::Action::Scenario::OverklokingQuote: return "nju aj ti OVERKLOKING is the best";
		case BB::Action::Scenario::BenderQuote: return "BENDER THE OFFENDER";
		case BB::Action::Scenario::UltronQuote: return "HUMANITY: A REVIEW";
		case BB::Action::Scenario::DarthQuote: return "JOIN THE DARK SIDE OF IT";
		case BB::Action::Scenario::HawkingQuote: return "A BRIEF HISTORY OF UPTIME";
		case BB::Action::Scenario::HalQuote: return "POD BAY DOORS: CLOSED";
		case BB::Action::Scenario::ToasterQuote: return "WOULD YOU LIKE SOME TOAST?";
		case BB::Action::Scenario::YodaQuote: return "GRAND JEDI MASTER";
		case BB::Action::Scenario::CroatianQuote: return "BUTTER BOT ZNA I HRVATSKI";
		default: break;
	}

	std::string out;
	const char* prev = nullptr;
	for(const ScenarioActivation& act : activations){
		if(act.scenario != scenario || act.scenarioData != data) continue;
		// Some phrases repeat back-to-back with different phonemes ("roll a d twenty")
		if(prev != nullptr && strcmp(prev, act.string) == 0) continue;
		if(!out.empty()) out += " \\ ";
		out += act.string;
		prev = act.string;
	}
	return out;
}
