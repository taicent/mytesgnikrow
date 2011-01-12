// EnDecryptStringClass.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "CryptString.h"

int main(int argc, char* argv[])
{
	// Init the crypto class
	CCryptString oCrypt;

	// input password
	printf("Input password: ");
	TCHAR szPassword[32] = _T("");
	_getts(szPassword);

	// The Key: has to be the same for encrypt and decryption!
	//TCHAR szKey[] = _T("Mz6@a0i*");
	TCHAR szKey[] = _T("georg");

	// encrpyt password
	TCHAR szEncryptPwd[32] = _T("");
	oCrypt.EncryptString(szPassword,szEncryptPwd,szKey);
	printf("The encrpted pasword is %hs\r\n", szEncryptPwd);

	// decrypt password
	TCHAR szPasswordDecrypted[32] = _T("");
	oCrypt.DecryptString(szEncryptPwd,szPasswordDecrypted,szKey);
	printf("The decrypted pasword is %s\r\n", szPasswordDecrypted);

	// finish
	printf("Press enter for end the program!\n");
	char cIn[100];
	gets(cIn);

	return 0;

	return 0;
}

