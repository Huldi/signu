#include "SHA256.h"

uint32_t sha256_pre_handle_data(unsigned char* data, uint32_t length, uint32_t* data_to_long)
{
    uint32_t size_arr;
    uint32_t j;
	if (length / 64 == 0x00)
	{
        unsigned char diff = (unsigned char)(64 - length);

		if (diff <= 8)
		{
			size_arr = 128;
		}
		else
			size_arr = 64;
		unsigned char* tmp_char = new unsigned char[size_arr];
		for (j = 0; j < length; j++)
			tmp_char[j] = data[j];
		tmp_char[length] = 0x80;
		for (j = int(length + 1); j < size_arr - 4; j++)
			tmp_char[j] = 0x00;
		length *= 8;
		for (j = 0; j < 4; j++)
            tmp_char[size_arr - 4 + j] = (unsigned char)((length >> ((3 - j) * 8)) & 0xFF);
		for (j = 0; j < size_arr / 4; j++)
			data_to_long[j] = (tmp_char[j * 4] << 24) + (tmp_char[j * 4 + 1] << 16) + (tmp_char[j * 4 + 2] << 8) + (tmp_char[j * 4 + 3]);
		delete[] tmp_char;
	}
	else if (length / 64 != 0x00)
	{
        uint32_t num_block = length / 64 + 1;
        unsigned char diff = (unsigned char)(64 - length % 64);
		if (diff <= 8)
			size_arr = (num_block + 1) * 64;
		else
			size_arr = num_block * 64;

		unsigned char* tmp_char = new unsigned char[size_arr];
		for (j = 0; j < length; j++)
			tmp_char[j] = data[j];
		tmp_char[length] = 0x80;
		for (j = int(length + 1); j < size_arr - 4; j++)
			tmp_char[j] = 0x00;
		length *= 8;
		for (j = 0; j < 4; j++)
            tmp_char[size_arr - 4 + j] = (unsigned char)((length >> ((3 - j) * 8)) & 0xFF);
		for (j = 0; j < size_arr / 4; j++)
			data_to_long[j] = (tmp_char[j * 4] << 24) + (tmp_char[j * 4 + 1] << 16) + (tmp_char[j * 4 + 2] << 8) + (tmp_char[j * 4 + 3]);
		delete[] tmp_char;
	}
	return (size_arr / 64);
}

void sha256_calc_hash(unsigned char* data, uint32_t length, uint32_t* hash)
{
    uint32_t* tmp_long = new uint32_t[length / 4 + 16];

    uint32_t numb_iteration = sha256_pre_handle_data(data, length, tmp_long);

    uint32_t tmp[16];
    uint32_t current_hash[8];
    uint32_t  temp[8];
	for (int j = 0; j < 8; j++)
	{
		current_hash[j] = SHA256_FIRST_FILLING[j];
		temp[j] = SHA256_FIRST_FILLING[j];
	}
    for (uint32_t j = 0; j < numb_iteration; j++)
	{
		for (int k = 0; k < 16; k++)
			tmp[k] = tmp_long[j * 16 + k];

        uint32_t words[64];
        uint32_t s0, s1;
		for (int j = 0; j < 64; j++)
		{
			if (j < 16)
				words[j] = tmp[j];
			else
			{
				s0 = ((words[j - 15] << 25) + (words[j - 15] >> 7)) ^ ((words[j - 15] << 14) + (words[j - 15] >> 18)) ^ ((words[j - 15] >> 3));
				s1 = ((words[j - 2] << 15) + (words[j - 2] >> 17)) ^ ((words[j - 2] << 13) + (words[j - 2] >> 19)) ^ ((words[j - 2] >> 10));
				words[j] = words[j - 16] + s0 + s1 + words[j - 7];
			}
		}
		for (int j = 0; j < 8; j++)
			temp[j] = current_hash[j];

        uint32_t summ0, summ1, Ma, t1, t2, Ch;
		for (int j = 0; j < 64; j++)
		{
			summ0 = ((temp[0] << 30) + (temp[0] >> 2)) ^ ((temp[0] << 19) + (temp[0] >> 13)) ^ ((temp[0] << 10) + (temp[0] >> 22));
			Ma = (temp[0] & temp[1]) ^ (temp[0] & temp[2]) ^ (temp[1] & temp[2]);
			t2 = summ0 + Ma;
			summ1 = ((temp[4] << 26) + (temp[4] >> 6)) ^ ((temp[4] << 21) + (temp[4] >> 11)) ^ ((temp[4] << 7) + (temp[4] >> 25));
			Ch = (temp[4] & temp[5]) ^ ((temp[4] ^ 0xFFFFFFFF)&temp[6]);
			t1 = temp[7] + summ1 + Ch + SHA256_CONSTANTS_CUBIC[j] + words[j];

			temp[7] = temp[6];
			temp[6] = temp[5];
			temp[5] = temp[4];
			temp[4] = temp[3] + t1;
			temp[3] = temp[2];
			temp[2] = temp[1];
			temp[1] = temp[0];
			temp[0] = t1 + t2;

		}
		for (int j = 0; j < 8; j++)
		{
			current_hash[j] += temp[j];
		}
	}
	delete[] tmp_long;
	for (int j = 0; j < 8; j++)
		hash[j] = current_hash[j];
}

