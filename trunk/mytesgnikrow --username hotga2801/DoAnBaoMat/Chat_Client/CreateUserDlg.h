#pragma once
#define WSAGETSELECTERROR(lParam) HIWORD(lParam)
#define WSAGETSELECTEVENT(lParam) LOWORD(lParam)
#define WM_SOCKET WM_USER+1
// CreateUserDlg dialog

class CreateUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CreateUserDlg)

public:
	CreateUserDlg(CWnd* pParent = NULL);   // standard constructor
	CreateUserDlg(SOCKET clientSock);
	virtual ~CreateUserDlg();
// Dialog Data
	enum { IDD = IDD_DIALOGCreateAcc };
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void SockMsg(WPARAM wParam, LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtoncancel();
	afx_msg void OnBnClickedButtoncreateuser();
	SOCKET m_socketClient;
	int m_msgType ;
	int m_msgLength;
};
