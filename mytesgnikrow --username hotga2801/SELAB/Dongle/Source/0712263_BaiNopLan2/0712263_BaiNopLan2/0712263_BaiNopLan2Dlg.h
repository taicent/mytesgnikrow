// 0712263_BaiNopLan2Dlg.h : header file
//

#pragma once
#include "Word.h"
#include "Authentication.h"
#include "myaes.h"
/*
	Van Duy Thanh Long
	0712263
*/

// CMy0712263_BaiNopLan2Dlg dialog
#define AUTHENTICATION_CODE 1
#define CHECK_EXISTANCE_CODE 2

class CMy0712263_BaiNopLan2Dlg : public CDialog
{
// Construction
public:
	CMy0712263_BaiNopLan2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY0712263_BAINOPLAN2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString m_str;
	vector<TIENG> m_arrWord;
	CWnd *m_pWnd;
	CListBox *m_pList;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedExit();
public:
	afx_msg void OnBnClickedButton2();
	BOOL Authenticate();
	void SetOutReport();
	void GetInReport();
	void OnError();
	CAuthentication* authen;
	BOOL m_isRunning;
	int m_DeviceID;
	BYTE m_OutReport[256];
	int m_OutReportLen;
	BYTE m_InReport[256];
	int m_InReportLen;

	int m_Nr;
	int m_Nk;
	unsigned char m_Key[32];
	unsigned char m_Encryp[17];
	unsigned char m_Decryp[17];
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
