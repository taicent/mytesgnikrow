// usbhidiocDlg.h : header file
//

#if !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)
#define AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg dialog

class CUsbhidiocDlg : public CDialog
{
// Construction
public:
	CUsbhidiocDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUsbhidiocDlg)
	enum { IDD = IDD_USBHIDIOC_DIALOG };
	CButton	m_Continuous;
	CButton	m_cbutAutoIncrement;
	CComboBox	m_cboByteToSend1;
	CComboBox	m_cboByteToSend0;
	CListBox	m_BytesReceived;
	CListBox	m_ResultsList;
	CButton	m_Once;
	BOOL	m_AutoIncrement;
	CString	m_ResultsString;
	CString	m_strBytesReceived;
	CString	m_strByteToSend0;
	CString	m_strByteToSend1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsbhidiocDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUsbhidiocDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnce();
	afx_msg void OnChangeResults();
	afx_msg void OnContinuous();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	bool FindTheHID();
	void DisplayCurrentTime();
	void DisplayData(CString cstrDataToDisplay);
	void DisplayInputReport();
	void DisplayLastError(CString Operation);
	void DisplayReceivedData(char ReceivedByte);
	void GetDeviceCapabilities();
	void PrepareForOverlappedTransfer();
	void ScrollToBottomOfListBox(USHORT idx);
	void ReadAndWriteToDevice();
	void ReadReport();
	void WriteReport();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)
