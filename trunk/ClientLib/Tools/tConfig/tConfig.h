// tConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
//#include "SystemInfo.h"
#include "HardwareInfoSystem.h"
#include "GameConfigManager.h"

// CtConfigApp:
// �йش����ʵ�֣������ tConfig.cpp
//

class CtConfigApp : public CWinApp
{
public:
	CtConfigApp();

// ��д
	public:
	virtual BOOL InitInstance();
    virtual int  ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:

    int mExitCode;
};

extern CtConfigApp theApp;
//extern CSystemInfo g_theSystemInfo;
extern HardwareInfoSystem gHardwareInfoSystem;

extern CGameConfigManager g_theGameConfigs;
//�Ƿ��һ������
extern BOOL g_bFirstRun;
