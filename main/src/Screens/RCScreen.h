#ifndef BUTTERBOTCTRL_FIRMWARE_RCSCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_RCSCREEN_H

#include <LV_Interface/LVScreen.h>
#include <Memory/SmartPtr/StrongObjectPtr.h>
#include <CtrlData.h>
#include <Enums.hpp>
#include <Components/TopBar.h>
#include <Services/ButtonInput.h>
#include <Services/Com.h>
#include <Util/WiFiAccessPoint.h>

class RCScreen : public LVScreen{
public:
	RCScreen();
	virtual ~RCScreen() override;

private:
	virtual void loop() override;

private:
	StrongObjectPtr<WiFiAccessPoint> accessPoint;
	StrongObjectPtr<class Feed> feed;

	TopBar* topBar;

	DriveData lastData = {};
	uint64_t lastDataSend = 0;
	bool overridePressed = false;

	uint8_t* feedBuf;
	lv_img_dsc_t imgDsc = {
		.header = {
			.magic = LV_IMAGE_HEADER_MAGIC,
			.cf = LV_COLOR_FORMAT_RGB565,
			.flags = 0,
			.w = 128, .h = 128,
			.stride = 128 * 2,
			.reserved_2 = 0,
		},
		.data_size = 128 * 128 * 2,
	};
	lv_obj_t* feedImg;

private:
	void handleButtonEvent(Button btn, ButtonInput::Action action);
	void onConnectionStatus(Com::ConnStatus status);
	void onAPConnectionStatus(WiFiAccessPoint::EventType type);
};

#endif //BUTTERBOTCTRL_FIRMWARE_RCSCREEN_H