#ifndef BUTTERBOTCTRL_FIRMWARE_INTRUDERDETECTIONWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_INTRUDERDETECTIONWINDOW_H

#include "Components/HomeWindow.h"

class IntruderDetectionWindow : public HomeWindow {
protected:
	IntruderDetectionWindow(lv_obj_t* parent, const PIRModuleData* params, bool activated);

private:
	static constexpr const char* TITLE = "Intruder Detection";

	void buildUI(PIRModuleData data, bool activated);
};

class IntruderDetectionOnWindow : public IntruderDetectionWindow {
public:
	IntruderDetectionOnWindow(lv_obj_t* parent, const PIRModuleData* params)
		: IntruderDetectionWindow(parent, params, true) {}
};

class IntruderDetectionOffWindow : public IntruderDetectionWindow {
public:
	IntruderDetectionOffWindow(lv_obj_t* parent, const PIRModuleData* params)
		: IntruderDetectionWindow(parent, params, false) {}
};

#endif //BUTTERBOTCTRL_FIRMWARE_INTRUDERDETECTIONWINDOW_H
