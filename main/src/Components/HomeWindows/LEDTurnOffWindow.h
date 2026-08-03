#ifndef BUTTERBOTCTRL_FIRMWARE_LEDTURNOFFWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_LEDTURNOFFWINDOW_H

#include "Components/HomeWindow.h"

class LEDTurnOffWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB LEDTurnOff action
	 * @param parent parent LVGL object
	 * @param params data for LEDTurnOff action
	 */
	LEDTurnOffWindow(lv_obj_t* parent, const LEDTurnOffData* params);

private:
	static constexpr const char* TITLE = "LED Turn Off";
	static constexpr const char* Sentence = "Lightless. Loveless.";

	void buildUI(LEDTurnOffData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_LEDTURNOFFWINDOW_H