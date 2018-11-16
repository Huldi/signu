#include "Kuznyechik.h"

unsigned char TABLE_MULTIPLY_148[256];
unsigned char TABLE_MULTIPLY_32[256];
unsigned char TABLE_MULTIPLY_133[256];
unsigned char TABLE_MULTIPLY_16[256];
unsigned char TABLE_MULTIPLY_194[256];
unsigned char TABLE_MULTIPLY_192[256];
unsigned char TABLE_MULTIPLY_251[256];

const unsigned short PRIMITIVE_POLINOM = 451;
const unsigned char SIZE_BLOCK_DATA = 16;
const unsigned char NUMBER_ROUND = 9;

unsigned char TABLE_SPEED[65536];

unsigned char SPEED_TABLE_1[65536];

unsigned char expansion_key_array[160];

unsigned char CONSTANTS_C[512];

const unsigned char S_TABLE[256] =
{ 252, 238, 221, 17, 207, 110, 49, 22, 251, 196, 250, 218, 35, 197, 4, 77,
 233, 119, 240, 219, 147, 46, 153, 186, 23, 54, 241, 187, 20, 205, 95, 193,
 249, 24, 101, 90, 226, 92, 239, 33, 129, 28, 60, 66, 139, 1, 142, 79,
 5, 132, 2, 174, 227, 106, 143, 160, 6, 11, 237, 152, 127, 212, 211, 31,
 235, 52, 44, 81, 234, 200, 72, 171, 242, 42, 104, 162, 253, 58, 206, 204,
 181, 112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71, 156, 183, 93, 135,
 21, 161, 150, 41, 16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
 50, 117, 25, 61, 255, 53, 138, 126, 109, 84, 198, 128, 195, 189, 13, 87,
 223, 245, 36, 169, 62, 168, 67, 201, 215, 121, 214, 246, 124, 34, 185, 3,
 224, 15, 236, 222, 122, 148, 176, 188, 220, 232, 40, 80, 78, 51, 10, 74,
 167, 151, 96, 115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159, 38, 65,
 173, 69, 70, 146, 39, 94, 85, 47, 140, 163, 165, 125, 105, 213, 149, 59,
 7, 88, 179, 64, 134, 172, 29, 247, 48, 55, 107, 228, 136, 217, 231, 137,
 225, 27, 131, 73, 76, 63, 248, 254, 141, 83, 170, 144, 202, 216, 133, 97,
 32, 113, 103, 164, 45, 43, 9, 91, 203, 155, 37, 208, 190, 229, 108, 82,
 89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57, 75, 99, 182
};

const unsigned char L_SPEED[256] =
{
0xCF,0x98,0x74,0xBF,0x93,0x8E,0xF2,0xF3,0x0A,0xBF,0xF6,0xA9,0xEA,0x8E,0x4D,0x6E,
0x6E,0x20,0xC6,0xDA,0x90,0x48,0x89,0x9C,0xC1,0x64,0xB8,0x2D,0x86,0x44,0xD0,0xA2,
0xA2,0xC8,0x87,0x70,0x68,0x43,0x1C,0x2B,0xA1,0x63,0x30,0x6B,0x9F,0x30,0xE3,0x76,
0x76,0x33,0x10,0x0C,0x1C,0x11,0xD6,0x6A,0xA6,0xD7,0xF6,0x49,0x07,0x14,0xE8,0x72,
0x72,0xF2,0x6B,0xCA,0x20,0xEB,0x02,0xA4,0x8D,0xD4,0xC4,0x01,0x65,0xDD,0x4C,0x6C,
0x6C,0x76,0xEC,0x0C,0xC5,0xBC,0xAF,0x6E,0xA3,0xE1,0x90,0x58,0x0E,0x02,0xC3,0x48,
0x48,0xD5,0x62,0x17,0x06,0x2D,0xC4,0xE7,0xD5,0xEB,0x99,0x78,0x52,0xF5,0x16,0x7A,
0x7A,0xE6,0x4E,0x1A,0xBB,0x2E,0xF1,0xBE,0xD4,0xAF,0x37,0xB1,0xD4,0x2A,0x6E,0xB8,
0xB8,0x49,0x87,0x14,0xCB,0x8D,0xAB,0x49,0x09,0x6C,0x2A,0x01,0x60,0x8E,0x4B,0x5D,
0x5D,0xD4,0xB8,0x2F,0x8D,0x12,0xEE,0xF6,0x08,0x54,0x0F,0xF3,0x98,0xC8,0x7F,0x27,
0x27,0x9F,0xBE,0x68,0x1A,0x7C,0xAD,0xC9,0x84,0x2F,0xEB,0xFE,0xC6,0x48,0xA2,0xBD,
0xBD,0x95,0x5E,0x30,0xE9,0x60,0xBF,0x10,0xEF,0x39,0xEC,0x91,0x7F,0x48,0x89,0x10,
0x10,0xE9,0xD0,0xD9,0xF3,0x94,0x3D,0xAF,0x7B,0xFF,0x64,0x91,0x52,0xF8,0x0D,0xDD,
0xDD,0x99,0x75,0xCA,0x97,0x44,0x5A,0xE0,0x30,0xA6,0x31,0xD3,0xDF,0x48,0x64,0x84,
0x84,0x2D,0x74,0x96,0x5D,0x77,0x6F,0xDE,0x54,0xB4,0x8D,0xD1,0x44,0x3C,0xA5,0x94,
0x94,0x20,0x85,0x10,0xC2,0xC0,0x01,0xFB,0x01,0xC0,0xC2,0x10,0x85,0x20,0x94,0x01
};


void Kuznyechik_start()
{
	create_multiply_table();
	calculation_constants();
	generate_speed_table();
}

void expansion_key(unsigned char* master_key)
{
	unsigned char temp_array[32];
	unsigned char shift_position = 0x00;

	for(int j=0;j<SIZE_BLOCK_DATA;j++)// �������� ������ 2 �������� �� ������-�����
	{
		expansion_key_array[j] = master_key[j];
		expansion_key_array[j+SIZE_BLOCK_DATA] = master_key[j+SIZE_BLOCK_DATA];
	}
	unsigned char temp1[SIZE_BLOCK_DATA],temp2[SIZE_BLOCK_DATA],temp_left[SIZE_BLOCK_DATA];

	for(int j=0;j<4;j++)// ���������� ��� ��������� (3,4) (5,6) (7,8) (9,10)
	{
		for(int t=0;t<SIZE_BLOCK_DATA;t++)
		{
			temp1[t] = expansion_key_array[2*j*SIZE_BLOCK_DATA+t];
			temp2[t] = expansion_key_array[2*j*SIZE_BLOCK_DATA + SIZE_BLOCK_DATA + t];
		}
		for(int k=0;k<8;k++)// ������ ���������������� �������������� �� ������ ���� ��������
		{
			for(int t=0;t<SIZE_BLOCK_DATA;t++)
				temp_left[t] = temp1[t];//��������� ������� �������� ����� ����� ��� �������� ���� ��������
			for(int i=0;i<SIZE_BLOCK_DATA;i++) //SX[C]
			{
				temp1[i] ^= CONSTANTS_C[8*j*SIZE_BLOCK_DATA + k*SIZE_BLOCK_DATA + i];// ��������� �����
				temp1[i] = S_TABLE[temp1[i]];// �������������� ����� �� ������� �����
			}
			//L-��������������
			for(int k=16;k<32;k++)// �������� � ������� ����� "�-�� ������" ������� ��������� ����������
			{
				temp_array[k] = temp1[k-16];
			}
			shift_position = 0x00;
			for(int k=0;k<16;k++)
			{
				unsigned char l = TABLE_MULTIPLY_148[temp_array[16-shift_position]] ^
							  TABLE_MULTIPLY_32[temp_array[16-shift_position+1]] ^
							  TABLE_MULTIPLY_133[temp_array[16-shift_position+2]] ^	
							  TABLE_MULTIPLY_16[temp_array[16-shift_position+3]] ^
							  TABLE_MULTIPLY_194[temp_array[16-shift_position+4]] ^
							  TABLE_MULTIPLY_192[temp_array[16-shift_position+5]] ^
							  temp_array[16-shift_position+6] ^ 
							  TABLE_MULTIPLY_251[temp_array[16-shift_position+7]] ^
							  temp_array[16-shift_position+8] ^
							  TABLE_MULTIPLY_192[temp_array[16-shift_position+9]] ^
							  TABLE_MULTIPLY_194[temp_array[16-shift_position+10]] ^
							  TABLE_MULTIPLY_16[temp_array[16-shift_position+11]] ^
							  TABLE_MULTIPLY_133[temp_array[16-shift_position+12]] ^
							  TABLE_MULTIPLY_32[temp_array[16-shift_position+13]] ^
							  TABLE_MULTIPLY_148[temp_array[16-shift_position+14]] ^
							  temp_array[16-shift_position+15];
				shift_position++;// �������� ������ � ������� ������
				temp_array[16-shift_position] = l;// �������� ������� ������ ����������� l-��������������
			}
			// L-�����������
			for(int i=0;i<SIZE_BLOCK_DATA;i++)
				temp1[i] = temp_array[i];
			// ��������� ������ �������� �� �����
			for(int t=0;t<SIZE_BLOCK_DATA;t++)
			{
				temp1[t] ^= temp2[t];
				temp2[t] = temp_left[t];
			}			
			
		}
		for(int i=0;i<SIZE_BLOCK_DATA;i++)// �������� ���������� ��������� � ������� ������
		{
			expansion_key_array[2*(j+1)*SIZE_BLOCK_DATA + i] = temp1[i];
			expansion_key_array[2*(j+1)*SIZE_BLOCK_DATA + SIZE_BLOCK_DATA + i] = temp2[i];
		}
	}
}

void create_multiply_table()
{
	for(int j=0;j<256;j++)
	{
		TABLE_MULTIPLY_148[j] = myltiply_in_field_8(148,j);
		TABLE_MULTIPLY_32[j]  = myltiply_in_field_8(32,j);
		TABLE_MULTIPLY_133[j] = myltiply_in_field_8(133,j);
		TABLE_MULTIPLY_16[j]  = myltiply_in_field_8(16,j);
		TABLE_MULTIPLY_194[j] = myltiply_in_field_8(194,j);
		TABLE_MULTIPLY_192[j] = myltiply_in_field_8(192,j);
		TABLE_MULTIPLY_251[j] = myltiply_in_field_8(251,j);
	}
}

void calculation_constants()
{
	unsigned char temp[SIZE_BLOCK_DATA];
	unsigned char temp_array[32];
	unsigned char shift_position = 0x00;
	for(int j=0;j<32;j++)
	{
		for(int k=0;k<SIZE_BLOCK_DATA;k++)
			temp[k] = 0x00;
		temp[SIZE_BLOCK_DATA-1] = j+1;

		//L-��������������
		for(int k=16;k<32;k++)// �������� � ������� ����� "�-�� ������" ������� ��������� ����������
		{
			temp_array[k] = temp[k-16];
		}
		shift_position = 0x00;
		for(int k=0;k<16;k++)
		{
			unsigned char l = TABLE_MULTIPLY_148[temp_array[16-shift_position]] ^
						  TABLE_MULTIPLY_32[temp_array[16-shift_position+1]] ^
						  TABLE_MULTIPLY_133[temp_array[16-shift_position+2]] ^	
						  TABLE_MULTIPLY_16[temp_array[16-shift_position+3]] ^
						  TABLE_MULTIPLY_194[temp_array[16-shift_position+4]] ^
						  TABLE_MULTIPLY_192[temp_array[16-shift_position+5]] ^
						  temp_array[16-shift_position+6] ^ 
						  TABLE_MULTIPLY_251[temp_array[16-shift_position+7]] ^
						  temp_array[16-shift_position+8] ^
						  TABLE_MULTIPLY_192[temp_array[16-shift_position+9]] ^
						  TABLE_MULTIPLY_194[temp_array[16-shift_position+10]] ^
						  TABLE_MULTIPLY_16[temp_array[16-shift_position+11]] ^
						  TABLE_MULTIPLY_133[temp_array[16-shift_position+12]] ^
						  TABLE_MULTIPLY_32[temp_array[16-shift_position+13]] ^
						  TABLE_MULTIPLY_148[temp_array[16-shift_position+14]] ^
						  temp_array[16-shift_position+15];
			shift_position++;// �������� ������ � ������� ������
			temp_array[16-shift_position] = l;// �������� ������� ������ ����������� l-��������������
		}
		for(int k=0;k<SIZE_BLOCK_DATA;k++)
			CONSTANTS_C[j*SIZE_BLOCK_DATA+k] = temp_array[k];
	}
}

unsigned char myltiply_in_field_8(unsigned char a,unsigned char b)
{
	unsigned char res = 0;
	unsigned short temp = 0x0000;
	for(int j=0;j<8;j++)// ��������� �� �������� ����
	{
		if(((b>>(7-j))&0x01) == 1)
			temp ^= (a<<(7-j));
	}
	// ������� �� ������� ������������ �� ���������� ������� ����
	if(temp != 0)
	{
		unsigned char t = 15;// ����� �������� ��������� t - ����� �������� ���������� ���� temp - ������������
		while(t > 7)
		{
			t = find_elder_bit(temp);
			temp ^= (PRIMITIVE_POLINOM<<(t-8));// ������� ��� - ������� ��������� ��� � ���������� ��������
		}
		res = (unsigned char)temp;
	}
	else
		res = 0x00;
	return res;
}

unsigned char find_elder_bit(unsigned short a)// ����� ������ ��������� ��� (������� �� ��������� ��������, ���� � = 0�00 - ��� ��������� �����)
{
	unsigned char res = 0;
	for(int j=0;j<16;j++)
	{
		if(((a>>(15-j))&0x01) == 0x01)
		{
			res = 15-j;
			break;
		}
	}
	return res;
}

void L_transform(unsigned char* data)
{
	//L-��������������
	for(int k=0;k<16;k++)
	{
		unsigned char l = TABLE_MULTIPLY_148[data[0]] ^
					  TABLE_MULTIPLY_32[data[1]] ^
					  TABLE_MULTIPLY_133[data[2]] ^	
					  TABLE_MULTIPLY_16[data[3]] ^
					  TABLE_MULTIPLY_194[data[4]] ^
					  TABLE_MULTIPLY_192[data[5]] ^
					  data[6] ^ 
					  TABLE_MULTIPLY_251[data[7]] ^
					  data[8] ^
					  TABLE_MULTIPLY_192[data[9]] ^
					  TABLE_MULTIPLY_194[data[10]] ^
					  TABLE_MULTIPLY_16[data[11]] ^
					  TABLE_MULTIPLY_133[data[12]] ^
					  TABLE_MULTIPLY_32[data[13]] ^
					  TABLE_MULTIPLY_148[data[14]] ^
					  data[15];
		for(int k=15;k>0;k--)
			data[k] = data[k-1];
		data[0] = l;
	}
}

void generate_speed_table()
{
	unsigned char counter_for_speed_table[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
												 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char temp[16];
	for(int j=0;j<16;j++)
	{
		for(int x=0;x<16;x++)
			counter_for_speed_table[x] = 0x00;
		for(int k=0;k<256;k++)
		{
			if(k == 31)
				k = 31;
			counter_for_speed_table[j] = k;
			for(int t=0;t<16;t++)
				temp[t] = counter_for_speed_table[t];
			L_transform(temp);
			for(int i=0;i<16;i++)
				TABLE_SPEED[j*4096+k*16+i] = temp[i];	
		}
	}
	for(int t=0;t<16;t++)
	{
		for(int j=0;j<256;j++)
		{
			for(int k=0;k<16;k++)
				SPEED_TABLE_1[t*4096+ j*16 + k] = TABLE_SPEED[t*4096 + S_TABLE[j]*16 + k];
		}
	}
}

void speed_encryption(unsigned char* plain_text)
{
	unsigned char temp[16];
	int *pint_plain_text, *pint_temp, *pint_key;
	pint_plain_text = (int *)plain_text;
	pint_temp = (int *)temp;

	for(int j=0;j<NUMBER_ROUND;j++)
	{
		pint_key = (int *)(expansion_key_array + j*SIZE_BLOCK_DATA);
		for (int k = 0; k < SIZE_BLOCK_DATA/4; k++)
		{
			pint_plain_text[k] ^= pint_key[k];
		}

		for(int k=0;k<4;k++)
		{
			pint_temp[k] = 0;
			for(int i=0;i<16;i++)
			{
				pint_key = (int*)(SPEED_TABLE_1 + i * 4096 +plain_text[i] * 16);
				pint_temp[k] ^= pint_key[k];
			}				
		}

		for(int t=0;t<16;t++)
			plain_text[t] = temp[t];
	}

	pint_key = (int *)(expansion_key_array + 144);
	for(int j=0;j<4;j++)
	{
		pint_plain_text[j] ^= pint_key[j];
	}
}
