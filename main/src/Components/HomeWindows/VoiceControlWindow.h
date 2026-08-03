#ifndef BUTTERBOTCTRL_FIRMWARE_VOICECONTROLWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_VOICECONTROLWINDOW_H

#include "Components/HomeWindow.h"

class VoiceControlWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB VoiceControl action
	 * @param parent parent LVGL object
	 * @param params data for VoiceControl action
	 */
	VoiceControlWindow(lv_obj_t* parent, const VoiceControlData* params);

	void onData(const BBData* data) override;

private:
	static constexpr const char* TITLE = "Voice Control";

	void buildUI(VoiceControlData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_VOICECONTROLWINDOW_H
