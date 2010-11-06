// eMotionUSB.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CeMotionUSBApp:
// See eMotionUSB.cpp for the implementation of this class
//

class CeMotionUSBApp : public CWinApp
{
public:
	CeMotionUSBApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CeMotionUSBApp theApp;