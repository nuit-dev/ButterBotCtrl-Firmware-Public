#ifndef BUTTERBOTCTRL_FIRMWARE_BATTERYLOWWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_BATTERYLOWWINDOW_H

#include "Components/HomeWindow.h"

class BatteryLowWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB BatteryLow action
	 * @param parent parent LVGL object
	 * @param params data for BatteryLow action
	 */
	BatteryLowWindow(lv_obj_t* parent, const BatteryData* params);

private:
	static constexpr const char* TITLE = "Battery Low";

	void buildUI(BatteryData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_BATTERYLOWWINDOW_H
