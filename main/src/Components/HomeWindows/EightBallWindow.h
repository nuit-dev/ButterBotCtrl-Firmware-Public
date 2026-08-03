#ifndef BUTTERBOTCTRL_FIRMWARE_EIGHTBALLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_EIGHTBALLWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class EightBallWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB EightBall action
	 * @param parent parent LVGL object
	 * @param params data for EightBall action
	 */
	EightBallWindow(lv_obj_t* parent, const EightBallData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "Eight Ball";
	static constexpr const char* ThinkingSentence = "Consulting the void";

	LVGIF* gif = nullptr;

	void buildUI(EightBallData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_EIGHTBALLWINDOW_H
