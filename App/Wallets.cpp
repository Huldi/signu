#include "Wallets.h"
#include <iostream>

unsigned char data_from_go[8];// id

secp256k1_point Generator;

BIGD secp256k1_p;
BIGD secp256k1_n;

unsigned char master_key[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

unsigned char gpsch_key[32] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,0};
unsigned char gpsch_iv[16] = {9,6,3,8,5,2,7,4,1,0,7,8,9,6,5};

unsigned char iv_external_re_keying[32];

unsigned char init_vector[32];
unsigned char out_iv[32] = {'s','e','c','r','e','t','t','o','p','r','a','s','s','t','r','e','l','n','a','m','e','s','t','e','b','a','r','m','a','l','e','i',};

unsigned char summand_coordinates[256];

void start_secp256k1_param()
{
	bdConvFromDecimal(Generator.X, "55066263022277343669578718895168534326250603453777594175500187360389116729240");
	bdConvFromDecimal(Generator.Y, "32670510020758816978083085130507043184471273380659243275938904335757337482424");
	secp256k1_p = bdNew();
	bdConvFromDecimal(secp256k1_p, "115792089237316195423570985008687907853269984665640564039457584007908834671663");
	secp256k1_n = bdNew();
	bdConvFromHex(secp256k1_n, "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
}

void calc_angle_factor_summ(secp256k1_point p, secp256k1_point q, BIGD result)
{
	BIGD tmp_nom, tmp_denom, tmp;
	
	tmp_nom = bdNew();
	tmp_denom = bdNew();
	tmp = bdNew();
	
	bdModSub(tmp_nom, q.Y, p.Y, secp256k1_p);
	bdModSub(tmp_denom, q.X, p.X, secp256k1_p);
	bdModInv(tmp, tmp_denom, secp256k1_p);
	bdModMult(result, tmp_nom, tmp, secp256k1_p);

	bdFree(&tmp_nom);
	bdFree(&tmp_denom);
	bdFree(&tmp);
}

void calc_angle_factor_double_point(secp256k1_point p, BIGD result)
{
	BIGD const_3, const_2;

	const_3 = bdNew();
	const_2 = bdNew();
	bdSetShort(const_3, 3);
	bdSetShort(const_2, 2);

	BIGD sqr, tmp3, tmp2, denom;

	sqr = bdNew();
	tmp3 = bdNew();
	tmp2 = bdNew();
	denom = bdNew();

	bdModSquare(sqr, p.X, secp256k1_p);

	bdModMult(tmp3, const_3, sqr, secp256k1_p);

	bdModMult(tmp2, const_2, p.Y, secp256k1_p);

	bdModInv(denom, tmp2, secp256k1_p);

	bdModMult(result, tmp3, denom, secp256k1_p);

	bdFree(&sqr);
	bdFree(&tmp3);
	bdFree(&tmp2);
	bdFree(&denom);
}

void summ_point(secp256k1_point p, secp256k1_point q, secp256k1_point result)
{
	BIGD value_0;
	value_0 = bdNew();
	bdSetShort(value_0, 0);
		
	if ((bdCompare(p.X, value_0) == 0) && (bdCompare(p.Y, value_0) == 0))
	{
		bdSetEqual(result.X, q.X);
		bdSetEqual(result.Y, q.Y);
		bdFree(&value_0);
	}
	else if ((bdCompare(q.X, value_0) == 0) && (bdCompare(q.Y, value_0) == 0))
	{
		bdSetEqual(result.X, p.X);
		bdSetEqual(result.Y, p.Y);
		bdFree(&value_0);
	}
	else
	{
		BIGD angle_factor, sqr, sub;
		
		angle_factor = bdNew();
		sqr = bdNew();
		sub = bdNew();

		calc_angle_factor_summ(p, q, angle_factor);

		bdModSquare(sqr, angle_factor, secp256k1_p);
		bdModSub(sub, sqr, p.X, secp256k1_p);
		bdModSub(result.X, sub, q.X, secp256k1_p);

		bdFree(&sqr);
		bdFree(&sub);

		BIGD diff, mul;
		diff = bdNew();
		mul = bdNew();

		bdModSub(diff, p.X, result.X, secp256k1_p);
		bdModMult(mul, angle_factor, diff, secp256k1_p);
		bdModSub(result.Y, mul, p.Y, secp256k1_p);

		bdFree(&diff);
		bdFree(&mul);
		bdFree(&angle_factor);
	}	
}

void double_point(secp256k1_point p, secp256k1_point result)
{
	BIGD angle_factor;

	angle_factor = bdNew();
	calc_angle_factor_double_point(p, angle_factor);

	BIGD sqr, mul, value_2;
	sqr = bdNew();
	mul = bdNew();
	value_2 = bdNew();

	bdSetShort(value_2, 2);

	bdModSquare(sqr, angle_factor, secp256k1_p);
	bdModMult(mul, p.X, value_2, secp256k1_p);
	bdModSub(result.X, sqr, mul, secp256k1_p);

	bdFree(&sqr);
	bdFree(&mul);
	bdFree(&value_2);

	BIGD diff, mult;
	diff = bdNew();
	mult = bdNew();

	bdModSub(diff, p.X, result.X, secp256k1_p);
	bdModMult(mult, angle_factor, diff, secp256k1_p);
	bdModSub(result.Y, mult, p.Y, secp256k1_p);

	bdFree(&diff);
	bdFree(&mult);
}



void calc_summand_coordinates(unsigned char* private_key)
{
	for (int j = 0; j < 32; j++)
	{
		for (int k = 0; k < 8; k++)
		{
			if (((private_key[j] >> (7 - k)) & 0x01) == 0x00)
				summand_coordinates[j * 8 + k] = 0x00;
			else
				summand_coordinates[j * 8 + k] = 0x01;
		}
	}
}

void generate_public_key(unsigned char* private_key, unsigned char* result)
{
	secp256k1_point public_key;
	generator_to_scalar_multi(private_key,public_key);

//	bdPrintHex("public key X = ", public_key.X, "\n");
//	bdPrintHex("public key Y = ", public_key.Y, "\n");

	from_BIGD_to_uchar_array(public_key.X,result,32);
	from_BIGD_to_uchar_array(public_key.Y,result+32,32);
	
	bdFree(&public_key.X);
	bdFree(&public_key.Y);	
}

void compress_public_key(unsigned char* public_key, unsigned char* public_key_compress)
{
	unsigned char added_byte;
	if (public_key[63] % 2 == 0)
		added_byte = 0x02;
	else
		added_byte = 0x03;
	public_key_compress[0] = added_byte;
	for (int j = 0; j < 32; j++)
		public_key_compress[j + 1] = public_key[j];
}

string encoding_to_BASE58(unsigned char* data)
{
	BIGD base256;
	base256 = bdNew();
	from_uchar_array_to_BIGD(data,25,base256);
	unsigned char base58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	
	BIGD base;
	base = bdNew();
	bdSetShort(base, 58);

	string result = "";
	
	unsigned short count = 0;

	BIGD value_0;
	value_0 = bdNew();
	bdSetShort(value_0, 0);

	BIGD r;
	r = bdNew();
	unsigned char byte;

	BIGD swp;
	swp = bdNew();

	while (bdCompare(base256, value_0) > 0)
	{
		bdModulo(r, base256, base);
        byte = (unsigned char)(bdToShort(r));
		result += base58[byte];
		bdDivide_s(swp, r, base256, base);
		bdSetEqual(base256, swp);
		count++;
	}

	bdFree(&value_0);
	bdFree(&base);
	bdFree(&r);
	bdFree(&base256);
	bdFree(&swp);

	unsigned char count_ones = 0;

	for (int j = 0; j < 25; j++)
	{
		if (data[j] == 0)
			count_ones++;
		else
			break;
	}

	for (int j = 0; j < count_ones; j++)
		result += "1";

	string total;

	for (int j = 0; j < count + 1; j++)
		total += result[count - j];
	return total;
}

string BTC_adress_from_public_key(unsigned char* public_key_compress)
{
    uint32_t sha256_hash[8];
    uint32_t ripemd160_hash[5];
	unsigned char temp[32];
	unsigned char hashing_key[21];
	unsigned char adr[25];

	sha256_calc_hash(public_key_compress, 33, sha256_hash);

	for (int j = 0; j < 8; j++)
	{
		temp[j * 4] = sha256_hash[j] >> 24;
		temp[j * 4 + 1] = (sha256_hash[j] >> 16) & 0xFF;
		temp[j * 4 + 2] = (sha256_hash[j] >> 8) & 0xFF;
		temp[j * 4 + 3] = (sha256_hash[j]) & 0xFF;
	}

	ripemd160_calc_hash(temp, 32, ripemd160_hash);

	for (int j = 0; j < 5; j++)
	{
		hashing_key[4 * j + 1] = ripemd160_hash[j] >> 24;
		hashing_key[4 * j + 2] = (ripemd160_hash[j] >> 16) & 0xFF;
		hashing_key[4 * j + 3] = (ripemd160_hash[j] >> 8) & 0xFF;
		hashing_key[4 * j + 4] = ripemd160_hash[j] & 0xFF;
	}
	hashing_key[0] = 0x00;

	sha256_calc_hash(hashing_key, 21, sha256_hash);

	for (int j = 0; j < 8; j++)
	{
		temp[j * 4] = sha256_hash[j] >> 24;
		temp[j * 4 + 1] = (sha256_hash[j] >> 16) & 0xFF;
		temp[j * 4 + 2] = (sha256_hash[j] >> 8) & 0xFF;
		temp[j * 4 + 3] = (sha256_hash[j]) & 0xFF;
	}

	sha256_calc_hash(temp, 32, sha256_hash);

	for (int j = 0; j < 21; j++)
		adr[j] = hashing_key[j];

	adr[21] = sha256_hash[0] >> 24;
	adr[22] = (sha256_hash[0] >> 16) & 0xFF;
	adr[23] = (sha256_hash[0] >> 8) & 0xFF;
	adr[24] = sha256_hash[0] & 0xFF;

	return encoding_to_BASE58(adr);
}

string ETH_adress_from_public_key(unsigned char* public_key)
{   
	unsigned char hash[32];
	unsigned char temp[40], checksumm[40];
	keccak_calc_hash(public_key,64,hash);

	for (int j = 0; j < 20; j++)
	{
		temp[2*j] = hash[j+12]>>4;
		temp[2*j+1] = hash[j+12]&0x0F;
		if(temp[2*j] < 10)
			temp[2*j] += 0x30;
		else
			temp[2*j] += 87;
		if(temp[2*j+1] < 10)
			temp[2*j+1] += 0x30;
		else
			temp[2*j+1] += 87;	
	}
		
	keccak_calc_hash(temp,40,hash);
		
	for (int j = 0; j < 20; j++)
	{
		checksumm[2*j] = hash[j]>>4;
		checksumm[2*j+1] = hash[j]&0x0F;
		if(checksumm[2*j] < 10)
			checksumm[2*j] += 0x30;
		else
			checksumm[2*j] += 87;
		if(checksumm[2*j+1] < 10)
			checksumm[2*j+1] += 0x30;
		else
			checksumm[2*j+1] += 87;	
	}
	
	string result = "0x";
	
	for (int j = 0; j < 40; j++)
	{
		if (checksumm[j] >= '8')
		{
			if (temp[j] > 0x39)
				result += (temp[j] - 0x20);
			else
				result += temp[j];
		}
		else
			result += temp[j];
	}
	return result;
}

void generate_user_private_key(unsigned char* id, unsigned char* private_key)
{
	expansion_key(master_key);
	for (int j = 12; j < 16; j++)
	{
		init_vector[j] = id[j-12];
		init_vector[j + 4] = id[j - 8];
	}
		
	speed_encryption(init_vector);
	
	for (int j = 0; j < 16; j++)
		iv_external_re_keying[j] = init_vector[j];
	
	
	speed_encryption(init_vector+16);
	for (int j = 16; j < 32; j++)
		iv_external_re_keying[j] = init_vector[j];
	
	expansion_key(iv_external_re_keying);

	unsigned char tmp_iv[32];
	for (int j = 0; j < 32; j++)
		tmp_iv[j] = out_iv[j];
	
	bool ok = false;
	
	while (ok != true)
	{
		speed_encryption(tmp_iv);
		for (int j = 0; j < 16; j++)
			private_key[j] = tmp_iv[j];
		speed_encryption(tmp_iv + 16);
		for (int j = 16; j < 32; j++)
			private_key[j] = tmp_iv[j];

		ok = check_key_in_field(private_key);
		if (ok == 1)
		{
			for(int j=0;j<32;j++)
				tmp_iv[j] = private_key[j];
		}
	}
	for(int j=0;j<32;j++)
		init_vector[j] = 0x00;
}

bool check_key_in_field(unsigned char* private_key)
{
	BIGD base256, value_1,compare_value;
	base256 = bdNew();
	value_1 = bdNew();
	compare_value = bdNew();
	bdSetShort(value_1,1);
	
	bdSubtract_s(compare_value,secp256k1_n,value_1);

	from_uchar_array_to_BIGD(private_key,32,base256);
	
	if (bdCompare(compare_value, base256) == 1)
	{
		bdFree(&base256);
		return true;
	}
	bdFree(&base256);
	return false;
}

string generate_ETH_adress(unsigned char* id){
										
	unsigned char private_key[32];
	unsigned char public_key[64];

	generate_user_private_key(id,private_key);

	generate_public_key(private_key,public_key);

	return ETH_adress_from_public_key(public_key);	
}

string generate_BTC_adress(unsigned char* id){
	unsigned char private_key[32];
	unsigned char public_key[64];
	unsigned char public_key_compress[33];

	generate_user_private_key(id,private_key);

	generate_public_key(private_key,public_key);
	
	compress_public_key(public_key,public_key_compress);
	
	string result = BTC_adress_from_public_key(public_key_compress);
	
	return result;
}

void ecdsa_sign_data_with_id(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* id,unsigned int cryptocurrency){
	
	unsigned char private_key[32];
	generate_user_private_key(id,private_key);
	ecdsa_sign_data(data,len_data,signature,private_key, cryptocurrency);	
}

bool ecdsa_verify_sign_with_id(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* id, unsigned int cryptocurrency)
{	
	unsigned char private_key[32];
	generate_user_private_key(id,private_key);
	unsigned char public_key[64];
	generate_public_key(private_key,public_key);
	
	return ecdsa_verify_sign(data,len_data,signature,public_key,cryptocurrency);	
}

void point_by_scalar_multi(secp256k1_point P, unsigned char* Scalar, secp256k1_point result)
{
	BIGD value_0;
	value_0 = bdNew();
	bdSetShort(value_0, 0);

	secp256k1_point cur;
	cur.X = bdNew();
	cur.Y = bdNew();

	bdSetEqual(cur.X, P.X);
	bdSetEqual(cur.Y,P.Y);

	secp256k1_point tmp_s, tmp_pub;
	
	bdSetEqual(result.X, value_0);
	bdSetEqual(result.Y, value_0);
	
	calc_summand_coordinates(Scalar);

	for (int j = 0; j < 256; j++)
	{
		if (summand_coordinates[255 - j] == 0x01)
		{
			summ_point(cur, result,tmp_pub);
		}
		double_point(cur,tmp_s);
		bdSetEqual(cur.X, tmp_s.X);
		bdSetEqual(cur.Y, tmp_s.Y);
		bdSetEqual(result.X, tmp_pub.X);
		bdSetEqual(result.Y, tmp_pub.Y);
	}

	bdFree(&value_0);
	bdFree(&cur.X);
	bdFree(&cur.Y);
}

void from_uchar_array_to_BIGD(unsigned char* prototype, uint32_t len_prototype, BIGD type)
{
	BIGD multiplier;
	multiplier = bdNew();
	bdSetShort(multiplier, 256);

	bdSetShort(type, 1);

	BIGD temp, temp_char;
	temp = bdNew();
	temp_char = bdNew();

	bdSetShort(temp, 1);

	BIGD value_1;
	value_1 = bdNew();
	bdSetShort(value_1, 1);

	BIGD current_mul;
	current_mul = bdNew();

	bdSetShort(temp_char, prototype[len_prototype-1]);
	bdMultiply_s(type, temp_char, value_1);

	for (int j = len_prototype-2; j >= 0; j--)
	{
		bdSetShort(temp_char, prototype[j]);
		bdMultiply_s(temp, temp, multiplier);
		bdMultiply_s(current_mul, temp_char, temp);
		bdAdd_s(type, current_mul, type);
	}

	bdFree(&temp);
	bdFree(&temp_char);
	bdFree(&multiplier);
	bdFree(&current_mul);
	bdFree(&value_1);
}

void from_BIGD_to_uchar_array(BIGD prototype, unsigned char* type, uint32_t len_type)
{
	BIGD value_0;
	value_0 = bdNew();
	bdSetShort(value_0, 0);

	BIGD mask, byte;
	mask = bdNew();
	byte = bdNew();
	bdSetShort(mask, 256);

    uint32_t numb;
	char count = len_type-1;

	while (bdCompare(prototype, value_0) > (0))
	{
		bdModulo_s(byte, prototype, mask);
		numb = bdToShort(byte);
		type[count] = (unsigned char)numb;
		count--;
		bdShiftRight(prototype, prototype, 8);
	}
	for (int j = 0; j <= count; j++)
		type[j] = 0x00;

	bdFree(&value_0);
	bdFree(&mask);
	bdFree(&byte);
}

void generator_to_scalar_multi(unsigned char* key, secp256k1_point result)
{
	secp256k1_point s;
	
	bdSetEqual(s.X, Generator.X);
	bdSetEqual(s.Y, Generator.Y);
			
	BIGD value_0;
	value_0 = bdNew();
	bdSetShort(value_0, 0);

	secp256k1_point tmp_s, tmp_pub;
	
	bdSetEqual(result.X, value_0);
	bdSetEqual(result.Y, value_0);
	
	calc_summand_coordinates(key);

	for (int j = 0; j < 256; j++)
	{
		if (summand_coordinates[255 - j] == 0x01)
		{
			summ_point(s, result,tmp_pub);
		}
		double_point(s,tmp_s);
		bdSetEqual(s.X, tmp_s.X);
		bdSetEqual(s.Y, tmp_s.Y);
		bdSetEqual(result.X, tmp_pub.X);
		bdSetEqual(result.Y, tmp_pub.Y);
	}

	bdFree(&s.X);
	bdFree(&s.Y);
	bdFree(&value_0);
	bdFree(&tmp_s.X);
	bdFree(&tmp_s.Y);
	bdFree(&tmp_pub.X);
	bdFree(&tmp_pub.Y);
}

void ecdsa_sign_data(unsigned char* data,uint32_t len_data, unsigned char* signature, unsigned char* private_key,unsigned int cryptocurrency)
{
	BIGD Secret;
	Secret = bdNew();
	from_uchar_array_to_BIGD(private_key,32,Secret);
	
	unsigned char hash[32];
	uint32_t tmp_hash[8];
	
	// 0 - расчет хэша от данных
	if(cryptocurrency == 0x01){
		sha256_calc_hash(data,len_data,tmp_hash);
	
		for(int j=0;j<8;j++)
		{	
			hash[j*4] = tmp_hash[j]>>24;
			hash[j*4+1] = (tmp_hash[j]>>16)&0xFF;
			hash[j*4+2] = (tmp_hash[j]>>8)&0xFF;
			hash[j*4+3] = tmp_hash[j]&0xFF;
		}
	}
	else if(cryptocurrency == 0x02){
		keccak_calc_hash(data,len_data,hash);
	}	

	BIGD Hash;
	Hash = bdNew();
	from_uchar_array_to_BIGD(hash,32,Hash);
											
	unsigned char efemerial_key[32];
	bool ok = false;
	//Содержимое нового gpsch_iv - сохранить в энергонезависимую память
	// 1 - Вычисление случайного числа K от ГПСЧ на основе блочного шифра
	while (ok != true)
	{
		expansion_key(gpsch_key);
		speed_encryption(gpsch_iv);
		for(int j=0;j<16;j++)
			efemerial_key[j] = gpsch_iv[j];
		speed_encryption(gpsch_iv);
		for(int j=0;j<16;j++)
			efemerial_key[j+16] = gpsch_iv[j];
		ok = check_key_in_field(efemerial_key);
	}
	//2 - получение маскирующей точки r = G*K
	secp256k1_point R;
	
	generator_to_scalar_multi(efemerial_key,R);
	// 3 - нахождение обратного по модулю от эфемерного ключа	
	BIGD Efem;
	Efem = bdNew();
	from_uchar_array_to_BIGD(efemerial_key,32,Efem);
		
	BIGD S;
	S = bdNew();
	
	bdModInv(S,Efem,secp256k1_n);
	// 3 - вычисление подписи	
	BIGD XR;
	XR = bdNew();
	bdModMult(XR,Secret,R.X,secp256k1_n);
	BIGD H_XR;
	H_XR = bdNew();
	bdModAdd(H_XR,Hash,XR,secp256k1_n);
	BIGD tt;
	tt = bdNew();
	bdModMult(tt,S,H_XR,secp256k1_n);
	from_BIGD_to_uchar_array(R.X,signature,32);
	from_BIGD_to_uchar_array(tt,signature+32,32);

	bdFree(&Secret);
	bdFree(&Hash);
	bdFree(&R.X);
	bdFree(&R.Y);
	bdFree(&Efem);
	bdFree(&S);
	bdFree(&XR);
	bdFree(&H_XR);
}

bool ecdsa_verify_sign(unsigned char* data, uint32_t len_data, unsigned char* signature, unsigned char* public_key, unsigned int cryptocurrency)
{
	secp256k1_point Pub;
	Pub.X = bdNew();
	Pub.Y = bdNew();
	from_uchar_array_to_BIGD(public_key,32,Pub.X);
	from_uchar_array_to_BIGD(public_key+32,32,Pub.Y);
		
	// 1 - проверить подпись и эфемерный ключ на принадлежность полю под кривой
	bool ok = check_key_in_field(signature);
	if(ok == false)
	{
		bdFree(&Pub.X);
		bdFree(&Pub.Y);
		return false;
	}	
	ok = check_key_in_field(signature+32);
	if(ok == false)
	{
		bdFree(&Pub.X);
		bdFree(&Pub.Y);
		return false;
	}
	// 2 - расчет хэша от данных
	unsigned char hash[32];
	uint32_t tmp_hash[8];

	if(cryptocurrency == 0x01){
		sha256_calc_hash(data,len_data,tmp_hash);
	
		for(int j=0;j<8;j++)
		{
			hash[j*4] = tmp_hash[j]>>24;
			hash[j*4+1] = (tmp_hash[j]>>16)&0xFF;
			hash[j*4+2] = (tmp_hash[j]>>8)&0xFF;
			hash[j*4+3] = tmp_hash[j]&0xFF;
		}
	}
	else if(cryptocurrency == 0x02){
		keccak_calc_hash(data,len_data,hash);
	}

	BIGD Hash;
	Hash = bdNew();
	from_uchar_array_to_BIGD(hash,32,Hash);
	// 3 - вычисление U1 и U2
	
	BIGD Sign, R, S_inv, U1, U2;
	Sign = bdNew();
	R = bdNew();
	S_inv = bdNew();
	U1 = bdNew();
	U2 = bdNew();

	from_uchar_array_to_BIGD(signature,32,R);
	from_uchar_array_to_BIGD(signature+32,32,Sign);
	
	bdModInv(S_inv,Sign,secp256k1_n);
	bdModMult(U1,S_inv,Hash,secp256k1_n);
	bdModMult(U2,S_inv,R,secp256k1_n);
	
	// 4 - получение верифицирующей точки

	secp256k1_point Sum1, Sum2;
	unsigned char u1[32];
	unsigned char u2[32];

	from_BIGD_to_uchar_array(U1,u1,32);
	from_BIGD_to_uchar_array(U2,u2,32);

	point_by_scalar_multi(Generator,u1,Sum1);
	point_by_scalar_multi(Pub,u2,Sum2);

	secp256k1_point total;
	unsigned char res[32];
	summ_point(Sum1,Sum2, total);

	
	from_BIGD_to_uchar_array(total.X,res,32);
	
	bdFree(&Hash);
	bdFree(&Sign);
	bdFree(&R);
	bdFree(&S_inv);
	bdFree(&U1);
	bdFree(&U2);
	bdFree(&Pub.X);
	bdFree(&Pub.Y);

	// 5 - проверка полученной подписи
	for(int j=0;j<32;j++)
	{
		if(res[j] != signature[j])
			return false;	
	}

	return true;
}