#ifndef BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H
#define BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H

#include <string>
#include <vector>
#include "Components/HomeWindow.h"

/**
 * Custom (NUIT): shows a quote from the quote menu items. Long quotes (Ultron, Daisy) are shown in full in a
 * scrolling box that follows the robot sentence by sentence.
 */
class QuoteWindow : public HomeWindow {
public:
	QuoteWindow(lv_obj_t* parent, const QuoteData* params);

	void onData(const BBData* data) override;

private:
	lv_obj_t* sentenceLabel;
	lv_obj_t* viewport = nullptr; // long quotes only
	std::vector<uint32_t> partStarts; // letter index where each sentence starts

	static constexpr int32_t ViewportWidth = 92;
	static constexpr int32_t ViewportHeight = 70;

	void setText(const QuoteData& data);
};

#endif //BUTTERBOTCTRL_FIRMWARE_QUOTEWINDOW_H
