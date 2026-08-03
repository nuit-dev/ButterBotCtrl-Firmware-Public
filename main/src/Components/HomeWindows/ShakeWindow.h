#ifndef BUTTERBOTCTRL_FIRMWARE_SHAKEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_SHAKEWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class ShakeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Shake action
	 * @param parent parent LVGL object
	 * @param params data for Shake action
	 */
	ShakeWindow(lv_obj_t* parent, const ShakeData* params);

	void onDismiss() override;

private:
	LVGIF* gif;
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Shake";
	static constexpr const char* Sentence = "Blender mode??? Nooooooo";

	void buildUI(ShakeData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_SHAKEWINDOW_H
