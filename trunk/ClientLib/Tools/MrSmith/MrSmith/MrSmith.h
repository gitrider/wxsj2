// MrSmith.h : MrSmith Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CMrSmithApp:
// �йش����ʵ�֣������ MrSmith.cpp
//

class CMrSmithApp : public CWinApp
{
public:
	CMrSmithApp();
	virtual ~CMrSmithApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMrSmithApp theApp;
