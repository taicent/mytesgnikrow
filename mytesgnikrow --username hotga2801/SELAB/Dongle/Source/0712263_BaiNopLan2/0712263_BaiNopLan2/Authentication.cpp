// Authentication.cpp : implementation file
//

#include "stdafx.h"
#include "0712263_BaiNopLan2.h"
#include "Authentication.h"


// CAuthentication dialog

IMPLEMENT_DYNAMIC(CAuthentication, CDialog)

CAuthentication::CAuthentication(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthentication::IDD, pParent)
{

}

CAuthentication::~CAuthentication()
{
}

void CAuthentication::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAuthentication, CDialog)
END_MESSAGE_MAP()


// CAuthentication message handlers

void CAuthentication::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}
