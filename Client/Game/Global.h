
#pragma once


#include "GIKernel.h"



class tKernel;
class tDebuger;
class tVariableSystem;
class tSoundSystem;
class tWorldSystem;
class tEventSystem;
class tTimeSystem;
class tResourceProvider;
class tGfxSystem;
class tDataBaseSystem;



extern tDebuger*			g_pDebuger;
extern tVariableSystem*		g_pVaribleSys;
extern tSoundSystem*		g_pSoundSystem;
extern tWorldSystem*		g_pWorldSystem;
extern tEventSystem*		g_pEventSys;
extern tTimeSystem*			g_pTimeSystem;
extern tResourceProvider*	g_pResourceProvider;
extern tDataBaseSystem*    g_pDataBase;

/// 数据核心
extern tKernel				g_theKernel;

/// 主窗口
extern HWND					g_hMainWnd;
/// 当前主程序模块句柄
extern HINSTANCE			g_hInstance;
/// 当前版本号
extern const CHAR			VERSION_INFO[];
/// 标题
extern const CHAR			GAME_TITLE[];

//Debug 模式
extern bool				g_bDebugMode;
