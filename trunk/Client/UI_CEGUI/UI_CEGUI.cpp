
// UI_CEGUI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "UIGlobal.h"
#include "UISystem.h"
#include "GIPlugin.h"
#include "GIException.h"
#include "GIKernel.h"
#include "OgreRoot.h"

// Magic Code
extern "C" UINT DllMagicCode = DLL_MAGIC_CODE | KL_ENGINE_VERSION;

// �����ʼ��
extern "C" VOID __stdcall DllInit( tKernel* pKernel )
{
	KLAssert(pKernel);

	g_pKernel = pKernel;

	pKernel->ReisgerClass(GETCLASS(CUISystem));

	
}

extern VOID InstallUISystem( tKernel* pKernel )
{
	KLAssert(pKernel);
	g_pKernel = pKernel;
	pKernel->ReisgerClass(GETCLASS(CUISystem));
}

// ����ͷ�
extern "C" VOID __stdcall DllRelease(VOID)
{
}