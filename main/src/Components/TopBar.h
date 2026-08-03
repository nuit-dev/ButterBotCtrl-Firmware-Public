#ifndef BUTTERBOTCTRL_FIRMWARE_TOPBAR_H
#define BUTTERBOTCTRL_FIRMWARE_TOPBAR_H

#include <LV_Interface/LVObject.h>
#include "Services/ThemeService.h"
#include "Services/Battery.h"
#include "Components/BatteryElement.h"
#include "Components/SignalElement.h"
#include "Components/ShutUpElement.h"
#include "Services/Com.h"
#include "Services/RobotState.h"

class TopBar : public LVObject {
public:
	/**
	 * UI element for top bar with all containing elements
	 * @param parent parent LVGL object
	 */
	TopBar(lv_obj_t* parent);
	~TopBar() override;

	/* Main update loop, called by the parent */
	void loop();

private:
	ThemeService* theme;
	Battery* battery;
	Com* com;
	RobotState* robotState;

	uint64_t lastLoopMillis = 0;

	BatteryElement* ctrlBattery;
	lv_obj_t* ctrlIcon;
	BatteryElement* botBattery;
	lv_obj_t* botIcon;
	SignalElement* signalEl;
	ShutUpElement* shutupEl;

	void updateShutUpEl() const;
	void updateBotBattery(const std::vector<uint8_t>& data) const;

	void buildUI();
};


#endif //BUTTERBOTCTRL_FIRMWARE_TOPBAR_H
