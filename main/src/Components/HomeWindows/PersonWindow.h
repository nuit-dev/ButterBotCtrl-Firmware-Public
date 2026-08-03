#ifndef BUTTERBOTCTRL_FIRMWARE_PERSONWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_PERSONWINDOW_H

#include "Components/HomeWindow.h"

class PersonWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Person action
	 * @param parent parent LVGL object
	 * @param params data for Person action
	 */
	PersonWindow(lv_obj_t* parent, const PersonData* params);

private:
	static constexpr const char* TITLE = "Person";

	void buildUI(PersonData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_PERSONWINDOW_H
