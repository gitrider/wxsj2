// Launch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CLaunchApp:
// �йش����ʵ�֣������ Launch.cpp
//

class CLaunchApp : public CWinApp
{
public:
	CLaunchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLaunchApp theApp;
