/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#include <stdio.h>
#include "read_rng.h"
#include "demo_debug.h"

hsm_err_t readRandom(int len, uint8_t *ptr)
{
    hsm_hdl_t hsm_session_hdl;
    open_session_args_t open_session_args = {0};
    hsm_err_t err;
    op_get_random_args_t rng_get_random_args={0};

    open_session_args.mu_type = HSM1;
    err = hsm_open_session(&open_session_args,
                                &hsm_session_hdl);
    if (err != HSM_NO_ERROR) {
            ERROR("hsm_open_session failed err:0x%x\n", err);
            goto exit;
    } else {
        INFO("hsm_open_session success\n");
    }
    
    rng_get_random_args.output = ptr;

    rng_get_random_args.random_size = len;
    err = hsm_do_rng(hsm_session_hdl, &rng_get_random_args);
    if (err != HSM_NO_ERROR) {
        ERROR("Failed while generating random, err=0x%X\n",err);
        goto rng_fail;
    } else{
        INFO("hsm get random success\n");
    }

rng_fail:
    err = hsm_close_session(hsm_session_hdl);
exit:
    return err; 
}
