
// OSISP4.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COSISP4App:
// See OSISP4.cpp for the implementation of this class
//

class COSISP4App : public CWinApp
{
public:
	COSISP4App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COSISP4App theApp;