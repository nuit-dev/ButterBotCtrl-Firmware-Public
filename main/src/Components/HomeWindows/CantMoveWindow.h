#ifndef BUTTERBOTCTRL_FIRMWARE_CANTMOVEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_CANTMOVEWINDOW_H

#include "Components/HomeWindow.h"

class CantMoveWindow : public HomeWindow {
public:
	CantMoveWindow(lv_obj_t* parent, const CantMoveData* params);

private:
	static constexpr const char* TITLE = "Can't Move";
	static constexpr const char* SentenceCharging = "Can't Move. Charging";
	static constexpr const char* SentenceUnstable = "Can't Move. Unstable";

	void buildUI(CantMoveData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_CANTMOVEWINDOW_H
