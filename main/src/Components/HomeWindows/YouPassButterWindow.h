#ifndef BUTTERBOTCTRL_FIRMWARE_YOUPASSBUTTERWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_YOUPASSBUTTERWINDOW_H

#include "Components/HomeWindow.h"

class YouPassButterWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB YouPassButter action
	 * @param parent parent LVGL object
	 * @param params data for YouPassButter action
	 */
	YouPassButterWindow(lv_obj_t* parent, const YouPassButterData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "You Pass Butter";

	void buildUI(YouPassButterData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_YOUPASSBUTTERWINDOW_H
