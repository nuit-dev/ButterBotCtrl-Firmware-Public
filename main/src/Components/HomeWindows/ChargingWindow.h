#ifndef BUTTERBOTCTRL_FIRMWARE_CHARGINGWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_CHARGINGWINDOW_H

#include "Components/HomeWindow.h"

class ChargingWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Charging action
	 * @param parent parent LVGL object
	 * @param params data for Charging action
	 */
	ChargingWindow(lv_obj_t* parent, const BatteryData* params);

private:
	static constexpr const char* TITLE = "Charging";

	void buildUI(BatteryData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_CHARGINGWINDOW_H
