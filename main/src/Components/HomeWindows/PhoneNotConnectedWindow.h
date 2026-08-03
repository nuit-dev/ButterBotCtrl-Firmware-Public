#ifndef BUTTERBOTCTRL_FIRMWARE_PHONENOTCONNECTEDWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_PHONENOTCONNECTEDWINDOW_H

#include "Components/HomeWindow.h"

class PhoneNotConnectedWindow : public HomeWindow {
public:
	/**
	 * Window shown when a phone scenario is attempted without a connected phone.
	 * @param parent parent LVGL object
	 * @param params data for the PhoneNotConnected action
	 */
	PhoneNotConnectedWindow(lv_obj_t* parent, const PhoneNotConnectedData* params);

private:
	static constexpr const char* TITLE = "Phone";
	static constexpr const char* Sentence = "Phone? What phone?";

	void buildUI();
};

#endif //BUTTERBOTCTRL_FIRMWARE_PHONENOTCONNECTEDWINDOW_H
