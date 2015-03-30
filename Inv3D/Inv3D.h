// Inv3D.h : main header file for the Inv3D application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CInv3DApp:
// See Inv3D.cpp for the implementation of this class
//

class CInv3DApp : public CWinApp
{
public:
	CInv3DApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CInv3DApp theApp;