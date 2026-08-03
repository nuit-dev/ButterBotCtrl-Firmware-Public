#include "BrightnessSlider.h"

#include <cmath>
#include <Fonts/font.hpp>
#include <misc/lv_event_private.h>
#include <Services/Settings.h>

DEFINE_LOG(BrightnessSlider)

BrightnessSlider::BrightnessSlider(lv_obj_t* parent, const float initVal, const std::function<void(float)>& valCb, const std::function<void(uint32_t)>& keyCb)
	: LVObject(parent), currentVal(initVal), valCb(valCb), keyCb(keyCb){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();

	// CALLBACKS
	lv_obj_add_event_cb(*this, [](lv_event_t* e) {
		uint32_t key = lv_event_get_key(e);
		const auto brightnessSlider = (BrightnessSlider*)(e->user_data);
		const int32_t sliderVal = lv_slider_get_value(brightnessSlider->slider);

		if(key != LV_KEY_RIGHT && key != LV_KEY_LEFT && key != LV_KEY_UP && key != LV_KEY_DOWN) return;
		if(key == LV_KEY_UP || key == LV_KEY_DOWN){
			// WINDOW CALLBACK
			brightnessSlider->keyCb(key);
			return;
		}
		if(key == LV_KEY_RIGHT && sliderVal >= SliderMax) return;
		if(key == LV_KEY_LEFT && sliderVal <= SliderMin) return;

		lv_obj_send_event(brightnessSlider->slider, LV_EVENT_KEY, &key);

		const int32_t newSliderVal = lv_slider_get_value(brightnessSlider->slider);
		const float brightVal = (float)newSliderVal / SliderMax;
		brightnessSlider->valCb(brightVal);
	}, LV_EVENT_KEY, this);

	buildUI(parent);
}

BrightnessSlider::~BrightnessSlider(){}

void BrightnessSlider::buildUI(lv_obj_t* parent){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	lv_obj_set_size(*this, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_style_bg_opa(*this, LV_OPA_TRANSP, 0);

	// LABEL DEFAULT STYLE
	lv_style_set_text_font(labelDefaultStyle, &lv_font_butter);
	lv_style_set_text_line_space(labelDefaultStyle, 3);
	lv_style_set_text_color(labelDefaultStyle, colorPrim);
	lv_style_set_size(labelDefaultStyle, 80, LV_SIZE_CONTENT);
	lv_style_set_bg_opa(labelDefaultStyle, LV_OPA_TRANSP);

	// Brightness label
	widgetLabel = lv_label_create(*this);
	lv_obj_add_style(widgetLabel, labelDefaultStyle, 0);
	lv_label_set_text(widgetLabel, "BRIGHTNESS");
	lv_obj_set_width(widgetLabel, 52);
	lv_obj_set_pos(widgetLabel, 2, 0);

	// Central line
	lv_obj_t* sliderLine = lv_obj_create(*this);
	lv_obj_set_size(sliderLine, 58, 1);
	lv_obj_set_pos(sliderLine, 53, 3);
	lv_obj_set_style_bg_color(sliderLine, colorPrim, 0);

	// Brightness slider
	slider = lv_slider_create(*this);
	lv_obj_set_size(slider, 58, 7);
	lv_obj_set_pos(slider, 53, 0);
	lv_obj_set_style_anim_duration(slider, 0, 0);
	lv_slider_set_range(slider, SliderMin - SliderBuffer, SliderMax + SliderBuffer);
	// SLIDER VAL
	const int32_t sliderVal = std::clamp((int32_t)std::round(currentVal * SliderMax), SliderMin, SliderMax);
	lv_slider_set_value(slider, sliderVal, LV_ANIM_OFF);

	// Set styles
	lv_style_set_border_color(sliderStyleBg, colorPrim);
	lv_style_set_bg_color(sliderStyleBg, colorTert);
	lv_style_set_bg_opa(sliderStyleBg, LV_OPA_TRANSP);
	lv_style_set_border_width(sliderStyleBg, 1);
	lv_style_set_radius(sliderStyleBg, 0);
	lv_style_set_anim_duration(sliderStyleBg, 1000);
	lv_style_set_border_side(sliderStyleBg, (lv_border_side_t)(LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT));

	lv_style_set_bg_opa(sliderStyleIndic, LV_OPA_TRANSP);
	lv_style_set_radius(sliderStyleIndic, 0);

	lv_style_set_bg_color(sliderStyleKnob, colorPrim);
	lv_style_set_border_color(sliderStyleKnob, colorTert);
	lv_style_set_border_width(sliderStyleKnob, 1);
	lv_style_set_radius(sliderStyleKnob, 0);
	lv_style_set_pad_ver(sliderStyleKnob, 0);
	lv_style_set_pad_hor(sliderStyleKnob, -2);

	lv_obj_add_style(slider, sliderStyleBg, 0);
	lv_obj_add_style(slider, sliderStyleIndic, LV_PART_INDICATOR);
	lv_obj_add_style(slider, sliderStyleKnob, LV_PART_KNOB);
}
