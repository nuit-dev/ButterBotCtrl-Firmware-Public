#ifndef BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H

#include "Components/HomeWindow.h"

/** Custom (NUIT): shows a quote from the OVERKLOKING / BENDER / ULTRON items; long quotes follow the robot sentence by sentence */
class QuoteWindow : public HomeWindow {
public:
	QuoteWindow(lv_obj_t* parent, const QuoteData* params);

	void onData(const BBData* data) override;

private:
	lv_obj_t* sentenceLabel;

	void setText(const QuoteData& data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H
