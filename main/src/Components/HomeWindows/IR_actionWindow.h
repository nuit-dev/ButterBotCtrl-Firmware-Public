#ifndef BUTTERBOTCTRL_FIRMWARE_IR_ACTIONWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_IR_ACTIONWINDOW_H

#include "Components/HomeWindow.h"

class IR_actionWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB IR_action action
	 * @param parent parent LVGL object
	 * @param params data for IR_action action
	 */
	IR_actionWindow(lv_obj_t* parent, const IR_actionData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "IR_action";

	void buildUI(IR_actionData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_IR_ACTIONWINDOW_H
