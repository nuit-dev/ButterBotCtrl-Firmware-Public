#include "RambleWindow.h"

#include <string>
#include <Phrases.h>
#include <QuoteText.h>

RambleWindow::RambleWindow(lv_obj_t* parent, const RambleData* params) : HomeWindow(parent, TITLE){
	buildUI(*params);
}

void RambleWindow::buildUI(const RambleData data){
	// Custom (NUIT): kind picks the list (time-of-day / Thursday / toaster lines)
	const std::string text = Phrases::mapShown(rambleKindPhrase(static_cast<RambleKind>(data.kind)), data.id);

	lv_obj_t* labelParent = innerContent;
	if(text.size() > LongThreshold){
		// Whole text in a fixed box that scrolls down while the robot speaks
		viewport = lv_obj_create(innerContent);
		lv_obj_set_size(viewport, ViewportWidth, ViewportHeight);
		lv_obj_set_style_bg_opa(viewport, LV_OPA_TRANSP, 0);
		lv_obj_set_style_border_width(viewport, 0, 0);
		lv_obj_set_style_pad_all(viewport, 0, 0);
		lv_obj_set_scroll_dir(viewport, LV_DIR_VER);
		lv_obj_set_scrollbar_mode(viewport, LV_SCROLLBAR_MODE_OFF);
		labelParent = viewport;
	}

	// Sentence label
	sentenceLabel = lv_label_create(labelParent);
	lv_label_set_long_mode(sentenceLabel, LV_LABEL_LONG_WRAP);
	lv_obj_add_style(sentenceLabel, labelDefaultStyle, 0);
	lv_obj_set_size(sentenceLabel, viewport ? ViewportWidth - 2 : LabelWidth, LV_SIZE_CONTENT);
	lv_label_set_text(sentenceLabel, text.c_str());

	updateLayout();

	if(viewport){
		lv_obj_update_layout(viewport);
		const int32_t range = lv_obj_get_scroll_bottom(viewport);
		if(range > 0){
			lv_anim_t anim;
			lv_anim_init(&anim);
			lv_anim_set_var(&anim, viewport);
			lv_anim_set_exec_cb(&anim, [](void* obj, int32_t y){
				lv_obj_scroll_to_y(static_cast<lv_obj_t*>(obj), y, LV_ANIM_OFF);
			});
			lv_anim_set_values(&anim, 0, range);
			lv_anim_set_delay(&anim, ScrollDelayMs);
			lv_anim_set_duration(&anim, static_cast<uint32_t>(text.size()) * ScrollMsPerChar);
			lv_anim_set_path_cb(&anim, lv_anim_path_linear);
			lv_anim_start(&anim);
		}
	}
}
