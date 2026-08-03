#ifndef BUTTERBOTCTRL_FIRMWARE_LISTENWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_LISTENWINDOW_H

#include "Components/HomeWindow.h"

class ListenWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Listen action
	 * @param parent parent LVGL object
	 * @param params data for Listen action
	 */
	ListenWindow(lv_obj_t* parent, const ListenData* params);

	void loop() override;
	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Listen";

	void buildUI(ListenData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_LISTENWINDOW_H
