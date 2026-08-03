#ifndef BUTTERBOTCTRL_FIRMWARE_FACEFORGETWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_FACEFORGETWINDOW_H

#include "Components/HomeWindow.h"

class FaceForgetWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB FaceForget action
	 * @param parent parent LVGL object
	 * @param params data for FaceForget action
	 */
	FaceForgetWindow(lv_obj_t* parent, const FaceForgetData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Face Forget";

	void buildUI(FaceForgetData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_FACEFORGETWINDOW_H