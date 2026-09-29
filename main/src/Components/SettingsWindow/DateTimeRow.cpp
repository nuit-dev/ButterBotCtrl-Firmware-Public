#include "DateTimeRow.h"

#include <cstdio>
#include <Fonts/font.hpp>
#include <misc/lv_event_private.h>

DateTimeRow::DateTimeRow(lv_obj_t* parent, const Kind kind, const std::function<void(uint32_t)>& keyCb)
	: LVObject(parent), kind(kind), keyCb(keyCb){
	theme = Application::getApp()->getService<ThemeService>();

	lv_obj_add_event_cb(*this, [](lv_event_t* e) {
		const uint32_t key = lv_event_get_key(e);
		const auto row = (DateTimeRow*)(e->user_data);

		if(!row->editing){
			if(key == LV_KEY_UP || key == LV_KEY_DOWN){
				row->keyCb(key); // WINDOW CALLBACK
			}
			return;
		}

		if(key == LV_KEY_LEFT){
			row->field = (row->field + row->fieldCount() - 1) % row->fieldCount();
		}else if(key == LV_KEY_RIGHT){
			row->field = (row->field + 1) % row->fieldCount();
		}else if(key == LV_KEY_UP){
			row->changeValue(1);
		}else if(key == LV_KEY_DOWN){
			row->changeValue(-1);
		}else{
			return;
		}
		row->refresh(true);
	}, LV_EVENT_KEY, this);

	buildUI();
	refresh(false);
}

void DateTimeRow::buildUI(){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	lv_obj_set_size(*this, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_style_bg_opa(*this, LV_OPA_TRANSP, 0);

	lv_style_set_text_font(labelDefaultStyle, &lv_font_butter);
	lv_style_set_text_color(labelDefaultStyle, colorPrim);
	lv_style_set_bg_opa(labelDefaultStyle, LV_OPA_TRANSP);

	lv_style_set_bg_color(fieldSelectedStyle, colorPrim);
	lv_style_set_bg_opa(fieldSelectedStyle, LV_OPA_COVER);
	lv_style_set_text_color(fieldSelectedStyle, colorTert);

	widgetLabel = lv_label_create(*this);
	lv_obj_add_style(widgetLabel, labelDefaultStyle, 0);
	lv_label_set_text_static(widgetLabel, kind == Kind::Date ? "DATE" : "TIME");
	lv_obj_set_width(widgetLabel, 24);
	lv_obj_set_pos(widgetLabel, 2, 4);

	// Box like THEME / SLEEP
	lv_obj_t* box = lv_obj_create(*this);
	lv_obj_set_size(box, 83, 15);
	lv_obj_set_pos(box, 28, 0);
	lv_obj_set_style_border_width(box, 1, 0);
	lv_obj_set_style_border_color(box, colorPrim, 0);
	lv_obj_set_style_bg_color(box, colorTert, 0);
	lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
	lv_obj_set_style_bg_image_src(box, theme->getAsset(Asset::Grid), 0);
	lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);

	// Fields and separators in a centered row
	lv_obj_t* line = lv_obj_create(box);
	lv_obj_set_size(line, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_style_bg_opa(line, LV_OPA_TRANSP, 0);
	lv_obj_set_style_border_width(line, 0, 0);
	lv_obj_set_style_pad_all(line, 0, 0);
	lv_obj_remove_flag(line, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_layout(line, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(line, LV_FLEX_FLOW_ROW);
	lv_obj_center(line);

	const char* separator = kind == Kind::Date ? "." : ":";
	for(uint8_t i = 0; i < fieldCount(); i++){
		if(i > 0){
			lv_obj_t* sep = lv_label_create(line);
			lv_obj_add_style(sep, labelDefaultStyle, 0);
			lv_label_set_text_static(sep, separator);
		}
		fields[i] = lv_label_create(line);
		lv_obj_add_style(fields[i], labelDefaultStyle, 0);
		lv_obj_add_style(fields[i], fieldSelectedStyle, LV_STATE_CHECKED);
		lv_obj_set_style_pad_hor(fields[i], 1, 0);
	}
}

void DateTimeRow::show(bool known, const Value& shown){
	if(editing) return;
	value = shown;
	refresh(known);
}

void DateTimeRow::startEdit(const Value& start){
	value = start;
	editing = true;
	field = 0;
	refresh(true);
}

DateTimeRow::Value DateTimeRow::finishEdit(){
	editing = false;
	refresh(true);
	return value;
}

void DateTimeRow::refresh(bool known){
	char buf[3][6];
	if(kind == Kind::Date){
		if(known){
			snprintf(buf[0], sizeof(buf[0]), "%02u", value.day);
			snprintf(buf[1], sizeof(buf[1]), "%02u", value.month);
			snprintf(buf[2], sizeof(buf[2]), "%04u", value.year);
		}else{
			snprintf(buf[0], sizeof(buf[0]), "--");
			snprintf(buf[1], sizeof(buf[1]), "--");
			snprintf(buf[2], sizeof(buf[2]), "----");
		}
	}else{
		if(known){
			snprintf(buf[0], sizeof(buf[0]), "%02u", value.hour);
			snprintf(buf[1], sizeof(buf[1]), "%02u", value.minute);
		}else{
			snprintf(buf[0], sizeof(buf[0]), "--");
			snprintf(buf[1], sizeof(buf[1]), "--");
		}
	}

	for(uint8_t i = 0; i < fieldCount(); i++){
		lv_label_set_text(fields[i], buf[i]);
		if(editing && i == field){
			lv_obj_add_state(fields[i], LV_STATE_CHECKED);
		}else{
			lv_obj_remove_state(fields[i], LV_STATE_CHECKED);
		}
	}
}

uint8_t DateTimeRow::daysInMonth(uint16_t year, uint8_t month){
	static constexpr uint8_t Days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if(month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) return 29;
	return Days[(month - 1) % 12];
}

void DateTimeRow::changeValue(int dir){
	const auto wrap = [](int v, int min, int max){
		if(v > max) return min;
		if(v < min) return max;
		return v;
	};

	if(kind == Kind::Date){
		if(field == 0){
			value.day = wrap(value.day + dir, 1, daysInMonth(value.year, value.month));
		}else if(field == 1){
			value.month = wrap(value.month + dir, 1, 12);
		}else{
			value.year = wrap(value.year + dir, 2024, 2099);
		}
		// Keep the day valid for the new month / year
		const uint8_t maxDay = daysInMonth(value.year, value.month);
		if(value.day > maxDay) value.day = maxDay;
	}else{
		if(field == 0){
			value.hour = wrap(value.hour + dir, 0, 23);
		}else{
			value.minute = wrap(value.minute + dir, 0, 59);
		}
	}
}
