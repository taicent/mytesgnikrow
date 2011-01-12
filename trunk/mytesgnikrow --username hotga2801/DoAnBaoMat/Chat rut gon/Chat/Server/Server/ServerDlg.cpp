// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServerDlg dialog




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_message(_T(""))
	, bClosed(false)
	, m_strMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TBX_MSG, m_msgCtrl);
	DDX_Text(pDX, IDC_TBX_MSG, m_strMsg);
	DDX_Control(pDX, IDC_LST_FRIENDSLIST, m_lstFriends);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE (WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_STARTSERVER, &CServerDlg::OnBnClickedBtnStartserver)
	ON_BN_CLICKED(IDC_BTN_STOPSERVER, &CServerDlg::OnBnClickedBtnStopserver)
	ON_WM_CLOSE()
	//ON_CBN_SELCHANGE(IDC_COMBO1, &CServerDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested=MAKEWORD(2,2);
	int err=WSAStartup(wVersionRequested,&wsaData);
	if (err!=0)
		MessageBox(_T("Can't initialize socket"));
	//Clear list client
	m_lstclients.empty();
	bClosed = false;
	/////////////////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
    {
		closesocket(wParam);
    }
	list<Client>::iterator i;

	switch(WSAGETSELECTEVENT(lParam))
    {
		case FD_ACCEPT:
			clients.socket = accept(wParam,NULL,NULL);
			clients.msgType = 0;
			clients.name = _T("");
			m_lstclients.push_back(clients);			
			
			break;
		case FD_READ:
			for( i = m_lstclients.begin(); i != m_lstclients.end(); i++)
				if( i->socket == wParam)			//tim socket gui		
					break;
				
			if (!i->msgType)
			{
				i->buffLength =0;
				recv(i->socket,(char*)&(i->buffLength),sizeof(int),0);//nhan so luong byte
				i->msgType = 1;
			}
			else
			{
				UpdateData();
				char* buffer=new char[i->buffLength+1];
				int recieved =0;
				CString msg;
				CString stringData;
				while (recieved < i->buffLength)
				{
					recieved+= recv(i->socket,&buffer[recieved],i->buffLength - recieved,0);//nhan du lieu chat
				}
				buffer[i->buffLength]='\0';
				i->msgType = 0;

				stringData = buffer;

				msg=stringData.Left (5);
				CString temp;
				if(msg=="LOGIN") // Stringdata = LOGIN username passwd
				{
					temp = TachChuoi (stringData ,6,stringData .GetLength ());
					CheckNameOfClients(temp,i,buffer,i->buffLength);
				}
				else if(msg=="CHPBL")
				{
						UpdateData();
						m_strMsg += TachChuoi(stringData ,6,stringData .GetLength ());
						m_strMsg+= "\r\n";
						UpdateData(false);
						m_msgCtrl.SetSel(m_msgCtrl.GetWindowTextLength(),m_msgCtrl.GetWindowTextLength());
						BroadcasttoAllClientsExceptNoname(i,buffer,i->buffLength);
				}
	
				delete buffer;
			}
			break;

		case FD_CLOSE:					
			for( i = m_lstclients.begin(); i != m_lstclients.end(); i++)
				if( i->socket == wParam)
				{
					if(!bClosed)
					{
						CString strAnswer = _T("");
						if(i->name != _T(""))
						{
							//
							UpdateData();
							m_strMsg += "Status: " + i->name ;
							m_strMsg+=" log off.\r\n";
							UpdateData(false);
							//
							/*strAnswer ='2' +  i->name;
							strAnswer += " log off";*/
							for(int k = 0;k < m_lstFriends.GetCount();k++)
							{
								CString nameItem;
								m_lstFriends.GetText(k,nameItem);
								if(nameItem == i->name)
								{								
									m_lstFriends.DeleteString(k);
									nameItem = "LOGUT " + nameItem;
									int Len=nameItem.GetLength();
									char* sendBuff=new char[Len+1];
									strcpy(sendBuff,LPCTSTR(nameItem));


									BroadcasttoAllClientsExceptNoname(i,sendBuff,Len);	
									delete sendBuff;

									break;
								}
							}
						}
						m_lstclients.erase(i);
						/*if(strAnswer != _T(""))
						{						
							BroadcasttoAllClientsExceptNoname(strAnswer,m_lstclients.end());
						}*/
						break;
					}
					else
					{
						m_lstclients.erase(i);
						break;
					}
				}			
			closesocket(wParam);
			break;
			
	}
	return 0;
}
void CServerDlg::BroadcasttoAllClients(list<Client>::iterator i,char * buffer, int bufferSize )
{
	list<Client>::iterator j;

	for( j = m_lstclients.begin() ; j != m_lstclients.end(); j++)
	{
		if(i == j)
			continue;
		send(j->socket,(char*)&bufferSize,sizeof(bufferSize),0);
		send(j->socket,(char*)buffer,bufferSize,0);
	}
}

void CServerDlg::BroadcasttoAllClientsExceptNoname( list<Client>::iterator i,char * buffer, int bufferSize )
{
	list<Client>::iterator j;
	for( j = m_lstclients.begin() ; j != m_lstclients.end(); j++)
	{
		if((i == j )|| (j->name == _T("")))
			continue;
		send(j->socket,(char*)&bufferSize,sizeof(bufferSize),0);
		send(j->socket,(char*)buffer,bufferSize,0);
	}
}

void CServerDlg::CheckNameOfClients(CString msg, list<Client>::iterator i,char * buffer, int bufferSize )
{
	CString name = _T("");
	name = msg;
	for(list<Client>::iterator j = m_lstclients.begin() ; j != m_lstclients.end(); j++)
		if( j ->name == name)
		{
			char* bufferError = "LOGIN DENY\0";
			int len = 10;
			send(i->socket,(char*)&len,sizeof(int),0);
			send(i->socket,bufferError,10,0);
			m_lstclients.erase(i);
			name = _T("");
			break;
		}
	if(name !=  _T(""))
	{
		//
		UpdateData();
		m_strMsg += "Status: " + name ;
		m_strMsg +=" log on.\r\n";
		UpdateData(false);
		//
		i->name = name;
		m_lstFriends.AddString(name);
		char* bufferAccept = "LOGIN ACCEPT\0";
		int len = 12;
		send(i->socket,(char*)&len,sizeof(int),0);
		send(i->socket,bufferAccept,12,0);
		CString mess;
		mess += "CHPBL " + name;
		mess +=" log on.";


		char * charMessaage;
		int Len=mess.GetLength();
		charMessaage=new char[Len+1];
		strcpy(charMessaage,LPCTSTR(mess));
		BroadcasttoAllClients(m_lstclients.end(),charMessaage,Len);
		delete charMessaage;


		mess = "USERS " + name;
		Len=mess.GetLength();
		charMessaage=new char[Len+1];
		strcpy(charMessaage,LPCTSTR(mess));

		BroadcasttoAllClients(m_lstclients.end(),charMessaage,Len);
		delete charMessaage;


		for(int k = 0; k < m_lstFriends.GetCount(); k++)
		{
				CString nameMember;
				m_lstFriends.GetText(k,nameMember);
				if(nameMember != i->name )
				{
					mess = "USERS "+ nameMember;
					Len=mess.GetLength();
					charMessaage=new char[Len+1];
					strcpy(charMessaage,LPCTSTR(mess));
					SendToClients(nameMember,i->socket,"USERS ",charMessaage,Len);
					delete charMessaage;
				}
		}

	}
}

void CServerDlg::SendToClients(CString name, SOCKET sockClient, CString type,char * buffer, int bufferSize )
{

	send(sockClient,(char*)&bufferSize,sizeof(bufferSize),0);
	send(sockClient,(char*)buffer,bufferSize,0);
}

void CServerDlg::OnBnClickedBtnStartserver()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_port = htons(PORT);    
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockServer, (SOCKADDR *)&serverAddress, sizeof(serverAddress));
	listen(sockServer,5);
	int err=WSAAsyncSelect(sockServer,m_hWnd,WM_SOCKET,
		FD_READ|FD_ACCEPT|FD_CLOSE);
	if (err)
		MessageBox(_T("Cant call WSAAsyncSelect"));	
	GetDlgItem(IDC_BTN_STARTSERVER)->EnableWindow(FALSE);
	return;
}

void CServerDlg::OnBnClickedBtnStopserver()
{
	// TODO: Add your control notification handler code here
	OnClose();
	DestroyWindow();
}

void CServerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	for(list<Client>::iterator j = m_lstclients.begin() ; j != m_lstclients.end(); j++)
	{
		shutdown(j->socket,SD_BOTH);
	}
	closesocket(sockServer);
	bClosed = true;
	CDialog::OnClose();
}
CString CServerDlg ::TachChuoi (CString msg,int i,int j)
{
	CString result;

	for(int temp = i;temp <=j;temp++)
	{
		result+=msg[temp];
	}
	return result;
}
SOCKET CServerDlg ::TimClient (CString User)
{
	for(list<Client>::iterator j = m_lstclients.begin() ; j != m_lstclients.end(); j++)
	{
		if(j->name == User)
		{
			return j->socket ;
		}
	}
	return INVALID_SOCKET ;
}
CString CServerDlg ::TachTen (CString msg,int start)
{
		CString temp;
		int i = msg.Find (' ',start);
		int j = msg.Find (' ',i+1);
		if (j ==-1)
		{
			temp=msg.Right(msg.GetLength ()-i-1);
			return temp;
		}
		else
		{
			temp=msg.Mid (i+1,j-i-1);
			return temp;
		}
}

void CServerDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}
