
#include "StdAfx.h"
#include "Global.h"
//版本记录:
//0.12.06.01 客户端第一个运行版本
//0.03.02.01 客户端第一个正式版本
//0.04.13.01 版本主要新增
//			 1.装备基本玩法
//           2.组队
//           3.好友
//           4.角色属性
//           5.聊天
//0.04.29.01 版本主要新增
//			 1.登陆流程已通
//           2.全场景寻路
//           3.摆摊

tDebuger*			g_pDebuger =NULL;
tVariableSystem*	g_pVaribleSys =NULL;
tSoundSystem*		g_pSoundSystem =NULL;
tWorldSystem*		g_pWorldSystem =NULL;
tEventSystem*		g_pEventSys =NULL;
tTimeSystem*		g_pTimeSystem =NULL;
tResourceProvider*	g_pResourceProvider =NULL;
tDataBaseSystem*    g_pDataBase =NULL;


// 数据核心
tKernel				g_theKernel;
// 主窗口
HWND				g_hMainWnd			= NULL;
// 当前主程序模块句柄
HINSTANCE			g_hInstance			= NULL;
// 当前版本号
const CHAR			VERSION_INFO[]		= "0.04.29.01";
// 标题
const CHAR			GAME_TITLE[]		="武侠世界II";

//Debug 模式
bool				g_bDebugMode = FALSE;


