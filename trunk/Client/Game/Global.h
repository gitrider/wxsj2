
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

/// ���ݺ���
extern tKernel				g_theKernel;

/// ������
extern HWND					g_hMainWnd;
/// ��ǰ������ģ����
extern HINSTANCE			g_hInstance;
/// ��ǰ�汾��
extern const CHAR			VERSION_INFO[];
/// ����
extern const CHAR			GAME_TITLE[];

//Debug ģʽ
extern bool				g_bDebugMode;
