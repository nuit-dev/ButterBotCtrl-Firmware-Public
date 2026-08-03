#ifndef BUTTERBOTCTRL_FIRMWARE_INTROSCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_INTROSCREEN_H

#include <LV_Interface/LVScreen.h>
#include <LV_Interface/LVGIF.h>

class IntroScreen : public LVScreen {
public:
	IntroScreen();

private:
	LVGIF* introGif = nullptr;
};

#endif //BUTTERBOTCTRL_FIRMWARE_INTROSCREEN_H
