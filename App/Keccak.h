#pragma once
#include <stdint.h>
#include <stdlib.h>


#ifdef _MSC_VER
// Windows
//typedef unsigned __int8  uint8_t;
//typedef unsigned __int64 uint64_t;
#else
// GCC
#include <stdint.h>
#endif

void keccak_calc_hash(unsigned char* data, uint32_t numBytes, unsigned char* hash);
void keccak_reset();
void keccak_add(unsigned char* data, uint32_t numBytes);
void keccak_processBlock(unsigned char* data);
void keccak_processBuffer();

uint64_t rotateLeft(uint64_t x, uint8_t numBits);
uint64_t swap(uint64_t x);
uint32_t mod5(uint32_t x);