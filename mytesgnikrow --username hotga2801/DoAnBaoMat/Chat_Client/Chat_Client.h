// Chat_Client.h : main header file for the CHAT_CLIENT application
//

#if !defined(AFX_CHAT_CLIENT_H__BAECC8B0_BEA3_4D4F_82FB_4537FAC0AC0C__INCLUDED_)
#define AFX_CHAT_CLIENT_H__BAECC8B0_BEA3_4D4F_82FB_4537FAC0AC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChat_ClientApp:
// See Chat_Client.cpp for the implementation of this class
//

class CChat_ClientApp : public CWinApp
{
public:
	CChat_ClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChat_ClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChat_ClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAT_CLIENT_H__BAECC8B0_BEA3_4D4F_82FB_4537FAC0AC0C__INCLUDED_)
