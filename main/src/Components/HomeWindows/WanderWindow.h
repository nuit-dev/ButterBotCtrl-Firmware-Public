#ifndef BUTTERBOTCTRL_FIRMWARE_WANDERWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_WANDERWINDOW_H

#include "Components/HomeWindow.h"

class WanderWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Wander action
	 * @param parent parent LVGL object
	 * @param params data for Wander action
	 */
	WanderWindow(lv_obj_t* parent, const WanderData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Wander";

	void buildUI(WanderData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_WANDERWINDOW_H
