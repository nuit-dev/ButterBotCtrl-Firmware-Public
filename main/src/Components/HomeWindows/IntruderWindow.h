#ifndef BUTTERBOTCTRL_FIRMWARE_INTRUDERWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_INTRUDERWINDOW_H

#include "Components/HomeWindow.h"

class IntruderWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Intruder action
	 * @param parent parent LVGL object
	 * @param params data for Intruder action
	 */
	IntruderWindow(lv_obj_t* parent, const IntruderData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Intruder Detection";

	void buildUI(IntruderData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_INTRUDERWINDOW_H