#ifndef BUTTERBOTCTRL_FIRMWARE_HOLDPOPUPWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_HOLDPOPUPWINDOW_H

#include "Components/HomeWindow.h"

class HoldPopupWindow : public HomeWindow {
public:
	/**
	 * Pop-up shown while a button is held, with a bar that fills over the hold duration.
	 * @param parent parent LVGL object
	 * @param title header title of the pop-up
	 * @param fillDurationMs time for the bar to fill completely, measured from creation
	 */
	HoldPopupWindow(lv_obj_t* parent, const char* title, uint64_t fillDurationMs = DefaultFillDurationMs);

	void loop() override;

	/**
	 * Remove the fill bar and show a text hint instead. The pop-up no longer auto-hides
	 * once in hint mode; the owner is expected to clear it.
	 * @param text hint text to display
	 */
	void showHint(const char* text);

	/** @return true if the fill bar is present and has filled completely */
	bool isBarFull() const;

private:
	static constexpr uint64_t DefaultFillDurationMs = 2000;

	lv_obj_t* loadBar = nullptr;
	uint64_t createdTime = 0;
	uint64_t fillDurationMs;

	void buildUI();
};

#endif //BUTTERBOTCTRL_FIRMWARE_HOLDPOPUPWINDOW_H
