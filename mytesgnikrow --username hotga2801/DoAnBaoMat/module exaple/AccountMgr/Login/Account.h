#pragma once


#include <vector>

class Account
{
public:
	Account();
	~Account();
	
	CString m_StrUserName;
	CString m_StrPassword;
	CString m_StrFullName;
	CString m_StrDescription;
	std::vector<CString> m_ArrGroup;
	int iType;
};