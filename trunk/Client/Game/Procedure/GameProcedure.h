
/** ��Ϸ���̻���
 */

#pragma once


#include "CampAndStand.h"
#include "GIUISystem.h"
#include <vector>
#include <string>



class CGamePro_Login;
class CGamePro_CharSel;
class CGamePro_CharCreate;
class CGamePro_Enter;
class CGamePro_Main;
class CGamePro_ChangeScene;
class tNode;
class CNetManager;
class tInputSystem;
class tWorldSystem;
class tObjectSystem;
class tGfxSystem;
class tTimeSystem;
class tSoundSystem;
class tDataBaseSystem;
class tVariableSystem;
class tGameInterfaceBase;
class CMissionDataMgr;
class CTextResMgr;
class CBuffImpactMgr;
class CDirectlyImpactMgr;
class CBulletDataMgr;
class CSkillDataMgr;
class CActionSetMgr;
class tDebuger;
class tUISystem;
class tUIDataPool;
class tDataPool;
class tCursorSystem;
class tScriptSystem;
class tEventSystem;
class tActionSystem;
class tFakeObjSystem;
class tHelperSystem;
class tResourceProvider;
class tSoundSource;
class CBulletEntityManager;
struct EditorCreateBulletCallback;
class DaytipsDataMgr;
class QuestLogDataMgr;
class tTransferItemSystem;

// must use STRING as szMsg's type
#define ADDNEWDEBUGMSG( szMsg )	\
	if(CGameProcedure::s_pEventSystem) \
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, (szMsg)##.c_str());

/*LOGMSGFUNC_EXAMPLE
 |	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
 |	STRING format("log:%s"), msg("funcname");
 |
 |	if(bLog)
 |	{
 |		NOCOLORMSGFUNC(format.c_str(), msg.c_str());
 |		COLORMSGFUNC(format.c_str(), msg.c_str());
 |	}
 |  ......
 */

// care use!!!! CGameProcedure::s_pUISystem must not NULL see LOGMSGFUNC_EXAMPLE
#define NOCOLORMSGFUNC	\
	CGameProcedure::s_pUISystem->ParserString_NoColor_VarParam

// care use!!!! CGameProcedure::s_pUISystem must not NULL see LOGMSGFUNC_EXAMPLE
#define COLORMSGFUNC \
	CGameProcedure::s_pUISystem->ParserString_VarParam

#define NOPARAMMSGFUNC( szMsg )	\
	(CGameProcedure::s_pUISystem)?(CGameProcedure::s_pUISystem->ParserString_NoColor_NoVarParam( (STRING(szMsg).c_str()) )):"";



// ���Ϸ��ַ�������
typedef std::vector<std::string>  UNUSE_STRING_VECTOR;

class CGameProcedure 
{
public:

	/// ��ʼ����̬����
	static VOID		InitStaticMemeber(VOID);
	///	��һ����Ϸѭ������
	static VOID		SetActiveProc(CGameProcedure* pToActive);
	/// ���뵱ǰ��Ϸѭ���������߼�����
	static VOID		TickActive(VOID);
	/// ���¼�ϵͳ������Ϸ�¼�
	static VOID		ProcessGameEvent(VOID);
	/// �����¼�������¼��Ĵ���
	static VOID		ProcessActiveInput(VOID);
	/// ��������˳������¼�
	static VOID		ProcessCloseRequest(VOID);
	/// ���뵱ǰ��Ϸѭ������Ⱦ����
	static VOID		RenderActive(VOID);
	/// �ͷž�̬����
	static VOID		ReleaseStaticMember(VOID);
	/// ��Ϣ��ѭ��
	static VOID		MainLoop(VOID);
	///�����ڴ�С�����ı�����
	static VOID		HandlePossibleSizeChange(UINT message, WPARAM wParam, LPARAM lParam);

	/// �õ���ǰ�����ѭ��
	static CGameProcedure*	GetActiveProcedure(VOID) { return s_pProcActive; }
	/// �������Ƿ��ڼ���״̬
	static BOOL				IsWindowActive(VOID) { return m_bActive; }
	

	static VOID _Tick(VOID);

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ϸ���е�����.
	//
	
	/// ��¼��Ϸѭ��
	static CGamePro_Login*			s_pProcLogIn;
	/// ����ѡ������
	static CGamePro_CharSel*		s_pProcCharSel;
	/// ���ﴴ������
	static CGamePro_CharCreate*		s_pProcCharCreate;
	/// �ȴ����볡������
	static CGamePro_Enter*			s_pProcEnter;
	/// ����Ϸѭ��
	static CGamePro_Main*			s_pProcMain;
	/// ��ǰ���������.
//	static CGameProcedure*          s_pActiveProcedure;
	
	// �л�����������
	static CGamePro_ChangeScene*	s_pProcChangeScene;

	//
	// ��Ϸ���еĹ���
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///���������
	static tInputSystem*			s_pInputSystem;
	///���������
	static CNetManager*				s_pNetManager;
	///����������
	static tWorldSystem*			s_pWorldManager;
	///���������
	static tObjectSystem*			s_pObjectManager;
	///������
//	static tDebuger*				s_pDebuger;
	/// ��Ⱦ��ָ��
	static tGfxSystem*				s_pGfxSystem;
	/// UIϵͳ
	static tUISystem*				s_pUISystem;
	/// ��ʱ��
	static tTimeSystem*				s_pTimeSystem;
	// ����������
	static tSoundSystem*			s_pSoundSystem;
	// ���ݿ������
	static tDataBaseSystem*			s_pDataBaseSystem;
	// ����������
	static tVariableSystem*			s_pVariableSystem;
	// �߼��ӿڹ�����
	static tGameInterfaceBase*		s_pGameInterface;
	// UI���ݳ�
	static tUIDataPool*				s_pUIDataPool;
	static tDataPool*				s_pDataPool;
	// ��������
	static tCursorSystem*			s_pCursorMng;
	//�ű�������
	static tScriptSystem*			s_pScriptSystem;
	//��Ϸ�¼�������
	static tEventSystem*			s_pEventSystem;
	//��Ϸ����������
	static tActionSystem*			s_pActionSystem;
	//UIģ����ʾ����
	static tFakeObjSystem*			s_pFakeObjSystem;
	//��Ӱ���ϵͳ
	static tHelperSystem*			s_pHelperSystem;
	//��Դ�ṩ
	static tResourceProvider*		s_pResourceProvider;
	//��Ʒ����ϵͳ
	static tTransferItemSystem*	s_pTransferItemSystem;

	static CBuffImpactMgr			*s_pBuffImpactMgr;
	static CDirectlyImpactMgr		*s_pDirectlyImpactMgr;
	static CBulletDataMgr			*s_pBulletDataMgr;
	static CSkillDataMgr			*s_pSkillDataMgr;
	static CMissionDataMgr			*s_pMissionDataMgr;
	static CBulletEntityManager		*s_pBulletEntityManager;
	//��Ӫ
	static CampAndStandDataMgr_T	*s_pCampDataMgr;

	static CActionSetMgr			*s_pActionSetMgr;

	// ÿ�����ѹ���
	static DaytipsDataMgr			*s_pDaytipsDataMgr;
	// �����б�
	static QuestLogDataMgr			*s_pQuestLogDataMgr;

protected:

	/// ��ǰ�����ѭ��
	static CGameProcedure*			s_pProcActive;
	/// ��һ�������ѭ����ֻ���л�������ʹ��
	static CGameProcedure*			s_pProcPrev;

public:

	// �õ���ǰ�������Ϸ����.
	//static CGameProcedure*	GetActiveProcedure();

	// �õ�ǰһ����Ϸ����.
	static CGameProcedure*	GetPreProcedure();

protected:

	virtual VOID	Init(VOID) = 0;
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID) = 0;
	virtual VOID	Release(VOID) = 0;
	virtual VOID	ProcessInput(VOID);
	virtual VOID	CloseRequest(VOID);
	virtual LRESULT	MainWndProc(HWND, UINT, WPARAM, LPARAM);

protected:

	//-------------------------------------------
	//�ڲ����Ʊ���

	//������С��
	static BOOL			m_bMinimized;
	//�������
	static BOOL			m_bMaximized;
	//����ȫ����
	static BOOL			m_bFullScreen;
	//����λ��
	static RECT			m_rectWindow;
	//���ڱ���(3:4=0.75)
	static FLOAT		m_fWindowFOV;
	//Frame<->Client�Ĳ�ֵ
	static RECT			m_rectFCOffset;
	//���ڴ��ڽ���״̬
	static BOOL			m_bActive;
	//��Ⱦ��ͣ
	static BOOL			m_bRenderingPaused;
	//ui�������״̬
	static BOOL			m_bUIHandleInput;

	//���ٿ���
	static FLOAT		m_fFPS;
	//ϵͳTick�����¼�
	static MMRESULT		m_hEventTimer;
	//Tick֪ͨEvent
	static HANDLE		m_hTickEvent;
	//ϵͳTick��������
	static VOID CALLBACK EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	//Render Event
	static HANDLE		m_hRenderEvent;

public:

	static bool			m_bNeedFreshMinimap;
	static bool			m_bWaitNeedFreshMinimap;

private:

	///����������
	static VOID		CreateMainWnd(VOID);
	///�ͷ�������
	static VOID		DestroyMainWnd(VOID);
	///��������Ϣ������
	static LRESULT CALLBACK	_MainWndProc(HWND, UINT, WPARAM, LPARAM);
	///�����ڱ��������ı�ʱ
	/* dwChanging-
	|		WMSZ_LEFT/WMSZ_RIGHT
	|		WMSZ_TOP/WMSZ_BOTTOM
	| dwAnchor-
	|		WMSZ_TOPLEFT    
	|		WMSZ_TOPRIGHT   
	|		WMSZ_BOTTOM     
	|		WMSZ_BOTTOMLEFT 
	|		WMSZ_BOTTOMRIGHT
	*/
	static VOID		KeepWindowFOV(RECT* pRect, UINT dwChanging, UINT dwAnchor);

};
