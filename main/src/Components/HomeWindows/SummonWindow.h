#ifndef BUTTERBOTCTRL_FIRMWARE_SUMMONWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_SUMMONWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class SummonWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Summon action
	 * @param parent parent LVGL object
	 * @param params data for Summon action
	 */
	SummonWindow(lv_obj_t* parent, const SummonData* params);

	void loop() override;
	void onDismiss() override;

private:
	LVGIF* gif = nullptr;

	uint64_t lastMillis = 0;
	int32_t progressVal = 0;

	static constexpr const char* TITLE = "Summon";
	static constexpr int32_t ProgressBarMax = 2000;

	void buildUI(SummonData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_SUMMONWINDOW_H
