#ifndef BUTTERBOTCTRL_FIRMWARE_GASMODULEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_GASMODULEWINDOW_H

#include "Components/HomeWindow.h"

class GasModuleWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB GasModule action
	 * @param parent parent LVGL object
	 * @param params data for GasModule action
	 */
	GasModuleWindow(lv_obj_t* parent, const GasData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Gas Module";

	// Truncate too long sentences
	static constexpr int32_t MaxSentenceLines = 3;

	void buildUI(GasData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_GASMODULEWINDOW_H
