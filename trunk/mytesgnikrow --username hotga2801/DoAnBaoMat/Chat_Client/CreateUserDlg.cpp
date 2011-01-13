// CreateUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat_Client.h"
#include "CreateUserDlg.h"
#include "Crytography.h"

// CreateUserDlg dialog
void sendText(SOCKET sock,CString message);
void EndConnect(SOCKET & socketClient);

IMPLEMENT_DYNAMIC(CreateUserDlg, CDialog)

CreateUserDlg::CreateUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CreateUserDlg::IDD, pParent)
{

}
CreateUserDlg::CreateUserDlg(SOCKET clientSock)
{
	m_socketClient = clientSock;
}

CreateUserDlg::~CreateUserDlg()
{
}

void CreateUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateUserDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTONCancel, &CreateUserDlg::OnBnClickedButtoncancel)
	ON_BN_CLICKED(IDC_BUTTONCreateUser, &CreateUserDlg::OnBnClickedButtoncreateuser)
END_MESSAGE_MAP()


// CreateUserDlg message handlers

void CreateUserDlg::OnBnClickedButtoncancel()
{
	// TODO: Add your control notification handler code here
	this->EndDialog(IDD_DIALOGCreateAcc);
}

void CreateUserDlg::OnBnClickedButtoncreateuser()
{
	// TODO: Add your control notification handler code here
	CString strUser ;
	CString strPass;
	CString strFullname;
	CString strDescription;
	GetDlgItemText(IDC_EDITusername,strUser);
	GetDlgItemText(IDC_EDITPassword,strPass);
	GetDlgItemText(IDC_EDITFullname,strFullname);
	GetDlgItemText(IDC_EDITDescription,strDescription);
	if(strUser.IsEmpty() || strPass.IsEmpty() 
		|| strFullname.IsEmpty() || strDescription.IsEmpty())
	{
		AfxMessageBox("Please enter all infomations !");
	}
	else
	{
		// chuoi gui di
		CString strMesg = "NEWUSER " + strUser + " " + strPass + " " + strFullname + " " + strDescription;
		// gui qua socket
		sendText(m_socketClient,strMesg);
		// cho nhan tra loi
		
		
	}

}

LRESULT CreateUserDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_CREATE)
	{
		m_msgType = 0;
		m_msgLength = 0;
	}
	if (message == WM_SOCKET)
	{
		this->SockMsg(wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
void CreateUserDlg::SockMsg(WPARAM wParam, LPARAM lParam)
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
			if(strncmp(buffer,"NEWUSER 1",9)== 0)
			{				
				shutdown(m_socketClient,SD_BOTH);
				closesocket(m_socketClient);
				this->EndDialog(IDD_DIALOGCreateAcc);

					
			}
			else{
				MessageBox("Can not create account !");
			}
			m_msgLength=0;

		}
		break;
	case FD_CLOSE:
		EndConnect(wParam);
		break;
	}
}

