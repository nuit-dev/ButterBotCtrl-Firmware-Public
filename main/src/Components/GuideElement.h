#ifndef BUTTERBOTCTRL_FIRMWARE_GUIDEELEMENT_H
#define BUTTERBOTCTRL_FIRMWARE_GUIDEELEMENT_H

#include <LV_Interface/LVObject.h>
#include <Services/ThemeService.h>

class GuideElement : public LVObject {
public:
	/**
	 * Idle-screen usage hints. Floating: ignores then parent's layout and scroll.
	 * The owner toggles LV_OBJ_FLAG_HIDDEN to show/hide it.
	 * @param parent parent LVGL object
	 */
	GuideElement(lv_obj_t* parent);

private:
	ThemeService* theme;

	void buildUI();
};


#endif //BUTTERBOTCTRL_FIRMWARE_GUIDEELEMENT_H
