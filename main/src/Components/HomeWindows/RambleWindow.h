#ifndef BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H

#include "Components/HomeWindow.h"

class RambleWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Ramble action
	 * @param parent parent LVGL object
	 * @param params data for Ramble action
	 */
	RambleWindow(lv_obj_t* parent, const RambleData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Ramble";

	void buildUI(RambleData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H
