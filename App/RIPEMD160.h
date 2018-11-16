#pragma once
#include <stdint.h>

const uint32_t RIPEMD160_ROUND_CONSTATNS[10] = {
	0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E,
	0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000
};

const unsigned char RIPEMD160_S_0_15_R[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
const unsigned char RIPEMD160_S_16_31_R[16] = { 7,4,13,1,10,6,15,3,12,0,9,5,2,14,11,8 };
const unsigned char RIPEMD160_S_32_47_R[16] = { 3,10,14,4,9,15,8,1,2,7,0,6,13,11,5,12 };
const unsigned char RIPEMD160_S_48_63_R[16] = { 1,9,11,10,0,8,12,4,13,3,7,15,14,5,6,2 };
const unsigned char RIPEMD160_S_64_79_R[16] = { 4,0,5,9,7,12,2,10,14,1,3,8,11,6,15,13 };
const unsigned char RIPEMD160_S_0_15_R1[16] = { 5,14,7,0,9,2,11,4,13,6,15,8,1,10,3,12 };
const unsigned char RIPEMD160_S_16_31_R1[16] = { 6,11,3,7,0,13,5,10,14,15,8,12,4,9,1,2 };
const unsigned char RIPEMD160_S_32_47_R1[16] = { 15,5,1,3,7,14,6,9,11,8,12,2,10,0,4,13 };
const unsigned char RIPEMD160_S_48_63_R1[16] = { 8,6,4,1,3,11,15,0,5,12,2,13,9,7,10,14 };
const unsigned char RIPEMD160_S_64_79_R1[16] = { 12,15,10,4,1,5,8,7,6,2,13,14,0,3,9,11 };

const unsigned char RIPEMD160_SHIFT_0_15_R[16] = { 11,14,15,12,5,8,7,9,11,13,14,15,6,7,9,8 };
const unsigned char RIPEMD160_SHIFT_16_31_R[16] = { 7,6,8,13,11,9,7,15,7,12,15,9,11,7,13,12 };
const unsigned char RIPEMD160_SHIFT_32_47_R[16] = { 11,13,6,7,14,9,13,15,14,8,13,6,5,12,7,5 };
const unsigned char RIPEMD160_SHIFT_48_63_R[16] = { 11,12,14,15,14,15,9,8,9,14,5,6,8,6,5,12 };
const unsigned char RIPEMD160_SHIFT_64_79_R[16] = { 9,15,5,11,6,8,13,12,5,12,13,14,11,8,5,6 };
const unsigned char RIPEMD160_SHIFT_0_15_R1[16] = { 8,9,9,11,13,15,15,5,7,7,8,11,14,14,12,6 };
const unsigned char RIPEMD160_SHIFT_16_31_R1[16] = { 9,13,15,7,12,8,9,11,7,7,12,7,6,15,13,11 };
const unsigned char RIPEMD160_SHIFT_32_47_R1[16] = { 9,7,15,11,8,6,6,14,12,13,5,14,13,13,7,5 };
const unsigned char RIPEMD160_SHIFT_48_63_R1[16] = { 15,5,8,11,14,14,6,14,6,9,12,9,12,5,15,8 };
const unsigned char RIPEMD160_SHIFT_64_79_R1[16] = { 8,5,12,9,12,5,14,6,8,13,6,5,15,13,11,11 };

const uint32_t RIPEMD160_START_STATE[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };

uint32_t ripemd160_f_0_15(uint32_t x, uint32_t y, uint32_t z);
uint32_t ripemd160_f_16_31(uint32_t x, uint32_t y, uint32_t z);
uint32_t ripemd160_f_32_47(uint32_t x, uint32_t y, uint32_t z);
uint32_t ripemd160_f_48_63(uint32_t x, uint32_t y, uint32_t z);
uint32_t ripemd160_f_64_79(uint32_t x, uint32_t y, uint32_t z);

uint32_t ripemd160_pre_handle_data(unsigned char* data, uint32_t length, uint32_t* data_to_hash);
void ripemd160_calc_hash(unsigned char* data, uint32_t length, uint32_t* hash);

uint32_t rol_word(uint32_t word, unsigned char shift);
