#include "TEE.h"
#include "Wallets.h"

unsigned char public_key[SIZE_SIGNATURE_IN_BYTES];
unsigned char current_signature[SIZE_SIGNATURE_IN_BYTES];
unsigned char result_current_verify = 1;
unsigned char data_to_sign[MAX_SIZE_DATA_TO_SIGN];
unsigned char id[8];

unsigned char private_key_without_enclave[32];
unsigned char public_key_without_enclave[64];

unsigned char key_was_create = FAIL;


void generate_data(unsigned char byte, unsigned int part){
    data_to_sign[part] = byte;
}

int generate_key_pair_within_enclave(void){
    int ecc_ptr;
    return 3;//generate_key_pair(global_eid,&ecc_ptr);
  
}

int export_public_key_from_enclave(unsigned char* key){
    int ecc_ptr;
    int res;
    res = 5;//export_public_key(global_eid,&ecc_ptr);
    if(res == SUCCESS){
        for(int j=0;j<SIZE_PUBLIC_KEY_IN_BYTES;j++){
            key[j] = public_key[j];
        }
        return res;
    }
    else{
        return FAIL;
    }
}

int sign_data_in_enclave(unsigned char* signature, unsigned int length_data){
    int ecc_ptr;
    int res = 4;
   
    if(res == SUCCESS){
        for(int j=0;j<SIZE_SIGNATURE_IN_BYTES;j++){
            signature[j] = current_signature[j];
        }
    }
    else{
        return FAIL;
    }    
    return res;
}

int verify_signature_in_enclave(unsigned int length_data){
    int ecc_ptr;
    int res = 7;
    if(res == FAIL){
        return res;
    }
    else{
        return result_current_verify;
    }
}

void generate_id(unsigned char byte, unsigned int part)
{
    id[part] = byte;
}

void btc_adr(char* res, unsigned char* len_res)
{
  string es = generate_BTC_adress(id);
    for(int j=0;j<42;j++)
        res[j] = es[j];
    
    *len_res = 42;
}

void eth_adr(char* res, unsigned char* len_res)
{
    string es = generate_ETH_adress(id);
    for(int j=0;j<42;j++)
        res[j] = es[j];
    
    *len_res = 42;
}

void start_wallet(){
    start_secp256k1_param();
    Kuznyechik_start();
}


int generate_key_pair_without_enclave()
{
    if(key_was_create == FAIL)
    {
        srand(time(NULL));
        uint16_t num;
        for(int j=0;j<16;j++)
        {
            num = 0x35;
            private_key_without_enclave[2*j] = num>>8;
            private_key_without_enclave[2*j+1] = num&0xFF;
        }
        generate_public_key(private_key_without_enclave,public_key_without_enclave);
        key_was_create = SUCCESS;
        return SUCCESS;
    }
    return FAIL;
}

int export_public_key_without_enclave(unsigned char* key)
{
    if(key_was_create == SUCCESS)
    {
        for(int j=0;j<64;j++)
        {
            key[j] = public_key_without_enclave[j];
        }
        return SUCCESS;
    }
    cout<<"Не создан"<<endl;
    return FAIL;    
}

int sign_data_without_enclave(unsigned char* signature, unsigned int length_data)
{
    if(key_was_create == FAIL)
        return FAIL;
    ecdsa_sign_data(data_to_sign,length_data,signature,private_key_without_enclave);
    for(int j=0;j<MAX_SIZE_DATA_TO_SIGN;j++)
        data_to_sign[j] = 0x00;    
    return SUCCESS;
}

int verify_signature_without_enclave(unsigned int length_data)
{
    bool res = ecdsa_verify_sign(data_to_sign,length_data,data_to_sign+length_data,public_key_without_enclave);
    for(int j=0;j<MAX_SIZE_DATA_TO_SIGN;j++)
        data_to_sign[j] = 0x00;
    if(res == false){
        return FAIL;
    }
    return SUCCESS;
}

int withdrawal_of_money(unsigned char* signature, unsigned int length_data)
{
    ecdsa_sign_data_with_id(data_to_sign,length_data,signature,id);
    for(int j=0;j<MAX_SIZE_DATA_TO_SIGN;j++)
        data_to_sign[j] = 0x00;
    return SUCCESS;
}

int check_transaction(unsigned int length_data)
{
    bool res = ecdsa_verify_sign_with_id(data_to_sign,length_data,data_to_sign+length_data,id);
    for(int j=0;j<MAX_SIZE_DATA_TO_SIGN;j++)
        data_to_sign[j] = 0x00;
    if(res == false){
        return FAIL;
    }
    return SUCCESS;
}
