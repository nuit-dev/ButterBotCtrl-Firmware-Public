#ifndef BUTTERBOTCTRL_FIRMWARE_IR_LISTWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_IR_LISTWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class IR_listWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB IR_list action
	 * @param parent parent LVGL object
	 * @param params data for IR_list action
	 */
	IR_listWindow(lv_obj_t* parent, const IR_listData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "IR_list";

	LVGIF* gif = nullptr;

	void buildUI(IR_listData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_IR_LISTWINDOW_H
