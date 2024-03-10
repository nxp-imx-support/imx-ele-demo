/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "cryptoExample.h"
#include "demo_debug.h"

static hsm_err_t openSession(hsm_hdl_t *hsm_session_hdl){
    //open session
    open_session_args_t open_session_args = {0};
    hsm_err_t hsmret = HSM_GENERAL_ERROR;

    open_session_args.session_priority = 0;
    open_session_args.operating_mode = 0;
    hsmret = hsm_open_session(&open_session_args,
                                hsm_session_hdl);
    
    return hsmret;
}

#if 0
static int readData(char * filename, uint8_t ** ptr, uint32_t * len){
    int fd; // File descriptor
    // Open the file in reading mode
    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }

    *len = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    INFO("file size:%d\n",(*len));

    (*ptr)=(uint8_t *)malloc((*len) * sizeof(uint8_t));
    ssize_t bytesRead = read(fd, *ptr, (*len));
    if (bytesRead == -1) {
        perror("read");
        close(fd);
        return -1;
    } else if (bytesRead == 0) {
        INFO("File is empty\n");
        return -1;
    }
    close(fd);

    return 0;
}


static int writeData(char * filename, uint8_t * buffer, uint32_t len){
    int fd; // File descriptor
    // Open the file in writing mode
    if ((fd = open(filename, O_WRONLY | O_APPEND)) == -1) {
        perror("open");
        return -1;
    }
    
    // Write new data to the file
    ssize_t bytesWritten = write(fd, buffer, len);
    if (bytesWritten == -1) {
        perror("write");
        close(fd);
        return -1;
    }
    
    // Close the file
    close(fd);
    return 0;
}

static int clear_file(char * filename)
{
    int fd; // File descriptor
    // Open the file in writing mode
    if ((fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC,6666)) == -1) {
        perror("open");
        return -1;
    }
    close(fd);
    return 0;
}
#endif

int loadData(crypto_example_t * example, uint8_t * rawData, uint32_t len){
    if(!example || !rawData || len <= 0){
        ERROR("raw data invalid\r\n");
        return -EINVAL;
    }

    example->images.dataLen = len;
    /**
     * todo: improve the math
    */
    example->images.fakeLen = len + (0x10 - len % 0x10);

    example->images.rawImage = malloc(example->images.fakeLen);
    example->images.encImage = malloc(example->images.fakeLen);
    example->images.decImage = malloc(example->images.fakeLen);

    if(!example->images.rawImage || !example->images.encImage || !example->images.decImage){
        ERROR("failed to malloc memory\r\n");
        return -ENOMEM;
    }

    memset(example->images.rawImage, 0, example->images.fakeLen);
    memset(example->images.encImage, 0, example->images.fakeLen);
    memset(example->images.decImage, 0, example->images.fakeLen);

    memcpy(example->images.rawImage, rawData, len);

    return 0;
}

hsm_err_t exampleInit(crypto_example_t * example){
    hsm_err_t hsmret = HSM_GENERAL_ERROR;

    memset(example, 0, sizeof(crypto_example_t));
    example->encryptedImageValid = false;

    hsmret = dataParmInit(&example->passWD,SHA256);
    if(hsmret != HSM_NO_ERROR){
        return hsmret;
    }

    //open session
    hsmret = openSession(&example->hsm_session_hdl);
    if (hsmret != HSM_NO_ERROR) {
            ERROR("hsm_open_session failed err:0x%x\n", hsmret);
            return hsmret;
    } else {
        INFO("hsm_open_session success\n");
    }

    //open key store service
    open_svc_key_store_args_t open_svc_key_store_args = {0};

    open_svc_key_store_args.key_store_identifier = 0xABCD;
    open_svc_key_store_args.authentication_nonce = 0x1234;
    open_svc_key_store_args.flags           = 1 | (1<<7);   //create,sync

    hsmret = hsm_open_key_store_service(example->hsm_session_hdl, &open_svc_key_store_args, &example->key_store_hdl);
    if(hsmret==HSM_KEY_STORE_CONFLICT){
        INFO("create key store failed, try to load key store\n");

        hsmret=hsm_close_session(example->hsm_session_hdl);
        if(hsmret==HSM_NO_ERROR){
            INFO("hsm_close_session success\n");
        }else{
            ERROR("hsm_close_session failed, err=0x%X\n",hsmret);
        }
        
        hsmret = openSession(&example->hsm_session_hdl);
        if (hsmret != HSM_NO_ERROR) {
                ERROR("hsm_open_session failed err:0x%x\n", hsmret);
                return hsmret;
        } else {
            INFO("hsm_open_session success\n");
        }
        
        open_svc_key_store_args.flags           = 0;
        hsmret = hsm_open_key_store_service(example->hsm_session_hdl, &open_svc_key_store_args, &example->key_store_hdl);
    }

    if(hsmret!=HSM_NO_ERROR){
        ERROR("hsm_open_key_store failed, err=0x%X\n",hsmret);
        return hsmret;
    }else {
        INFO("hsm_open_key_store success\n");
    }

    //open key management service
    open_svc_key_management_args_t key_mgmt_args;

    memset(&key_mgmt_args, 0, sizeof(key_mgmt_args));
    
    hsmret = hsm_open_key_management_service(
        example->key_store_hdl, &key_mgmt_args, &example->key_mgmt_hdl);
    if(hsmret!=HSM_NO_ERROR){
        ERROR("hsm_open_key_management failed, err=0x%X\n",hsmret);
        return hsmret;
    } else {
        INFO("hsm_open_key_management success\n");
    }

    //generate key
    op_generate_key_args_t key_gen_args = {0};
    example->keyID = PERM_TEST_KEY_ID;

    memset(&key_gen_args, 0, sizeof(key_gen_args));
    
    key_gen_args.key_identifier = &example->keyID;
    key_gen_args.out_size = 0;
    key_gen_args.key_group = PERM_TEST_KEY_GROUP;
    key_gen_args.flags = HSM_OP_KEY_GENERATION_FLAGS_STRICT_OPERATION;
    key_gen_args.key_lifetime = HSM_SE_KEY_STORAGE_PERSISTENT;
    key_gen_args.key_usage = HSM_KEY_USAGE_ENCRYPT | HSM_KEY_USAGE_DECRYPT;
    key_gen_args.permitted_algo = PERMITTED_ALGO_ALL_CIPHER;
    key_gen_args.bit_key_sz = HSM_KEY_SIZE_AES_256;
    key_gen_args.key_lifecycle = 0;
    key_gen_args.key_type = HSM_KEY_TYPE_AES;
    key_gen_args.out_key = NULL;

    //Generate Persist Key
    hsmret = hsm_generate_key(example->key_mgmt_hdl, &key_gen_args);
    if (hsmret == HSM_ID_CONFLICT) {
        INFO("Key ID (0x%x) Already Exists.\n", PERM_TEST_KEY_ID);
        example->keyID = PERM_TEST_KEY_ID;
        //Because In case of failure, Key ID is set 0 at SAB layer.
    } else if(hsmret == HSM_NO_ERROR){
        INFO("hsm_generate_key success\n");
    }else{
        ERROR("hsm_generate_key failed, err=0x%X\n",hsmret);
        return hsmret;
    }

    return HSM_NO_ERROR;
}

static uint8_t iv_data[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

hsm_err_t encrypt(crypto_example_t * example, hsm_op_cipher_one_go_algo_t mode){
    op_cipher_one_go_args_t cipher_args = {0};
    hsm_err_t hsmret = HSM_GENERAL_ERROR;
    uint32_t i=0;

    example->encryptedImageValid = false;
    cipher_args.key_identifier = example->keyID;

    if(mode==HSM_CIPHER_ONE_GO_ALGO_CTR || mode==HSM_CIPHER_ONE_GO_ALGO_CBC){
        cipher_args.iv = iv_data;
        cipher_args.iv_size = sizeof(iv_data);
    }else if(mode==HSM_CIPHER_ONE_GO_ALGO_ECB){
        cipher_args.iv = 0;
        cipher_args.iv_size = 0;
    }
    cipher_args.cipher_algo = mode;
    cipher_args.flags = HSM_CIPHER_ONE_GO_FLAGS_ENCRYPT;

    uint32_t dataLeft = example->images.fakeLen;
    for(i=0; dataLeft>0; i++){
        cipher_args.input = example->images.rawImage + i*SYM_DATA_SIZE;
        cipher_args.input_size = ((dataLeft >= SYM_DATA_SIZE ) ? SYM_DATA_SIZE : dataLeft);
        dataLeft -= cipher_args.input_size;
        cipher_args.output = example->images.encImage + i*SYM_DATA_SIZE;
        cipher_args.output_size = cipher_args.input_size;
    
        hsmret = hsm_do_cipher(example->key_store_hdl, &cipher_args);
        if(hsmret != HSM_NO_ERROR){
            ERROR("round %d:encrypt failed, ret=0x%X\n",i,hsmret);
            example->encryptedImageValid = false;
            return hsmret;
        }
    }
    INFO("encrypt success\n");
    example->encryptedImageValid = true;

    INFO("enc data:[0]:0x%X [%d]:0x%X\r\n",example->images.encImage[0],\
        example->images.dataLen-1, example->images.encImage[example->images.dataLen-1]);

    return HSM_NO_ERROR;
}

bool imageMatch(const crypto_example_t * example){
    int i=0;
    if(!example){
        return false;
    }

    for(;i<example->images.dataLen;i++){
        if(example->images.rawImage[i]!=example->images.decImage[i]){
            ERROR("[%d]:0x%X 0x%X\r\n",i,example->images.rawImage[i],\
                example->images.decImage[i]);
            return false;
        }
    }

    return true;
}

hsm_err_t decrypt(crypto_example_t * example, hsm_op_cipher_one_go_algo_t mode){
    op_cipher_one_go_args_t cipher_args = {0};
    hsm_err_t hsmret = HSM_GENERAL_ERROR;
    int i=0;

    if(!example->encryptedImageValid){
        ERROR("the encrypted image is not valid\n");
        return HSM_GENERAL_ERROR;
    }

    cipher_args.key_identifier = example->keyID;
    if(mode==HSM_CIPHER_ONE_GO_ALGO_CTR || mode==HSM_CIPHER_ONE_GO_ALGO_CBC){
        cipher_args.iv = iv_data;
        cipher_args.iv_size = sizeof(iv_data);
    }else if(mode==HSM_CIPHER_ONE_GO_ALGO_ECB){
        cipher_args.iv = 0;
        cipher_args.iv_size = 0;
    }
    cipher_args.cipher_algo = mode;
    cipher_args.flags = HSM_CIPHER_ONE_GO_FLAGS_DECRYPT;

    uint32_t dataLeft = example->images.fakeLen;
    for(i=0; dataLeft>0; i++){
        cipher_args.input = example->images.encImage + i*SYM_DATA_SIZE;
        cipher_args.input_size = (dataLeft >= SYM_DATA_SIZE ) ? SYM_DATA_SIZE : dataLeft;
        dataLeft -= cipher_args.input_size;
        cipher_args.output = example->images.decImage + i*SYM_DATA_SIZE;
        cipher_args.output_size = cipher_args.input_size;

        hsmret = hsm_do_cipher(example->key_store_hdl, &cipher_args);
        if(hsmret != HSM_NO_ERROR){
            ERROR("decrypt failed, ret=0x%X\n",hsmret);
            return hsmret;
        }
    }
    INFO("decrypt success\n");

    return HSM_NO_ERROR;
}

void exampleQuit(crypto_example_t * example){
    hsm_err_t hsmret = HSM_GENERAL_ERROR;

    sync();

    //clean passWD buffer
    dataParmClean(&example->passWD);

    //free malloc spaces
    if(example->images.rawImage){
        free(example->images.rawImage);
    }
    if(example->images.decImage){
        free(example->images.decImage);
    }
    if(example->images.encImage){
        free(example->images.encImage);
    }

    if(example->key_mgmt_hdl){
        // if(example->validKey){
        //     //delete key
        //     op_delete_key_args_t del_args = {0};
        //     memset(&del_args, 0, sizeof(del_args));
        //     del_args.key_identifier = example->keyID;
        //     del_args.flags = 1<<7;
        //     hsmret = hsm_delete_key(example->key_mgmt_hdl, &del_args);
        //     if(hsmret==HSM_NO_ERROR){
        //         INFO("hsm_delete_key success\n");
        //     }else{
        //         ERROR("hsm_delete_key failed, err=0x%X\n",hsmret);
        //     }
        //     example->validKey = false;
        // }

        //close service
        hsmret = hsm_close_key_management_service(example->key_mgmt_hdl);
        if(hsmret==HSM_NO_ERROR){
            INFO("hsm_close_key_management_service success\n");
        }else{
            ERROR("hsm_close_key_management_service failed, err=0x%X\n",hsmret);
        }
    }
    
    if(example->key_store_hdl){
        hsmret = hsm_close_key_store_service(example->key_store_hdl);
        if(hsmret==HSM_NO_ERROR){
            INFO("hsm_close_key_store_service success\n");
        }else{
            ERROR("hsm_close_key_store_service failed, err=0x%X\n",hsmret);
        }
    }

    if(example->hsm_session_hdl){
        hsmret = hsm_close_session(example->hsm_session_hdl);
        if(hsmret==HSM_NO_ERROR){
            INFO("hsm_close_session success\n");
        }else{
            ERROR("hsm_close_session failed, err=0x%X\n",hsmret);
        }
    }
    memset(example,0,sizeof(crypto_example_t));
    INFO("example quit\n");
}
