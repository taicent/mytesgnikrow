#pragma once


// CAuthentication dialog

class CAuthentication : public CDialog
{
	DECLARE_DYNAMIC(CAuthentication)

public:
	CAuthentication(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAuthentication();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
