#ifndef BUTTERBOTCTRL_FIRMWARE_SHUTDOWNWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_SHUTDOWNWINDOW_H

#include "Components/HomeWindow.h"

class ShutdownWindow : public HomeWindow {
public:
	/**
	 * Window shown when the ButterBot is about to shut down.
	 * @param parent parent LVGL object
	 * @param params data for the Shutdown action
	 */
	ShutdownWindow(lv_obj_t* parent, const ShutdownData* params);

private:
	static constexpr const char* TITLE = "Shutdown";
	static constexpr const char* Sentence = "Shutting down";

	void buildUI(ShutdownData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_SHUTDOWNWINDOW_H
