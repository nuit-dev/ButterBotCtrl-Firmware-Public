#ifndef BUTTERBOTCTRL_FIRMWARE_WHATSTHISWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_WHATSTHISWINDOW_H

#include "Components/HomeWindow.h"

class WhatsThisWindow : public HomeWindow {
public:
	/**
	 * Window that shows the BB WhatsThis object-detection action
	 * @param parent parent LVGL object
	 * @param params data for WhatsThis action
	 */
	WhatsThisWindow(lv_obj_t* parent, const WhatsThisData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "What's this?";

	void buildUI(WhatsThisData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_WHATSTHISWINDOW_H
