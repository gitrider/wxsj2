
#include "StdAfx.h"
#include "Resource.h"
#include "Global.h"
#include "Gameprocedure.h"
#include "GamePro_Login.h"
#include "GamePro_CharSel.h"
#include "GamePro_CharCreate.h"
#include "GamePro_Enter.h"
#include "GamePro_Main.h"
#include "Gamepro_ChangeScene.h"

#include "Interface\GMGameInterface.h"

#include "World\WorldManager.h"

#include "Network\NetManager.h"
#include "CGEnterScene.h"

#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Surface\Obj_Surface.h"
#include "Object\Logic\Surface\Obj_Effect.h"
#include "Object\Logic\Surface\Obj_Bullet.h"
#include "Object\Logic\Surface\Obj_SkillObj.h"
#include "Object\Logic\Surface\Obj_Special.h"
#include "Object\Logic\Obj_Map.h"
#include "Object\Logic\Obj_Static.h"
#include "Object\Logic\Obj_Building.h"
#include "Object\Logic\Obj_Dynamic.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_PlayerNPC.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "Object\Logic\ProjTex\Obj_ProjTex.h"
#include "Object\Logic\TripperObj\GMTripperObj_Transport.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemBox.h"
#include "Object\Logic\TripperObj\GMTripperObj_ItemDrop.h"
#include "Object\Logic\TripperObj\GMTripperObj_Resource.h"
#include "Object\Logic\TripperObj\GMTripperObj_Platform.h"
#include "Object\Logic\Bus\Obj_Bus.h"

#include "Input\GMInputSystem.h"
#include "Sound\GMSoundSystem.h"
#include "DBC\GMDataBase.h"
#include "Variable\GMVariable.h"

#include "BuffImpactMgr.h"
#include "DirectlyImpactMgr.h"
#include "Object\Manager\BulletDataManager.h"
#include "Object\Manager\BulletEntityManager.h"
#include "SkillDataMgr.h"
#include "Object\Manager\ActionSetManager.h"

#include "GITimeSystem.h"
#include "GIException.h"
#include "GIUISystem.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "GIdebuger.h"
#include "Cursor\CursorMng.h"
#include "DataPool\GMUIDataPool.h"
#include "Script\GMScriptSystem.h"
#include "Event\GMEventSystem.h"
#include "Action\GMActionSystem.h"
#include "GIProfile.h"
#include "AxProfile.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "Helper\GMHelperSystem.h"
#include "Interface\GMInterface_Script_SystemSetup.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "GIResourceProvider.h"
#include "Sound\GMSoundSystem.h"

#include "Engine/EngineInterface.h"
#include "Resource/ResourceProvider.h"
#include "..\Object\Manager\BulletEntityManager.h"

#include "DataPool/GMDP_Struct_Daytips.h"
#include "DataPool/GMDP_Struct_QuestLog.h"
#include "../Timer.h"

#include "TransferItem/GMTransferItemSystem.h"


const CHAR MAINWINDOW_CLASS[]	= "WuXiaShiJie WndClass";

const INT DEFWINDOW_WIDTH		= 1024;
const INT DEFWINDOW_HEIGHT		= 768;

const INT MINWINDOW_WIDTH		= 800;
const INT MINWINDOW_HEIGHT		= 600;

const UINT DEFWINDOW_STYLE		= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ; 



//��̬������ʼ��
CGamePro_Login*			CGameProcedure::s_pProcLogIn		= NULL;
CGamePro_CharSel*		CGameProcedure::s_pProcCharSel		= NULL;	
CGamePro_CharCreate*	CGameProcedure::s_pProcCharCreate	= NULL;
CGamePro_ChangeScene*   CGameProcedure::s_pProcChangeScene  = NULL;
CGamePro_Enter*			CGameProcedure::s_pProcEnter		= NULL;
CGamePro_Main*			CGameProcedure::s_pProcMain			= NULL;
//CGameProcedure*         CGameProcedure::s_pActiveProcedure  = NULL;


// ��ǰ�������Ϸ����
CGameProcedure*			CGameProcedure::s_pProcActive		= NULL;
// ǰһ�μ������Ϸ����.
CGameProcedure*			CGameProcedure::s_pProcPrev			= NULL;

tInputSystem*			CGameProcedure::s_pInputSystem		= NULL;
CNetManager*			CGameProcedure::s_pNetManager		= NULL;
tWorldSystem*			CGameProcedure::s_pWorldManager		= NULL;
tObjectSystem*			CGameProcedure::s_pObjectManager	= NULL;
//tDebuger*				CGameProcedure::s_pDebuger 			= NULL;
tGfxSystem*				CGameProcedure::s_pGfxSystem		= NULL;
tUISystem*				CGameProcedure::s_pUISystem			= NULL;
tTimeSystem*			CGameProcedure::s_pTimeSystem		= NULL;
tSoundSystem*			CGameProcedure::s_pSoundSystem		= NULL;
tDataBaseSystem*		CGameProcedure::s_pDataBaseSystem	= NULL;
tVariableSystem*		CGameProcedure::s_pVariableSystem	= NULL;
tGameInterfaceBase*		CGameProcedure::s_pGameInterface    = NULL;
tUIDataPool*			CGameProcedure::s_pUIDataPool		= NULL;
tDataPool*				CGameProcedure::s_pDataPool			= NULL;
tCursorSystem*			CGameProcedure::s_pCursorMng		= NULL;
tScriptSystem*			CGameProcedure::s_pScriptSystem		= NULL;
tEventSystem*			CGameProcedure::s_pEventSystem		= NULL;
tActionSystem*			CGameProcedure::s_pActionSystem		= NULL;
tFakeObjSystem*			CGameProcedure::s_pFakeObjSystem	= NULL;
tHelperSystem*			CGameProcedure::s_pHelperSystem		= NULL;
tResourceProvider*      CGameProcedure::s_pResourceProvider = NULL;
tTransferItemSystem*	CGameProcedure::s_pTransferItemSystem = NULL;

CBuffImpactMgr*			CGameProcedure::s_pBuffImpactMgr	= NULL;
CDirectlyImpactMgr*		CGameProcedure::s_pDirectlyImpactMgr= NULL;
CBulletDataMgr*			CGameProcedure::s_pBulletDataMgr	= NULL;
CBulletEntityManager*	CGameProcedure::s_pBulletEntityManager = NULL;
CSkillDataMgr*			CGameProcedure::s_pSkillDataMgr		= NULL;
CMissionDataMgr*		CGameProcedure::s_pMissionDataMgr	= NULL;
CampAndStandDataMgr_T*	CGameProcedure::s_pCampDataMgr		= NULL;
CActionSetMgr*			CGameProcedure::s_pActionSetMgr		= NULL;
DaytipsDataMgr*			CGameProcedure::s_pDaytipsDataMgr	= NULL;
QuestLogDataMgr*		CGameProcedure::s_pQuestLogDataMgr	= NULL;

BOOL					CGameProcedure::m_bMinimized		= FALSE;
BOOL					CGameProcedure::m_bMaximized		= FALSE;
BOOL					CGameProcedure::m_bFullScreen		= FALSE;
BOOL					CGameProcedure::m_bActive			= true;
RECT					CGameProcedure::m_rectWindow;
RECT					CGameProcedure::m_rectFCOffset;
FLOAT					CGameProcedure::m_fWindowFOV		= (FLOAT)DEFWINDOW_HEIGHT/(FLOAT)DEFWINDOW_WIDTH;
BOOL					CGameProcedure::m_bRenderingPaused	= FALSE;
BOOL					CGameProcedure::m_bUIHandleInput	= FALSE;
MMRESULT				CGameProcedure::m_hEventTimer		= NULL;
HANDLE					CGameProcedure::m_hTickEvent		= NULL;
bool					CGameProcedure::m_bWaitNeedFreshMinimap = false;
bool					CGameProcedure::m_bNeedFreshMinimap		= false;


////  ����ʹ�õ��ַ�����
//UNUSE_STRING_VECTOR		CGameProcedure::m_UnUseStrVector;

VOID CGameProcedure::InitStaticMemeber(VOID)
{
	//-------------------------------------------------------------------
	// ��ʼ���漴��������
	srand( (unsigned)timeGetTime() );

	//-------------------------------------------------------------------
	// ע�᱾����
	g_theKernel.ReisgerClass(GETCLASS(tNode));
	// ���������
	g_theKernel.ReisgerClass(GETCLASS(CNetManager));
	// ����������
	g_theKernel.ReisgerClass(GETCLASS(CWorldManager));
	// ���������
	g_theKernel.ReisgerClass(GETCLASS(CObjectManager));
	g_theKernel.ReisgerClass(GETCLASS(CObject));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Surface));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Effect));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Bullet));
	//g_theKernel.ReisgerClass(GETCLASS(CObject_SkillObj));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Special));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Map));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Static));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Building));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Dynamic));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Character));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerMySelf));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerNPC));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerOther));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex_MouseTarget));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex_AuraDure));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex_BloodStain));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Transport));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_ItemBox));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_ItemDrop));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Resource));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Platform));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Bus));

	// ͼ��ϵͳ
	g_theKernel.ReisgerClass(GETCLASS(CEngineInterface));
	// ���������
	g_theKernel.ReisgerClass(GETCLASS(CInputSystem));
	// ��ʱ��
	g_theKernel.ReisgerClass(GETCLASS(tTimeSystem));
	// ����������
	g_theKernel.ReisgerClass(GETCLASS(CSoundSystemFMod));
	// ���ݿ������
	g_theKernel.ReisgerClass(GETCLASS(CDataBaseSystem));
	// ���ݿ������
	g_theKernel.ReisgerClass(GETCLASS(CVariableSystem));
	// UI�����ӿ�
	g_theKernel.ReisgerClass(GETCLASS(CGameInterface));
	// UI���ݳ�
	g_theKernel.ReisgerClass(GETCLASS(CUIDataPool));
	g_theKernel.ReisgerClass(GETCLASS(CDataPool));
	// UI��������
	g_theKernel.ReisgerClass(GETCLASS(CCursorMng));
	// �ű�ϵͳ
	g_theKernel.ReisgerClass(GETCLASS(CScriptSystem));
	// �¼�ϵͳ
	g_theKernel.ReisgerClass(GETCLASS(CEventSystem));
	// ��������
	g_theKernel.ReisgerClass(GETCLASS(CActionSystem));
	// UIģ����ʾ����
	g_theKernel.ReisgerClass(GETCLASS(CFakeObjSystem));
	// ��Ӱ���ϵͳ
	g_theKernel.ReisgerClass(GETCLASS(CHelperSystem));
	// ��Դ
	g_theKernel.ReisgerClass(GETCLASS(CResourceProvider));
	// ��Ʒ����ϵͳ
	g_theKernel.ReisgerClass(GETCLASS(CTransferItemSystem));
	//ע�ᶨʱ��
	g_theKernel.ReisgerClass(GETCLASS(CTimerManager));

	//-------------------------------------------------------------------
	// ���ز���е���
//	PROFILE_PUSH(LoadPluginDll);
//	g_theKernel.LoadPlugin(_T("Debuger.dll"), &g_theKernel);
#ifdef USEOGRELIB
extern VOID InstallUISystem( tKernel* pKernel );

	InstallUISystem(&g_theKernel);
#else
	g_theKernel.LoadPlugin(_T("UI_CEGUI.dll"), &g_theKernel);
#endif
	

//	PROFILE_POP(LoadPluginDll);

	//-------------------------------------------------------------------
	// ��ʼ�����е�ѭ��ʵ��
	s_pProcLogIn			= new CGamePro_Login();			// ��¼ѭ��
	s_pProcCharSel			= new CGamePro_CharSel();		// ����ѡ������
	s_pProcCharCreate		= new CGamePro_CharCreate();	// ���ﴴ������
	s_pProcEnter			= new CGamePro_Enter();			// �ȴ����볡������
	s_pProcMain				= new CGamePro_Main();			// ����Ϸѭ��
	s_pProcChangeScene		= new CGamePro_ChangeScene();	// �������л�����


	//-------------------------------------------------------------------
	// ��ʼ�����ݺ���
	//0. ���������
	s_pInputSystem		= (tInputSystem*)		g_theKernel.NewNode( _T("CInputSystem"), _T("bin"), _T("input"));  
	//1. ��ʱ��
	s_pTimeSystem		= (tTimeSystem*)		g_theKernel.NewNode( _T("tTimeSystem"), _T("bin"), _T("time"));  
	//2. ���������
	s_pNetManager		= (CNetManager*)		g_theKernel.NewNode( _T("CNetManager"), _T("bin"), _T("netman"));  
	//3. ��Ч������
	s_pSoundSystem		= (tSoundSystem*)		g_theKernel.NewNode( _T("CSoundSystemFMod"), _T("bin"), _T("snd"));
	//4. ��Ⱦ���ڵ�
	s_pGfxSystem		= (tGfxSystem*)			g_theKernel.NewNode( _T("CEngineInterface"), _T("bin"), _T("gfx"));
	//5. �������ڵ�
//	s_pDebuger			= (tDebuger*)			g_theKernel.NewNode( _T("tDebuger"), _T("bin"), _T("debuger"));
	//6. ���������
	s_pObjectManager	= (tObjectSystem*)		g_theKernel.NewNode( _T("CObjectManager"), _T("bin"), _T("objman"));  
	//7. UI������
	s_pUISystem			= (tUISystem*)			g_theKernel.NewNode( _T("CUISystem"), _T("bin"), _T("ui_"));
	//8. ���ݿ������
	s_pDataBaseSystem	= (tDataBaseSystem*)	g_theKernel.NewNode( _T("CDataBaseSystem"), _T("bin"), _T("dbc"));
	//9. ���������
	s_pWorldManager		= (tWorldSystem*)		g_theKernel.NewNode( _T("CWorldManager"), _T("bin"), _T("worldman"));  
	//10. ϵͳ����������
	s_pVariableSystem	= (tVariableSystem*)	g_theKernel.NewNode( _T("CVariableSystem"), _T("bin"), _T("var"));
	//11. �ӿڹ�����
	s_pGameInterface	= (tGameInterfaceBase*) g_theKernel.NewNode( _T("CGameInterface"), _T("bin"), _T("interface"));
	//12. UI���ݳ�
	s_pUIDataPool		= ( tUIDataPool* )		g_theKernel.NewNode( _T( "CUIDataPool" ), _T( "bin" ), _T( "datapool" ) );
	s_pDataPool			= ( tDataPool* )		g_theKernel.NewNode( _T( "CDataPool" ), _T( "bin" ), _T( "datapool_" ) );
	//13. ���ָ�������
	s_pCursorMng		= ( tCursorSystem* )	g_theKernel.NewNode( _T( "CCursorMng"), _T( "bin" ), _T("cursor" ) );
	//14. �ű�ϵͳ
	s_pScriptSystem		= ( tScriptSystem* )	g_theKernel.NewNode( _T( "CScriptSystem"), _T( "bin" ), _T("script" ) );
	//15. �¼�ϵͳ
	s_pEventSystem		= ( tEventSystem* )		g_theKernel.NewNode( _T( "CEventSystem"), _T( "bin" ), _T("event" ) );
	//16. ��������
	s_pActionSystem		= ( tActionSystem* )	g_theKernel.NewNode( _T( "CActionSystem"), _T( "bin" ), _T("action" ) );
	//17. UIģ����ʾ����
	s_pFakeObjSystem	= ( tFakeObjSystem* )	g_theKernel.NewNode( _T( "CFakeObjSystem"), _T( "bin" ), _T("fake" ) );
	//18. ��Ӱ���ϵͳ
	s_pHelperSystem		= ( tHelperSystem* )	g_theKernel.NewNode( _T( "CHelperSystem"), _T( "bin" ), _T("helper" ) );
	//19. ��Դ�ṩ
	s_pResourceProvider	= (tResourceProvider*)	g_theKernel.NewNode( _T( "CResourceProvider"), _T( "bin" ), _T("resprovider" ) );
	//20.��ʱ��
	tTimerSystem* timerSys = (tTimerSystem*)g_theKernel.NewNode(_T("CTimerManager"), _T("bin"), _T("timer") );
	timerSys->Initial(NULL);
	//21.��Ʒ����ϵͳ
	s_pTransferItemSystem = (tTransferItemSystem*)g_theKernel.NewNode(_T("CTransferItemSystem"), _T("bin"), _T("transfer"));

	// �ڴ���������֮ǰ�ȼ������ñ�
	s_pVariableSystem	->Initial(NULL);

	//-------------------------------------------------------------------
	// ����������
	CreateMainWnd();

	//-------------------------------------------------------------------
	// ��ʼ�������ڵ�

	s_pEventSystem		->Initial(NULL);
	s_pGfxSystem		->Initial(&g_hMainWnd);
	s_pScriptSystem		->Initial(NULL);
	s_pDataBaseSystem	->Initial(NULL);
	s_pTimeSystem		->Initial(NULL);
	s_pNetManager		->Initial(NULL); 
	s_pWorldManager		->Initial(NULL); 
	s_pObjectManager	->Initial(NULL); 
//	if(s_pDebuger)		
//		s_pDebuger		->Initial(NULL); 
	if(s_pUISystem)		
		s_pUISystem		->Initial(NULL);
	s_pInputSystem		->Initial(NULL); 
	s_pSoundSystem		->Initial(&g_hMainWnd);
	s_pUIDataPool		->Initial(NULL);
	s_pDataPool			->Initial(NULL);
	s_pCursorMng		->Initial(&g_hInstance);
	s_pActionSystem		->Initial(NULL);
	s_pGameInterface	->Initial(NULL);
	s_pFakeObjSystem	->Initial(NULL);
	s_pHelperSystem		->Initial(NULL);
	s_pResourceProvider	->Initial(NULL);
	s_pTransferItemSystem->Initial(NULL);


	s_pSkillDataMgr	= new CSkillDataMgr;
	s_pSkillDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_SKILL_DATA )->GetDBCFile() );

	s_pBuffImpactMgr = new CBuffImpactMgr;
	s_pBuffImpactMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_BUFF_IMPACT )->GetDBCFile() );

	s_pDirectlyImpactMgr = new CDirectlyImpactMgr;
	s_pDirectlyImpactMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_DIRECTLY_IMPACT )->GetDBCFile() );

	s_pBulletDataMgr = new CBulletDataMgr;
	s_pBulletDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_BULLET_DATA ) );
	s_pBulletEntityManager = new CBulletEntityManager;
	
	// camp
	s_pCampDataMgr = new CampAndStandDataMgr_T;
	s_pCampDataMgr->InitFromDBMemory(s_pDataBaseSystem->GetDataBase(DBC_CAMP_AND_STAND)->GetDBCFile());

	s_pActionSetMgr	= new CActionSetMgr;
	s_pActionSetMgr->Init( "ActionSet/" );	

	// ÿ�ջ����
	s_pDaytipsDataMgr	= new DaytipsDataMgr;
	s_pDaytipsDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_DAY_TIPS )->GetDBCFile() );
	
	// �����
	s_pQuestLogDataMgr	= new QuestLogDataMgr;
	s_pQuestLogDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_QUEST_LOG )->GetDBCFile() );

	//-------------------------------------------------------------------
	// ����Tick����
	INT nMaxFPS = s_pVariableSystem->GetAs_Int("System_MaxFPS");
	if(nMaxFPS < 5 || nMaxFPS >150) 
		nMaxFPS = 30;

	m_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::ResetEvent(m_hTickEvent);
	m_hEventTimer = ::timeSetEvent((INT)(1000.0f/nMaxFPS), (INT)(1000.0f/nMaxFPS), EventTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 

	// ------------------------------------------------------------------
	// ͨ���رմ����¼�����ʵ�ֳ�ʼ�����д��ڡ�
	// ���������Ϸ�г�ʼ������ʱ�������⡣

	std::vector< STRING > vParam;
	CHAR szTemp[MAX_PATH];

	_snprintf(szTemp, MAX_PATH, "%d", 0);
	vParam.push_back(szTemp);

	vParam.push_back("distance");

	_snprintf(szTemp, MAX_PATH, "%.3f", 1.0f);
	vParam.push_back(szTemp);

	CEventSystem::GetMe()->PushEvent(GE_OBJECT_CARED_EVENT, vParam);
}

VOID CGameProcedure::ReleaseStaticMember(VOID)
{
	// �ر���������
	if(m_hEventTimer) 
		::timeKillEvent(m_hEventTimer);

	m_hEventTimer = NULL;
	CloseHandle(m_hTickEvent);
	m_hTickEvent = NULL;

	if( s_pActionSetMgr != NULL )
	{
		s_pActionSetMgr->CleanUp();
		delete s_pActionSetMgr;
		s_pActionSetMgr = NULL;
	}
	delete s_pCampDataMgr;		s_pCampDataMgr = NULL;
	delete s_pSkillDataMgr;		s_pSkillDataMgr = NULL;
	delete s_pBulletDataMgr;	s_pBulletDataMgr = NULL;
	delete s_pBuffImpactMgr;	s_pBuffImpactMgr = NULL;
	delete s_pDirectlyImpactMgr;s_pDirectlyImpactMgr = NULL;

	//�ͷ����е�ѭ��ʵ��
	if(s_pProcLogIn)			delete s_pProcLogIn;		s_pProcLogIn = NULL;
	if(s_pProcCharSel)			delete s_pProcCharSel;		s_pProcCharSel = NULL;
	if(s_pProcCharCreate)		delete s_pProcCharCreate;	s_pProcCharCreate = NULL;
	if(s_pProcEnter)			delete s_pProcEnter;		s_pProcEnter = NULL;
	if(s_pProcMain)				delete s_pProcMain;			s_pProcMain = NULL;
	if(s_pProcChangeScene)		delete s_pProcChangeScene;	s_pProcChangeScene = NULL;

	s_pProcPrev = s_pProcActive = NULL;

	//�ر����й����Ľڵ�
	//20.	��Ʒ����ϵͳ
	SAFE_RELEASE(s_pTransferItemSystem);
	//19. ��Դ�ṩ
	SAFE_RELEASE(s_pResourceProvider);
	//18. ��Ӱ���ϵͳ
	SAFE_RELEASE(s_pHelperSystem);
	//17. UIģ����ʾ����
	SAFE_RELEASE(s_pFakeObjSystem);
	//16. ��������
	SAFE_RELEASE( s_pActionSystem );
	//15. �¼�ϵͳ
	SAFE_RELEASE( s_pEventSystem );
	//14. �ű�ϵͳ
	SAFE_RELEASE( s_pScriptSystem );
	//13. ���ָ�������
	SAFE_RELEASE( s_pCursorMng );
	//12. UI���ݳ�
	SAFE_RELEASE(s_pDataPool);
	SAFE_RELEASE(s_pUIDataPool);
	//11. �ӿڹ�����
	SAFE_RELEASE( s_pGameInterface );
	//10. ϵͳ����������
	SAFE_RELEASE( s_pVariableSystem );
	//9. ���ݿ������
	SAFE_RELEASE( s_pDataBaseSystem );
	//8. ���������
	SAFE_RELEASE( s_pWorldManager );
	//7. UI������
	SAFE_RELEASE( s_pUISystem );				
	//6. ���������
	SAFE_RELEASE( s_pObjectManager );
	//5. �������ڵ�
//	SAFE_RELEASE( s_pDebuger );
	//4. ��Ⱦ���ڵ�
	SAFE_RELEASE( s_pGfxSystem );
	//3. ��Ч������
	SAFE_RELEASE( s_pSoundSystem );
	//2. ���������
	SAFE_RELEASE( s_pNetManager );
	//1. ��ʱ��
	SAFE_RELEASE( s_pTimeSystem );
	//0. ���������
	SAFE_RELEASE( s_pInputSystem );

	if(s_pBulletEntityManager)  delete s_pBulletEntityManager; s_pBulletEntityManager = NULL;

	// ���Ĺر�
	g_theKernel.ShutDown();

	// �ͷ�������
	DestroyMainWnd();
}

VOID CALLBACK CGameProcedure::EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	SetEvent(m_hTickEvent);
} 

VOID CGameProcedure::SetActiveProc(CGameProcedure* pProcToActive)
{
	if(NULL == pProcToActive || s_pProcActive == pProcToActive) 
		return;

	s_pProcPrev = s_pProcActive;
	s_pProcActive = pProcToActive;


	// temp
	if((s_pProcPrev == s_pProcEnter) && (s_pProcActive == s_pProcMain))
	{
		if(s_pProcActive)
			s_pProcActive->Init();
	}


/*
	// ���Ҫת���µ���Ϸѭ��...
	if(s_pProcActive != s_pProcPrev) 
	{
		// ���þ�ѭ�����ͷź���
		if(s_pProcPrev)
			s_pProcPrev->Release();

		// ������ѭ���ĳ�ʼ������
		if(s_pProcActive)
			s_pProcActive->Init();

		// ��ʼ�µ�ѭ��
		s_pProcPrev = s_pProcActive;
	}
*/
}

VOID CGameProcedure::MainLoop(VOID)
{
    MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(TRUE)
    {

//AUTO_SAMPLING theAutoSampling;


//AxProfile::AxProfile_PushNode("Msg");
		// ��ȡ����Ϣ�����е�������Ϣ������
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			// ������˳���Ϣ,�˳�
            if(msg.message == WM_QUIT) 
				return;

            // ����������Ϣ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};
//AxProfile::AxProfile_PopNode("Msg");

		// �ȴ����µ���Ϣ������Ϣ���л��ߵ�����Ⱦʱ��
		UINT dwResult = MsgWaitForMultipleObjects(1, &m_hTickEvent, FALSE, INFINITE, QS_ALLINPUT); 

        if(dwResult == WAIT_OBJECT_0)
        {
			// �����߼�Tick����Ⱦ

//AxProfile::AxProfile_PushNode("Tick");
			TickActive();
//AxProfile::AxProfile_PopNode("Tick");

//AxProfile::AxProfile_PushNode("Render");
			RenderActive();
//AxProfile::AxProfile_PopNode("Render");

//AxProfile::AxProfile_PushNode("Event");
			ProcessGameEvent();
//AxProfile::AxProfile_PopNode("Event");

        }
		// ������Ϣ������Ϣѭ��,������Ϣ
		else 
			continue;
	}
}

VOID CGameProcedure::TickActive(VOID)
{
	// ���Ҫת���µ���Ϸѭ��...
	if(s_pProcActive != s_pProcPrev) 
	{
		// ���þ�ѭ�����ͷź���
		if(s_pProcPrev) 
			s_pProcPrev->Release();

		// ������ѭ���ĳ�ʼ������
		if(s_pProcActive)
			s_pProcActive->Init();

		// ��ʼ�µ�ѭ��
		s_pProcPrev = s_pProcActive;
	}


	_Tick();

	// ִ�м���ѭ���������߼�
	if(s_pProcActive)	
		s_pProcActive->Tick();
}

VOID CGameProcedure::ProcessGameEvent(VOID)
{
	if(s_pEventSystem) 
		s_pEventSystem->ProcessAllEvent();
}

VOID CGameProcedure::RenderActive(VOID)
{
	// ִ�м���ѭ������Ⱦ����
	if(s_pProcActive == s_pProcPrev && s_pProcActive && !m_bMinimized && !m_bRenderingPaused)
	{
		// �����У�ֱ����Ⱦ
		if(m_bActive)
		{
			s_pProcActive->Render();
		}
		else
		{
			// ʹ�ô��ڷǷ���ʽ�ػ�
			InvalidateRect(g_hMainWnd, 0, FALSE);
			PostMessage(g_hMainWnd, WM_NCPAINT, TRUE, 0);
		}
	}
}

VOID CGameProcedure::_Tick(VOID)
{
}
VOID CGameProcedure::Tick(VOID)
{
	// ���п�ִ�нڵ���߼���ѯ

	s_pTimeSystem		->Tick();




//AxProfile::AxProfile_PushNode("Tick_Input");
	s_pInputSystem		->Tick(); 
//AxProfile::AxProfile_PopNode("Tick_Input");

//AxProfile::AxProfile_PushNode("Tick_UI");
	if(s_pUISystem)s_pUISystem->Tick();
//AxProfile::AxProfile_PopNode("Tick_UI");

//AxProfile::AxProfile_PushNode("Tick_NetManager");
	s_pNetManager		->Tick(); 
//AxProfile::AxProfile_PopNode("Tick_NetManager");

//AxProfile::AxProfile_PushNode("Tick_World");
	s_pWorldManager		->Tick(); 
//AxProfile::AxProfile_PopNode("Tick_World");

//AxProfile::AxProfile_PushNode("Tick_Object");
	s_pObjectManager	->Tick(); 
//AxProfile::AxProfile_PopNode("Tick_Object");

	s_pBulletEntityManager->Tick();

//AxProfile::AxProfile_PushNode("Tick_Sound");
	s_pSoundSystem		->Tick();
//AxProfile::AxProfile_PopNode("Tick_Sound");

//AxProfile::AxProfile_PushNode("Tick_Gfx");
	s_pGfxSystem		->Tick();
//AxProfile::AxProfile_PopNode("Tick_Gfx");

//AxProfile::AxProfile_PushNode("Tick_DataPool");
	s_pUIDataPool			->Tick();
	s_pDataPool		->Tick();
//AxProfile::AxProfile_PopNode("Tick_DataPool");

//AxProfile::AxProfile_PushNode("Tick_Action");
	s_pActionSystem		->Tick();
//AxProfile::AxProfile_PopNode("Tick_Action");

//AxProfile::AxProfile_PushNode("Tick_Fake");
	s_pFakeObjSystem	->Tick();
//AxProfile::AxProfile_PopNode("Tick_Fake");

//AxProfile::AxProfile_PushNode("Tick_Script");
	s_pScriptSystem		->Tick();
//AxProfile::AxProfile_PopNode("Tick_Script");


//	PROFILE_POP(MainTick);
}

VOID CGameProcedure::ProcessActiveInput(VOID)
{
	// ִ�м���ѭ���ļ��̺���
	if(s_pProcActive == s_pProcPrev && s_pProcActive )
	{
		s_pProcActive->ProcessInput();
	}
}

VOID CGameProcedure::ProcessCloseRequest(VOID)
{
	// ִ�м���ѭ������Ⱦ����
	if(s_pProcActive == s_pProcPrev && s_pProcActive )
	{
		s_pProcActive->CloseRequest();
	}
}

VOID CGameProcedure::ProcessInput(VOID)
{
}

VOID CGameProcedure::CloseRequest(VOID)
{
}

VOID CGameProcedure::CreateMainWnd(VOID)
{
	INT nWinWidth  = 0;
	INT nWinHeight = 0;

	if( s_pVariableSystem )
	{
		BOOL bHave = FALSE;
		fVector2 fResoution = CVariableSystem::GetMe()->GetAs_Vector2( "View_Resoution" , &bHave);

		if (bHave)
		{
			nWinWidth  = (INT)fResoution.x;
			nWinHeight = (INT)fResoution.y;
		}
		else
		{
			nWinWidth  = DEFWINDOW_WIDTH;
			nWinHeight = DEFWINDOW_HEIGHT;
		}
	}
	else
	{
		nWinWidth  = DEFWINDOW_WIDTH;
		nWinHeight = DEFWINDOW_HEIGHT;
	}


	// ���㴰�ڴ�С
	m_bMinimized		= FALSE;
	m_bFullScreen		= TRUE;

	SetRect( &m_rectWindow, 0, 0, nWinWidth, nWinHeight );
	AdjustWindowRect( &m_rectWindow, DEFWINDOW_STYLE, FALSE );

	SetRect( &m_rectFCOffset, 
			 m_rectWindow.left, 
			 m_rectWindow.top, 
			 m_rectWindow.right  - nWinWidth, 
			 m_rectWindow.bottom - nWinHeight
			);

	UINT dwX = (::GetSystemMetrics(SM_CXFULLSCREEN)-(m_rectWindow.right-m_rectWindow.left))/2;
	UINT dwY = (::GetSystemMetrics(SM_CYFULLSCREEN)-(m_rectWindow.bottom-m_rectWindow.top))/2;
	OffsetRect(&m_rectWindow, -m_rectFCOffset.left, -m_rectFCOffset.top);
	OffsetRect(&m_rectWindow, dwX, dwY);

	m_bActive			= true;
	m_bRenderingPaused	= FALSE;

	// ע�ᴰ����
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)_MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInstance;
	wcex.hIcon			= LoadIcon(g_hInstance, (LPCTSTR)IDD_GAME_DIALOG);
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)NULL; //GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= (LPCTSTR)NULL;
	wcex.lpszClassName	= MAINWINDOW_CLASS;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	RegisterClassEx(&wcex);

	// ��������
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "%s %s (%s %s)", GAME_TITLE, VERSION_INFO, __DATE__, __TIME__);

	HWND hWnd = CreateWindowEx(	NULL, MAINWINDOW_CLASS, szTitle, 
								DEFWINDOW_STYLE,
								m_rectWindow.left, m_rectWindow.top, 
								m_rectWindow.right-m_rectWindow.left, 
								m_rectWindow.bottom-m_rectWindow.top,
								NULL, NULL, g_hInstance, NULL);

	if(!hWnd)
	{
		KLThrow(_T("Can't create main window!"));
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	g_hMainWnd = hWnd;
}

VOID CGameProcedure::DestroyMainWnd(VOID)
{
	DestroyWindow( g_hMainWnd );
	UnregisterClass( MAINWINDOW_CLASS, NULL );
	g_hMainWnd = NULL;
}

// ��Ϣ��Ӧ����
LRESULT CALLBACK CGameProcedure::_MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���������������������Ϣ
	if(s_pInputSystem && s_pInputSystem->MessageProc( hWnd, message, wParam, lParam ))
		return 0;

	// UI������������
	if(s_pUISystem && s_pUISystem->MessageProc( hWnd, message, wParam, lParam ))
		return 0;

	// ִ�м���ѭ������Ϣ��Ӧ����
	if(s_pProcActive )
	{
		return s_pProcActive->MainWndProc(hWnd, message, wParam, lParam);
	}
	else return DefWindowProc(hWnd, message, wParam, lParam);
}

//ȱʡ��Ϣ��Ӧ����,�ɱ���Ӧ�����̼̳�
LRESULT CGameProcedure::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------
	// ����һЩȱʡ����Ϣ
	switch (message) 
	{
	// ���ڴ�С�����ı�
	case WM_SIZE:
		{
			// ��С��
            if( SIZE_MINIMIZED == wParam )
            {
                m_bRenderingPaused = true;		// ��С��ʱ��Ⱦ��ͣ

                m_bMinimized = true;
				m_bMaximized = FALSE;
            }
			// ���
            else if( SIZE_MAXIMIZED == wParam )
            {
                m_bRenderingPaused = FALSE;		//�ָ���Ⱦ����(������һ��״̬ʱ��С��)

				m_bMinimized = FALSE;
				m_bMaximized = true;
				HandlePossibleSizeChange( message, wParam, lParam);
			}
			// �ָ�����
            else if( SIZE_RESTORED == wParam )
            {
                if( m_bMaximized )			// �ղ������
                {
                    m_bMaximized = FALSE;
                    HandlePossibleSizeChange(message, wParam, lParam);
                }
                else if( m_bMinimized )		// �ղ�����С��
                {
                    m_bRenderingPaused = FALSE; 

					m_bMinimized = FALSE;
                    HandlePossibleSizeChange(message, wParam, lParam);
                }
                else
                {
					//�����һ��״̬�Ȳ������Ҳ������С��,��Ȼ��ε�WM_SIZE
					//��Ϣ����Ϊ�û��϶����ڱ�Ե����ģ�����������ǲ������ϻָ�
					//�豸�����ǵȴ��û�ֹͣ�϶����ڴ���Ҳ���Ƕ�WM_EXITSIZEMOVE
					//�Ĵ���...
                }
            }
		}
		break;

	// �û��϶����ڿ�ʼ
	case WM_ENTERSIZEMOVE:
		{
			m_bRenderingPaused = true;
		}
		break;
	// �û��϶���
	case WM_SIZING:
		{
			RECT* pRect = (RECT*)lParam;

			switch(wParam)
			{
			case WMSZ_RIGHT:
			case WMSZ_BOTTOMRIGHT:
				{
					KeepWindowFOV(pRect, WMSZ_RIGHT, WMSZ_TOPLEFT);
				}
				break;
			case WMSZ_LEFT:
			case WMSZ_BOTTOMLEFT:
				{
					KeepWindowFOV(pRect, WMSZ_LEFT, WMSZ_TOPRIGHT);
				}
				break;

			case WMSZ_TOP:
 			case WMSZ_TOPRIGHT:
				{
					KeepWindowFOV(pRect, WMSZ_TOP, WMSZ_BOTTOMLEFT);
				}
				break;

			case WMSZ_BOTTOM:
				{
					KeepWindowFOV(pRect, WMSZ_BOTTOM, WMSZ_TOPLEFT);
				}
				break;

			case WMSZ_TOPLEFT:
				{
					KeepWindowFOV(pRect, WMSZ_TOP, WMSZ_BOTTOMRIGHT);
				}
				break;
			}
			return TRUE;
		}
		break;
	// ���ڳߴ緢���˸ı��
	case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* pPos = (WINDOWPOS*)lParam;
			// �Ǵ�С�ı�
			if(pPos->flags & SWP_NOSIZE || pPos->flags & SWP_DRAWFRAME) 
				break;
			// �ǡ�ȫ����״̬
			if(s_pVariableSystem && s_pVariableSystem->GetAs_Int("View_FullScreen")) 
				break;

			// �����µ�Client
			RECT rectFrame;
			SetRect(&rectFrame, 0, 0, pPos->cx, pPos->cy);
			OffsetRect(&rectFrame, pPos->x, pPos->y);

			RECT rectNewFrame;
			CopyRect(&rectNewFrame, &rectFrame);
			KeepWindowFOV(&rectNewFrame, -1, WMSZ_TOPLEFT);

			// �����ͬ������
			if(EqualRect(&rectFrame, &rectNewFrame))
				break;

			pPos->x = rectNewFrame.left;
			pPos->y = rectNewFrame.top;
			pPos->cx = rectNewFrame.right-rectNewFrame.left;
			pPos->cy = rectNewFrame.bottom-rectNewFrame.top;

			return 0;
		}
		break;

	// ����ֱ��ʸı�
	case WM_DISPLAYCHANGE:
		{
			if( s_pEventSystem && s_pVariableSystem->GetAs_Int("View_FullScreen"))
			{
				s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_FullScreen", "1");
			}
			// ���÷ֱ���
			if(m_bMaximized)
			{
				ShowWindow(g_hMainWnd, SW_RESTORE);
				ShowWindow(g_hMainWnd, SW_MAXIMIZE);
			}
			else 
			{
				s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_Resoution", 
					s_pVariableSystem->GetAs_String("View_Resoution").c_str());
			}
			break;
		}
	// �û��϶����ڱ�Ե����
	case WM_EXITSIZEMOVE:
		{
			m_bRenderingPaused = FALSE;
			HandlePossibleSizeChange(message, wParam, lParam);

			// ���÷ֱ��ʱ���
			if(s_pVariableSystem)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				char szTemp[MAX_PATH];
				_snprintf(szTemp, MAX_PATH, "%d,%d", rect.right-rect.left, rect.bottom-rect.top);

				s_pVariableSystem->SetVariable("View_Resoution", szTemp, FALSE, FALSE);
				
			}
		}
		break;

	// �������������С�ߴ�
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;

			pMinMaxInfo->ptMinTrackSize.x = MINWINDOW_WIDTH;
			pMinMaxInfo->ptMinTrackSize.y = MINWINDOW_HEIGHT;

			// ���ߴ�Ϊ��������󻯡�ʱ�Ĵ�С
			RECT rect;
			SetRect(&rect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			AdjustWindowRect(&rect, GetWindowStyle(g_hMainWnd), FALSE);

			pMinMaxInfo->ptMaxSize.x = rect.right-rect.left;
			pMinMaxInfo->ptMaxSize.y = rect.bottom-rect.top;

			pMinMaxInfo->ptMaxTrackSize.x = rect.right-rect.left;
			pMinMaxInfo->ptMaxTrackSize.y = rect.bottom-rect.top;
		}
		break;

	// ���ù��
	case WM_SETCURSOR:
		{
			switch(LOWORD(lParam))
			{
			case HTCLIENT:
				{
					if(s_pCursorMng)
					{
						s_pCursorMng->OnSetCursor();
						return TRUE;
					}
				}
				break;

			default:
				break;
			}
		}
		break;

	// ���ڼ����л�
	case WM_ACTIVATEAPP:
		if( wParam == TRUE )
		{
			m_bActive = TRUE;
		}
		else 
		{
			m_bActive = FALSE;
		}

		// ֪ͨ������
		if(s_pInputSystem) 
			((CInputSystem*)s_pInputSystem)->OnActiveApp(m_bActive);
		break;

	// ��Ϣ��ѭ���Ƶ��˵���
	case WM_ENTERMENULOOP:
		{
			m_bRenderingPaused = true;
		}
		break;

	// ��Ϣ��ѭ���Ƴ��˵�
	case WM_EXITMENULOOP:
		{
			m_bRenderingPaused = FALSE;
		}
		break;
	
	// ϵͳ����
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			// ����F10
			case SC_KEYMENU:
				return 1;

			default:
				break;
			}
		}
		break;
	
	// �����ػ���Ϣ
	case WM_PAINT:
		{
			PAINTSTRUCT psStruct;
			::BeginPaint(hWnd, &psStruct);
			::EndPaint(hWnd, &psStruct);
			s_pGfxSystem->OnPaint();

		}
		break;

	// ���ڱ���ˢ
	case WM_ERASEBKGND:
		{
			return 1;
		}
		break;

	// ������ҵ��˳�����
	case WM_CLOSE:
		{
			//AxProfile::AxProfile_Report();

			// ��������,�������ò˵�
			if(GetActiveProcedure() == s_pProcMain && s_pUISystem)
			{
				if(wParam==0xC0DE && lParam==0XC0DE)
				{
					ProcessCloseRequest();
					break;
				}
				else
				{
					s_pEventSystem->PushEvent( GE_TOGLE_SYSTEMFRAME );
					return TRUE;
				}
			}
		}
		break;
	// ��������
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	// ���뷨�ı�
	case WM_INPUTLANGCHANGE:
		{
			if(s_pEventSystem)
			{
				s_pEventSystem->PushEvent(GE_CHAT_INPUTLANGUAGE_CHANGE);
			}
		}
		break;
	case WM_IME_NOTIFY:
		{
			if(wParam == IMN_SETOPENSTATUS || wParam == IMN_SETCONVERSIONMODE)
			{
				if(s_pEventSystem)
				{
					s_pEventSystem->PushEvent(GE_CHAT_INPUTLANGUAGE_CHANGE);
				}
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

//���Ա��ִ��ڱ����������ı�
//��󻯳���
VOID CGameProcedure::KeepWindowFOV(RECT* pRect, UINT dwChanging, UINT dwAnchor)
{
	return;
	RECT rectTemp;
	CopyRect(&rectTemp, pRect);

	rectTemp.left -= m_rectFCOffset.left;
	rectTemp.top -= m_rectFCOffset.top;
	rectTemp.right -= m_rectFCOffset.right;
	rectTemp.bottom -= m_rectFCOffset.bottom;

	if(WMSZ_LEFT == dwChanging || WMSZ_RIGHT == dwChanging)
	{
		//����
		rectTemp.bottom = rectTemp.top + (INT)((rectTemp.right-rectTemp.left)*m_fWindowFOV);
	}
	else if(WMSZ_TOP == dwChanging || WMSZ_BOTTOM == dwChanging)
	{
		//�߲���
		rectTemp.right = rectTemp.left + (INT)((rectTemp.bottom-rectTemp.top)/m_fWindowFOV);
	}
	else
	{
		//���ݱ����Զ�����
		FLOAT fFov = (FLOAT)(rectTemp.bottom-rectTemp.top)/(FLOAT)(rectTemp.right-rectTemp.left);
		if(fFov > m_fWindowFOV)
		{
			rectTemp.bottom = rectTemp.top + (INT)((rectTemp.right-rectTemp.left)*m_fWindowFOV);
		}
		else
		{
			rectTemp.right = rectTemp.left + (INT)((rectTemp.bottom-rectTemp.top)/m_fWindowFOV);
		}
	}

	AdjustWindowRect(&rectTemp, DEFWINDOW_STYLE, FALSE);

	switch(dwAnchor)
	{
	case WMSZ_TOPLEFT:
		{
			pRect->right = pRect->left + (rectTemp.right-rectTemp.left);
			pRect->bottom = pRect->top + (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_TOPRIGHT:
		{
			pRect->left = pRect->right - (rectTemp.right-rectTemp.left);
			pRect->bottom = pRect->top + (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_BOTTOMLEFT:
		{
			pRect->right = pRect->left + (rectTemp.right-rectTemp.left);
			pRect->top = pRect->bottom - (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_BOTTOMRIGHT:
		{
			pRect->left = pRect->right - (rectTemp.right-rectTemp.left);
			pRect->top = pRect->bottom - (rectTemp.bottom-rectTemp.top);
		}
		break;
	}
}

VOID CGameProcedure::HandlePossibleSizeChange( UINT message, WPARAM wParam, LPARAM lParam )
{
	//------------------------------------------------------
	// ���Ϸ���
	if(!g_hMainWnd || !s_pGfxSystem) 
		return;

	RECT rect;
	GetClientRect( g_hMainWnd, &rect );

	//------------------------------------------------------
	// �ָ��豸
	s_pGfxSystem->OnSizeChange( message, wParam, lParam );

	if( s_pUISystem )
	{
		s_pUISystem->OnSizeChange( message, wParam, lParam );
	}

	if( s_pEventSystem )
	{
		s_pEventSystem->PushEvent( GE_VIEW_RESOLUTION_CHANGED, (INT)message );
	}

	if( s_pObjectManager )
	{
		((CObjectManager*)s_pObjectManager)->OnScreenSizeChanged();
	}

//	if( s_pDebuger )
//	{
//		if( rect.right>rect.left && rect.bottom>rect.top )
//		{
//			s_pDebuger->InfoTrack_Reset( rect.right-rect.left, rect.bottom-rect.top );
//		}
//	}
}

// �õ�ǰһ����Ϸ����.
CGameProcedure*	CGameProcedure::GetPreProcedure()
{
	return s_pProcPrev;
}
