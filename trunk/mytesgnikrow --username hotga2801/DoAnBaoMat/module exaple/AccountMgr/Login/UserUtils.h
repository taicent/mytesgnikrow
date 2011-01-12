#pragma once

#include <string>
#include <vector>
#include "Group.h"
#include "Account.h"
#pragma comment(lib, "Netapi32.lib")

class UserUtilities
{
public:
	UserUtilities();
	~UserUtilities();

public:
	//Account management
	static std::vector<Account*> m_lstAccount;
	static bool CreateAccount(Account* account);
	static bool DeleteAccount(Account* account);
	static std::vector<Account*> GetAccountList();
	static bool ChangeAccountInfo(Account* account);

public:
	//Group management
	static std::vector<Group*> m_lstGroups;
	static bool CreateGroup(Group* account);
	static bool DeleteGroup(Group* account);
	static std::vector<Group*> GetGroupList();
	static std::vector<Group*> GetUserGroupList(CString strUserName);
	static bool ChangeGroupInfo(CString strGroupName, Group* account);
	static bool AddUserToGroup(CString strUserName, CString strGroupName);
	static bool RemoveUserToGroup(CString strUserName, CString strGroupName);

public:
	//Authentication
	static bool LoginUser(CString userName, CString userPass, HANDLE &hToken);
	static bool ImpersonateLoginUser(HANDLE hToken);
	static bool RevertToMainUser();
};