#ifndef BUTTERBOTCTRL_FIRMWARE_IR_FORGETWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_IR_FORGETWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class IR_forgetWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB IR_forget action
	 * @param parent parent LVGL object
	 * @param params data for IR_forget action
	 */
	IR_forgetWindow(lv_obj_t* parent, const IR_forgetData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "IR_forget";

	LVGIF* gif = nullptr;

	void buildUI(IR_forgetData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_IR_FORGETWINDOW_H
