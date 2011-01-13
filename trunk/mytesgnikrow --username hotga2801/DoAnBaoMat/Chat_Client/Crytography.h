#pragma once
#include "stdafx.h"
#include <wincrypt.h>
#define ENCRYPT_ALGORITHM1   CALG_RC2
   
#define ENCRYPT 1
#define DECRYPT 0
    
#define ENCRYPT_ALGORITHM2   CALG_RC4
 
#define KEYLENGTH 0x00800000


class Crytography
{
private :
	PCHAR _szPassword;
	CString _typeOfCrytography;
public:
	Crytography(CString typeOfCrytography /*loai ma hoa{RC2, RC4}*/, PCHAR password /*dung de tao key*/){
		_szPassword = password;
		_typeOfCrytography = typeOfCrytography;
	}
	~Crytography(void);
	CString Decrypt(CString mes); 
	CString Encrypt(CString mes);
	void init();
	HCRYPTPROV hProv   ;
    HCRYPTKEY hKey    ;
    HCRYPTKEY hXchgKey ;
    HCRYPTHASH hHash   ;	
	CString outputString(CString str , int toDo); 
};
