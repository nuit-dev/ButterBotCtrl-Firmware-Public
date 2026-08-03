#ifndef BUTTERBOTCTRL_FIRMWARE_PROFANITYWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_PROFANITYWINDOW_H

#include "Components/HomeWindow.h"

class ProfanityWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Profanity action
	 * @param parent parent LVGL object
	 * @param params data for Profanity action
	 */
	ProfanityWindow(lv_obj_t* parent, const ProfanityData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Profanity";

	void buildUI(ProfanityData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_PROFANITYWINDOW_H
