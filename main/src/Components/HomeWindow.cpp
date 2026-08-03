#include "HomeWindow.h"

#include "Fonts/font.hpp"

HomeWindow::HomeWindow(lv_obj_t* parent, const char* title) : LVObject(parent){
	const auto app = Application::getApp();
	theme = app->getService<ThemeService>();

	buildUI(parent, title);
}

HomeWindow::~HomeWindow(){}

void HomeWindow::buildUI(lv_obj_t* parent, const char* title){
	const lv_color_t colorPrim = theme->getPrimaryColor();
	const lv_color_t colorTert = theme->getTertiaryColor();

	titleEl = lv_obj_create(*this);
	titleLabel = lv_label_create(titleEl);
	lv_label_set_text(titleLabel, title);

	corner = lv_image_create(*this);
	lv_obj_add_flag(corner, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_add_flag(corner, LV_OBJ_FLAG_FLOATING);
	innerContent = lv_obj_create(*this);

	// CONTAINER
	lv_obj_set_size(*this, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_layout(*this, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(*this, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(*this, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_remove_flag(*this, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_style_border_width(innerContent, 1, 0);
	lv_obj_set_style_border_color(innerContent, colorPrim, 0);
	lv_obj_set_style_bg_color(*this, colorTert, 0);
	lv_obj_set_style_bg_image_src(*this, theme->getAsset(Asset::Grid), 0);

	// INNER WINDOW
	lv_obj_set_layout(innerContent, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(innerContent, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(innerContent, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_size(innerContent, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_style_pad_all(innerContent, 1, 0);
	lv_obj_set_style_bg_opa(innerContent, LV_OPA_TRANSP, 0);
	lv_obj_remove_flag(innerContent, LV_OBJ_FLAG_SCROLLABLE);

	// TITLE
	lv_obj_set_size(titleEl, LV_SIZE_CONTENT, 6);
	lv_obj_set_layout(titleEl, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(titleEl, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(titleEl, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_obj_set_style_pad_hor(titleEl, 1, 0);
	lv_obj_set_style_text_color(titleEl, colorTert, 0);
	lv_obj_set_style_text_font(titleEl, &lv_font_butter, 0);
	lv_obj_set_style_bg_color(titleEl, colorPrim, 0);
	lv_obj_set_style_bg_opa(titleEl, LV_OPA_COVER, 0);

	lv_obj_set_style_pad_right(titleLabel, 6, 0);
	lv_obj_set_style_pad_left(titleLabel, 1, 0);

	// LABEL DEFAULT STYLE
	lv_style_set_text_font(labelDefaultStyle, &lv_font_butter);
	lv_style_set_text_line_space(labelDefaultStyle, 3);
	lv_style_set_text_color(labelDefaultStyle, colorPrim);
	lv_style_set_size(labelDefaultStyle, 80, LV_SIZE_CONTENT);
	lv_style_set_pad_all(labelDefaultStyle, 2);
	lv_style_set_bg_opa(labelDefaultStyle, LV_OPA_TRANSP);

	// CORNER
	lv_obj_set_size(corner, 6, 6);
	lv_obj_set_style_bg_color(corner, colorTert, 0);
	lv_obj_set_style_bg_opa(corner, LV_OPA_COVER, 0);
	lv_image_set_src(corner, theme->getAsset(Asset::Corner));

	updateLayout();
}

void HomeWindow::updateLayout(){
	// Manually update layout so we can get the final object widths
	lv_obj_set_size(innerContent, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_update_layout(innerContent);
	lv_obj_update_layout(titleEl);

	const int32_t innerW = lv_obj_get_width(innerContent);
	const int32_t titleW = lv_obj_get_width(titleLabel);
	if(innerW > titleW){
		lv_obj_set_size(titleEl, innerW, 6);
		lv_obj_set_width(*this, innerW);
	} else{
		lv_obj_set_width(innerContent, titleW + 2);
	}

	lv_obj_align_to(corner, *this, LV_ALIGN_TOP_RIGHT, 0, 0);
};
