#if !defined(AFX_LOGINDIALOG_H__AE81BC76_EAD7_44D6_B03F_C6F4DDF04DFC__INCLUDED_)
#define AFX_LOGINDIALOG_H__AE81BC76_EAD7_44D6_B03F_C6F4DDF04DFC__INCLUDED_

#if _MSC_VER > 1000
#define WM_SOCKET WM_USER+1
#define WM_SHARE_FILE WM_USER+2

#define WSAGETSELECTERROR(lParam) HIWORD(lParam)
#define WSAGETSELECTEVENT(lParam) LOWORD(lParam)

#pragma once
#endif // _MSC_VER > 1000
// LoginDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LoginDialog dialog

class LoginDialog : public CDialog
{
// Construction
public:
	int m_msgLength;
	int m_msgType;
	void SockMsg(WPARAM wParam, LPARAM lParam);
	SOCKET m_socketClient;
	SOCKET ClientSock;
	bool doConnect();
	bool doCheckData();
	LoginDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LoginDialog)
	enum { IDD = IDD_LoginDlg };
	CIPAddressCtrl	m_ServerAddress;
	CString	m_UserAcount;
	UINT	m_Port;
	CString m_Passwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LoginDialog)
	public:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LoginDialog)
	afx_msg void OnExit();
	afx_msg void OnLogin();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCreateacc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__AE81BC76_EAD7_44D6_B03F_C6F4DDF04DFC__INCLUDED_)
