#ifndef BUTTERBOTCTRL_FIRMWARE_CHARGINGFULLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_CHARGINGFULLWINDOW_H

#include "Components/HomeWindow.h"

class ChargingFullWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB ChargingFull action
	 * @param parent parent LVGL object
	 * @param params data for ChargingFull action
	 */
	ChargingFullWindow(lv_obj_t* parent, const BatteryData* params);

private:
	static constexpr const char* TITLE = "Charged";

	void buildUI(BatteryData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_CHARGINGFULLWINDOW_H
