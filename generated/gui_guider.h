/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *g_kb_screen;
	lv_obj_t *screen_cont_1;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_cont_3;
	lv_obj_t *screen_notes;
	lv_obj_t *screen_generate;
	lv_obj_t *screen_generate_label;
	lv_obj_t *screen_cont_2;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_clear;
	lv_obj_t *screen_clear_label;
	lv_obj_t *screen_nums;
	lv_obj_t *screen_cont_4;
	lv_obj_t *screen_exit;
	lv_obj_t *screen_exit_label;
	lv_obj_t *screen_img_dec;
	lv_obj_t *screen_img_ori;
	lv_obj_t *screen_img_enc;
	lv_obj_t *screen_encrypt;
	lv_obj_t *screen_encrypt_label;
	lv_obj_t *screen_decrypt;
	lv_obj_t *screen_decrypt_label;
	lv_obj_t *screen_mod;
	lv_obj_t *screen_window;
	lv_obj_t *screen_OK_BTN;
	lv_obj_t *screen_OK_BTN_label;
	lv_obj_t *screen_content;
	lv_obj_t *screen_remind;
	lv_obj_t *screen_cont_5;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_NXP_logo;
}lv_ui;

void ui_init_style(lv_style_t * style);
void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;

void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_whitebg1_alpha_1280x720);
LV_IMG_DECLARE(_point_alpha_1x1);
// LV_IMG_DECLARE(_point_alpha_2x2);
LV_IMG_DECLARE(_penguin_alpha_184x274);
LV_IMG_DECLARE(_NXP_Logo_RGB_Colour_alpha_146x72);

// LV_FONT_DECLARE(lv_font_montserratMedium_25)
// LV_FONT_DECLARE(lv_font_montserratMedium_16)
// LV_FONT_DECLARE(lv_font_montserratMedium_30)
// LV_FONT_DECLARE(lv_font_montserratMedium_12)
// LV_FONT_DECLARE(lv_font_montserratMedium_20)
// LV_FONT_DECLARE(lv_font_montserratMedium_50)
// LV_FONT_DECLARE(lv_font_montserratMedium_18)


LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_10)
LV_FONT_DECLARE(lv_font_montserratMedium_20)
LV_FONT_DECLARE(lv_font_montserratMedium_13)
LV_FONT_DECLARE(lv_font_montserratMedium_33)


#ifdef __cplusplus
}
#endif
#endif
