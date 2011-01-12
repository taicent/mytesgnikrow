// Chat_ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat_Client.h"
#include "Chat_ClientDlg.h"
#include "LoginDialog.h"
#include "Communication.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
// ham phu
void sendText(SOCKET sock,CString message);
void sendText_1(SOCKET sock,CString MessageHeader,char * Data, int DataSize);


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAbouNo message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChat_ClientDlg dialog

CChat_ClientDlg::CChat_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChat_ClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChat_ClientDlg)
	m_CurrentMessage = _T("");
	m_AllMessage = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChat_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChat_ClientDlg)
	DDX_Text(pDX, IDC_EDIT4, m_CurrentMessage);
	DDX_Text(pDX, IDC_EDIT2, m_AllMessage);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST2, m_listFriend);
}

BEGIN_MESSAGE_MAP(CChat_ClientDlg, CDialog)
	//{{AFX_MSG_MAP(CChat_ClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnSendMessage)
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_LFRIEND, &CChat_ClientDlg::OnLbnDblclkLfriend)
	ON_LBN_SELCHANGE(IDC_LFRIEND, &CChat_ClientDlg::OnLbnSelchangeLfriend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChat_ClientDlg message handlers

BOOL CChat_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChat_ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChat_ClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}


HCURSOR CChat_ClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CChat_ClientDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CChat_ClientDlg::OnOK() 
{	
	CDialog::OnOK();
}
// Mo File dc share ra // share file





void CChat_ClientDlg::OnSendMessage() 
{
	UpdateData();
	CString message;
	message +="CHPBL "+ m_User+ " : "+ m_CurrentMessage;
	sendText(m_socketClient,message);
	m_AllMessage+="Me : " +m_CurrentMessage+"\r\n";
	m_CurrentMessage="";
	UpdateData(FALSE);

	
}

BOOL CChat_ClientDlg::DestroyWindow() 
{
	EndConnect(m_socketClient);
	return CDialog::DestroyWindow();
}

LRESULT CChat_ClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	if (message == WM_CREATE)
	{

		
		LoginDialog loginDlg ;
		int result =loginDlg.DoModal();
		if (result == IDLOGIN)
		{
			CString strTitle = _T ("User Name  : ");
			strTitle += loginDlg.m_UserAcount;	
			m_socketClient = loginDlg.m_socketClient;
			this->m_User = loginDlg.m_UserAcount;
			SetWindowText(strTitle);	
			WSAAsyncSelect(m_socketClient,m_hWnd,WM_SOCKET,FD_READ|FD_CLOSE);
		
		}
		else if(result == IDEXIT)
		{
			this->DestroyWindow();
		}
		m_msgType = 0;
		m_msgLength = 0;

	}
	
	if (message==WM_SOCKET){
		SockMsg(wParam,lParam);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CChat_ClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
    {
		closesocket(wParam);
    }
	switch(WSAGETSELECTEVENT(lParam))
    {
	case FD_READ:
		{

			if(!m_msgType)
			{
				m_msgLength =0;
				recv(m_socketClient,(char*)&m_msgLength,sizeof(int),0);
				m_msgType =1;
			}
			else
			{
				char* buffer=new char[m_msgLength+1];
				int recieved =0;
				while (recieved < m_msgLength)
				{
					recieved += recv(m_socketClient,&buffer[recieved],m_msgLength-recieved,0);
				}
				
				buffer[m_msgLength]='\0';
				if(strncmp(buffer,"CHPBL",5) == 0)
				{
					m_AllMessage +=&buffer[6];
					m_AllMessage+="\r\n";
				}
				if(strncmp(buffer,"USERS",5) == 0)
				{
					CString temp;
					temp = &buffer[6];
					if (temp!=m_User)
					{
						m_listFriend.AddString (temp);
					}
					
				}
				if(strncmp(buffer,"LOGUT",5) == 0)
				{
					CString userLogut;
					userLogut = &buffer[6];
					for (int i = 0 ; i < m_listFriend.GetCount(); i++)
					{
						CString userInfriendList;
						m_listFriend.GetText(i,userInfriendList);
						if (userLogut == userInfriendList)
						{
							m_listFriend.DeleteString(i);
							break;
						}
				}
				}
			

				delete buffer;
				UpdateData(FALSE);
				m_msgType =0;

			}
		}
		
		break;
	case FD_CLOSE:
		m_AllMessage = "Server log out";
		UpdateData (false);
		closesocket(wParam);
		break;
	}
}




void CChat_ClientDlg::OnLbnDblclkLfriend()
{
	// TODO: Add your control notification handler code here
	int index;
	index=m_listFriend.GetCurSel ();
//	m_listFriend.GetText (index,m_PrivateChat );
	UpdateData (false);
}


void CChat_ClientDlg::OnLbnSelchangeLfriend()
{
	// TODO: Add your control notification handler code here
}
