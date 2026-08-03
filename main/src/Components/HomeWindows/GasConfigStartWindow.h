#ifndef BUTTERBOTCTRL_FIRMWARE_GASCONFIGSTARTWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_GASCONFIGSTARTWINDOW_H

#include "Components/HomeWindow.h"

class GasConfigStartWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB started Gas module configuration
	 * @param parent parent LVGL object
	 * @param params data for GasConfigureStart action
	 */
	GasConfigStartWindow(lv_obj_t* parent, const GasConfigureStartData* params);

private:
	static constexpr const char* TITLE = "Gas Config Start";

	void buildUI(GasConfigureStartData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_GASCONFIGSTARTWINDOW_H
