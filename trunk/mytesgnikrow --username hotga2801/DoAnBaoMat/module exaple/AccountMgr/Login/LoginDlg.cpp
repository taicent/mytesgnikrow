
// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Login.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "UserUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoginDlg dialog
HANDLE hToken;

CString ToCString(int iNum)
{
	char buffer[100];
	char* str = itoa(iNum, buffer, 10);
	return CString(str);
}

int ToInt(CString str)
{
	return atoi((const char*)str.GetBuffer(str.GetLength()));
}

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTACCOUNT, m_lstAccount);
	DDX_Control(pDX, IDC_LSTGROUP, m_lstGroup);
	DDX_Control(pDX, IDC_LST_USERGROUPLIST, m_lstUserGroupList);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_IMPERSONATE, &CLoginDlg::OnBnClickedBtnImpersonate)
	ON_BN_CLICKED(IDC_BTN_CREATEUSER, &CLoginDlg::OnBnClickedBtnCreateuser)
	ON_BN_CLICKED(IDC_BTN_UPDATEUSER, &CLoginDlg::OnBnClickedBtnUpdateuser)
	ON_NOTIFY(NM_CLICK, IDC_LSTACCOUNT, &CLoginDlg::OnNMClickLstaccount)
	ON_NOTIFY(NM_CLICK, IDC_LSTGROUP, &CLoginDlg::OnNMClickLstgroup)
	ON_NOTIFY(NM_CLICK, IDC_LST_USERGROUPLIST, &CLoginDlg::OnNMClickLstUsergroup)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CLoginDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_ADDGROUP, &CLoginDlg::OnBnClickedBtnAddgroup)
	ON_BN_CLICKED(IDC_BTN_REMOVEGROUP, &CLoginDlg::OnBnClickedBtnRemovegroup)
	ON_BN_CLICKED(IDC_BTN_CREATEGROUP, &CLoginDlg::OnBnClickedBtnCreategroup)
	ON_BN_CLICKED(IDC_BTN_REVERT, &CLoginDlg::OnBnClickedBtnRevert)
END_MESSAGE_MAP()


// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LoadData();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLoginDlg::OnBnClickedBtnLogin()
{
	// TODO: Add your control notification handler code here
	CString s_userName;
	CString s_userPass;
	GetDlgItemText(IDC_TXT_USER, s_userName);
	GetDlgItemText(IDC_TXT_PASS, s_userPass);

	if(s_userName != "" && s_userPass != "")
	{
		BOOL res = UserUtilities::LoginUser(s_userName, s_userPass, hToken);
		if(res == TRUE)
		{
			::MessageBox(NULL, TEXT("Login successfully!"), TEXT("Alert"), 0);
		}
		else 
		{
			::MessageBox(NULL, TEXT("Login failed!"), TEXT("Alert"), 0);
		}
	}
	else
	{
		::MessageBox(NULL, TEXT("Please input user name or password."), TEXT("Error"), 0);
	}
}


void CLoginDlg::OnBnClickedBtnImpersonate()
{
	// TODO: Add your control notification handler code here
	BOOL res = UserUtilities::ImpersonateLoginUser(hToken);
	if(res == TRUE)
	{
		::MessageBox(NULL, TEXT("Impersonate logon user successfully!"), TEXT("Alert"), 0);
	}
	else 
	{
		::MessageBox(NULL, TEXT("Impersonate logon user failed!"), TEXT("Alert"), 0);
	}
}


void CLoginDlg::OnBnClickedBtnCreateuser()
{
	// TODO: Add your control notification handler code here
	CString s_userName;
	CString s_userPass;
	CString s_userFullName;
	CString s_userDescription;
	CString s_userGroup;

	GetDlgItemText(IDC_TXT_USERNAME, s_userName);
	GetDlgItemText(IDC_TXT_PASSWORD, s_userPass);
	GetDlgItemText(IDC_TXT_FULLNAME, s_userFullName);
	GetDlgItemText(IDC_TXT_DESCRIPTION, s_userDescription);

	Account* acc = new Account();
	acc->m_StrUserName = s_userName;
	acc->m_StrPassword = s_userPass;
	acc->m_StrFullName = s_userFullName;
	acc->m_StrDescription = s_userDescription;
	acc->iType = 1;
	BOOL res = UserUtilities::CreateAccount(acc);

	if(res == TRUE)
	{
		int length = m_lstAccount.GetItemCount();
		m_lstAccount.InsertItem(length, NULL);
		m_lstAccount.SetItemText(length, 0, ToCString(length + 1));
		m_lstAccount.SetItemText(length, 1, acc->m_StrUserName);
		m_lstAccount.SetItemText(length, 2, acc->m_StrFullName);
		m_lstAccount.SetItemText(length, 3, acc->m_StrDescription);
		::MessageBox(NULL, TEXT("Create account successfully!"), TEXT("Alert"), 0);

		if(acc != NULL)
		{
			delete acc;
			acc = NULL;
		}

		SetDlgItemText(IDC_TXT_USERNAME, L"");
		SetDlgItemText(IDC_TXT_PASSWORD, L"");
		SetDlgItemText(IDC_TXT_FULLNAME, L"");
		SetDlgItemText(IDC_TXT_DESCRIPTION, L"");
	}
	else 
	{
		::MessageBox(NULL, TEXT("Create account failed!"), TEXT("Alert"), 0);
	}
}

void CLoginDlg::InitListControlColumn()
{
	ListView_SetExtendedListViewStyle(m_lstAccount, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstAccount.InsertColumn(0, _T("STT"), LVCFMT_LEFT, 50);
	m_lstAccount.InsertColumn(1, _T("Username"), LVCFMT_LEFT, 200);
	m_lstAccount.InsertColumn(2, _T("Full Name"), LVCFMT_LEFT, 200);
	m_lstAccount.InsertColumn(3, _T("Description"), LVCFMT_LEFT, 200);

	ListView_SetExtendedListViewStyle(m_lstGroup, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstGroup.InsertColumn(0, _T("STT"), LVCFMT_LEFT, 50);
	m_lstGroup.InsertColumn(1, _T("Groupname"), LVCFMT_LEFT, 200);
	m_lstGroup.InsertColumn(2, _T("Description"), LVCFMT_LEFT, 200);

	ListView_SetExtendedListViewStyle(m_lstUserGroupList, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_lstUserGroupList.InsertColumn(0, _T("STT"), LVCFMT_LEFT, 50);
	m_lstUserGroupList.InsertColumn(1, _T("Groupname"), LVCFMT_LEFT, 200);
	m_lstUserGroupList.InsertColumn(2, _T("Description"), LVCFMT_LEFT, 200);
}

void CLoginDlg::LoadData()
{
	m_iSelectedGroup = -1;
	m_iSelectedAccount = -1;
	m_iUserGroupIndex = -1;

	InitListControlColumn();
	LoadAccountList();
	LoadGroupList();
}

void CLoginDlg::LoadAccountList()
{
	m_lstAccount.DeleteAllItems();
	std::vector<Account*> lstAccount = UserUtilities::GetAccountList();
	for(int i = 0;i < lstAccount.size();i++)
	{
		Account* acc = (Account*)lstAccount[i];
		m_lstAccount.InsertItem(i, NULL);
		m_lstAccount.SetItemText(i, 0, ToCString(i + 1));
		m_lstAccount.SetItemText(i, 1, acc->m_StrUserName);
		m_lstAccount.SetItemText(i, 2, acc->m_StrFullName);
		m_lstAccount.SetItemText(i, 3, acc->m_StrDescription);
	}
}

void CLoginDlg::LoadGroupList()
{
	m_lstGroup.DeleteAllItems();
	std::vector<Group*> lstGroup = UserUtilities::GetGroupList();
	for(int i = 0;i < lstGroup.size();i++)
	{
		Group* grp = (Group*)lstGroup[i];
		m_lstGroup.InsertItem(i, NULL);
		m_lstGroup.SetItemText(i, 0, ToCString(i + 1));
		m_lstGroup.SetItemText(i, 1, grp->m_StrGroupName);
		m_lstGroup.SetItemText(i, 2, grp->m_StrDescription);
	}
}

void CLoginDlg::OnBnClickedBtnUpdateuser()
{
	// TODO: Add your control notification handler code here
	CString s_userName;
	CString s_userPass;
	CString s_userFullName;
	CString s_userDescription;

	GetDlgItemText(IDC_TXT_USERNAME, s_userName);
	GetDlgItemText(IDC_TXT_PASSWORD, s_userPass);
	GetDlgItemText(IDC_TXT_FULLNAME, s_userFullName);
	GetDlgItemText(IDC_TXT_DESCRIPTION, s_userDescription);

	if(s_userName != "" && s_userPass != "")
	{
		Account* acc = new Account();
		acc->m_StrUserName = s_userName;
		acc->m_StrPassword = s_userPass;
		acc->m_StrFullName = s_userFullName;
		acc->m_StrDescription = s_userDescription;

		bool res = UserUtilities::ChangeAccountInfo(acc);
		if(res == true)
		{
			if(acc != NULL)
			{
				delete acc;
				acc = NULL;
			}
			::MessageBox(NULL, TEXT("Update user successfully!"), TEXT("Alert"), 0);
			LoadAccountList();
		}
		else
		{
			::MessageBox(NULL, TEXT("Update user failed!"), TEXT("Alert"), 0);
		}
	}
	else
	{
		::MessageBox(NULL, TEXT("Please input username or password!"), TEXT("Alert"), 0);
	}
}

void CLoginDlg::OnNMClickLstaccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNM = (NM_LISTVIEW*)pNMHDR;
	int nCount = m_lstAccount.GetItemCount();
    LVHITTESTINFO ht;
    ht.pt = pNM->ptAction;
    m_lstAccount.SubItemHitTest(&ht);

	if(ht.iItem >= 0 && ht.iItem < nCount)
	{
		m_iSelectedAccount = ht.iItem;
		SetDlgItemText(IDC_TXT_USERNAME, m_lstAccount.GetItemText(ht.iItem, 1));
		SetDlgItemText(IDC_TXT_FULLNAME, m_lstAccount.GetItemText(ht.iItem, 2));
		SetDlgItemText(IDC_TXT_DESCRIPTION, m_lstAccount.GetItemText(ht.iItem, 3));

		m_lstUserGroupList.DeleteAllItems();
		std::vector<Group*> lstGroup = UserUtilities::GetUserGroupList(m_lstAccount.GetItemText(ht.iItem, 1));
		for(int i = 0;i < lstGroup.size();i++)
		{
			m_lstUserGroupList.InsertItem(i, NULL);
			m_lstUserGroupList.SetItemText(i, 0, ToCString(i + 1));
			m_lstUserGroupList.SetItemText(i, 1, ((Group*)lstGroup[i])->m_StrGroupName);
			m_lstUserGroupList.SetItemText(i, 2, ((Group*)lstGroup[i])->m_StrDescription);
		}
	}
	else
	{
		m_iSelectedAccount = -1;
		SetDlgItemText(IDC_TXT_USERNAME, L"");
		SetDlgItemText(IDC_TXT_FULLNAME, L"");
		SetDlgItemText(IDC_TXT_DESCRIPTION, L"");
		m_lstUserGroupList.DeleteAllItems();
	}
	SetDlgItemText(IDC_TXT_PASSWORD, L"");
}

void CLoginDlg::OnNMClickLstgroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNM = (NM_LISTVIEW*)pNMHDR;
	int nCount = m_lstGroup.GetItemCount();
    LVHITTESTINFO ht;
    ht.pt = pNM->ptAction;
    m_lstGroup.SubItemHitTest(&ht);

	if(ht.iItem >= 0 && ht.iItem < nCount)
	{
		m_iSelectedGroup = ht.iItem;
		SetDlgItemText(IDC_TXT_GROUPNAME, m_lstGroup.GetItemText(m_iSelectedGroup, 1));
		SetDlgItemText(IDC_TXT_GROUPDESCRIPTION, m_lstGroup.GetItemText(m_iSelectedGroup, 2));
	}
	else
	{
		SetDlgItemText(IDC_TXT_GROUPNAME, L"");
		SetDlgItemText(IDC_TXT_GROUPDESCRIPTION, L"");
		m_iSelectedGroup = -1;
	}
}

void CLoginDlg::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	CString s_userName;
	CString s_userPass;
	CString s_userFullName;
	CString s_userDescription;

	GetDlgItemText(IDC_TXT_USERNAME, s_userName);
	GetDlgItemText(IDC_TXT_PASSWORD, s_userPass);
	GetDlgItemText(IDC_TXT_FULLNAME, s_userFullName);
	GetDlgItemText(IDC_TXT_DESCRIPTION, s_userDescription);
	if(s_userName != "")
	{
		Account* acc = new Account();
		acc->m_StrUserName = s_userName;
		acc->m_StrPassword = s_userPass;
		acc->m_StrFullName = s_userFullName;
		acc->m_StrDescription = s_userDescription;
		bool res = UserUtilities::DeleteAccount(acc);
		if(res == true)
		{
			if(acc != NULL)
			{
				delete acc;
				acc = NULL;
			}
			::MessageBox(NULL, TEXT("Delete user successfully!"), TEXT("Alert"), 0);
			SetDlgItemText(IDC_TXT_USERNAME, L"");
			SetDlgItemText(IDC_TXT_FULLNAME, L"");
			SetDlgItemText(IDC_TXT_DESCRIPTION, L"");
			SetDlgItemText(IDC_TXT_PASSWORD, L"");

			LoadAccountList();
			m_lstUserGroupList.DeleteAllItems();
		}
		else
		{
			::MessageBox(NULL, TEXT("Delete user failed!"), TEXT("Alert"), 0);
		}
	}
	else
	{
		::MessageBox(NULL, TEXT("Please choose user!"), TEXT("Alert"), 0);
	}
}

void CLoginDlg::OnBnClickedBtnAddgroup()
{
	// TODO: Add your control notification handler code here
	CString userName;
	CString groupName;
	CString description;

	if(m_iSelectedAccount >= 0)
	{
		userName = m_lstAccount.GetItemText(m_iSelectedAccount, 1);
	}
	else
	{
		::MessageBox(NULL, TEXT("Please choose user!"), TEXT("Alert"), 0);
		return;
	}

	if(m_iSelectedGroup >= 0)
	{
		groupName = m_lstGroup.GetItemText(m_iSelectedGroup, 1);
		description = m_lstGroup.GetItemText(m_iSelectedGroup, 2);
		int flag = 0;
		for(int i = 0;i < m_lstUserGroupList.GetItemCount();i++)
		{
			if(m_lstUserGroupList.GetItemText(i, 1) == groupName)
			{
				flag = 1;
				break;
			}
		}

		if(flag == 0)
		{
			UserUtilities::AddUserToGroup(userName, groupName);

			int index = m_lstUserGroupList.GetItemCount();
			m_lstUserGroupList.InsertItem(index, NULL);
			m_lstUserGroupList.SetItemText(index, 0, ToCString(index + 1));
			m_lstUserGroupList.SetItemText(index, 1, groupName);
			m_lstUserGroupList.SetItemText(index, 2, description);
		}
		else
		{
			::MessageBox(NULL, TEXT("User is added to group. Please choose difference group!"), TEXT("Alert"), 0);
		}
	}
	else
	{
		::MessageBox(NULL, TEXT("Please choose group!"), TEXT("Alert"), 0);
	}
}


void CLoginDlg::OnBnClickedBtnRemovegroup()
{
	// TODO: Add your control notification handler code here
	CString userName;
	CString groupName;

	if(m_iSelectedAccount >= 0)
	{
		userName = m_lstAccount.GetItemText(m_iSelectedAccount, 1);
	}
	else
	{
		::MessageBox(NULL, TEXT("Please choose user!"), TEXT("Alert"), 0);
		return;
	}

	if(m_iUserGroupIndex >= 0)
	{
		groupName = m_lstUserGroupList.GetItemText(m_iUserGroupIndex, 1);
		UserUtilities::RemoveUserToGroup(userName, groupName);
		m_lstUserGroupList.DeleteItem(m_iUserGroupIndex);
	}
	else
	{
		::MessageBox(NULL, TEXT("Please choose group!"), TEXT("Alert"), 0);
	}
}

void CLoginDlg::OnNMClickLstUsergroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNM = (NM_LISTVIEW*)pNMHDR;
	int nCount = m_lstUserGroupList.GetItemCount();
    LVHITTESTINFO ht;
    ht.pt = pNM->ptAction;
    m_lstUserGroupList.SubItemHitTest(&ht);

	if(ht.iItem >= 0 && ht.iItem < nCount)
	{
		m_iUserGroupIndex = ht.iItem;
	}
	else
	{
		m_iUserGroupIndex = -1;
	}
}

void CLoginDlg::OnBnClickedBtnCreategroup()
{
	// TODO: Add your control notification handler code here
	CString groupName;
	CString description;
	GetDlgItemText(IDC_TXT_GROUPNAME, groupName);
	GetDlgItemText(IDC_TXT_GROUPDESCRIPTION, description);

	if(groupName != "")
	{
		Group* grp = new Group();
		grp->m_StrGroupName = groupName;
		grp->m_StrDescription = description;
		bool res = UserUtilities::CreateGroup(grp);
		if(res == true)
		{
			::MessageBox(NULL, TEXT("Create group successfully!"), TEXT("Alert"), 0);
			int index = m_lstGroup.GetItemCount();
			m_lstGroup.InsertItem(index, NULL);
			m_lstGroup.SetItemText(index, 0, ToCString(index + 1));
			m_lstGroup.SetItemText(index, 1, groupName);
			m_lstGroup.SetItemText(index, 2, description);

			if(grp != NULL)
			{
				delete grp;
				grp = NULL;
			}

			SetDlgItemText(IDC_TXT_GROUPNAME, L"");
			SetDlgItemText(IDC_TXT_GROUPDESCRIPTION, L"");
		}
		else
		{
			::MessageBox(NULL, TEXT("Create group failed!"), TEXT("Alert"), 0);
		}
	}
	else
	{
		::MessageBox(NULL, TEXT("Please input group name!"), TEXT("Alert"), 0);
	}
}


void CLoginDlg::OnBnClickedBtnRevert()
{
	// TODO: Add your control notification handler code here
	bool res = UserUtilities::RevertToMainUser();
	if(res == true)
	{
		::MessageBox(NULL, TEXT("Revert successfully!"), TEXT("Alert"), 0);
	}
	else
	{
		::MessageBox(NULL, TEXT("Revert failed!"), TEXT("Alert"), 0);
	}
}
