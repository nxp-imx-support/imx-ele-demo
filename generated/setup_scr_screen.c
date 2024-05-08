/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

#include"read_rng.h"

#include <hsm_api.h>
#include "cryptoExample.h"

extern lv_img_dsc_t g_originalImage;
extern lv_img_dsc_t g_encryptedImage;
extern lv_img_dsc_t g_decryptedImage;

extern crypto_example_t example;
#define IMG_WIDTH 277
#define IMG_HEIGHT 412


void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	ui->g_kb_screen = lv_keyboard_create(ui->screen);
	lv_obj_add_event_cb(ui->g_kb_screen, kb_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_flag(ui->g_kb_screen, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_style_text_font(ui->g_kb_screen, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_size(ui->screen, 1920, 1080);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_1
	ui->screen_cont_1 = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_1, 0, 0);
	lv_obj_set_size(ui->screen_cont_1, 1920, 1080);
	lv_obj_set_scrollbar_mode(ui->screen_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen_cont_1);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1, &_whitebg1_alpha_1920x1080);
	lv_img_set_pivot(ui->screen_img_1, 50,50);
	lv_img_set_angle(ui->screen_img_1, 0);
	lv_obj_set_pos(ui->screen_img_1, 0, 0);
	lv_obj_set_size(ui->screen_img_1, 1920, 1080);
	lv_obj_set_scrollbar_mode(ui->screen_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_3
	ui->screen_cont_3 = lv_obj_create(ui->screen_cont_1);
	lv_obj_set_pos(ui->screen_cont_3, 0, 160);
	lv_obj_set_size(ui->screen_cont_3, 720, 920);
	lv_obj_set_scrollbar_mode(ui->screen_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_3, 148, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_3, lv_color_hex(0xadadad), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_notes
	ui->screen_notes = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_notes, "Disclaimer: 2d-plot represent a very \nsimplified routine for randomness.");
	lv_label_set_long_mode(ui->screen_notes, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_notes, 55, 820);
	lv_obj_set_size(ui->screen_notes, 573, 78);
	lv_obj_set_scrollbar_mode(ui->screen_notes, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_notes, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_notes, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_notes, &lv_font_montserratMedium_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_notes, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_notes, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_notes, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_generate
	ui->screen_generate = lv_btn_create(ui->screen_cont_3);
	ui->screen_generate_label = lv_label_create(ui->screen_generate);
	lv_label_set_text(ui->screen_generate_label, "GENERATE");
	lv_label_set_long_mode(ui->screen_generate_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_generate_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_generate, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_generate, 86, 686);
	lv_obj_set_size(ui->screen_generate, 202, 114);
	lv_obj_set_scrollbar_mode(ui->screen_generate, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_generate, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_generate, 179, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_generate, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_generate, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_generate, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_generate, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_generate, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_generate, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_generate, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_2
	ui->screen_cont_2 = lv_obj_create(ui->screen_cont_3);
	lv_obj_set_pos(ui->screen_cont_2, 90, 136);
	lv_obj_set_size(ui->screen_cont_2, 510, 510);
	lv_obj_set_scrollbar_mode(ui->screen_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_2, lv_color_hex(0xf5f5f5), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	// //Write codes screen_img_2
	// ui->screen_img_2 = lv_img_create(ui->screen_cont_2);
	// lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
	// lv_img_set_src(ui->screen_img_2, &_point_alpha_2x2);
	// lv_img_set_pivot(ui->screen_img_2, 50,50);
	// lv_img_set_angle(ui->screen_img_2, 0);
	// lv_obj_set_pos(ui->screen_img_2, 276, 398);
	// lv_obj_set_size(ui->screen_img_2, 2, 2);
	// lv_obj_set_scrollbar_mode(ui->screen_img_2, LV_SCROLLBAR_MODE_OFF);

	// //Write style for screen_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	// lv_obj_set_style_img_opa(ui->screen_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_label_4, "0");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_4, 20, 647);
	lv_obj_set_size(ui->screen_label_4, 100, 32);
	lv_obj_set_scrollbar_mode(ui->screen_label_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_label_2, "255");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_2, 574, 650);
	lv_obj_set_size(ui->screen_label_2, 100, 32);
	lv_obj_set_scrollbar_mode(ui->screen_label_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_label_3, "255");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_3, -3, 139);
	lv_obj_set_size(ui->screen_label_3, 100, 32);
	lv_obj_set_scrollbar_mode(ui->screen_label_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_clear
	ui->screen_clear = lv_btn_create(ui->screen_cont_3);
	ui->screen_clear_label = lv_label_create(ui->screen_clear);
	lv_label_set_text(ui->screen_clear_label, "CLEAR");
	lv_label_set_long_mode(ui->screen_clear_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_clear_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_clear, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_clear, 401, 686);
	lv_obj_set_size(ui->screen_clear, 202, 114);
	lv_obj_set_scrollbar_mode(ui->screen_clear, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_clear, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_clear, 181, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_clear, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_clear, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_clear, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_clear, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_clear, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_clear, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_clear, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_nums
	ui->screen_nums = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_nums, "");
	lv_label_set_long_mode(ui->screen_nums, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_nums, 72, 41);
	lv_obj_set_size(ui->screen_nums, 552, 63);
	lv_obj_set_scrollbar_mode(ui->screen_nums, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_nums, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_nums, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_nums, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_nums, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_nums, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_nums, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_4
	ui->screen_cont_4 = lv_obj_create(ui->screen_cont_1);
	lv_obj_set_pos(ui->screen_cont_4, 730, 160);
	lv_obj_set_size(ui->screen_cont_4, 1186, 920);
	lv_obj_set_scrollbar_mode(ui->screen_cont_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_4, 147, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_4, lv_color_hex(0xadadad), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_exit
	ui->screen_exit = lv_btn_create(ui->screen_cont_4);
	ui->screen_exit_label = lv_label_create(ui->screen_exit);
	lv_label_set_text(ui->screen_exit_label, "EXIT");
	lv_label_set_long_mode(ui->screen_exit_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_exit_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_exit, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_exit, 920, 686);
	lv_obj_set_size(ui->screen_exit, 190, 111);
	lv_obj_set_scrollbar_mode(ui->screen_exit, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_exit, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_exit, 183, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_exit, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_exit, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_exit, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_exit, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_exit, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_exit, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_dec
	ui->screen_img_dec = lv_img_create(ui->screen_cont_4);
	lv_obj_add_flag(ui->screen_img_dec, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_pivot(ui->screen_img_dec, 50,50);
	lv_img_set_angle(ui->screen_img_dec, 0);
	lv_obj_set_pos(ui->screen_img_dec, 840, 130);
	lv_obj_set_size(ui->screen_img_dec, IMG_WIDTH, IMG_HEIGHT);
	lv_obj_set_scrollbar_mode(ui->screen_img_dec, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_img_dec, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_dec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);




	//Write codes screen_img_ori
	ui->screen_img_ori = lv_img_create(ui->screen_cont_4);
	lv_obj_add_flag(ui->screen_img_ori, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_pivot(ui->screen_img_ori, 50,50);
	lv_img_set_angle(ui->screen_img_ori, 0);
	lv_obj_set_pos(ui->screen_img_ori, 80, 130);
	lv_obj_set_size(ui->screen_img_ori, IMG_WIDTH, IMG_HEIGHT);
	lv_obj_set_scrollbar_mode(ui->screen_img_ori, LV_SCROLLBAR_MODE_OFF);
	lv_img_set_src(ui->screen_img_ori,&g_originalImage);

	//Write style for screen_img_ori, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_ori, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_img_enc
	ui->screen_img_enc = lv_img_create(ui->screen_cont_4);
	lv_obj_add_flag(ui->screen_img_enc, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_pivot(ui->screen_img_enc, 50,50);
	lv_img_set_angle(ui->screen_img_enc, 0);
	lv_obj_set_pos(ui->screen_img_enc, 460, 130);
	lv_obj_set_size(ui->screen_img_enc, IMG_WIDTH, IMG_HEIGHT);
	lv_obj_set_scrollbar_mode(ui->screen_img_enc, LV_SCROLLBAR_MODE_OFF);
	//lv_img_set_src(ui->screen_img_enc,&g_encryptedImage);

	//Write style for screen_img_enc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_img_enc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_encrypt
	ui->screen_encrypt = lv_btn_create(ui->screen_cont_4);
	ui->screen_encrypt_label = lv_label_create(ui->screen_encrypt);
	lv_label_set_text(ui->screen_encrypt_label, "ENCRYPT");
	lv_label_set_long_mode(ui->screen_encrypt_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_encrypt_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_encrypt, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_encrypt, 420, 686);
	lv_obj_set_size(ui->screen_encrypt, 202, 114);
	lv_obj_set_scrollbar_mode(ui->screen_encrypt, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_encrypt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_encrypt, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_encrypt, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_encrypt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_encrypt, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_encrypt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_encrypt, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_encrypt, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_encrypt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_decrypt
	ui->screen_decrypt = lv_btn_create(ui->screen_cont_4);
	ui->screen_decrypt_label = lv_label_create(ui->screen_decrypt);
	lv_label_set_text(ui->screen_decrypt_label, "DECRYPT");
	lv_label_set_long_mode(ui->screen_decrypt_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_decrypt_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_decrypt, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_decrypt, 670, 686);
	lv_obj_set_size(ui->screen_decrypt, 202, 114);
	lv_obj_set_scrollbar_mode(ui->screen_decrypt, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_decrypt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_decrypt, 179, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_decrypt, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_decrypt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_decrypt, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_decrypt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_decrypt, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_decrypt, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_decrypt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_mod
	ui->screen_mod = lv_roller_create(ui->screen_cont_4);
	lv_roller_set_options(ui->screen_mod, "AES-256-ECB\nAES-256-CBC\nAES-256-CTR", LV_ROLLER_MODE_INFINITE);
	lv_roller_set_visible_row_count(ui->screen_mod, 3);
	lv_obj_set_pos(ui->screen_mod, 49, 686);
	lv_obj_set_width(ui->screen_mod, 319);
	lv_obj_set_scrollbar_mode(ui->screen_mod, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_mod, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_radius(ui->screen_mod, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_mod, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_mod, lv_color_hex(0x2FDAAE), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_mod, lv_color_hex(0x333333), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_mod, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_mod, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_mod, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_mod, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_mod, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_mod, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_mod, Part: LV_PART_SELECTED, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_mod, 255, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_mod, lv_color_hex(0x26B08C), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_mod, lv_color_hex(0xFFFFFF), LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_mod, &lv_font_montserratMedium_30, LV_PART_SELECTED|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_mod, LV_TEXT_ALIGN_CENTER, LV_PART_SELECTED|LV_STATE_DEFAULT);

	//Write codes screen_window
	ui->screen_window = lv_obj_create(ui->screen_cont_4);
	lv_obj_set_pos(ui->screen_window, 315, 41);
	lv_obj_set_size(ui->screen_window, 436, 237);
	lv_obj_set_scrollbar_mode(ui->screen_window, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_window, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_window, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_window, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_window, lv_color_hex(0xb7dad1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_window, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_OK_BTN
	ui->screen_OK_BTN = lv_btn_create(ui->screen_window);
	ui->screen_OK_BTN_label = lv_label_create(ui->screen_OK_BTN);
	lv_label_set_text(ui->screen_OK_BTN_label, "OK");
	lv_label_set_long_mode(ui->screen_OK_BTN_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_OK_BTN_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_OK_BTN, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_OK_BTN, 170, 162);
	lv_obj_set_size(ui->screen_OK_BTN, 100, 50);
	lv_obj_set_scrollbar_mode(ui->screen_OK_BTN, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_OK_BTN, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_OK_BTN, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_OK_BTN, lv_color_hex(0x26B08C), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_OK_BTN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_OK_BTN, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_OK_BTN, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_OK_BTN, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_OK_BTN, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_OK_BTN, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_content
	ui->screen_content = lv_textarea_create(ui->screen_window);
	lv_textarea_set_text(ui->screen_content, "");
	#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
		lv_obj_add_event_cb(ui->screen_content, ta_event_cb, LV_EVENT_ALL, ui->g_kb_screen);
	#endif
	lv_obj_set_pos(ui->screen_content, 63, 70);
	lv_obj_set_size(ui->screen_content, 329, 70);
	lv_obj_set_scrollbar_mode(ui->screen_content, LV_SCROLLBAR_MODE_OFF);

	lv_textarea_set_password_mode(ui->screen_content, true);

	//Write style for screen_content, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_content, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_content, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_content, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_content, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_content, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_content, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_content, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_content, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_content, lv_color_hex(0xe6e6e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_content, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_content, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_content, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_content, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_content, 4, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_content, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_content, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_content, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_content, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_remind
	ui->screen_remind = lv_label_create(ui->screen_window);
	lv_label_set_text(ui->screen_remind, "Set Password");
	lv_label_set_long_mode(ui->screen_remind, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_remind, 69, 28);
	lv_obj_set_size(ui->screen_remind, 314, 32);
	lv_obj_set_scrollbar_mode(ui->screen_remind, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_remind, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_remind, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_remind, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_remind, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_remind, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_remind, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_5
	ui->screen_cont_5 = lv_obj_create(ui->screen);
	lv_obj_set_pos(ui->screen_cont_5, 0, 0);
	lv_obj_set_size(ui->screen_cont_5, 1920, 150);
	lv_obj_set_scrollbar_mode(ui->screen_cont_5, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_5, 189, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_5, lv_color_hex(0x5e5e5e), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen_cont_5);
	lv_label_set_text(ui->screen_label_5, "EdgeLock Enclave");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_5, 61, 35);
	lv_obj_set_size(ui->screen_label_5, 581, 74);
	lv_obj_set_scrollbar_mode(ui->screen_label_5, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_5, &lv_font_montserratMedium_50, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);


	//Write codes screen_NXP_logo
	ui->screen_NXP_logo = lv_img_create(ui->screen_cont_5);
	lv_obj_add_flag(ui->screen_NXP_logo, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_NXP_logo, &_NXP_Logo_RGB_Colour_alpha_220x109);
	lv_img_set_pivot(ui->screen_NXP_logo, 50,50);
	lv_img_set_angle(ui->screen_NXP_logo, 0);
	lv_obj_set_pos(ui->screen_NXP_logo, 1659, 12);
	lv_obj_set_size(ui->screen_NXP_logo, 220, 109);
	lv_obj_set_scrollbar_mode(ui->screen_NXP_logo, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_NXP_logo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_opa(ui->screen_NXP_logo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

	
	//Init events for screen.
	events_init_screen(ui);
}
