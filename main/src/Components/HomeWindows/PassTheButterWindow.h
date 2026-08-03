#ifndef BUTTERBOTCTRL_FIRMWARE_PASSTHEBUTTERWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_PASSTHEBUTTERWINDOW_H

#include "Components/HomeWindow.h"

class PassTheButterWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB PassTheButter action
	 * @param parent parent LVGL object
	 * @param params data for PassTheButter action
	 */
	PassTheButterWindow(lv_obj_t* parent, const PassTheButterData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Pass The Butter";

	void buildUI(PassTheButterData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_PASSTHEBUTTERWINDOW_H
