#ifndef BUTTERBOTCTRL_FIRMWARE_GASCONFIGENDWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_GASCONFIGENDWINDOW_H

#include "Components/HomeWindow.h"

class GasConfigEndWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB finished Gas module configuration
	 * @param parent parent LVGL object
	 * @param params data for GasConfigureEnd action
	 */
	GasConfigEndWindow(lv_obj_t* parent, const GasConfigureEndData* params);

private:
	static constexpr const char* TITLE = "Gas Config End";

	void buildUI(GasConfigureEndData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_GASCONFIGENDWINDOW_H