#ifndef BUTTERBOTCTRL_FIRMWARE_OBSERVEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_OBSERVEWINDOW_H

#include "Components/HomeWindow.h"

class ObserveWindow : public HomeWindow {
public:
	/**
	 * Window that shows the BB Observe object-detection action
	 * @param parent parent LVGL object
	 * @param params data for Observe action
	 */
	ObserveWindow(lv_obj_t* parent, const ObserveData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Observation";

	void buildUI(ObserveData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_OBSERVEWINDOW_H
