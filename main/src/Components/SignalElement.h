#ifndef BUTTERBOTCTRL_FIRMWARE_SIGNALELEMENT_H
#define BUTTERBOTCTRL_FIRMWARE_SIGNALELEMENT_H

#include <LV_Interface/LVObject.h>
#include "Services/ThemeService.h"

class SignalElement : public LVObject {
public:
	/**
	 * UI element that shows the signal level of the device
	 * @param parent parent LVGL object
	 * @param level range 0-2
	 */
	SignalElement(lv_obj_t* parent, uint8_t level);

	/**
	 * Set device signal level
	 * @param level range 0-2
	 */
	void setLevel(uint8_t level);

private:
	static constexpr uint8_t minLevel = 0;
	static constexpr uint8_t maxLevel = 2;

	static constexpr Asset SigLevels[maxLevel + 1] = {
		Asset::Signal1, Asset::Signal2, Asset::Signal3
	};

	ThemeService* theme;
	lv_obj_t* img;

	void initStyles();
};


#endif //BUTTERBOTCTRL_FIRMWARE_SIGNALELEMENT_H
