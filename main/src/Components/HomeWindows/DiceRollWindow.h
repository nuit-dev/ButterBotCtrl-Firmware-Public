#ifndef BUTTERBOTCTRL_FIRMWARE_DICEROLLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_DICEROLLWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class DiceRollWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB DiceRoll action
	 * @param parent parent LVGL object
	 * @param params data for DiceRoll action
	 */
	DiceRollWindow(lv_obj_t* parent, const DiceRollData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "Dice Roll";

	LVGIF* gif = nullptr;

	void buildUI(DiceRollData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_DICEROLLWINDOW_H
