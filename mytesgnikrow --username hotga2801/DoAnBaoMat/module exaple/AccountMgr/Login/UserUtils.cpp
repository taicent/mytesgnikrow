#include "stdafx.h"
#include "UserUtils.h"
#include "Lm.h"

UserUtilities::UserUtilities()
{
}

UserUtilities::~UserUtilities()
{
}

//Authentication
bool UserUtilities::LoginUser(CString userName, CString userPass, HANDLE &hToken)
{
	BOOL res = LogonUser(userName, TEXT("."), userPass, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken);
	return res;
}

bool UserUtilities::ImpersonateLoginUser(HANDLE hToken)
{
	BOOL res = ImpersonateLoggedOnUser(hToken);
	return res;
}

bool UserUtilities::RevertToMainUser()
{
	return RevertToSelf();
}

//Group management
bool UserUtilities::CreateGroup(Group *group)
{
	LOCALGROUP_INFO_1* grp = new LOCALGROUP_INFO_1();
	grp->lgrpi1_name = group->m_StrGroupName.GetBuffer();
	grp->lgrpi1_comment = group->m_StrDescription.GetBuffer();
	int res = NetLocalGroupAdd(NULL, 1, (LPBYTE)grp, NULL);

	if(res == 0)
		return true;
	return false;
}

std::vector<Group*> UserUtilities::GetGroupList()
{
	std::vector<Group*> lstGroups;
	LOCALGROUP_INFO_1* grpInfo = NULL;
	LOCALGROUP_INFO_1* tempgrpInfo;
	DWORD entriesRead = 0;
	DWORD totalEntries = 0;
	DWORD resume = 0;

	int res = NetLocalGroupEnum(NULL, 1, (LPBYTE*)&grpInfo, -1, &entriesRead, &totalEntries, &resume);
	if(entriesRead > 0)
	{
		tempgrpInfo = grpInfo;
		for(int i = 0;i < entriesRead;i++)
		{
			Group* grp = new Group();
			grp->m_StrGroupName = tempgrpInfo->lgrpi1_name;
			grp->m_StrDescription = tempgrpInfo->lgrpi1_comment;
			lstGroups.push_back(grp);
			
			tempgrpInfo++;
		}
	}

	if(grpInfo)
	{
		NetApiBufferFree(grpInfo);
		grpInfo = NULL;
	}

	return lstGroups;
}

std::vector<Group*> UserUtilities::GetUserGroupList(CString strUserName)
{
	std::vector<Group*> lstUserGroups;
	LOCALGROUP_USERS_INFO_0 *grpInfo = NULL;
	LOCALGROUP_USERS_INFO_0 *tempgrpInfo = NULL;
	DWORD entriesRead = 0;
	DWORD totalEntries = 0;
	int res = NetUserGetLocalGroups(NULL, strUserName, 0, 0, (LPBYTE*)&grpInfo, MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries);
	if(entriesRead > 0)
	{
		tempgrpInfo = grpInfo;
		for(int i = 0;i < entriesRead;i++)
		{
			Group* grp = new Group();
			grp->m_StrGroupName = tempgrpInfo->lgrui0_name;
			lstUserGroups.push_back(grp);
			
			tempgrpInfo++;
		}
	}

	if(grpInfo)
	{
		NetApiBufferFree(grpInfo);
		grpInfo = NULL;
	}

	return lstUserGroups;
}

bool UserUtilities::DeleteGroup(Group* group)
{
	int res = NetLocalGroupDel(NULL, group->m_StrGroupName);
	if(res == 0)
		return true;
	return false;
}

bool UserUtilities::ChangeGroupInfo(CString strGroupName, Group* group)
{
	LOCALGROUP_INFO_1* groupInfo = new LOCALGROUP_INFO_1();
	groupInfo->lgrpi1_name = group->m_StrGroupName.GetBuffer();
	groupInfo->lgrpi1_comment = group->m_StrDescription.GetBuffer();
	int res = NetLocalGroupSetInfo(NULL, strGroupName, 1, (LPBYTE)groupInfo, NULL);
	if(res == 0)
		return true;
	return false;
}

//Account management
bool UserUtilities::CreateAccount(Account *account)
{
	USER_INFO_1 *userInfo = new USER_INFO_1();
	
	userInfo->usri1_name = account->m_StrUserName.GetBuffer();
	userInfo->usri1_password = account->m_StrPassword.GetBuffer();
	userInfo->usri1_priv = USER_PRIV_USER;
	userInfo->usri1_home_dir = NULL;
	userInfo->usri1_comment = account->m_StrDescription.GetBuffer();
	userInfo->usri1_flags = UF_SCRIPT;
	userInfo->usri1_script_path = NULL;

	DWORD errCode;
	DWORD res = NetUserAdd(NULL, 1, (LPBYTE)userInfo, &errCode);
	if(res == 0)
	{
		return true;
	}
	return false;
}

#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))

std::vector<Account*> UserUtilities::GetAccountList()
{
	std::vector<Account*> lstAccount;

	USER_INFO_2* user = NULL;
	USER_INFO_2* tempuser;

	DWORD level = 2;
	DWORD filter = 2;
	BYTE* iAccountNum;
	DWORD iEntriesRead = 0;
	DWORD iTotalEntries = 0;
	DWORD iResume = 0;

	int iRes = NetUserEnum(NULL, level, filter, (LPBYTE*)&user, -1, &iEntriesRead, &iTotalEntries, &iResume);
	tempuser = user;

	for(int i = 0;i < iEntriesRead;i++)
	{
		Account* acc = new Account();
		acc->m_StrUserName = tempuser->usri2_name;
		acc->m_StrFullName = tempuser->usri2_full_name;
		acc->m_StrDescription = tempuser->usri2_comment;
		acc->iType = tempuser->usri2_priv;
		
		lstAccount.push_back(acc);
		tempuser++;
	}

	if (user != NULL)
	{
		NetApiBufferFree(user);
		user = NULL;
	}
	
	return lstAccount;
}

bool UserUtilities::DeleteAccount(Account* account)
{
	DWORD res = NetUserDel(NULL, account->m_StrUserName);
	if(res == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

bool UserUtilities::ChangeAccountInfo(Account* account)
{
	USER_INFO_2 *userInfo = new USER_INFO_2();
	DWORD error = 0;
	DWORD level;
	DWORD res;

	int getRes = NetUserGetInfo(NULL, account->m_StrUserName, 2, (LPBYTE*)&userInfo);
	if(getRes != 0)
	{
		return false;
	}
	else
	{
		userInfo->usri2_full_name = account->m_StrFullName.GetBuffer();
		userInfo->usri2_password = account->m_StrPassword.GetBuffer();
		userInfo->usri2_comment = account->m_StrDescription.GetBuffer();

		res = NetUserSetInfo(NULL, account->m_StrUserName, 2, (LPBYTE)userInfo, &error);
		if(res == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool UserUtilities::AddUserToGroup(CString strUserName, CString strGroupName)
{
	LOCALGROUP_MEMBERS_INFO_3 *group = new LOCALGROUP_MEMBERS_INFO_3();
	group->lgrmi3_domainandname = strUserName.GetBuffer();	

	int res = NetLocalGroupAddMembers(NULL, strGroupName, 3, (LPBYTE)group, 1);
	if(res == 0)
	{
		return true;
	}
	return false;
}

bool UserUtilities::RemoveUserToGroup(CString strUserName, CString strGroupName)
{
	LOCALGROUP_MEMBERS_INFO_3 *group = new LOCALGROUP_MEMBERS_INFO_3();
	group->lgrmi3_domainandname = strUserName.GetBuffer();
	int res = NetLocalGroupDelMembers(NULL, strGroupName, 3, (LPBYTE)group, 1);
	if(res == 0)
	{
		return true;
	}
	return false;
}