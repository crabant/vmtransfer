// vmrefresh.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CvmrefreshApp:
// See vmrefresh.cpp for the implementation of this class
//

class CvmrefreshApp : public CWinApp
{
public:
	CvmrefreshApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CvmrefreshApp theApp;