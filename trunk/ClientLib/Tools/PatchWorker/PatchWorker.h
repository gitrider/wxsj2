// PatchWorker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CPatchWorkerApp:
// �йش����ʵ�֣������ PatchWorker.cpp
//

class CPatchWorkerApp : public CWinApp
{
public:
	CPatchWorkerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPatchWorkerApp theApp;
