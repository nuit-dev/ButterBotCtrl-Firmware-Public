#ifndef BUTTERBOTCTRL_FIRMWARE_FALLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_FALLWINDOW_H

#include "Components/HomeWindow.h"

class FallWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Fall action
	 * @param parent parent LVGL object
	 * @param params data for Fall action
	 */
	FallWindow(lv_obj_t* parent, const FallData* params);

private:
	lv_obj_t* img;
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Fall";
	static constexpr const char* Sentence = "My dignity... slipping";

	void buildUI(FallData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_FALLWINDOW_H
