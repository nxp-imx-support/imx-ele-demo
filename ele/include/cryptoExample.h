/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#ifndef CRYPTOEXAMPLE_H
#define CRYPTOEXAMPLE_H

#include <hsm_api.h>
#include "passWD.h"

//[ 0x00000001 ; 0x3FFFFFFF ]: persistent and permanent keys
#define PERM_TEST_KEY_ID        0x31110011u
#define PERM_TEST_KEY_GROUP     7
#define SYM_DATA_SIZE           0x2500u  //not work:0x...1/2/4/8/a

#define AES_256_ECB             "AES-256-ECB"
#define AES_256_CBC             "AES-256-CBC"
#define AES_256_CTR             "AES-256-CTR"

typedef struct{
    uint8_t * rawImage;
    uint8_t * encImage;
    uint8_t * decImage;
    uint32_t dataLen;
    uint32_t fakeLen;   //to satisfy times of 0x10
}Image_t;

typedef struct{
    hsm_hdl_t hsm_session_hdl;
    hsm_hdl_t key_store_hdl;
    hsm_hdl_t key_mgmt_hdl;
    uint32_t keyID;
    Image_t images;
    dataStorage_t passWD;
    bool encryptedImageValid;
}crypto_example_t;

hsm_err_t exampleInit(crypto_example_t * example);
int loadData(crypto_example_t * example, uint8_t * rawData, uint32_t len);
hsm_err_t encrypt(crypto_example_t * example, hsm_op_cipher_one_go_algo_t mode);
hsm_err_t decrypt(crypto_example_t * example, hsm_op_cipher_one_go_algo_t mode);
bool imageMatch(const crypto_example_t * example);
void exampleQuit(crypto_example_t * example);

#endif