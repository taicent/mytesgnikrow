#pragma once

#include <vector>

class Account
{
public:
	Account(CString username , CString passwd , CString fullName , CString description){
		m_StrUserName = username;
		m_StrPassword = passwd;
		m_StrFullName = fullName;
		m_StrDescription = description;
	}
	Account(){
		m_StrUserName = "";
		m_StrPassword = "";
		m_StrFullName = "";
		m_StrDescription = "";
	}

	~Account(void);
	CString m_StrUserName;
	CString m_StrPassword;
	CString m_StrFullName;
	CString m_StrDescription;
	int iType;
};
