// eMotionUSBDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CeMotionUSBDlg dialog
class CeMotionUSBDlg : public CDialog
{
// Construction
public:
	CeMotionUSBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EMOTIONUSB_DIALOG };
	CEdit       m_txtDeviceName;
	CString     m_strDeviceName;
	CListBox	m_lstLog;
	CString		m_strLog;
	BOOL		m_ReceiveData;		
	BOOL		m_SendData;
	CButton		m_btnStart;

	CComboBox m_cboByte1;
	CComboBox m_cboByte2;
	CComboBox m_cboByte3;
	CComboBox m_cboByte4;
	CComboBox m_cboByte5;
	CComboBox m_cboByte6;
	CComboBox m_cboByte7;
	CComboBox m_cboByte8;
	CComboBox m_cboByte9;
	CComboBox m_cboByte10;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg void On_chkReceiveData();
	afx_msg void On_chkSendData();
	afx_msg void On_btnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()


	void ScrollToBottomOfListBox(USHORT);
	void DisplayData(CString);
	void DisplayLastError(CString);
	
	bool HID_FindYourDevice(bool);
	void HID_RegisterForDeviceNotifications();
	void HID_GetDeviceCapabilities();
	void HID_PrepareForOverlappedTransfer();
	BOOL HID_DeviceNameMatch(LPARAM);
	void HID_ReadInputReport();
	void HID_WriteOutputReport();
	void HID_CloseHandles();
	
};
