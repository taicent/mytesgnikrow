// LedsCtrlAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Public.h"
#include "LedsCtrlApp.h"
#include "LedsCtrlAppDlg.h"
#include "USBInfoDlg.h"
#include "GetUSBInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSBInfoDlg dialog

CUSBInfoDlg::CUSBInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSBInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUSBInfoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CUSBInfoDlg message handlers

BOOL CUSBInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CLedsCtrlAppDlg *pParent = (CLedsCtrlAppDlg *)this->GetParent();
	PHANDLE pHDevice = pParent->GetUSBDeviceHandle();

	CString str = _T("");
	str = GetUsbConfigDescriptor(*pHDevice);

	this->GetDlgItem(IDC_EDIT_INFO)->SetWindowTextW(str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUSBInfoDlg::OnPaint()
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
HCURSOR CUSBInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

