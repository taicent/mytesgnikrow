/*
Author : Binh Nguyen - Bioz
Last Update : 21 Oct 2008	 
License : Free
Official site : www.sectic.com
Description : detect face by using haar model supplied by OpenCV lib. 
*/

// FaceDetectionDlg.h : header file
//

#pragma once
#include "highgui.h"
#include "OpenCVFaceDetection.h"
#include "Constant.h"

// CFaceDetectionDlg dialog
class CFaceDetectionDlg : public CDialog
{
// Construction
public:
	CFaceDetectionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FACEDETECTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	IplImage *image; // This is image pointer
	CString cascade_path; // path of cascade file
	CString cascade_root; // path of cascade folder
	CString image_path; // path of cascade file	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFileopen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCascadeOpen();
	void LoadFileListToListBox(int id_list, CString root, CString extFile);
	CString GetFileNameFromListBox(int id_list);
	afx_msg void OnLbnSelchangeListImage();
	afx_msg void OnLbnSelchangeListCascade();
	afx_msg void OnBnClickedButtonImageOpen();
};
