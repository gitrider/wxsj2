// PathRegionTransfer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPathRegionTransferApp:
// See PathRegionTransfer.cpp for the implementation of this class
//

class CPathRegionTransferApp : public CWinApp
{
public:
	CPathRegionTransferApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPathRegionTransferApp theApp;