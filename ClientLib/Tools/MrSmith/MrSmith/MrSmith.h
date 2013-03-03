// MrSmith.h : MrSmith 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CMrSmithApp:
// 有关此类的实现，请参阅 MrSmith.cpp
//

class CMrSmithApp : public CWinApp
{
public:
	CMrSmithApp();
	virtual ~CMrSmithApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMrSmithApp theApp;
