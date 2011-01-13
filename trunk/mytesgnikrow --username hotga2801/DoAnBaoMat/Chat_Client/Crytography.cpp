#include "stdafx.h"
#include "Crytography.h"




Crytography::~Crytography(void)
{
}

CString Crytography ::Encrypt(CString mes)
{
		
		CString m_cipher;
		unsigned long length = mes.GetLength() +1;
		unsigned char * cipherBlock = (unsigned char*)malloc(length);
		memset(cipherBlock, 0, length);
		memcpy(cipherBlock, mes, length -1);	

		if (!CryptEncrypt(hKey, 0, TRUE, 0, cipherBlock, &length, length))
		{
			//dwResult = GetLastError();
			AfxMessageBox("Error CryptEncrypt() failed.", MB_OK);
			return "";
		}

		m_cipher = cipherBlock;
		//m_clear = "";

		free(cipherBlock);
		//
		return m_cipher;
}

void Crytography:: init()
{
	hProv   = 0;
   hKey     = 0;
     hXchgKey = 0;
    hHash   = 0;
	//status = FALSE;
	//_szPassword;
	//_szPassword = "khoa";
	CString Loi="";
	if(!CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0)) {
       // printf("Error %x during CryptAcquireContext!\n", GetLastError());
		AfxMessageBox("Error during CryptAcquireContext!",MB_OK);
		return;
    }
	 if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
           // printf("Error %x during CryptCreateHash!\n", GetLastError());
			//MessageBox("Error %x during CryptCreateHash!\n"+GetLastError());
		 AfxMessageBox("Error during CryptCreateHash!",MB_OK);
			return;
            //goto done;
        }

        // Hash in the password data.
        if(!CryptHashData(hHash, (const unsigned char *)_szPassword, (DWORD)strlen(_szPassword), 0)) {
			//MessageBox("Error %x during CryptHashData!\n"+GetLastError());
			AfxMessageBox("Error during CryptHashData!",MB_OK);
			return;
            //printf("Error %x during CryptHashData!\n", GetLastError());
            //goto done;
        }

        // Derive a session key from the hash object.
		if(_typeOfCrytography=="RC2")
		{
			if(!CryptDeriveKey(hProv, ENCRYPT_ALGORITHM1, hHash, KEYLENGTH, &hKey)) {
				//MessageBox("Error %x during CryptDeriveKey!\n"+GetLastError());
				AfxMessageBox("Error during CryptDeriveKey!",MB_OK);
				return;
			   // printf("Error %x during CryptDeriveKey!\n", GetLastError());
			   // goto done;
			}
		}
		else//RC4
		{
			if(!CryptDeriveKey(hProv, ENCRYPT_ALGORITHM2, hHash, KEYLENGTH, &hKey)) {
				//MessageBox("Error %x during CryptDeriveKey!\n"+GetLastError());
				AfxMessageBox("Error during CryptDeriveKey!",MB_OK);
				return;
			   // printf("Error %x during CryptDeriveKey!\n", GetLastError());
			   // goto done;
			}
		}
        // Destroy the hash object.
        CryptDestroyHash(hHash);
        hHash = 0;
}
CString Crytography ::Decrypt(CString mes)
{
	
	 
		//
		CString m_clear;
		unsigned long length = mes.GetLength() +1;
	unsigned char * cipherBlock = (unsigned char*)malloc(length);
	memset(cipherBlock, 0, length);
	memcpy(cipherBlock, mes, length -1);	

	if (!CryptDecrypt(hKey, 0, TRUE, 0, cipherBlock, &length))
	{
		//dwResult = GetLastError();
		AfxMessageBox("Error CryptDecrypt() failed.", MB_OK);
		return "";
	}
	
	m_clear = cipherBlock;
	
		//m_clear = "";

		free(cipherBlock);
		//
		return m_clear;
}

CString Crytography ::outputString(CString str , int toDo ){
	this->init();
	switch (toDo)
	{
		case ENCRYPT:				
				return this->Encrypt(str);// Tra ve chuoi ma hoa			
				break;
		case DECRYPT:
				return this->Decrypt(str);//Tra ve chuoi giai ma
				break;
	}

}