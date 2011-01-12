// ServerDlg.h : header file
//

#define PORT 25000
#define WM_SOCKET WM_USER+1
using namespace std;
#include <list>
#include <fstream>
#include<iostream>
using namespace std;
#pragma once
#include "afxwin.h"

#pragma once

struct Client {
	SOCKET socket;
	CString name;
	int msgType;
	int buffLength;
};

// CServerDlg dialog
class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
private:
	SOCKET sockServer;
	SOCKET sockClient;
	struct sockaddr_in serverAddress;
	CString m_message;
	Client clients;
	list<Client> m_lstclients;
	bool bClosed;
public:
	void BroadcasttoAllClients( list<Client>::iterator i,char * buffer, int bufferSize );
	void BroadcasttoAllClientsExceptNoname(list<Client>::iterator i,char * buffer, int bufferSize );
	void CheckNameOfClients(CString msg, list<Client>::iterator i,char * buffer, int bufferSize );
	void SendToClients(CString name, SOCKET sockClient, CString type,char * buffer, int bufferSize);
	SOCKET TimClient(CString User);
	CString TachChuoi(CString msg,int i,int j);
	CString TachTen(CString msg,int start);
	afx_msg void OnBnClickedBtnStartserver();
	afx_msg void OnBnClickedBtnStopserver();
private:
	CEdit m_msgCtrl;
	CString m_strMsg;
	CListBox m_lstFriends;
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombo1();
};
