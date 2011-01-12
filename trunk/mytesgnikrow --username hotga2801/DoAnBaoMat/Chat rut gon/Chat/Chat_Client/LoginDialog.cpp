// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Chat_Client.h"
#include "LoginDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ham phu
void sendText(SOCKET sock,CString message);
void EndConnect(SOCKET & socketClient);



/////////////////////////////////////////////////////////////////////////////
// LoginDialog dialog

LoginDialog::LoginDialog(CWnd* pParent /*=NULL*/)
: CDialog(LoginDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(LoginDialog)
	m_UserAcount = _T("");
	m_Passwd = _T("");
	m_Port = 25000;
	//}}AFX_DATA_INIT
}


void LoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoginDialog)
	DDX_Control(pDX, IDC_IPADDRserveraddr, m_ServerAddress);
	DDX_Text(pDX, IDC_EDITusername, m_UserAcount);
	DDX_Text(pDX, IDC_EDITserverPort, m_Port);
	DDX_Text(pDX, IDC_EDITpasswd, m_Passwd);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoginDialog, CDialog)
//{{AFX_MSG_MAP(LoginDialog)
ON_BN_CLICKED(IDEXIT, OnExit)
ON_BN_CLICKED(IDLOGIN, OnLogin)
ON_WM_CLOSE()
//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_CreateAcc, &LoginDialog::OnBnClickedCreateacc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoginDialog message handlers

void LoginDialog::OnExit() 
{
	this->EndDialog(IDEXIT);
}

void LoginDialog::OnLogin() 
{
	if (doCheckData() == TRUE)// kiem tra nhap du lieu do dung khong
	{
		if (doConnect() == TRUE)// khoi tao ket noi den server
		{
			CString message;
			message="LOGIN "+ m_UserAcount + " " + m_Passwd; // LOGIN user passwd
			sendText(m_socketClient,message);// gui message di
			
		}
	}
}



bool LoginDialog::doCheckData()
{
	UpdateData();	
	if (m_Port<1023)
	{
		AfxMessageBox("Nhap 1023<Port < 65535 vao",MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_UserAcount == "" )
	{
		AfxMessageBox("Nhap day du User ",MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_UserAcount.GetLength() > 19)
	{
		AfxMessageBox("Ten User dai hon 20 ki tu",MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	
	if ( m_UserAcount.Find(' ',0)!= -1)
	{
		AfxMessageBox("Khong chua Space trong User Name",MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}

SOCKET Register(int port, CString serverAddress )
{
	SOCKET socketClient = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in server_Add;
	
	server_Add.sin_addr.s_addr = inet_addr((char * )(LPCTSTR)serverAddress);
	server_Add.sin_family = AF_INET;
	
	server_Add.sin_port = htons(port);
	
	// goi ket noi
	int err = connect(socketClient,(sockaddr*)&server_Add,sizeof(server_Add)); 
	if (err == SOCKET_ERROR)
	{
		return NULL;
	}
	return socketClient;
	
}



bool LoginDialog::doConnect()
{
	CString message = _T("Ket noi toi Server tai dia chi :");
	
	int temp ;
	char Buff [10];
	CString Address;	
	unsigned char  Field [4];
	m_ServerAddress.GetAddress(Field[0],Field[1],Field[2],Field[3]);
	
	for (int i =0 ; i < 4 ; i++)
	{
		
		temp= Field[i];
		itoa(temp,Buff,10);
		Address +=Buff;
		if(i <3)
			Address +='.';
	}	

	
	message += Address;

	m_socketClient = Register(m_Port,Address);
	if (m_socketClient == NULL)
	{
		AfxMessageBox("Connect Fail");
		return FALSE;
	}

	AfxMessageBox(message,MB_OK|MB_ICONINFORMATION);
	WSAAsyncSelect(m_socketClient,m_hWnd,WM_SOCKET,FD_READ|FD_CLOSE);



	return TRUE;
}

BOOL LoginDialog::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_socketClient = NULL;
	return CDialog::Create(IDD, pParentWnd);
}

void LoginDialog::OnClose() 
{
	if(m_socketClient!=NULL)
		EndConnect(m_socketClient);
	this->EndDialog(IDEXIT);

	
	CDialog::OnClose();
}

LRESULT LoginDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_CREATE)
	{
		m_msgType = 0;
		m_msgLength = 0;
	}
	if (message == WM_SHOWWINDOW)
	{
			m_ServerAddress.SetAddress(127,0,0,1);
	}
	if (message == WM_SOCKET)
	{
		this->SockMsg(wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
void LoginDialog::SockMsg(WPARAM wParam, LPARAM lParam)
{
	
	if (WSAGETSELECTERROR(lParam))
    {
		
		closesocket(wParam);
    }
	switch(WSAGETSELECTEVENT(lParam))
    {
	case FD_READ:
		if(!m_msgType) // neu message type la 0 thi nhan so
		{
			recv(m_socketClient,(char*)&m_msgLength,sizeof(int),0);
			m_msgType=1;	
		
		}
		else
		{
			m_msgType =0;

			char * buffer = new char[m_msgLength+1];
			recv(m_socketClient,buffer,m_msgLength,0);
			buffer[m_msgLength]='\0';
			if(strncmp(buffer,"LOGIN ACCEPT",12)== 0)
			{
					this->EndDialog(IDLOGIN);
			}
			else
			if(strncmp(buffer,"LOGIN DENY",10)== 0)
			{
					AfxMessageBox("Kiem tra lai user va mat khau");
					shutdown(m_socketClient,SD_BOTH);
					closesocket(m_socketClient);

			}
			m_msgLength=0;

		}
		break;
	case FD_CLOSE:
		EndConnect(wParam);
		break;
	}
}

void LoginDialog::OnBnClickedCreateacc()
{
	// TODO: Add your control notification handler code here

}
