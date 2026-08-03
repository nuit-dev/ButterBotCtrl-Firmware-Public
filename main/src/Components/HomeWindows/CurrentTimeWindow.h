#ifndef BUTTERBOTCTRL_FIRMWARE_CURRENTTIMEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_CURRENTTIMEWINDOW_H

#include "Components/HomeWindow.h"

class CurrentTimeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB CurrentTime action
	 * @param parent parent LVGL object
	 * @param params data for CurrentTime action
	 */
	CurrentTimeWindow(lv_obj_t* parent, const CurrentTimeData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Current Time";

	void buildUI(CurrentTimeData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_CURRENTTIMEWINDOW_H
