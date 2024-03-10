/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#include "passWD.h"
#include <openssl/evp.h>
#include "demo_debug.h"

static int getSHA(char * data, uint32_t dataLen, uint8_t * hash, uint32_t hashLen){
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = NULL;
    int ret = 0;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    uint32_t tmpLen = hashLen;

    if(tmpLen * 8 == 256){
        md = EVP_get_digestbyname(SHA256);
    }else if(tmpLen * 8 == 512){
        md = EVP_get_digestbyname(SHA512);
    }else{
        ERROR("digest length not supported yet %d\n",tmpLen * 8);
    }

    if (md == NULL) {
        ERROR("Unknown message digest length %d\n",tmpLen * 8);
        return -1;
    }

    mdctx = EVP_MD_CTX_new();
    if (!EVP_DigestInit_ex2(mdctx, md, NULL)) {
        ERROR("Message digest initialization failed.\n");
        ret = -1;
        goto exit;
    }

    if (!EVP_DigestUpdate(mdctx, data, dataLen)) {
        ERROR("Message digest update failed.\n");
        ret = -1;
        goto exit;
    }

    if (!EVP_DigestFinal_ex(mdctx, md_value, &tmpLen)) {
        ERROR("Message digest finalization failed.\n");
        ret = -1;
        goto exit;
    }

    if(tmpLen!=hashLen){
        ERROR("sha length [%d]bits not expected\n",tmpLen*8);
        ret = -1;
        goto exit;
    }else{
        memcpy(hash,md_value,tmpLen);
    }
    ret = 0;

exit:
    EVP_MD_CTX_free(mdctx);
    return ret;
}

static void printDigst(const dataStorage_t * ELEdata){
    int i=0;
    INFO("digest size:%dbits\n",ELEdata->dataSize*8);
    for(i=0;i<ELEdata->dataSize;i++){
        INFO("%02X",ELEdata->data[i]);
    }
    INFO("\n");
}

hsm_err_t dataParmInit(dataStorage_t * ELEdata, char * algo){
    memset(ELEdata, 0, sizeof(dataStorage_t));
    ELEdata->dataID=PASSWD_DATAID;
    if(strcmp(algo,SHA256)==0){
        ELEdata->dataSize=256/8;
    }else if(strcmp(algo,SHA512)==0){
        ELEdata->dataSize=512/8;
    }else{
        ERROR("digest [%s] not supported yet\n",algo);
        return HSM_GENERAL_ERROR;
    }

    ELEdata->data = malloc(ELEdata->dataSize);
    if(!ELEdata->data){
        ERROR("arry malloc error\n");
        return HSM_GENERAL_ERROR;
    }
    memset(ELEdata->data,0,ELEdata->dataSize);
    return HSM_NO_ERROR;
}

hsm_err_t storePassWDHash(hsm_hdl_t key_store_hdl, dataStorage_t * ELEdata, char * passWD, uint32_t len){
    int tmp = 0;
    hsm_err_t err = HSM_GENERAL_ERROR;
    op_data_storage_args_t data_storage_args;

    if(!ELEdata->data || ELEdata->dataSize<=0 || ELEdata->dataID<=0){
        ERROR("data parameters not init\n");
        return HSM_GENERAL_ERROR;
    }
    if(!passWD || len<=0){
        ERROR("input data invalid\n");
        return HSM_GENERAL_ERROR;
    }

    memset(&data_storage_args, 0, sizeof(op_data_storage_args_t));

    tmp = getSHA(passWD, len, ELEdata->data, ELEdata->dataSize);
    if(tmp){
        ERROR("calculate input digest failed\n");
        return HSM_GENERAL_ERROR;
    }else{
        INFO("calculate input digest success\n");
    }

    printDigst(ELEdata);

    data_storage_args.svc_flags = 0;
	data_storage_args.data = ELEdata->data;
	data_storage_args.data_size = ELEdata->dataSize;
	data_storage_args.data_id = ELEdata->dataID;
	data_storage_args.flags |= HSM_OP_DATA_STORAGE_FLAGS_STORE;
	err = hsm_data_ops(key_store_hdl, &data_storage_args);
	if (err) {
		ERROR("store data to ele error: 0x%X\n", err);
		return err;
	}else{
        INFO("store data to ele success\n");
    }

    //after data stored, clean data in RAM
    memset(ELEdata->data,0,ELEdata->dataSize);

    return HSM_NO_ERROR;
}

static hsm_err_t readData(hsm_hdl_t key_store_hdl, dataStorage_t * ELEdata){
    hsm_err_t err = HSM_GENERAL_ERROR;
    op_data_storage_args_t data_storage_args;

    if(!ELEdata->data || ELEdata->dataSize<=0 || ELEdata->dataID<=0){
        ERROR("data parameters not init\n");
        return HSM_GENERAL_ERROR;
    }
    
    memset(&data_storage_args, 0, sizeof(op_data_storage_args_t));
	data_storage_args.svc_flags = 0;
	data_storage_args.data = ELEdata->data;
	data_storage_args.data_size = ELEdata->dataSize;
	data_storage_args.data_id = ELEdata->dataID;
	data_storage_args.flags |= HSM_OP_DATA_STORAGE_FLAGS_RETRIEVE;
	err = hsm_data_ops(key_store_hdl, &data_storage_args);
	if (err) {
		ERROR("read data from ele error: 0x%X\n", err);
		return err;
	}else{
        INFO("read data from ele success\n");
    }
    return HSM_NO_ERROR;
}

static bool compareArry(uint8_t* arry1, uint8_t* arry2, uint32_t len){
    int i=0;
    for(i=0;i<len;i++){
        if(arry1[i]!=arry2[i]){
            return false;
        }
    }
    return true;
}

bool verifyPassWD(hsm_hdl_t key_store_hdl, dataStorage_t * ELEdata, char * passWD, uint32_t len){
    hsm_err_t err = HSM_GENERAL_ERROR;
    uint8_t * tmpHashBuffer=NULL;
    bool ret=false;

    if(!ELEdata->data || ELEdata->dataSize<=0 || ELEdata->dataID<=0){
        ERROR("data parameters not init\n");
        return false;
    }
    if(!passWD || len<=0){
        ERROR("input data invalid\n");
        return false;
    }

    tmpHashBuffer = malloc(sizeof(uint8_t)*ELEdata->dataSize);
    if(getSHA(passWD,len,tmpHashBuffer,ELEdata->dataSize)){
        ERROR("get hash failed\n");
    }else{
        INFO("get hash success\n");
    }

    err = readData(key_store_hdl,ELEdata);
    if(err!=HSM_NO_ERROR){
        ERROR("read passwd hash failed\n");
    }else{
        INFO("read passwd hash success\n");
    }

    ret = compareArry(tmpHashBuffer,ELEdata->data,ELEdata->dataSize);

    memset(tmpHashBuffer,0,ELEdata->dataSize);
    free(tmpHashBuffer);
    tmpHashBuffer = NULL;
    return ret;
}

void dataParmClean(dataStorage_t * ELEdata){
    if(ELEdata->data){
        memset(ELEdata->data,0,ELEdata->dataSize);
        free(ELEdata->data);
        ELEdata->data=NULL;
    }
}