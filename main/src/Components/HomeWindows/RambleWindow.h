#ifndef BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H

#include "Components/HomeWindow.h"

class RambleWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Ramble action
	 * @param parent parent LVGL object
	 * @param params data for Ramble action
	 */
	RambleWindow(lv_obj_t* parent, const RambleData* params);

private:
	lv_obj_t* sentenceLabel = nullptr;
	lv_obj_t* viewport = nullptr; // Custom (NUIT): lines over LongThreshold only

	static constexpr const char* TITLE = "Ramble";

	// Custom (NUIT): same sizes as QuoteWindow - up to 140 characters fit in a 90 px wide label, longer ones scroll
	static constexpr size_t LongThreshold = 140;
	static constexpr int32_t LabelWidth = 90;
	static constexpr int32_t ViewportWidth = 92;
	static constexpr int32_t ViewportHeight = 70;
	static constexpr uint32_t ScrollDelayMs = 1500; // let the first lines be read before it moves
	static constexpr uint32_t ScrollMsPerChar = 60; // about the speed the robot speaks

	void buildUI(RambleData data);
};


#endif //BUTTERBOTCTRL_FIRMWARE_RAMBLEWINDOW_H
