// CEGUIImagesetShow.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CCEGUIImagesetShowApp:
// �йش����ʵ�֣������ CEGUIImagesetShow.cpp
//

class CCEGUIImagesetShowApp : public CWinApp
{
public:
	CCEGUIImagesetShowApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCEGUIImagesetShowApp theApp;
