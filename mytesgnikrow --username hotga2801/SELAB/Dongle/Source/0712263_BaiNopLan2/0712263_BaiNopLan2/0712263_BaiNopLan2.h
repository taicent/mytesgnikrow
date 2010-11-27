// 0712263_BaiNopLan2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy0712263_BaiNopLan2App:
// See 0712263_BaiNopLan2.cpp for the implementation of this class
//

class CMy0712263_BaiNopLan2App : public CWinApp
{
public:
	CMy0712263_BaiNopLan2App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy0712263_BaiNopLan2App theApp;