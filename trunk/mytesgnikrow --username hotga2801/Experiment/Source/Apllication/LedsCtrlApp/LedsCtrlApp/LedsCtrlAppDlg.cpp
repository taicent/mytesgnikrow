// LedsCtrlAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LedsCtrlApp.h"
#include "LedsCtrlAppDlg.h"
#include "USBInfoDlg.h"
#include "Public.h"
#include "Song.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CLedsCtrlAppDlg::CreateAllSongFile()
{
	// Create SongLamp.slpc
	CFile oFileLamb(_T("SongLamb.slpc"), CFile::modeCreate | CFile::modeWrite);
	oFileLamb.Write(&song_lamb, sizeof(song_lamb));
	oFileLamb.Close();

	// Create SongJoy.slpc
	CFile oFileJoy(_T("SongJoy.slpc"), CFile::modeCreate | CFile::modeWrite);
	oFileJoy.Write(&song_joy, sizeof(song_joy));
	oFileJoy.Close();	
}

PHANDLE CLedsCtrlAppDlg::GetUSBDeviceHandle()
{
	return &this->hDevice;
}

void CLedsCtrlAppDlg::NewLog()
{
	CListBox *pListLog = (CListBox *)this->GetDlgItem(IDC_LIST_LOG);
	if (pListLog->GetCount() > 0)
		pListLog->AddString(_T(""));
}

void CLedsCtrlAppDlg::LogMessage(BOOL bShowMessageBox/* = FALSE*/, CString message/* = _T("")*/)
{
	SYSTEMTIME st;
	CString str = _T("");

	if (message.GetLength() != 0 && message != _T(""))
		this->m_strLog = message;

	GetLocalTime(&st);
	str.Format(_T("%02i/%02i/%04i %02i:%02i:%02i: "), st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	CListBox *pListLog = (CListBox *)this->GetDlgItem(IDC_LIST_LOG);
	pListLog->AddString(str + this->m_strLog);
	pListLog->SetCurSel(pListLog->GetCount() - 1);

	if (bShowMessageBox)
		MessageBox(this->m_strLog, NULL, MB_ICONERROR);
}

void CLedsCtrlAppDlg::UpdateSwitchPackToAllCheckBox()
{
	CButton *pLed0 = (CButton *)this->GetDlgItem(IDC_CHECK0);
	CButton *pLed1 = (CButton *)this->GetDlgItem(IDC_CHECK1);
	CButton *pLed2 = (CButton *)this->GetDlgItem(IDC_CHECK2);
	CButton *pLed3 = (CButton *)this->GetDlgItem(IDC_CHECK3);

	CButton *pLed4 = (CButton *)this->GetDlgItem(IDC_CHECK4);
	CButton *pLed5 = (CButton *)this->GetDlgItem(IDC_CHECK5);
	CButton *pLed6 = (CButton *)this->GetDlgItem(IDC_CHECK6);
	CButton *pLed7 = (CButton *)this->GetDlgItem(IDC_CHECK7);

	pLed0->SetCheck(((this->m_switchPack >> 0) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed1->SetCheck(((this->m_switchPack >> 1) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed2->SetCheck(((this->m_switchPack >> 2) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed3->SetCheck(((this->m_switchPack >> 3) & 0x01) ? BST_CHECKED : BST_UNCHECKED);

	pLed4->SetCheck(((this->m_switchPack >> 4) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed5->SetCheck(((this->m_switchPack >> 5) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed6->SetCheck(((this->m_switchPack >> 6) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
	pLed7->SetCheck(((this->m_switchPack >> 7) & 0x01) ? BST_CHECKED : BST_UNCHECKED);
}

void CLedsCtrlAppDlg::GetSwitchPackFromAllCheckBox()
{
	CButton *pLed0 = (CButton *)this->GetDlgItem(IDC_CHECK0);
	CButton *pLed1 = (CButton *)this->GetDlgItem(IDC_CHECK1);
	CButton *pLed2 = (CButton *)this->GetDlgItem(IDC_CHECK2);
	CButton *pLed3 = (CButton *)this->GetDlgItem(IDC_CHECK3);

	CButton *pLed4 = (CButton *)this->GetDlgItem(IDC_CHECK4);
	CButton *pLed5 = (CButton *)this->GetDlgItem(IDC_CHECK5);
	CButton *pLed6 = (CButton *)this->GetDlgItem(IDC_CHECK6);
	CButton *pLed7 = (CButton *)this->GetDlgItem(IDC_CHECK7);

	this->m_switchPack = 0x00;	// reset
	this->m_switchPack |= (pLed0->GetCheck() << 0);
	this->m_switchPack |= (pLed1->GetCheck() << 1);
	this->m_switchPack |= (pLed2->GetCheck() << 2);
	this->m_switchPack |= (pLed3->GetCheck() << 3);

	this->m_switchPack |= (pLed4->GetCheck() << 4);
	this->m_switchPack |= (pLed5->GetCheck() << 5);
	this->m_switchPack |= (pLed6->GetCheck() << 6);
	this->m_switchPack |= (pLed7->GetCheck() << 7);
}

BOOL CLedsCtrlAppDlg::CheckDevice(CString logText/* = _T("")*/)
{
	CString str = _T("");

	if (logText != _T(""))
		this->LogMessage(FALSE, logText);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		this->LogMessage(TRUE, _T("  Device is not ready!"));
		return FALSE;
	}

	return TRUE;
}

// CLedsCtrlAppDlg dialog

CLedsCtrlAppDlg::CLedsCtrlAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLedsCtrlAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->hDevice = INVALID_HANDLE_VALUE;
	this->m_switchPack = 0x01;	// Led0
}

void CLedsCtrlAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLedsCtrlAppDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLedsCtrlAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLedsCtrlAppDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CLedsCtrlAppDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CLedsCtrlAppDlg::OnBnClickedButtonSet)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GET, &CLedsCtrlAppDlg::OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_USBINFO, &CLedsCtrlAppDlg::OnBnClickedButtonViewUsbinfo)
	ON_BN_CLICKED(IDC_BUTTON5, &CLedsCtrlAppDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CLEAR_LCD, &CLedsCtrlAppDlg::OnBnClickedClearLcd)
	ON_BN_CLICKED(IDC_SET_LCD_TEXT, &CLedsCtrlAppDlg::OnBnClickedSetLCDText)
	ON_BN_CLICKED(IDC_BUTTON_MUSIC1, &CLedsCtrlAppDlg::OnBnClickedButtonMusic1)
	ON_BN_CLICKED(IDC_BUTTON_MUSIC2, &CLedsCtrlAppDlg::OnBnClickedButtonMusic2)
	ON_BN_CLICKED(IDC_BUTTON_MUSIC_BY_DATA, &CLedsCtrlAppDlg::OnBnClickedButtonMusicByData)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CLedsCtrlAppDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_STOP_MUSIC, &CLedsCtrlAppDlg::OnBnClickedButtonStopMusic)
	ON_BN_CLICKED(IDC_BUTTON_STOP_MUSIC1, &CLedsCtrlAppDlg::OnBnClickedButtonStopMusic)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, &CLedsCtrlAppDlg::OnNMReleasedcaptureSliderVolume)
	ON_LBN_SELCHANGE(IDC_LIST_LOG, &CLedsCtrlAppDlg::OnLbnSelchangeListLog)
	ON_BN_CLICKED(IDC_BTNAES, &CLedsCtrlAppDlg::OnBnClickedBtnaes)
END_MESSAGE_MAP()


// CLedsCtrlAppDlg message handlers

BOOL CLedsCtrlAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CComboBox *pDevice = (CComboBox *)this->GetDlgItem(IDC_COMBO_DEVICE);
	pDevice->AddString(_T("WDF Basic Driver (Root\\WdfBasic)"));
	pDevice->AddString(_T("LPC2300 Leds Controller Driver (USB\\VID_C251&PID_1703)"));

	pDevice->SetCurSel(1);

	// Create song file
	this->CreateAllSongFile();

	// Init Slide Value
	CSliderCtrl * pSlide = (CSliderCtrl *) this->GetDlgItem(IDC_SLIDER_VOLUME);	
	pSlide->SetRange(0, 128, TRUE);
	pSlide->SetPos(64);
	this->SetDlgItemTextW(IDC_STATIC_CUR_VOLUME, _T("64"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLedsCtrlAppDlg::OnPaint()
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
HCURSOR CLedsCtrlAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLedsCtrlAppDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	this->hDevice = INVALID_HANDLE_VALUE;

	CComboBox *pDevice = (CComboBox *)this->GetDlgItem(IDC_COMBO_DEVICE);
	CButton *pButton1 = (CButton *)this->GetDlgItem(IDC_BUTTON1);
	CButton *pButton2 = (CButton *)this->GetDlgItem(IDC_BUTTON2);
	CButton *pBInfo = (CButton *)this->GetDlgItem(IDC_BUTTON_VIEW_USBINFO);

	TCHAR *DevicePath = new TCHAR[MAX_LEN];
	CString str = _T("");
	
	this->NewLog();
	this->LogMessage(FALSE, _T("Open Device"));

	if (pDevice->GetCurSel() != -1)
	{
		DevicePath = ListDevice[pDevice->GetCurSel()];
		str = _T("  Device Path: ");
		this->LogMessage(FALSE, str + DevicePath);
	}
	else
	{
		MessageBox(_T("  Device path is invalid!"), NULL, MB_ICONERROR);
		return;
	}

	//MessageBox(DevicePath);

    hDevice = CreateFile((LPCWSTR)DevicePath,
                     GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
		this->LogMessage(TRUE, _T("  Can't open device! Make sure that device is connected!"));
	else
	{
		str.Format(_T("  Device is ready! hDevice = 0x%08x"), hDevice);
		this->LogMessage(FALSE, str);

		pDevice->EnableWindow(FALSE);
		pButton1->EnableWindow(FALSE);
		pButton2->EnableWindow(TRUE);
		pBInfo->EnableWindow(TRUE);

		// Get LED Array State
		//this->OnBnClickedButtonGet();
	}
}

void CLedsCtrlAppDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CComboBox *pDevice = (CComboBox *)this->GetDlgItem(IDC_COMBO_DEVICE);
	CButton *pButton1 = (CButton *)this->GetDlgItem(IDC_BUTTON1);
	CButton *pButton2 = (CButton *)this->GetDlgItem(IDC_BUTTON2);
	CButton *pBInfo = (CButton *)this->GetDlgItem(IDC_BUTTON_VIEW_USBINFO);
	CString str = _T("");

	BOOL bCmd;

	this->NewLog();
	str.Format(_T("Close device! hDevice = 0x%08x"), hDevice);
	this->LogMessage(FALSE, str);

	if (hDevice != INVALID_HANDLE_VALUE)
	{
		bCmd = CloseHandle(hDevice);

		this->LogMessage(!bCmd, (bCmd ? _T("  Closed!") : _T("Close device fail!")));

		pDevice->EnableWindow(bCmd);
		pButton1->EnableWindow(TRUE);
		pButton2->EnableWindow(FALSE);
		pBInfo->EnableWindow(FALSE);

		hDevice = INVALID_HANDLE_VALUE;
	}
}

void CLedsCtrlAppDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	ULONG bytesReturned = 0;
	ULONG bLength = MAX_WRITE_PACKET_SIZE;
	CString str = _T("");

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Write File")) == FALSE)
		return;

	PUCHAR WriteBuffer = CreatePatternBuffer(bLength);

	str.Format(_T("  Sending %i bytes at address 0x%08x..."), bLength, WriteBuffer);
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = WriteFile(	hDevice,
						WriteBuffer,
						bLength,	// 64 Byte
						&bytesReturned,
						NULL);

	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been write: %i"), bytesReturned);

	if (bCmd)
		str = "  Write success! ";
	else
		str = "  Write fail! ";

	this->LogMessage(!bCmd, str + strResult);

	// Free memory buffer
	free(WriteBuffer);
}

void CLedsCtrlAppDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	ULONG bytesReturned = 0;
	ULONG bLength = MAX_READ_PACKET_SIZE;

	CString str = _T("");

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Read File")) == FALSE)
		return;

	PUCHAR ReadBuffer = CreateBuffer(bLength);

	str.Format(_T("  Reading %i bytes to address 0x%08x..."), bLength, ReadBuffer);
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = ReadFile(hDevice,
					ReadBuffer,
					bLength,	// 64 Byte
					&bytesReturned,
					NULL);

	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been read: %i"), bytesReturned);

	if (bCmd)
		str = "  Read success! ";
	else
		str = "  Read fail! ";

	this->LogMessage(!bCmd, str + strResult);

	// Free memory buffer
	free(ReadBuffer);
}

void CLedsCtrlAppDlg::OnBnClickedButtonSet()
{
	// TODO: Add your control notification handler code here
	ULONG bytesReturned = 0;
	ULONG bLength = MAX_LED_IOCTRL_PACKET_SIZE;

	CString str = _T("");

	this->NewLog();

	// Check Device
	if (this->CheckDevice(_T("Set LED switch pack")) == FALSE)
		return;

	PUCHAR pInBuffer = CreateBuffer(bLength);

	// Get switchPack from LED Array of form 
	this->GetSwitchPackFromAllCheckBox();

	// Set buffer by m_switchPack
	*pInBuffer = this->m_switchPack;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_SET_LIGHTBAR, Size: %i bytes"), MAX_LED_IOCTRL_PACKET_SIZE);
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_SET_LIGHTBAR,		// Command Code
							pInBuffer,						// Input Buffer
							MAX_LED_IOCTRL_PACKET_SIZE,		// Input Buffer Length
							NULL,							// Output Buffer Length
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been write: %i"), bytesReturned);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str + strResult);

	str.Format(_T("  Current m_switchPack = 0x%02x"), m_switchPack);
	this->LogMessage(FALSE, str);
	
	// free memory
	free(pInBuffer);
}

void CLedsCtrlAppDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	// Close Device
	if (hDevice != INVALID_HANDLE_VALUE)
        CloseHandle(hDevice);

	// Delete all song file
	DeleteFile(_T("SongLamb.slpc"));
	DeleteFile(_T("SongJoy.slpc"));

	CDialog::OnClose();
}

void CLedsCtrlAppDlg::OnBnClickedButtonGet()
{
	// TODO: Add your control notification handler code here
	ULONG bytesReturned = 0;
	ULONG bLength = MAX_LED_IOCTRL_PACKET_SIZE;

	CString str = _T("");

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Get LED switch pack")) == FALSE)
		return;

	PUCHAR pOutBuffer = CreateBuffer(bLength);

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_GET_LIGHTBAR, Size: %i bytes"), MAX_LED_IOCTRL_PACKET_SIZE);
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_GET_LIGHTBAR,		// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							pOutBuffer,						// Output Buffer Length
							MAX_LED_IOCTRL_PACKET_SIZE,		// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been read: %i"), bytesReturned);

	if (bCmd)
	{
		str = "  IO success! ";

		// Update switch pack
		this->m_switchPack = *pOutBuffer;

		// Update to form
		this->UpdateSwitchPackToAllCheckBox();
	}
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str + strResult);

	str.Format(_T("  Current m_switchPack = 0x%02x"), m_switchPack);
	this->LogMessage(FALSE, str);

	// free memory
	free(pOutBuffer);
}

void CLedsCtrlAppDlg::OnBnClickedButtonViewUsbinfo()
{
	// TODO: Add your control notification handler code here
	CUSBInfoDlg *pDlg = new CUSBInfoDlg(this);
	pDlg->DoModal();
}

void CLedsCtrlAppDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	CListBox *pListLog = (CListBox *)this->GetDlgItem(IDC_LIST_LOG);
	pListLog->SendMessage(LB_RESETCONTENT);
}

void CLedsCtrlAppDlg::OnBnClickedClearLcd()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	ULONG bytesReturned = 0;

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Clear LCD")) == FALSE)
		return;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_LCD_CLEAR, Size (Buffer): 0 bytes"));
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_LCD_CLEAR,		// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							NULL,							// Output Buffer
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str);
}

void CLedsCtrlAppDlg::OnBnClickedSetLCDText()
{
	// TODO: Add your control notification handler code here
	BOOL bCmd;
	ULONG bytesReturned = 0;
	ULONG bLength = NUM_LCD_CHAR;
	DWORD bNumOfChar = 0;

	CString str = _T("");

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Set Text to LCD")) == FALSE)
		return;

	this->GetDlgItemTextW(IDC_EDIT_LCD_TEXT, str);

	// Check LCD Text Length
	if (str.Trim() == _T(""))
	{
		this->LogMessage(TRUE, _T("  LCD Text is empty!"));
		return;
	}

	bNumOfChar = (DWORD) str.GetLength();
	if (bNumOfChar > NUM_LCD_CHAR)
		bNumOfChar = NUM_LCD_CHAR;

	// Set buffer by Textbox
	PUCHAR pInBuffer = CreateBuffer(bLength);
	for (int i = 0; i < (int)bNumOfChar; i++)
		pInBuffer[i] = (char)str.GetAt(i);

	/************************************************************************/
	/* Set LCD Data Text Methods 2											*/
	/************************************************************************/
	//str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_LCD_SET_TEXT, Size: %i bytes"), bLength);
	//this->LogMessage(FALSE, str);

	//BOOL bCmd;
	//bCmd = DeviceIoControl(	hDevice,						// Device
	//						IOCTL_WDF_USB_LCD_SET_TEXT,		// Command Code
	//						pInBuffer,						// Input Buffer
	//						NUM_LCD_CHAR,					// Input Buffer Length
	//						NULL,							// Output Buffer Length
	//						0,								// Output Buffer Length
	//						&bytesReturned,					// Number of byte returned
	//						NULL);

	//CString strResult = _T("");
	//strResult.Format(_T("NumOfBytes has been sent: %i"), bytesReturned);

	//if (bCmd)
	//	str = "  IO success! ";
	//else
	//	str = "  IO fail! ";

	//this->LogMessage(!bCmd, str + strResult);

	/************************************************************************/
	/* Set LCD Data Text Methods 2											*/
	/************************************************************************/
	// Send Data to Device
	str.Format(_T("  Sending %i bytes of LCD Text to device"), bNumOfChar);
	this->LogMessage(FALSE, str);

	bCmd = WriteFile(	hDevice,
						pInBuffer,
						bNumOfChar,
						&bytesReturned,
						NULL);

	if (!bCmd)
	{
		this->LogMessage(TRUE, _T("  Can't send LCD data text to device!"));
		return;
	}
	else
	{
		str.Format(_T("  Application has sent %i byte(s) of LCD data text buffer to Device..."), bytesReturned);
		this->LogMessage(FALSE, str);
	}

	// Send IO Command to Device
	bCmd = DeviceIoControl(	hDevice,								// Device
							IOCTL_WDF_USB_LCD_SET_TEXT_NO_DATA,		// Command Code
							NULL,									// Input Buffer
							0,										// Input Buffer Length
							NULL,									// Output Buffer Length
							0,										// Output Buffer Length
							&bytesReturned,							// Number of byte returned
							NULL);

	if (bCmd)
		str = "  All IO success! ";
	else
		str = "  Send IOCTL fail! ";

	this->LogMessage(!bCmd, str);
	
	// free memory
	free(pInBuffer);
}

void CLedsCtrlAppDlg::OnBnClickedButtonMusic1()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	ULONG bytesReturned = 0;

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Play DAC Music 1")) == FALSE)
		return;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_DAC_MUSIC1, Size (Buffer): 0 bytes"));
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_DAC_MUSIC1,		// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							NULL,							// Output Buffer Length
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str);
}

void CLedsCtrlAppDlg::OnBnClickedButtonMusic2()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	ULONG bytesReturned = 0;

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Play DAC Music 2")) == FALSE)
		return;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_DAC_MUSIC2, Size (Buffer): 0 bytes"));
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_DAC_MUSIC2,		// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							NULL,							// Output Buffer Length
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str);
}

void CLedsCtrlAppDlg::OnBnClickedButtonMusicByData()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	ULONG bytesReturned = 0;
	BOOL bCmd;

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Play DAC Music By Data")) == FALSE)
		return;

	// Read file
	CString filename = _T("");
	UINT dataLength = 0;
	PUCHAR dataBuf = CreateBuffer(MAX_DATA_SIZE);

	this->GetDlgItemTextW(IDC_EDIT_MUSIC_DATA_FILE, filename);

	// Check filename
	if (filename.Trim() == _T(""))
	{
		this->LogMessage(TRUE, _T("  Data song filename is empty!"));
		return;
	}

	this->LogMessage(FALSE, _T("  Readfile data song!"));
	CFile iFile(filename, CFile::modeRead);
	dataLength = iFile.Read(dataBuf, MAX_DATA_SIZE);

	if (dataLength == 0)
	{
		this->LogMessage(TRUE, _T("  Can't open data song file!"));
		return;
	}

	str.Format(_T("  Sending %i song's data to Device..."), dataLength);
	this->LogMessage(FALSE, str);

	// Send song's data to device
	bCmd = WriteFile(	hDevice,
						dataBuf,
						dataLength,
						&bytesReturned,
						NULL);
	
	if (!bCmd)
	{
		this->LogMessage(TRUE, _T("  Can't send song data to device!"));
		return;
	}
	else
	{
		str.Format(_T("  Application has sent %i byte(s) of data buffer to Device..."), bytesReturned);
		this->LogMessage(FALSE, str);
	}

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_DAC_MUSIC_BY_DATA"));
	this->LogMessage(FALSE, str);

	bCmd = DeviceIoControl(	hDevice,								// Device
							IOCTL_WDF_USB_DAC_MUSIC_BY_DATA,		// Command Code
							NULL,									// Input Buffer
							0,										// Input Buffer Length
							NULL,									// Output Buffer Length
							0,										// Output Buffer Length
							&bytesReturned,							// Number of byte returned
							NULL);

	if (bCmd)
		str = "  All IO success! ";
	else
		str = "  Send IOCTL fail! ";

	this->LogMessage(!bCmd, str);

	// free mem
	free(dataBuf);
}

void CLedsCtrlAppDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	CString buffer, appName;
	appName = theApp.m_pszExeName;
	appName += _T(".exe");

	TCHAR filePath[256];
	GetFullPathName(appName, 256, filePath, NULL);

	buffer = filePath;
	buffer.Replace(appName, _T(""));
	buffer += _T("SongLamb.slpc");
	
	//this->MessageBox(buffer);

	CFileDialog dlg(TRUE, _T("*.slpc"), buffer, 4 | 2,
					_T("LPC2300 Song File (*.slpc)|*.slpc|All file (*.*)|*.*|"),
					NULL, NULL);

	if (dlg.DoModal() == IDOK)
		this->SetDlgItemTextW(IDC_EDIT_MUSIC_DATA_FILE, (LPCTSTR)dlg.GetPathName());
}

void CLedsCtrlAppDlg::OnBnClickedButtonStopMusic()
{
	// TODO: Add your control notification handler code here
	CString str = _T("");
	ULONG bytesReturned = 0;

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Stop DAC Music")) == FALSE)
		return;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_DAC_STOP_MUSIC, Size (Buffer): 0 bytes"));
	this->LogMessage(FALSE, str);

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_DAC_STOP_MUSIC,	// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							NULL,							// Output Buffer Length
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str);
}
void CLedsCtrlAppDlg::OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	ULONG bytesReturned = 0;
	CString str = _T("");
	ULONG bLength = 1;		// 1 Byte DAC Volume

	CSliderCtrl * pSlide = (CSliderCtrl *) this->GetDlgItem(IDC_SLIDER_VOLUME);

	// Show current volume label
	str.Format(_T("%i"), pSlide->GetPos());
	this->SetDlgItemTextW(IDC_STATIC_CUR_VOLUME, str);

	/************************************************************************/
	/* Send IOCTL Set DAC Volume											*/
	/************************************************************************/
	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Set DAC Volume")) == FALSE)
		return;

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_DAC_SET_VOLUME, Size (Buffer): 1 bytes"));
	this->LogMessage(FALSE, str);

	PUCHAR pInBuffer = CreateBuffer(bLength); 
	*pInBuffer = pSlide->GetPos();

	BOOL bCmd;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_DAC_SET_VOLUME,	// Command Code
							pInBuffer,						// Input Buffer
							bLength,						// Input Buffer Length
							NULL,							// Output Buffer
							0,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);
	
	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been sent: %i"), bytesReturned);

	if (bCmd)
		str = "  IO success! ";
	else
		str = "  IO fail! ";

	this->LogMessage(!bCmd, str + strResult);

	// free mem
	free(pInBuffer);

	*pResult = 0;
}

void CLedsCtrlAppDlg::OnLbnSelchangeListLog()
{
	// TODO: Add your control notification handler code here
}

void CLedsCtrlAppDlg::OnBnClickedBtnaes()
{
	// TODO: Add your control notification handler code here
	BOOL bCmd;
	ULONG bytesReturned = 0;
	ULONG bLength = NUM_LCD_CHAR;
	DWORD bNumOfChar = 0;

	CString str = _T("");

	this->NewLog();
	// Check Device
	if (this->CheckDevice(_T("Test AES Algorithm")) == FALSE)
		return;

	this->GetDlgItemTextW(IDC_MESSAGE, str);

	// Check LCD Text Length
	if (str.Trim() == _T(""))
	{
		this->LogMessage(TRUE, _T(" Text is empty!"));
		return;
	}

	bNumOfChar = (DWORD) str.GetLength();
	if (bNumOfChar > NUM_LCD_CHAR)
		bNumOfChar = NUM_LCD_CHAR;

	// Set buffer by Textbox
	PUCHAR pInBuffer = CreateBuffer(bLength);
	for (int i = 0; i < (int)bNumOfChar; i++)
		pInBuffer[i] = (char)str.GetAt(i);

	/************************************************************************/
	/* Set LCD Data Text Methods 2											*/
	/************************************************************************/
	//str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_LCD_SET_TEXT, Size: %i bytes"), bLength);
	//this->LogMessage(FALSE, str);

	//BOOL bCmd;
	//bCmd = DeviceIoControl(	hDevice,						// Device
	//						IOCTL_WDF_USB_LCD_SET_TEXT,		// Command Code
	//						pInBuffer,						// Input Buffer
	//						NUM_LCD_CHAR,					// Input Buffer Length
	//						NULL,							// Output Buffer Length
	//						0,								// Output Buffer Length
	//						&bytesReturned,					// Number of byte returned
	//						NULL);

	//CString strResult = _T("");
	//strResult.Format(_T("NumOfBytes has been sent: %i"), bytesReturned);

	//if (bCmd)
	//	str = "  IO success! ";
	//else
	//	str = "  IO fail! ";

	//this->LogMessage(!bCmd, str + strResult);

	/************************************************************************/
	/* Set LCD Data Text Methods 2											*/
	/************************************************************************/
	// Send Data to Device
	str.Format(_T("  Sending %i bytes of AES Text to device"), bNumOfChar);
	this->LogMessage(FALSE, str);

	bCmd = WriteFile(	hDevice,
						pInBuffer,
						bNumOfChar,
						&bytesReturned,
						NULL);

	if (!bCmd)
	{
		this->LogMessage(TRUE, _T("  Can't send AES data text to device!"));
		return;
	}
	else
	{
		str.Format(_T("  Application has sent %i byte(s) of AES data text buffer to Device..."), bytesReturned);
		this->LogMessage(FALSE, str);
	}

	// Send IO Command to Device

	str = _T("");

	PUCHAR pOutBuffer = CreateBuffer(bLength);

	str.Format(_T("  Sending IOCTL Code: IOCTL_WDF_USB_GET_LIGHTBAR, Size: %i bytes"), bLength);
	this->LogMessage(FALSE, str);
	bytesReturned = 0;
	bCmd = DeviceIoControl(	hDevice,						// Device
							IOCTL_WDF_USB_GET_ENCODED_DATA,		// Command Code
							NULL,							// Input Buffer
							0,								// Input Buffer Length
							pOutBuffer,						// Output Buffer Length
							bLength,								// Output Buffer Length
							&bytesReturned,					// Number of byte returned
							NULL);

	CString strResult = _T("");
	strResult.Format(_T("NumOfBytes has been read: %i"), bytesReturned);
	this->LogMessage(FALSE, strResult);
	if (bCmd)
	{
		str = "  IO success! ";

		// Update switch pack
		//this->m_switchPack = *pOutBuffer;
		str.Format(_T("Result: %s"), pOutBuffer);
		// Update to form
		//this->UpdateSwitchPackToAllCheckBox();
	}
	else
	{
		str = "  IO fail! ";
	}
	this->LogMessage(FALSE, str);

	// free memory
	free(pOutBuffer);
	
	// free memory
	free(pInBuffer);
}
