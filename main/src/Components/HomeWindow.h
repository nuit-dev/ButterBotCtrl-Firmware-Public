#ifndef BUTTERBOTCTRL_FIRMWARE_HOMEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_HOMEWINDOW_H

#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <BBData.h>

#include "Services/ThemeService.h"

class HomeWindow : public LVObject {
public:
	/**
	 * Generic UI element that contains home screen windows
	 * @param parent parent LVGL object
	 * @param title header title of the window
	 */
	HomeWindow(lv_obj_t* parent, const char* title);
	~HomeWindow() override;

	/* Main update loop, called by the parent */
	virtual void loop(){};

	/**
	 * Update the window based on received data
	 * @param data new received data
	 */
	virtual void onData(const BBData* data){};

	/* Cleanup before window is dismissed from screen */
	virtual void onDismiss(){};

protected:
	ThemeService* theme;
	lv_obj_t* innerContent;
	LVStyle labelDefaultStyle;

	/* Properly scales window top bar with inner content */
	void updateLayout();

private:
	lv_obj_t* titleEl;
	lv_obj_t* titleLabel;
	lv_obj_t* corner;

	void buildUI(lv_obj_t* parent, const char* title);
};


#endif //BUTTERBOTCTRL_FIRMWARE_HOMEWINDOW_H
