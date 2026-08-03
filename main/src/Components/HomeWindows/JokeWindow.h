#ifndef BUTTERBOTCTRL_FIRMWARE_JOKEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_JOKEWINDOW_H

#include "Components/HomeWindow.h"

class JokeWindow : public HomeWindow {
public:
	/**
	 * Window that shows BB Joke action
	 * @param parent parent LVGL object
	 * @param params data for Joke action
	 */
	JokeWindow(lv_obj_t* parent, const JokeData* params);

private:
	lv_obj_t* sentenceLabel;

	static constexpr const char* TITLE = "Joke";

	void buildUI(JokeData data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_JOKEWINDOW_H
