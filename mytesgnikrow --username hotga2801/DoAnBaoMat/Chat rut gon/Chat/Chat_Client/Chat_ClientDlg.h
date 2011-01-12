// Chat_ClientDlg.h : header file
//

#if !defined(AFX_CHAT_CLIENTDLG_H__2805D4A7_5319_4C14_BB94_D6631C612388__INCLUDED_)
#define AFX_CHAT_CLIENTDLG_H__2805D4A7_5319_4C14_BB94_D6631C612388__INCLUDED_

#include "afxwin.h"
#define LISTEN_PORT 17000

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChat_ClientDlg dialog

class CChat_ClientDlg : public CDialog
{
// Construction
public:
	CString m_User;
	int m_msgType;
	char m_DataBuff[1024];
	int m_msgLength;
	void SockMsg(WPARAM wParam, LPARAM lParam);
	SOCKET m_socketClient;
	CChat_ClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChat_ClientDlg)
	enum { IDD = IDD_CHAT_CLIENT_DIALOG };
	CString	m_CurrentMessage;
	CString	m_AllMessage;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChat_ClientDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChat_ClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSendMessage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listFriend;
	afx_msg void OnLbnDblclkLfriend();
	afx_msg void OnLbnSelchangeLfriend();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAT_CLIENTDLG_H__2805D4A7_5319_4C14_BB94_D6631C612388__INCLUDED_)
