#ifndef BUTTERBOTCTRL_FIRMWARE_BATTERYELEMENT_H
#define BUTTERBOTCTRL_FIRMWARE_BATTERYELEMENT_H

#include <LV_Interface/LVObject.h>
#include <BBData.h>
#include "Services/ThemeService.h"

class BatteryElement : public LVObject {
public:
	/**
	 * UI element that shows the battery level of a device
	 * @param parent parent LVGL object
	 * @param level range 0-6
	 */
	BatteryElement(lv_obj_t* parent, uint8_t level);

	/**
	 * Set device power level
	 * @param level range 0-6
	 */
	void setLevel(uint8_t level);

	/**
	 * Set charge state. While Charging, loop() cycles through battery images;
	 * Full pins the highest level; Unplugged shows the real level.
	 */
	void setChargeStatus(ChargeStatus status);

	void blink();

	/* Drives the charging animation; call once per frame from the parent's loop. */
	void loop();

private:
	static constexpr uint8_t MinLevel = 0;
	static constexpr uint8_t MaxLevel = 6;

	static constexpr Asset BatLevels[MaxLevel + 1] = {
		Asset::Battery1, Asset::Battery1, Asset::Battery1, Asset::Battery2, Asset::Battery3, Asset::Battery4, Asset::Battery5
	};

	// Distinct battery images cycled through while charging.
	static constexpr Asset ChargeFrames[] = {
		Asset::Battery1, Asset::Battery2, Asset::Battery3, Asset::Battery4, Asset::Battery5
	};
	static constexpr uint32_t ChargeFrameMs = 200;

	ThemeService* theme;
	lv_obj_t* img;
	bool isHidden = false;

	uint8_t currentLevel = 0;
	ChargeStatus charge = ChargeStatus::Unplugged;
	uint8_t chargeFrame = 0;
	uint64_t lastChargeMillis = 0;

	void initStyles();
};


#endif //BUTTERBOTCTRL_FIRMWARE_BATTERYELEMENT_H
