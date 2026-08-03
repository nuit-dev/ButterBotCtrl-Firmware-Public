#ifndef BUTTERBOTCTRL_FIRMWARE_FACTWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_FACTWINDOW_H

#include "Components/HomeWindow.h"

class FactWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Fact action
	 * @param parent parent LVGL object
	 * @param params data for Fact action
	 */
	FactWindow(lv_obj_t* parent, const FactData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Fact";

	void buildUI(FactData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_FACTWINDOW_H
