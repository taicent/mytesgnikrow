// ThemEventSource.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "afx.h"
#include "ServerLog.h"
#include <vector>


#define BUFFER_SIZE	2048
bool ThemEventSource()
{
    // DLL Chua event messages (descriptions).
	LPCTSTR dllName = _T("%systemroot%\\SYSTEM\\ServerLog.dll"); 

    // Con So categories cho event source.
    DWORD dwCategoryNum = 1;
   
   HKEY hk; 
   DWORD dwData, dwDisp; 
   TCHAR szBuf[58]; 
   size_t cchSize = MAX_PATH;

   // Tao ra event source Nhu mot subkey cua log. 
   strcpy(szBuf, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\ServerLog"));
 
   if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, szBuf, 
          0, NULL, REG_OPTION_NON_VOLATILE,
          KEY_WRITE, NULL, &hk, &dwDisp)) 
   {
      return 0;
   }
 
   // Dat ten cua message file. 
   if (RegSetValueEx(hk,             // subkey handle 
          _T("EventMessageFile"),        // value name 
          0,                         // must be zero 
          REG_EXPAND_SZ,             // value type 
          (LPBYTE) dllName,          // pointer to value data 
          (DWORD) (strlen((const char*)dllName)+1)*sizeof(TCHAR))) // data size
   { 
      RegCloseKey(hk); 
      return 0;
   }
 
   // Dat loai event ho tro 
   dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | 
        EVENTLOG_INFORMATION_TYPE; 
 
   if (RegSetValueEx(hk,      // subkey handle 
           _T("TypesSupported"),  // value name 
           0,                 // must be zero 
           REG_DWORD,         // value type 
           (LPBYTE) &dwData,  // pointer to value data 
           sizeof(DWORD)))    // length of value data 
   {
      RegCloseKey(hk); 
      return 0;
   }
 
   // Dat category message file vaf number of categories.

   if (RegSetValueEx(hk,              // subkey handle 
           _T("CategoryMessageFile"),     // value name 
           0,                         // must be zero 
           REG_EXPAND_SZ,             // value type 
           (LPBYTE) dllName,          // pointer to value data 
           (DWORD) (lstrlen((const char*)dllName)+1)*sizeof(TCHAR))) // data size
   {
      RegCloseKey(hk); 
      return 0;
   }
 
   if (RegSetValueEx(hk,            // subkey handle 
           _T("CategoryCount"),         // value name 
           0,                       // must be zero 
           REG_DWORD,               // value type 
           (LPBYTE) &dwCategoryNum, // pointer to value data 
           sizeof(DWORD)))          // length of value data 
   {
      RegCloseKey(hk); 
      return 0;
   }

   RegCloseKey(hk); 
   return 1;//OK het
}

bool user2sid(CString csUsername, PSID Sid)
{
	UCHAR buffer2[2048];
	UCHAR buffer3[4];
	DWORD length = 900;

	LPCTSTR lpAccountName = csUsername;    // address of string for account name
	LPDWORD cbSid = &length;            // address of size sid
	LPTSTR ReferencedDomainName = (LPTSTR)buffer2;      // address of string for referenced domain
	LPDWORD cbReferencedDomainName = &length;   // address of size domain string
	PSID_NAME_USE peUse = (PSID_NAME_USE)buffer3;         // address of structure for SID type

	if (LookupAccountName(NULL, lpAccountName, Sid, cbSid,
                            ReferencedDomainName, cbReferencedDomainName,
                            peUse))
	{
		return 1;
	}
	
	return 0;
}

bool sid2user(PSID lpSid, CString &csUsername)
{
     UCHAR szName[1024];
     UCHAR szDomain[1024];
     SID_NAME_USE snu;
     DWORD cbName = 256;
     DWORD cbDomain = 256;

     if (LookupAccountSid(NULL, lpSid, (char*)szName, &cbName, 
			(char*)szDomain, &cbDomain, &snu))
	{
        // Return the user's name.
		csUsername = szName;
		return true;
    }

	return false;
}

bool WriteEventMessage(CString csUserName, WORD wType, DWORD dwEventID, LPCTSTR szMsg)
{
	CString sourceName = "ServerLog";
	HANDLE h; //Handle event
	LPCSTR csData = "Thong tin log file cua ung dung";
	DWORD dwDataSize = 13;
	PSID sidUser;//SID cua user
	UCHAR buffer1[2048];
	sidUser = buffer1;
	user2sid(csUserName, sidUser);//chuyen qua sid de dua vao ham ReportEvent

    // Dang ky mot handle cho event log.
    h = RegisterEventSource(NULL,  // local computer. 
            sourceName);           // Event source name. 
    if (h == NULL) 
    {
        return 0;
    }

    // Report the event. 
      if (!ReportEvent(h,           // Event log handle. 
            wType,					// Event type. 
            2,						// Event category.  
            dwEventID,            // Event identifier. 
            sidUser,                 // No user security identifier. 
            1,						// Number of substitution strings. 
            dwDataSize,                    // No data. 
            (const char**)&szMsg,               // Pointer to strings. 
            (void*)csData))                // No data. 
	{
		return 0;//Loi, khong tao duoc event
    }
 
    DeregisterEventSource(h); 
	return true;
}

CString GetDescriptionLog(EVENTLOGRECORD pevlr)
{
	char lpMsgBuf1[BUFFER_SIZE];
	CString csDescription;
	CString dllName = L"C:\\WINDOWS\\SYSTEM\\ServerLog.dll";
   
    HANDLE ghResDll;

	//Load dll
	ghResDll =  LoadLibrary(dllName);
	
    // Lay ve chuoi description. -------------------------
    FormatMessage(  FORMAT_MESSAGE_FROM_HMODULE, // Format of message
                    ghResDll,                    // Handle to the DLL file
                    pevlr.EventID,              // Event message identifier
                    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                    (LPTSTR) &lpMsgBuf1,         // Buffer that contains message
                    BUFFER_SIZE,                 // Size of buffer
                    NULL);                       // Array of insert values
	csDescription = lpMsgBuf1;

	return csDescription;
}

std::vector<CString> ReadEventMessageList()
{
	std::vector<CString>arrLog;
	CString csOneLog;

	// Ten cua event log.
    CString logName = L"ServerLog";  
	CString sourceName = L"ServerLog"; 
	CString dllName = L"C:\\WINDOWS\\SYSTEM\\ServerLog.dll";
   
    HANDLE h,  ghResDll;
	DWORD dwMessageID = 1014; 
    EVENTLOGRECORD *pevlr;
    BYTE bBuffer[BUFFER_SIZE];
    DWORD dwRead, dwNeeded, dwThisRecord;
    LPCTSTR lpSourceName;

    // buoc1: Mo event log.
    h = OpenEventLog( NULL,     // local computer.
        logName);
    if (h == NULL)
    {
        return arrLog;
    }
    
    // Buoc 2: khoi tao event record buffer.
    pevlr = (EVENTLOGRECORD *) &bBuffer;

	//Buoc 3: Load dll
	ghResDll =  LoadLibrary(dllName);

	//Buoc 4: Get the record number of the oldest event log record. 
    GetOldestEventLogRecord(h, &dwThisRecord);

    // Step 5: ---------------------------------------------------------
    //Doc tu dau den cuoi
    while (ReadEventLog(h,                // Event log handle
        EVENTLOG_FORWARDS_READ |          // Reads forward
        EVENTLOG_SEQUENTIAL_READ,         // Sequential read
        0,                                // Ignored for sequential read
        pevlr,                            // Pointer to buffer
        BUFFER_SIZE,                      // Size of buffer
        &dwRead,                          // Number of bytes read
        &dwNeeded))                       // Bytes in the next record
    {
        while (dwRead > 0)
        {
            // Lay ve event source name.
            lpSourceName = (LPCTSTR) ((LPBYTE) pevlr + sizeof(EVENTLOGRECORD)); 
			
            if ((lstrcmp(lpSourceName,sourceName) == 0))
			{
				//them
				PSID lpSid = (PSID)((LPBYTE) pevlr + pevlr->UserSidOffset);
				CString csUsername;
				sid2user(lpSid, csUsername);

				csOneLog = GetDescriptionLog(*pevlr);
				arrLog.push_back(csOneLog);
				csOneLog = "";
            }
         
            dwRead -= pevlr->Length;
            pevlr = (EVENTLOGRECORD *) ((LPBYTE) pevlr + pevlr->Length);
        }

        pevlr = (EVENTLOGRECORD *) bBuffer;
    }
     
    // Buoc 7: Dong event log.
    CloseEventLog(h);
	
	return arrLog;
}


int main(int argc, char* argv[])
{
	bool bKq = ThemEventSource();

	if(bKq)
		printf("Them event source vao registry thanh cong\n");
	else
		printf("Them event source vao registry khong thanh cong\n");

	bool res = WriteEventMessage("manhnn", EVENTLOG_ERROR_TYPE, EV_START_SUCCESS, "");

	std::vector<CString> arrLog = ReadEventMessageList();
	for(int i = 0;i < arrLog.size();i++)
	{
		printf(arrLog[i].GetBuffer());
	}

	return 0;
}
