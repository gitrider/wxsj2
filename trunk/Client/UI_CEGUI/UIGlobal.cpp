#include "StdAfx.h"
#include "UIGlobal.h"



tKernel*				g_pKernel = NULL;
tScriptSystem*			g_pScriptSys = NULL;

tActionSystem*			g_pActionSys = NULL;
tInputSystem*			g_pInputSys = NULL;
tGfxSystem*				g_pGfxSystem = NULL;
tObjectSystem*			g_pObjectSystem = NULL;

tCursorSystem*			g_pCursorSystem = NULL;

tTimeSystem *			g_pTimer = NULL;
tFakeObjSystem*			g_pFakeSystem = NULL;
tVariableSystem*		g_pVariableSystem = NULL;
tGameInterfaceBase*		g_pGameInterface = NULL;
tDataBaseSystem*		g_pDataBaseSystem = NULL;
tTransferItemSystem*	g_pTransferItemSystem = NULL;
tSoundSystem*		g_pSoundSystem = NULL;

#ifdef USEOGRELIB
#else
tDebuger*				g_pDebuger = NULL;
tEventSystem*			g_pEventSys = NULL;
HINSTANCE				g_hInstance;
HWND					g_hMainWnd;
tWorldSystem*			g_pWorldSystem = NULL;
#endif