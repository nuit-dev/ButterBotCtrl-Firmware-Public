#ifndef BUTTERBOTCTRL_FIRMWARE_POKEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_POKEWINDOW_H

#include "Components/HomeWindow.h"

class PokeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Poke action
	 * @param parent parent LVGL object
	 * @param params data for Poke action
	 */
	PokeWindow(lv_obj_t* parent, const PokeData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Poke";

	void buildUI(PokeData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_POKEWINDOW_H
