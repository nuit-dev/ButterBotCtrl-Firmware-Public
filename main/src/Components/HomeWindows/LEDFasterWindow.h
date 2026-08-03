#ifndef BUTTERBOTCTRL_FIRMWARE_LEDFASTERWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_LEDFASTERWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class LEDFasterWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB LEDFaster action
	 * @param parent parent LVGL object
	 * @param params data for LEDFaster action
	 */
	LEDFasterWindow(lv_obj_t* parent, const LEDFasterData* params);

	void onDismiss() override;

private:
	static constexpr const char* TITLE = "LED Faster";
	static constexpr const char* Sentence = "Fast and luminous";

	LVGIF* gif = nullptr;

	void buildUI(LEDFasterData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_LEDFASTERWINDOW_H
