#ifndef BUTTERBOTCTRL_FIRMWARE_SHUTUPELEMENT_H
#define BUTTERBOTCTRL_FIRMWARE_SHUTUPELEMENT_H

#include <LV_Interface/LVObject.h>
#include <Services/ThemeService.h>

class ShutUpElement : public LVObject {
public:
	/**
	 * UI element that shows if the device is muted
	 * @param parent parent LVGL object
	 * @param muted initial muted state
	 */
	ShutUpElement(lv_obj_t* parent, bool muted = false);

	void setMutedStatus(bool muted);

private:
	ThemeService* theme;
	lv_obj_t* img;
	bool isMuted = false;

	void update();
};


#endif //BUTTERBOTCTRL_FIRMWARE_SHUTUPELEMENT_H