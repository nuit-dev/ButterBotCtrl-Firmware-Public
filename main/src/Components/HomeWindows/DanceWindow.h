#ifndef BUTTERBOTCTRL_FIRMWARE_DANCEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_DANCEWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class DanceWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Dance action
	 * @param parent parent LVGL object
	 * @param params data for Dance action
	 */
	DanceWindow(lv_obj_t* parent, const DanceData* params);

	void onDismiss() override;

private:
	static constexpr const char* TITLE = "Dance";

	LVGIF* gif = nullptr;

	void buildUI(DanceData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_DANCEWINDOW_H
