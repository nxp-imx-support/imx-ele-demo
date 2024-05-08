/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#ifndef PASSWD_H
#define PASSWD_H

#include <hsm_api.h>
#include <stdbool.h>

typedef struct{
    uint32_t dataID;
    uint32_t dataSize;
    uint8_t *data;
}dataStorage_t;

#define PASSWD_DATAID       0x980923
#define SHA256              "sha256"
#define SHA512              "sha512"

hsm_err_t dataParmInit(dataStorage_t * ELEdata, char * algo);
hsm_err_t storePassWDHash(hsm_hdl_t key_store_hdl, dataStorage_t * ELEdata, char * passWD, uint32_t len);
bool verifyPassWD(hsm_hdl_t key_store_hdl, dataStorage_t * ELEdata, char * passWD, uint32_t len);
void dataParmClean(dataStorage_t * ELEdata);

#endif