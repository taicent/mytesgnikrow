#include "stdafx.h"
#include "Account.h"

Account::Account()
{
	m_StrUserName = "";
	m_StrPassword = "";
	m_StrFullName = "";
	m_StrDescription = "";
	iType = 1;
}

Account::~Account()
{
}