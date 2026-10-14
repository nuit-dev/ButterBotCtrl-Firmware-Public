#ifndef BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H

#include <LV_Interface/LVScreen.h>
#include <Services/ButtonInput.h>
#include "Enums.hpp"
#include "BLE/GAP.h"
#include "Services/Com.h"
#include "Services/LEDController.h"

class PairingScreen : public LVScreen {
public:
	PairingScreen();
	~PairingScreen() override;

private:
	BLE::GAP* gap;
	Com* com;
	LEDController* ledController;
	ButtonInput* buttonInput;

	// Custom (NUIT): FAST START may connect before this screen exists, so loop() also checks the status
	bool leaving = false;
	void goHome();

	void loop() override;

	void buildSettingsHint();

	void handleButtonEvent(Button btn, ButtonInput::Action action);
};


#endif //BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H
