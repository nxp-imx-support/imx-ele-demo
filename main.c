/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "lvgl/lvgl.h"
#include "lvgl.h"
#include "lv_drivers/wayland/wayland.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lv_drivers/indev/evdev.h"
#include "cryptoExample.h"
#include <signal.h>
#include <termios.h>

#include "demo_debug.h"

#define DISP_BUF_SIZE (1280 * 720)

#define WAYLAND_HOR_RES      1280
#define WAYLAND_VER_RES      720
#define WAYLAND_SURF_TITLE   "LVGL"

lv_ui guider_ui;
char img_path[100];
crypto_example_t example;

void killApp(int ip)
{
    exampleQuit(&example);
    exit(0);
}

int addReaction(void){
    int ret=0;
    struct sigaction action = {0,};

    action.sa_handler = killApp;

    /* handle kill signal */
    ret = sigaction(SIGTERM, &action, NULL);
    if (ret)
        ERROR("failed to register kill signal handler\n");

    /* handle ctrl-c */
    ret = sigaction(SIGINT, &action, NULL);
    if (ret)
        ERROR("failed to register ctrl-c signal handler\n");

    return ret;
}

bool close_cb(lv_disp_t * disp)
{
    INFO("Inside cb\n");
    return 1;
}

extern const lv_img_dsc_t _penguin_alpha_277x412;
lv_img_dsc_t g_originalImage;
lv_img_dsc_t g_encryptedImage;
lv_img_dsc_t g_decryptedImage;

static void imageInit(const lv_img_dsc_t * originalImage){
    memcpy(&g_originalImage,originalImage,sizeof(lv_img_dsc_t));
    memcpy(&g_encryptedImage,originalImage,sizeof(lv_img_dsc_t));
    memcpy(&g_decryptedImage,originalImage,sizeof(lv_img_dsc_t));
}

int main(int argc, char *argv[])
{
    hsm_err_t hsmret = HSM_GENERAL_ERROR;

    INFO("crypto demo version:%s\n", GITVERSION);
    hsmret = exampleInit(&example);
    if(hsmret != HSM_NO_ERROR){
        ERROR("example init failed\n");
        exampleQuit(&example);
        return -1;
    }
    INFO("example init succeed\n");

    imageInit(&_penguin_alpha_184x274);
    INFO("%d\r\n",g_originalImage.data_size);
    if(loadData(&example,(uint8_t *)g_originalImage.data, g_originalImage.data_size)){
        ERROR("load data failed\n");
        exampleQuit(&example);
        return -1;
    }
    INFO("load success\r\n");

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    //fbdev_init();
    //drm_init();
    lv_wayland_init();

    lv_disp_t * disp = lv_wayland_create_window(WAYLAND_HOR_RES,WAYLAND_VER_RES,WAYLAND_SURF_TITLE,close_cb);

    static lv_color_t buf[DISP_BUF_SIZE];

    addReaction();
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    while(1) {
   
            lv_task_handler();
            //lv_tick_inc(5*1000);
            usleep(5000);

    }

    return 0;
}


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
