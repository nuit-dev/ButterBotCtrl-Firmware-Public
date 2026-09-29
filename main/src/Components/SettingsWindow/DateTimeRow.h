#ifndef BUTTERBOTCTRL_FIRMWARE_DATETIMEROW_H
#define BUTTERBOTCTRL_FIRMWARE_DATETIMEROW_H

#include <array>
#include <functional>
#include <LV_Interface/LVObject.h>
#include <LV_Interface/LVStyle.h>
#include <Services/ThemeService.h>

/**
 * Custom (NUIT): Settings rows DATE (DD.MM.YYYY) and TIME (HH:MM, 24 h) - the robot's clock.
 * Joystick press (handled by SettingsWindow) starts editing: left / right picks the field, up / down changes it,
 * another press sets the robot's clock.
 */
class DateTimeRow : public LVObject {
public:
	enum class Kind : uint8_t { Date, Time };

	struct Value {
		uint16_t year = 2026;
		uint8_t month = 1, day = 1, hour = 12, minute = 0;
	};

	DateTimeRow(lv_obj_t* parent, Kind kind, const std::function<void(uint32_t)>& keyCb);
	~DateTimeRow() override = default;

	lv_obj_t* widgetLabel;

	// Not editing: shows the robot's time, or dashes when it isn't known
	void show(bool known, const Value& value);

	bool isEditing() const{ return editing; }
	void startEdit(const Value& value);
	Value finishEdit();

private:
	ThemeService* theme;
	LVStyle labelDefaultStyle;
	LVStyle fieldSelectedStyle;

	const Kind kind;
	std::function<void(uint32_t)> keyCb;

	bool editing = false;
	uint8_t field = 0;
	Value value;

	static constexpr uint8_t MaxFields = 3;
	std::array<lv_obj_t*, MaxFields> fields{};
	uint8_t fieldCount() const{ return kind == Kind::Date ? 3 : 2; }

	void buildUI();
	void refresh(bool known);
	void changeValue(int dir);
	static uint8_t daysInMonth(uint16_t year, uint8_t month);
};

#endif //BUTTERBOTCTRL_FIRMWARE_DATETIMEROW_H
