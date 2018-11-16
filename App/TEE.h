#ifdef __cplusplus  
#include <stdio.h>
#include <iostream>

#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

extern "C" {  

#endif  

const unsigned short SIZE_PUBLIC_KEY_IN_BYTES = 64;
const unsigned short SIZE_SIGNATURE_IN_BYTES = 64;
const unsigned short SIZE_SIGNATURE_IN_WORDS = 16;
const unsigned long MAX_SIZE_DATA_TO_SIGN = 1024;

const unsigned char SUCCESS = 0x00;
const unsigned char FAIL = 0x01;


void generate_data(unsigned char byte, unsigned int part);

void generate_id(unsigned char byte, unsigned int part);

void start_wallet();

void btc_adr(char* res, unsigned char* len_res);
void eth_adr(char* res, unsigned char* len_res);

int withdrawal_of_money(unsigned char* signature, unsigned int length_data);
int check_transaction(unsigned int length_data);

int generate_key_pair_without_enclave();
int export_public_key_without_enclave(unsigned char* key);
int sign_data_without_enclave(unsigned char* signature, unsigned int length_data);
int verify_signature_without_enclave(unsigned int length_data);


#ifdef __cplusplus  
}  
#endif 

