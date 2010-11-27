#ifndef _AES_H_
#define _AES_H_

// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4

void KeyExpansion(unsigned char Key[32], int Nk, int Nr);
void Cipher(unsigned char in[], unsigned char out[], int Nr);
void InvCipher(unsigned char in[], unsigned char out[], int Nr);


#endif