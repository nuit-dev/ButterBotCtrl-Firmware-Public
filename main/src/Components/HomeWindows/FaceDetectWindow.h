#ifndef BUTTERBOTCTRL_FIRMWARE_FACEDETECTWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_FACEDETECTWINDOW_H

#include <LV_Interface/LVGIF.h>
#include "Components/HomeWindow.h"

class FaceDetectWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB FaceDetect action
	 * @param parent parent LVGL object
	 * @param params data for FaceDetect action
	 */
	FaceDetectWindow(lv_obj_t* parent, const FaceDetectData* params);

	void onData(const BBData* data) override;
	void onDismiss() override;

private:
	static constexpr const char* TITLE = "Face Detection";

	LVGIF* gif = nullptr;

	void buildUI(FaceDetectData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_FACEDETECTWINDOW_H