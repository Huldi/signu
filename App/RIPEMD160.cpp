#include "RIPEMD160.h"

uint32_t ripemd160_f_0_15(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}

uint32_t ripemd160_f_16_31(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x&y) | ((~x)&z));
}

uint32_t ripemd160_f_32_47(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x | (~y)) ^ z);
}

uint32_t ripemd160_f_48_63(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x&z) | (y&(~z)));
}

uint32_t ripemd160_f_64_79(uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ (y | (~z)));
}

uint32_t ripemd160_pre_handle_data(unsigned char* data, uint32_t length, uint32_t* data_to_hash)
{
    uint32_t size_arr;
    uint32_t j;
	if (length / 64 == 0)
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
			data_to_hash[j] = (tmp_char[j * 4 + 3] << 24) + (tmp_char[j * 4 + 2] << 16) + (tmp_char[j * 4 + 1] << 8) + (tmp_char[j * 4]);

		data_to_hash[14] = length;
		data_to_hash[15] = 0x00;

		delete[] tmp_char;
	}
	else
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
			data_to_hash[j] = (tmp_char[j * 4 + 3] << 24) + (tmp_char[j * 4 + 2] << 16) + (tmp_char[j * 4 + 1] << 8) + (tmp_char[j * 4]);

		data_to_hash[((size_arr / 64) - 1) * 16 + 14] = length;
		data_to_hash[((size_arr / 64) - 1) * 16 + 15] = 0x00;

		delete[] tmp_char;
	}
	return (size_arr / 64);
}

void ripemd160_calc_hash(unsigned char* data, uint32_t length, uint32_t* hash)
{
    uint32_t* tmp_long = new uint32_t[length / 4 + 16];

    uint32_t numb_iteration = ripemd160_pre_handle_data(data, length, tmp_long);

    uint32_t temp[5], temp_inv[5];
    uint32_t round_state;
    uint32_t round_data[16];
	for (int j = 0; j < 5; j++)
		hash[j] = RIPEMD160_START_STATE[j];

    for (uint32_t k = 0; k < numb_iteration; k++)
	{
		for (int t = 0; t < 5; t++)
		{
			temp[t] = hash[t];
			temp_inv[t] = hash[t];
		}
		for (int t = 0; t < 16; t++)
		{
			round_data[t] = tmp_long[16 * k + t];
		}

		for (int j = 0; j < 80; j++)
		{
			if (j < 16)
			{
				round_state = rol_word(temp[0] + ripemd160_f_0_15(temp[1], temp[2], temp[3]) + round_data[RIPEMD160_S_0_15_R[j]] + RIPEMD160_ROUND_CONSTATNS[0], RIPEMD160_SHIFT_0_15_R[j]) + temp[4];
				temp[0] = temp[4];
				temp[4] = temp[3];
				temp[3] = rol_word(temp[2], 10);
				temp[2] = temp[1];
				temp[1] = round_state;
				round_state = rol_word(temp_inv[0] + ripemd160_f_64_79(temp_inv[1], temp_inv[2], temp_inv[3]) + round_data[RIPEMD160_S_0_15_R1[j]] + RIPEMD160_ROUND_CONSTATNS[5], RIPEMD160_SHIFT_0_15_R1[j]) + temp_inv[4];
				temp_inv[0] = temp_inv[4];
				temp_inv[4] = temp_inv[3];
				temp_inv[3] = rol_word(temp_inv[2], 10);
				temp_inv[2] = temp_inv[1];
				temp_inv[1] = round_state;
			}
			else if (j >= 16 && j <= 31)
			{
				round_state = rol_word(temp[0] + ripemd160_f_16_31(temp[1], temp[2], temp[3]) + round_data[RIPEMD160_S_16_31_R[j - 16]] + RIPEMD160_ROUND_CONSTATNS[1], RIPEMD160_SHIFT_16_31_R[j - 16]) + temp[4];
				temp[0] = temp[4];
				temp[4] = temp[3];
				temp[3] = rol_word(temp[2], 10);
				temp[2] = temp[1];
				temp[1] = round_state;
				round_state = rol_word(temp_inv[0] + ripemd160_f_48_63(temp_inv[1], temp_inv[2], temp_inv[3]) + round_data[RIPEMD160_S_16_31_R1[j - 16]] + RIPEMD160_ROUND_CONSTATNS[6], RIPEMD160_SHIFT_16_31_R1[j - 16]) + temp_inv[4];
				temp_inv[0] = temp_inv[4];
				temp_inv[4] = temp_inv[3];
				temp_inv[3] = rol_word(temp_inv[2], 10);
				temp_inv[2] = temp_inv[1];
				temp_inv[1] = round_state;
			}
			else if (j >= 32 && j <= 47)
			{
				round_state = rol_word(temp[0] + ripemd160_f_32_47(temp[1], temp[2], temp[3]) + round_data[RIPEMD160_S_32_47_R[j - 32]] + RIPEMD160_ROUND_CONSTATNS[2], RIPEMD160_SHIFT_32_47_R[j - 32]) + temp[4];
				temp[0] = temp[4];
				temp[4] = temp[3];
				temp[3] = rol_word(temp[2], 10);
				temp[2] = temp[1];
				temp[1] = round_state;
				round_state = rol_word(temp_inv[0] + ripemd160_f_32_47(temp_inv[1], temp_inv[2], temp_inv[3]) + round_data[RIPEMD160_S_32_47_R1[j - 32]] + RIPEMD160_ROUND_CONSTATNS[7], RIPEMD160_SHIFT_32_47_R1[j - 32]) + temp_inv[4];
				temp_inv[0] = temp_inv[4];
				temp_inv[4] = temp_inv[3];
				temp_inv[3] = rol_word(temp_inv[2], 10);
				temp_inv[2] = temp_inv[1];
				temp_inv[1] = round_state;
			}
			else if (j >= 48 && j <= 63)
			{
				round_state = rol_word(temp[0] + ripemd160_f_48_63(temp[1], temp[2], temp[3]) + round_data[RIPEMD160_S_48_63_R[j - 48]] + RIPEMD160_ROUND_CONSTATNS[3], RIPEMD160_SHIFT_48_63_R[j - 48]) + temp[4];
				temp[0] = temp[4];
				temp[4] = temp[3];
				temp[3] = rol_word(temp[2], 10);
				temp[2] = temp[1];
				temp[1] = round_state;
				round_state = rol_word(temp_inv[0] + ripemd160_f_16_31(temp_inv[1], temp_inv[2], temp_inv[3]) + round_data[RIPEMD160_S_48_63_R1[j - 48]] + RIPEMD160_ROUND_CONSTATNS[8], RIPEMD160_SHIFT_48_63_R1[j - 48]) + temp_inv[4];
				temp_inv[0] = temp_inv[4];
				temp_inv[4] = temp_inv[3];
				temp_inv[3] = rol_word(temp_inv[2], 10);
				temp_inv[2] = temp_inv[1];
				temp_inv[1] = round_state;
			}
			else if (j >= 64 && j <= 79)
			{
				round_state = rol_word(temp[0] + ripemd160_f_64_79(temp[1], temp[2], temp[3]) + round_data[RIPEMD160_S_64_79_R[j - 64]] + RIPEMD160_ROUND_CONSTATNS[4], RIPEMD160_SHIFT_64_79_R[j - 64]) + temp[4];
				temp[0] = temp[4];
				temp[4] = temp[3];
				temp[3] = rol_word(temp[2], 10);
				temp[2] = temp[1];
				temp[1] = round_state;
				round_state = rol_word(temp_inv[0] + ripemd160_f_0_15(temp_inv[1], temp_inv[2], temp_inv[3]) + round_data[RIPEMD160_S_64_79_R1[j - 64]] + RIPEMD160_ROUND_CONSTATNS[9], RIPEMD160_SHIFT_64_79_R1[j - 64]) + temp_inv[4];
				temp_inv[0] = temp_inv[4];
				temp_inv[4] = temp_inv[3];
				temp_inv[3] = rol_word(temp_inv[2], 10);
				temp_inv[2] = temp_inv[1];
				temp_inv[1] = round_state;
			}
		}
		round_state = hash[1] + temp[2] + temp_inv[3];
		hash[1] = hash[2] + temp[3] + temp_inv[4];
		hash[2] = hash[3] + temp[4] + temp_inv[0];
		hash[3] = hash[4] + temp[0] + temp_inv[1];
		hash[4] = hash[0] + temp[1] + temp_inv[2];
		hash[0] = round_state;
	}

	for (int j = 0; j < 5; j++)
	{
		hash[j] = ((hash[j] & 0xFF) << 24) + (((hash[j] >> 8) & 0xFF) << 16) + (((hash[j] >> 16) & 0xFF) << 8) + (hash[j] >> 24);
	}

	delete[] tmp_long;
}

uint32_t rol_word(uint32_t word, unsigned char shift)
{
    uint32_t temp = word >> (32 - shift);
	return ((word << shift) + temp);
}

