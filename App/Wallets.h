  
#include "bigd.h"
#include "Keccak.h"
#include "Kuznyechik.h"
#include "SHA256.h"
#include "RIPEMD160.h"
#include <string>

using namespace std;

typedef struct {
    BIGD X = bdNew();
    BIGD Y = bdNew();
} secp256k1_point;


const unsigned char TYPE_HASH_ECDSA_SIMPLE = 0x00;
const unsigned char TYPE_HASH_ECDSA_BITCOIN = 0x01;
const unsigned char TYPE_HASH_ECDSA_ETHEREUM = 0x02;

string encoding_to_BASE58(unsigned char* data);

string BTC_adress_from_public_key(unsigned char* public_key);
string ETH_adress_from_public_key(unsigned char* public_key);

void generate_user_private_key(unsigned char* id, unsigned char* private_key);
bool check_key_in_field(unsigned char* private_key);

void calc_angle_factor_summ(secp256k1_point p, secp256k1_point q, BIGD result);
void summ_point(secp256k1_point p, secp256k1_point q, secp256k1_point result);
void calc_angle_factor_double_point(secp256k1_point p, BIGD result);
void double_point(secp256k1_point p, secp256k1_point result);
void start_secp256k1_param();

void sha256_calc_double_hash(unsigned char* data, uint32_t len_data, uint32_t* hash);

void calc_summand_coordinates(unsigned char* private_key);


void generate_public_key(unsigned char* private_key, unsigned char* result);
void compress_public_key(unsigned char* public_key, unsigned char* public_key_compress);

void export_compress_public_key_on_id(unsigned char* id, unsigned char* compress_public_key);
     
string generate_BTC_adress(unsigned char* id);
string generate_ETH_adress(unsigned char* id);

void from_uchar_array_to_BIGD(unsigned char* prototype,uint32_t len_prototype, BIGD type);
void from_BIGD_to_uchar_array(BIGD prototype, unsigned char* type, uint32_t len_type);
void generator_to_scalar_multi(unsigned char* key, secp256k1_point result);
void point_by_scalar_multi(secp256k1_point P, unsigned char* Scalar, secp256k1_point result);

void ecdsa_sign_data_with_id(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* id, unsigned char type_hash);
bool ecdsa_verify_sign_with_id(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* id, unsigned char type_hash);

void ecdsa_sign_data(unsigned char* data,uint32_t len_data, unsigned char* signature, unsigned char* private_key, unsigned char type_hash);
bool ecdsa_verify_sign(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* public_key, unsigned char type_hash);
