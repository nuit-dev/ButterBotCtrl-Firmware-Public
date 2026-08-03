#ifndef BUTTERBOTCTRL_FIRMWARE_IR_TRAINWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_IR_TRAINWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class IR_trainWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB IR_train action
	 * @param parent parent LVGL object
	 * @param params data for IR_train action
	 */
	IR_trainWindow(lv_obj_t* parent, const IR_trainData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "IR_train";

	LVGIF* gif = nullptr;

	void buildUI(IR_trainData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_IR_TRAINWINDOW_H
