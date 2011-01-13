#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// EditPasswdDlg dialog

class EditPasswdDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(EditPasswdDlg)

public:
	EditPasswdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~EditPasswdDlg();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOGEditPasswd, IDH = IDR_HTML_EDITPASSWDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
