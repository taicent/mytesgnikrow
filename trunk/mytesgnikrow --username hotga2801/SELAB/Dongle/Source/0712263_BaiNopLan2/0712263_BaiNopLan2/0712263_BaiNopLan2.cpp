// 0712263_BaiNopLan2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "0712263_BaiNopLan2.h"
#include "0712263_BaiNopLan2Dlg.h"

/*
	Van Duy Thanh Long
	0712263
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy0712263_BaiNopLan2App

BEGIN_MESSAGE_MAP(CMy0712263_BaiNopLan2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy0712263_BaiNopLan2App construction

CMy0712263_BaiNopLan2App::CMy0712263_BaiNopLan2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMy0712263_BaiNopLan2App object

CMy0712263_BaiNopLan2App theApp;


// CMy0712263_BaiNopLan2App initialization

BOOL CMy0712263_BaiNopLan2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CMy0712263_BaiNopLan2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
