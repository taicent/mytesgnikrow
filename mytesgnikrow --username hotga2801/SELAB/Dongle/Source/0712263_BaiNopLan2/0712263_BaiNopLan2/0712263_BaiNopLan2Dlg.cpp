// 0712263_BaiNopLan2Dlg.cpp : implementation file
//
/*
	Van Duy Thanh Long
	0712263
*/
#include "stdafx.h"
#include "0712263_BaiNopLan2.h"
#include "0712263_BaiNopLan2Dlg.h"
#include "HID.h"

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


// CMy0712263_BaiNopLan2Dlg dialog




CMy0712263_BaiNopLan2Dlg::CMy0712263_BaiNopLan2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy0712263_BaiNopLan2Dlg::IDD, pParent)
{
	authen = new CAuthentication();
	m_isRunning = FALSE;

	m_Nr = 128;
	m_Nk = m_Nr / 32;
	m_Nr = m_Nk + 6;
	for(int i =0;i < 16;i++)
	{
		m_Key[i] = i;
	}
	KeyExpansion(m_Key, m_Nk, m_Nr);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy0712263_BaiNopLan2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy0712263_BaiNopLan2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMy0712263_BaiNopLan2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMy0712263_BaiNopLan2Dlg::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMy0712263_BaiNopLan2Dlg message handlers

BOOL CMy0712263_BaiNopLan2Dlg::OnInitDialog()
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
	while(!m_isRunning)
	{
		if(authen->DoModal() == IDOK)
		{
			Authenticate();
		}
		else
		{
			this->EndDialog(1);
			m_isRunning = TRUE;
		}
	}

	m_pWnd = GetDlgItem(IDC_BUTTON1);
	m_pWnd->GetFocus();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy0712263_BaiNopLan2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy0712263_BaiNopLan2Dlg::OnPaint()
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
HCURSOR CMy0712263_BaiNopLan2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
	Van Duy Thanh Long
	0712263
*/

void CMy0712263_BaiNopLan2Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_arrWord.clear();
	m_pList = (CListBox*) GetDlgItem(IDC_LIST1);
	// Xoa tat ca item tren list box
	int iNumOfListItem = m_pList->GetCount();
	for(int i = 0;i < iNumOfListItem;i++)
	{
		m_pList->DeleteString(0);
	}

	// Lay chuoi tu edit box vao str
	m_pWnd = GetDlgItem(IDC_EDIT1);
	m_pWnd->GetWindowTextW(m_str);
	
	if(IsDlgButtonChecked(IDC_CHECK1) == 0)
	{
		m_str.MakeLower();
	}

	int iN = 0;	//so phan tu cua m_arrWord

	// Duyet het chieu dai doan van ban load tu edit box
	for(int i = 0;i < m_str.GetLength();i++)
	{
		//TH ki tu thu i la so hoac chu
		if(iswalnum(m_str[i]))
		{
			TIENG temp;
			//Duyet tu ki tu i de lay vao chuoi
			int j = i;
			for( ;j < m_str.GetLength() && iswalnum(m_str[j]);j++)
			{
				temp.str += m_str[j];
			}
			
			// TH chua co phan tu tieng nao tong m_arrWord
			if(iN == 0)
			{
				m_arrWord.push_back(temp);
				m_arrWord[iN].iNumOfWord = 1;
				iN++;
			}
			else
			{
				bool Flag = 0;	// 0 : ko ton tai 1 tieng tuong tu; 1 : da ton tai 1 tieng trong m_arrWord 
				for(int k = 0;k < iN;k++)
				{
					//TH tieng moi load vao da duoc them vao m_arrWord truoc do
					if(temp.str == m_arrWord[k].str)
					{
						m_arrWord[k].iNumOfWord++;
						Flag = 1;
						break;
					}
						
				}
				//TH ko ton tai temp trong m_arrWord
				if(Flag == 0)
				{
					m_arrWord.push_back(temp);
					m_arrWord[iN].iNumOfWord = 1;
					iN++;
				}
			}
			i = j;	// gan i cho ki tu tiep theo
		}
	}

	// In ra nhung tieng da add vao m_arrWord
	for(int i = 0;i < iN; i++)
	{
		//Dinh dang chuoi in ra de no bao gom so lan xuat hien cua tieng do
		m_arrWord[i].str.AppendFormat(_T(" : %d") , m_arrWord[i].iNumOfWord);
		m_pList->AddString(m_arrWord[i].str);
	}
}



void CMy0712263_BaiNopLan2Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}

BOOL CMy0712263_BaiNopLan2Dlg::Authenticate()
{
	char buf[256];
	DWORD cnt;
	m_DeviceID = -1;
	m_isRunning = FALSE;
	HID_Init();

	int n = HID_FindDevices();

	for (int i = 0; i < n; i++) 
	{
		if (HID_GetName(i, buf, sizeof(buf))) 
		{
			/*char temp[17];
			for(int j = 0;j < 16;j++)
			{
				temp[j] = buf[j+4];
			}
			temp[16] = 0;
		    if(strcmp(temp, "Keil MCB2300 HID") == 0)
		    {*/
			   m_DeviceID = i;
			   break;
		    /*}*/

		}
	}
	if(m_DeviceID != -1)
	{
		if (HID_Open(m_DeviceID)) 
		{
		
			SetOutReport();
			if (!HID_Write(m_OutReport, 34, &cnt)) 
			{
				OnError();
				return FALSE;
			}
			if (!HID_Read(m_InReport, 18, &cnt)) 
			{
				OnError();
				return FALSE;
			}
			GetInReport();
			if(m_isRunning)
			{
				SetTimer(1, 5000, NULL);
			}
			//m_isRunning = TRUE;
		}
	}
	return m_isRunning;
}

void CMy0712263_BaiNopLan2Dlg::SetOutReport()
{
	m_OutReport[0] = 0;
	if(m_isRunning)
	{
		m_OutReport[1] = CHECK_EXISTANCE_CODE;
	}
	else
	{
		m_OutReport[1] = AUTHENTICATION_CODE;
		m_Decryp[0] = 5;
		m_Decryp[1] = 'H';
		m_Decryp[2] = 'e';
		m_Decryp[3] = 'l';
		m_Decryp[4] = 'l';
		m_Decryp[5] = 'o';
		Cipher(m_Decryp, m_Encryp, m_Nr);
		for(int i = 0;i < 6;i++)
		{
			m_OutReport[i+2] = m_Encryp[i];
		}
		
	}
}

void CMy0712263_BaiNopLan2Dlg::GetInReport()
{
	for(int i = 0;i < 16;i++)
	{
		m_Encryp[i] = m_InReport[i+2];
	}
	InvCipher(m_Encryp, m_Decryp, m_Nr);
	m_Decryp[16] = 0;
	if(m_InReport[1] == AUTHENTICATION_CODE)
	{
		
		if(strcmp((char*)m_Decryp, "Keil MBC2300 HID") == 0)
		{
			m_isRunning = TRUE;
		}
	}
	if(m_InReport[1] == CHECK_EXISTANCE_CODE)
	{
		if(strcmp((char*)m_Decryp, "Check existance!") != 0)
		{
			m_isRunning = FALSE;
		}
	}
}

void CMy0712263_BaiNopLan2Dlg::OnError()
{
	  HID_Close();
}

void CMy0712263_BaiNopLan2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	DWORD cnt;
	SetOutReport();
	if (HID_Write(m_OutReport, 34, &cnt)) 
	{
		if (HID_Read(m_InReport, 18, &cnt)) 
		{
			GetInReport();
		}
		else
		{
			m_isRunning = FALSE;
		}
	}
	else
	{
		m_isRunning = FALSE;
	}
	
	
	if(!m_isRunning)
	{
		KillTimer(1);
		while(!m_isRunning)
		{
			if(authen->DoModal() == IDOK)
			{
				Authenticate();
			}
			else
			{
				this->EndDialog(1);
				m_isRunning = TRUE;
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}
