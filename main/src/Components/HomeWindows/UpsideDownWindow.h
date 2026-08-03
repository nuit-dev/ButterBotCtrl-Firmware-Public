#ifndef BUTTERBOTCTRL_FIRMWARE_UPSIDEDOWNWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_UPSIDEDOWNWINDOW_H

#include "Components/HomeWindow.h"

class UpsideDownWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB UpsideDown action
	 * @param parent parent LVGL object
	 * @param params data for UpsideDown action
	 */
	UpsideDownWindow(lv_obj_t* parent, const UpsideDownData* params);

private:
	lv_obj_t* img;
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Upside Down";
	static constexpr const char* Sentence = "Wrong way, human";

	void buildUI(UpsideDownData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_UPSIDEDOWNWINDOW_H
