// AdaboostFaceDetectionDlg.h : header file
//

#pragma once
#include "IntImage.h"

// CAdaboostFaceDetectionDlg dialog
class CAdaboostFaceDetectionDlg : public CDialog
{
// Construction
public:
	CAdaboostFaceDetectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADABOOSTFACEDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	afx_msg void OnBnClickedTraining();
	afx_msg void OnBnClickedBtnfilename();
	afx_msg void OnBnClickedBtnlabel();
	afx_msg void OnBnClickedBtntestfile();

	void OnExperiment();

	IntImage m_Image;
	bool m_ExpReady;
	afx_msg void OnBnClickedBtnexp();
	afx_msg void OnBnClickedBtntest();
	afx_msg void OnBnClickedBtnhome();
	afx_msg void OnBnClickedBtntrain();
};
