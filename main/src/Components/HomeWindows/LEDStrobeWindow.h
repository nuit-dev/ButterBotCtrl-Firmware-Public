#ifndef BUTTERBOTCTRL_FIRMWARE_LEDSTROBEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_LEDSTROBEWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class LEDStrobeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB LEDStrobe action
	 * @param parent parent LVGL object
	 * @param params data for LEDStrobe action
	 */
	LEDStrobeWindow(lv_obj_t* parent, const LEDStrobeData* params);

	void onDismiss() override;

private:
	static constexpr const char* TITLE = "LED Strobe";
	static constexpr const char* Sentence = "Warning: rave imminent";

	LVGIF* gif = nullptr;

	void buildUI(LEDStrobeData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_LEDSTROBEWINDOW_H
