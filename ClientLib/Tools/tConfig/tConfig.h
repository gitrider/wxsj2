// tConfig.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号
//#include "SystemInfo.h"
#include "HardwareInfoSystem.h"
#include "GameConfigManager.h"

// CtConfigApp:
// 有关此类的实现，请参阅 tConfig.cpp
//

class CtConfigApp : public CWinApp
{
public:
	CtConfigApp();

// 重写
	public:
	virtual BOOL InitInstance();
    virtual int  ExitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

private:

    int mExitCode;
};

extern CtConfigApp theApp;
//extern CSystemInfo g_theSystemInfo;
extern HardwareInfoSystem gHardwareInfoSystem;

extern CGameConfigManager g_theGameConfigs;
//是否第一次运行
extern BOOL g_bFirstRun;
