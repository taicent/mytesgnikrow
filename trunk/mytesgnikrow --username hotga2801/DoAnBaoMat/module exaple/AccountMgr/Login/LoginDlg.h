
// LoginDlg.h : header file
//

#pragma once


// CLoginDlg dialog
class CLoginDlg : public CDialogEx
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CListCtrl m_lstAccount;
	CListCtrl m_lstGroup;
	CListCtrl m_lstUserGroupList;

public:
	int m_iSelectedGroup;
	int m_iSelectedAccount;
	int m_iUserGroupIndex;

public:
	void InitListControlColumn();
	void LoadData();
	void LoadAccountList();
	void LoadGroupList();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnImpersonate();
	afx_msg void OnBnClickedBtnCreateuser();
	afx_msg void OnBnClickedBtnUpdateuser();
	afx_msg void OnNMClickLstaccount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLstgroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLstUsergroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnAddgroup();
	afx_msg void OnBnClickedBtnRemovegroup();
	afx_msg void OnBnClickedBtnCreategroup();
	afx_msg void OnBnClickedBtnRevert();
};
