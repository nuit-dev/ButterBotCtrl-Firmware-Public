#ifndef BUTTERBOTCTRL_FIRMWARE_IR_FORGETALLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_IR_FORGETALLWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class IR_forgetAllWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB IR_forget action
	 * @param parent parent LVGL object
	 * @param params data for IR_forget action
	 */
	IR_forgetAllWindow(lv_obj_t* parent, const IR_forgetAllData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "IR Forget All";

	LVGIF* gif = nullptr;

	void buildUI(IR_forgetAllData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_IR_FORGETALLWINDOW_H