/*
* Copyright 2023 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>

#include "lvgl.h"
#include"custom.h"
#include "cryptoExample.h"
#include "read_rng.h"
#include "passWD.h"
#include "demo_debug.h"

lv_obj_t* point_group[100];

int wrong_times = 0;
int select_num = 0;
bool encrypt_flag = 0;
bool decrypt_flag = 0;
char remind_wrong_times[100];

extern crypto_example_t example;
extern lv_img_dsc_t g_originalImage;
extern lv_img_dsc_t g_encryptedImage;
extern lv_img_dsc_t g_decryptedImage;

void encrypt_img(){
    hsm_err_t hsmret = HSM_GENERAL_ERROR;
    select_num = lv_roller_get_selected(guider_ui.screen_mod);
    if (select_num == 0){
        hsmret = encrypt(&example, HSM_CIPHER_ONE_GO_ALGO_ECB);
    }else if(select_num == 1){
        hsmret = encrypt(&example, HSM_CIPHER_ONE_GO_ALGO_CBC);
    }else{
        hsmret = encrypt(&example, HSM_CIPHER_ONE_GO_ALGO_CTR);
    }

    if(hsmret != HSM_NO_ERROR){
        ERROR("encrypt failed\n");
        return;
    }

    g_encryptedImage.data=example.images.encImage;

}

void decrypt_img(){
    hsm_err_t hsmret = HSM_GENERAL_ERROR;
    select_num = lv_roller_get_selected(guider_ui.screen_mod);
    if (select_num == 0){
        hsmret = decrypt(&example, HSM_CIPHER_ONE_GO_ALGO_ECB);
    }else if(select_num == 1){
        hsmret = decrypt(&example, HSM_CIPHER_ONE_GO_ALGO_CBC);
    }else{
        hsmret = decrypt(&example, HSM_CIPHER_ONE_GO_ALGO_CTR);
    }
    if(hsmret != HSM_NO_ERROR){
        ERROR("decrypt failed\n");
        return;
    }
    
    g_decryptedImage.data=example.images.decImage;
}

static void screen_generate_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
        hsm_err_t hsmret = HSM_GENERAL_ERROR;
        uint8_t random[200] = {0,};
        
        hsmret = readRandom(200,random);

        if(hsmret != HSM_NO_ERROR){
            ERROR("read random failed\n");
         }
        INFO("random%02X\n",random[0]);
        INFO("random%02X\n",random[20]);
        char random_temp[200];

        for (int i, j = 0; i < 20; i++)
        {
                j += sprintf(random_temp + j, "%02X", random[i]);
        }

        INFO("random_temp:%s\n",random_temp); 
        lv_label_set_text(guider_ui.screen_nums,random_temp);

        for(int i = 0; i< 100; i++){
            point_group[i] = lv_img_create(guider_ui.screen_cont_2);
            lv_obj_add_flag(point_group[i], LV_OBJ_FLAG_CLICKABLE);
            lv_img_set_src(point_group[i], &_point_alpha_2x2);
            lv_img_set_pivot(point_group[i], 50,50);
            lv_img_set_angle(point_group[i], 0);
            lv_obj_set_pos(point_group[i], 2*random[i*2], 2*random[i*2+1]);
            lv_obj_set_size(point_group[i], 5, 5);
            lv_obj_set_scrollbar_mode(point_group[i], LV_SCROLLBAR_MODE_OFF);

            //Write style for screen_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
            lv_obj_set_style_img_opa(point_group[i], 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        }
		break;
	}
	default:
		break;
	}
}

static void screen_clear_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clean(guider_ui.screen_cont_2);
        lv_label_set_text(guider_ui.screen_nums, "");
        break;
    }
    default:
        break;
    }
}

static void screen_exit_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
        exampleQuit(&example);
        exit(0);
		break;
	}
	default:
		break;
	}
}

static void screen_encrypt_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
        wrong_times = 0;
		lv_obj_clear_flag(guider_ui.screen_window, LV_OBJ_FLAG_HIDDEN);
        encrypt_flag = 1;
        decrypt_flag = 0;
        lv_label_set_text(guider_ui.screen_remind, "Set password");
        lv_obj_set_style_img_opa(guider_ui.screen_img_enc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_img_opa(guider_ui.screen_img_dec, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        //int ret2 = remove(new_enc_img_path);
		
		break;
	}
	default:
		break;
	}
}
static void screen_decrypt_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_window, LV_OBJ_FLAG_HIDDEN);
        decrypt_flag = 1;
        encrypt_flag = 0;
        lv_label_set_text(guider_ui.screen_remind, "Verify password"); 
		break;
	}
	default:
		break;
	}
}
static void screen_OK_BTN_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
    hsm_err_t hsmret = HSM_GENERAL_ERROR;

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		char *passWD = (char *)lv_textarea_get_text(guider_ui.screen_content);
        //lv_textarea_set_text(guider_ui.screen_content,"");
        INFO("passWD:%s\n",passWD );
        if(strlen(passWD) == 0){
            lv_label_set_text(guider_ui.screen_remind, "Invalid Password,Please try again");
            lv_textarea_set_text(guider_ui.screen_content,"");
            return;
        }

        if(encrypt_flag){

            hsmret = storePassWDHash(example.key_store_hdl,&example.passWD,passWD,strlen(passWD));
            if(hsmret != HSM_NO_ERROR){
                ERROR("store passwd hash failed\n");
                lv_textarea_set_text(guider_ui.screen_content,"");
                return;
            }else{
                INFO("store passwd hash success\n");
            }

            encrypt_img();
            lv_img_set_src(guider_ui.screen_img_enc,&g_encryptedImage);
            lv_obj_set_style_img_opa(guider_ui.screen_img_enc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_add_flag(guider_ui.screen_window, LV_OBJ_FLAG_HIDDEN);

        }

        if(decrypt_flag){
                      
            if (verifyPassWD(example.key_store_hdl,&example.passWD,passWD,strlen(passWD)))
            {
                decrypt_img();
                wrong_times = 0;
                
                lv_obj_add_flag(guider_ui.screen_window, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(guider_ui.screen_img_dec,&g_decryptedImage);
                lv_obj_set_style_img_opa(guider_ui.screen_img_dec, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

            }else{
                wrong_times++;
                if (wrong_times>=3){
                    lv_obj_add_flag(guider_ui.screen_window, LV_OBJ_FLAG_HIDDEN);
                    wrong_times = 0;
                }else{
                    sprintf(remind_wrong_times,"Password wrong, %d times left",3-wrong_times);
                    lv_label_set_text(guider_ui.screen_remind, remind_wrong_times);
                }
            }

           
        }
        lv_textarea_set_text(guider_ui.screen_content,"");
		break;
	}
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_generate, screen_generate_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_exit, screen_exit_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_clear, screen_clear_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_encrypt, screen_encrypt_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_decrypt, screen_decrypt_event_handler, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(ui->screen_OK_BTN, screen_OK_BTN_event_handler, LV_EVENT_ALL, NULL);
}

void events_init(lv_ui *ui)
{

}
