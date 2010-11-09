// LedsCtrlAppDlg.h : header file
//

#pragma once


// CLedsCtrlAppDlg dialog
class CLedsCtrlAppDlg : public CDialog
{
// Construction
public:
	CLedsCtrlAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LEDSCTRLAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HANDLE hDevice;
	BYTE m_switchPack;
	CString m_strLog;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButtonSet();
public:
	afx_msg void OnClose();

public:
	void LogMessage(BOOL bShowMessageBox = FALSE, CString message = _T(""));
	void NewLog();
	PHANDLE GetUSBDeviceHandle();
	void UpdateSwitchPackToAllCheckBox();
	void GetSwitchPackFromAllCheckBox();
	void CreateAllSongFile();
	BOOL CheckDevice(CString logText = _T(""));

public:
	afx_msg void OnBnClickedButtonGet();
public:
	afx_msg void OnBnClickedButtonViewUsbinfo();
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedClearLcd();
public:
	afx_msg void OnBnClickedSetLCDText();
public:
	afx_msg void OnBnClickedButtonMusic1();
public:
	afx_msg void OnBnClickedButtonMusic2();
public:
	afx_msg void OnBnClickedButtonMusicByData();
public:
	afx_msg void OnBnClickedButtonBrowse();
public:
	afx_msg void OnBnClickedButtonStopMusic();
public:
	afx_msg void OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLbnSelchangeListLog();
public:
	afx_msg void OnBnClickedBtnaes();
};
