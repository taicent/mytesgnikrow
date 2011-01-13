// EditPasswdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat_Client.h"
#include "EditPasswdDlg.h"


// EditPasswdDlg dialog

IMPLEMENT_DYNCREATE(EditPasswdDlg, CDHtmlDialog)

EditPasswdDlg::EditPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(EditPasswdDlg::IDD, EditPasswdDlg::IDH, pParent)
{

}

EditPasswdDlg::~EditPasswdDlg()
{
}

void EditPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL EditPasswdDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(EditPasswdDlg, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(EditPasswdDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// EditPasswdDlg message handlers

HRESULT EditPasswdDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT EditPasswdDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
