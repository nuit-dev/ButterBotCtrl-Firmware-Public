#ifndef BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H
#define BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H

#include <LV_Interface/LVScreen.h>
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

	void loop() override;
};


#endif //BUTTERBOTCTRL_FIRMWARE_PAIRINGSCREEN_H
