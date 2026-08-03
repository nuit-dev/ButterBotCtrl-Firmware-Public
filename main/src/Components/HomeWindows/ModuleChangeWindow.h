#ifndef BUTTERBOTCTRL_FIRMWARE_MODULECHANGEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_MODULECHANGEWINDOW_H

#include "Components/HomeWindow.h"

class ModuleChangeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB ModuleChange action
	 * @param parent parent LVGL object
	 * @param params data for ModuleChange action
	 */
	ModuleChangeWindow(lv_obj_t* parent, const ModuleData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Module Change";
	static constexpr const char* ModuleRemovedSentence = "Lost a limb";

	void buildUI(ModuleData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_MODULECHANGEWINDOW_H
