
#include "StdAfx.h"
#include "Global.h"
//�汾��¼:
//0.12.06.01 �ͻ��˵�һ�����а汾
//0.03.02.01 �ͻ��˵�һ����ʽ�汾
//0.04.13.01 �汾��Ҫ����
//			 1.װ�������淨
//           2.���
//           3.����
//           4.��ɫ����
//           5.����
//0.04.29.01 �汾��Ҫ����
//			 1.��½������ͨ
//           2.ȫ����Ѱ·
//           3.��̯

tDebuger*			g_pDebuger =NULL;
tVariableSystem*	g_pVaribleSys =NULL;
tSoundSystem*		g_pSoundSystem =NULL;
tWorldSystem*		g_pWorldSystem =NULL;
tEventSystem*		g_pEventSys =NULL;
tTimeSystem*		g_pTimeSystem =NULL;
tResourceProvider*	g_pResourceProvider =NULL;
tDataBaseSystem*    g_pDataBase =NULL;


// ���ݺ���
tKernel				g_theKernel;
// ������
HWND				g_hMainWnd			= NULL;
// ��ǰ������ģ����
HINSTANCE			g_hInstance			= NULL;
// ��ǰ�汾��
const CHAR			VERSION_INFO[]		= "0.04.29.01";
// ����
const CHAR			GAME_TITLE[]		="��������II";

//Debug ģʽ
bool				g_bDebugMode = FALSE;


