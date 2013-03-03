/////////////////////////////////////////////////////////////////////////////////
//�ļ�����Scene.h
//�����ߣ�jack
//������������Ϸ����������������Ϸ�߼����ܵ�ִ����
//			ӵ����������Ϸ�����е���Ϸ�߼����ݣ��ж�����ִ���߳�������������ִ��
//			�������������ڳ����еĿͻ����������ӣ��ܹ���������ͻ���������Ϣ
//�޸ļ�¼��2005-3-23����
//�޸ļ�¼��2005-11-8�������ļ��ṹ��֮�����жԳ������޸Ķ���Ҫ���¼�¼
//�޸ļ�¼��2005-11-14���ӳ������¼���Ӧ
//�޸ļ�¼��2005-11-15���ӳ����ĸ���ϵͳ������ݺͽӿ�
//�޸ļ�¼��2006-2-7����̯λ������
/////////////////////////////////////////////////////////////////////////////////

#ifndef __SCENE_H__
#define __SCENE_H__

#include "Type.h"
#include "Map.h"
#include "ScenePlayerManager.h"
#include "SceneFile.h"
#include "ObjManager.h"
#include "HumanManager.h"


#include "Zone.h"
#include "ItemManager.h"

#include "PlatformManager.h"
#include "SpecialManager.h"
#include "GameUtil.h"
#include "MisBuf.h"
#include "SkillLogic.h"
#include "CopySceneManager.h"
#include "GrowPointManager.h"
#include "StallInfoManager.h"
#include "SceneDropPosManager.h"
#include "Performance.h"
#include "EventCore.h"
#include "MsgMediatorFactory.h"
#include "TimerDoingManager.h"

class Obj_Monster;
class Obj_Character;
class LuaInterface;
class CFileDataMgr;
class MonsterManager;
class PetManager;
class HorseManager;
class StoreManager;
class AreaManager;
class SceneTimer;
class PatrolPathMgr;
class ChatPipe ;
class Packet;
class ScanOperator;
struct MISSION_DATA;
class GamePlayer ;
class RecyclePlayerManager;
class ItemBoxManager ;
class Obj_ItemManager;
class BusManager;

using Combat_Module::Skill_Module::SkillLogic_T;
using namespace Event_Module;

#define SCENE_SCRIPTID 888888

struct _OBJ_MONSTER_INIT ;
//
//// ��������
//enum ENUM_SCENE_TYPE
//{
//	SCENE_TYPE_INVALID	= -1,
//	SCENE_TYPE_NORMAL,			// ��������
//	SCENE_TYPE_COPY,			// ����
//	SCENE_TYPE_BATTLEFIELD,		// ս��
//	SCENE_TYPE_ARENA,			// ��̨
//
//	SCENE_TYPE_NUMBERS
//};
//
//

class Scene
{
public :
/////////////////////////////////////////////////////////////////////////////////
//��ײ���Դ����������
/////////////////////////////////////////////////////////////////////////////////
	Scene( SceneID_t SceneID ) ;
	~Scene( ) ;

	//��ȡ������������Ϣ
	BOOL					Load( SCENE_LOAD* load ) ;

	//��ʼ��
	BOOL					Init( ) ;
	//������ִ�нӿڣ��˽ӿڻᱻ�����̶߳�ʱ����
	BOOL					Tick( ) ;
	//���Ƶ�ǰ����״̬���߼��ӿ�
	BOOL					StatusLogic( ) ;

/////////////////////////////////////////////////////////////////////////////////
//��Ϸ�߼��ײ㴦��
/////////////////////////////////////////////////////////////////////////////////
protected :
	//�������߼��ӿ�
	BOOL					HeartBeat( ) ;

	//�������Ļ�����Ϣ
	BOOL					ProcessCacheCommands( ) ;

	//��ȡ��ǰ����������Ϣ
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//���µ��������С
	BOOL					ResizeCache( ) ;

	//ɾ��ĳ��Player���ڳ�����Ϣ�����е���Ϣ
	BOOL					MovePacket( PlayerID_t PlayerID ) ;


public :
/////////////////////////////////////////////////////////////////////////////////
//�����¼���
/////////////////////////////////////////////////////////////////////////////////
	//������ʼ������¼�
	VOID					OnSceneInit( ) ;
	//������ʱ���¼�
	VOID					OnSceneTimer( UINT uTime ) ;
	//�����˳�ǰ�¼�
	VOID					OnSceneQuit( ) ;
	//�����н���һ�����
	VOID					OnScenePlayerEnter( GamePlayer* pGamePlayer ) ;
	//������һ����Ҷ���
	VOID					OnScenePlayerDisconnect( GamePlayer* pGamePlayer ) ;
	//������һ������뿪
	VOID					OnScenePlayerLeave( GamePlayer* pGamePlayer ) ;
	//ĳ�������������߳��г����Ѿ�������
	VOID					OnSceneNotify( SceneID_t DestSceneID ) ;
	//������һ�����������
	VOID					OnSceneHumanLevelUp( Obj_Human* pHuman ) ;
	//������һ���������
	VOID					OnSceneHumanDie(Obj_Human* pHuman, ObjID_t idKiller);
	//������һ����Ҹ���
	VOID					OnSceneHumanRelive(Obj_Human* pHuman, INT ReliveType);

	//������һ����ҽ������������ж�
	BOOL					OnAcceptMissionCheck(Obj_Human* pHuman,ScriptID_t idMissionScript) ;
	//������һ��NpcĬ�϶Ի�����ʾ
	BOOL					OnNpcDefaultDialog(Obj_Human* pHuman,ObjID_t idNpc) ;
	//������һ��NpcĬ��EventList
	BOOL					OnNpcEventList(Obj_Human* pHuman,ObjID_t idNpc) ;

/////////////////////////////////////////////////////////////////////////////////
//ͨ�û����ӿ�
/////////////////////////////////////////////////////////////////////////////////
public :
	//��ȡ��ǰ����״̬
	INT						GetSceneStatus(){ return m_nSceneStatus ; }
	//���õ�ǰ����״̬
	VOID					SetSceneStatus( INT status ){ m_nSceneStatus = status ; }

	//��ȡ��ǰ�����ĳ���ID
	SceneID_t				SceneID(){ return m_SceneID ; } ;

	//�رյ�ǰ����
	BOOL					CloseScene( ) ;

	//��ȡ��ǰ�����ĸ�������ģ��ָ��
	Map*					GetMap(){ return m_pMap ; } ;
	ScenePlayerManager*		GetScenePlayerManager(){ return m_pScenePlayerManager ; } ;
	ScenePlayerManager*		GetPlayerManager(){ return m_pScenePlayerManager ; } ;
	ObjManager*				GetObjManager(){ return m_pObjManager ; } ;
	HumanManager*			GetHumanManager(){ return m_pHumanManager ; } ;
	MonsterManager*			GetMonsterManager(){ return m_pMonsterManager ; } ;
	PetManager*				GetPetManager(){ return m_pPetManager ; } ;
	HorseManager*			GetHorseManager(){ return m_pHorseManager; }
	ItemBoxManager*			GetItemBoxManager(){return m_pItemBoxManager;	};
	Obj_ItemManager*		GetObj_ItemManager(){return m_pObjItemManager; };
	GrowPointManager*		GetGrowPointManager(){return &m_GrowPointGroup;}
	StallInfoManager*		GetStallInfoManager(){return m_pStallInfoManager;}
	StoreManager*			GetStoreManager(){return m_pStoreManager;}
	AreaManager*			GetAreaManager( VOID ){ return m_pAreaManager; }
	TimerDoingManager*		GetTimerDoingManager( VOID ){ return m_pTimerDoingManager; }
	SceneTimer*				GetSceneTimer(){ return m_pSceneTimers ;	}
	ChatPipe*				GetChatPipe( ){ return m_pChatPipe ; } ;
	PatrolPathMgr*			GetPatrolPathMgr( ) const { return m_pPatrolPathMgr; } ;
	_ZONE_INFO*				GetZoneInfo( ){ return &m_ZoneInfo ; } ;
	LuaInterface*           GetLuaInterface() { return m_pLuaInterface;}
	CFileDataMgr*           GetScriptFileMgr()  { return m_pScriptFileMgr; }
	inline Zone*			GetZone( ZoneID_t ZoneID ) ;
	RecyclePlayerManager*	GetRecyclePlayerManager(){ return m_pRecyclePlayerManager; };
	EventCore_T&			GetEventCore(VOID) {return m_EventCore;};
	SpecialManager*			GetSpecialManager(){ return m_pSpecialManager ; }
	PlatformManager*		GetPlatformManager(){ return m_pPlatformManager ; }
	SceneDropPosManager*	GetSceneDropPosManager(){ return m_pDropPosManager ; }
	BusManager*				GetBusManager(){ return m_pBusManager ; }

public :
	//*********
	//*********
	//��˳���������Ϣ
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ��ǲ�ͬ������ͨѶ��Ψһ�ӿ�
	//ע�⣺pPacket��Ϣ��Ҫ��g_pPacketFactoryManager�����������������ɾ��
	//��� PlayerIDΪINVALID_ID, ��㲥����Ϣ����������
	//��� PlayerIDΪINVALID_ID_EX, �����Ϣ�ᱻ����ִ�У�PlayerΪNULL��
	BOOL					SendPacket( Packet* pPacket, 
										PlayerID_t PlayerID, 
										uint Flag=PF_NONE ) ;
	//*********
	//*********


/////////////////////////////////////////////////////////////////////////////////
//����ɨ�����
public :
	//��׼�ĳ���ɨ��ӿ�
	BOOL			Scan( ScanOperator* pScan ) ;

	//��pPos��Ϊ���ģ���ΧΪfRange���ڵ�HumanList
	BOOL			ScanHuman( WORLD_POS* pPos, FLOAT fRange, HUMANLIST* pHumanList ) ;
	BOOL			ScanHuman( ZoneID_t idZone, INT nRadius_ZoneCount, HUMANLIST *pOutHumanList );
	
	//��ǰ�����ڷ��͹㲥��Ϣ���㲥�����ĵ�λ��ΪpOwnCharacter����λ��
	//�㲥����Ϊlen����λ�����lenΪ-1����㲥������������
	//���bSendMe����Ϊ�棬����ϢͬʱҲ����pOwnCharacter�������
	BOOL			BroadCast_Scene( Packet* pPacket ) ;
	
	//��pOwnCharacter���ڵ�λ�ù㲥һ����Ϣ
	//��Ϣ�ķ�ΧΪ��ǰ��ZoneA��ZoneA��Χ��NȦZone
	//NΪ���úõ���Ϣ
	BOOL			BroadCast(	Packet* pPacket, 
								Obj_Character* pOwnCharacter,
								BOOL bSendMe=FALSE ) ;

	//��ZoneID��������Ϊ���Ĺ㲥һ����Ϣ
	//��ΧΪZoneID���������Լ���ΧNȦZone
	//NΪ���úõ���Ϣ
	BOOL			BroadCast(Packet* pPacket,
							  ZoneID_t	zoneID);

	//�㲥������Ϣʱʹ��
	//��ZoneID��������Ϊ���Ĺ㲥һ����Ϣ
	//��ΧΪZoneID���������Լ���ΧNȦZone
	//NΪ���úõ���Ϣ
	//zoneIDΪINVALID_IDʱ��ȫ�����㲥
	BOOL			BroadCast_Chat(Packet* pPacket,
								  ZoneID_t	zoneID);

	BOOL			ScanHuman( ZoneID_t idZone, UINT uZoneRadius, HUMANLIST *plistOutHuman );
	// = idZoneA - (idZoneA & idZoneB)
	BOOL			ScanHuman_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, HUMANLIST *plistOutHuman );

	BOOL			ScanObj( ZoneID_t idZone, UINT uZoneRadius, OBJLIST *plistOutObj );
	// = idZoneA - (idZoneA & idZoneB)
	BOOL			ScanObj_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, OBJLIST *plistOutObj );

	// ��HumanList�е�ÿ��Obj_Human������Ϣ
	BOOL			SendPacket( Packet *pPacket, HUMANLIST *plistHuman );
	BOOL			SendPacket_CrateAttr( Obj *pObj, HUMANLIST *plistHuman );


/////////////////////////////////////////////////////////////////////////////////
//�����߼��������
public :
	//ͨ����Ϸ�ӿ�
	Obj* 			GetSpecificObjByID(ObjID_t nID)const;
	BOOL			ObjectEnterScene( Obj *pObj );
	VOID			ObjectLeaveScene( Obj *pObj );

	// �� Area ע�ᵽ�����ϣ�ע�ᵽÿ�� zone ���棩
	BOOL			RegisterArea( const Area* pArea);
	const Area*		CheckArea( Obj_Human* pHuman );
	ZoneID_t		CalcZoneID( const WORLD_POS* pos ) ;

	// ZONE����OBJECT���
	BOOL			ObjZone_Register( Obj *pObj, ZoneID_t idZone );
	BOOL			ObjZone_Unregister( Obj *pObj, ZoneID_t idZone );
	BOOL			ObjZone_Changed( Obj *pObj, ZoneID_t idNew, ZoneID_t idOld );

	//�ж��Ƿ������ﵽ��������
	BOOL			IsCanEnter( ) ;
	//�ж��Ƿ�������ȫ����
	BOOL			IsFull( ) ;
	//�ж��û����޽��볡����Ȩ��, 0-���ܽ���1-���Խ��룬2-�������ݳ���
	INT				CheckEnter( PlayerID_t PlayerID ) ;

	VOID			GetRectInRadius( VRECT* pRect, INT nRadius, ZoneID_t zid ) ;

	ObjID_t			CreateTempMonster(const	_OBJ_MONSTER_INIT* pMonsterInit ) ;
	BOOL			DeleteTempMonster(Obj_Monster* pMonster);

	Obj*			NewObject( Obj::ObjType eType );
	VOID			DeleteObject( Obj *pObj );

	inline VOID		SetSceneLoad_Map( const CHAR* szMap ) ;
	inline VOID		SetSceneLoad_Monster( const CHAR* szMonster ) ;
	inline VOID		SetSceneLoad_Platform( const CHAR* szPlatform ) ;
	inline VOID		SetSceneLoad_GrowPointData( const CHAR* szGrowPointData ) ;
	inline VOID		SetSceneLoad_GrowPointSetup( const CHAR* szGrowPointSetup ) ;
	inline VOID		SetSceneLoad_Area( const CHAR* szArea ) ;
	inline VOID		SetSceneLoad_Pet( const CHAR* szPet ) ;
	inline VOID		SetSceneLoad_PatrolPointData( const CHAR* szPatrolPointData ) ;
	inline VOID		SetSceneLoad_Bus( const CHAR* szBus );
	
	inline VOID		SetCopySceneData_SourSceneID( const SceneID_t id ) ;
	inline VOID		SetCopySceneData_TeamLeader( const GUID_t leader ) ;
	inline VOID		SetCopySceneData_NoUserCloseTime( const UINT closetime ) ;
	inline VOID		SetCopySceneData_Timer( const UINT scenetimer ) ;
	inline VOID		SetCopySceneData_Param( const UINT Index, const INT value ) ;
	
	inline SceneID_t	GetCopySceneData_SourSceneID( ) ;
	inline GUID_t		GetCopySceneData_TeamLeader( ) ;
	inline UINT			GetCopySceneData_NoUserCloseTime( ) ;
	inline UINT			GetCopySceneData_Timer( ) ;
	inline INT			GetCopySceneData_Param( const UINT Index ) ;

	inline INT			GetCopyScene_HumanCount( ) ;
	inline ObjID_t		GetCopyScene_HumanObjID( INT nIndex ) ;
	// 0~99���������ÿ��������һ��Rand Table.
	INT					GetRand100(VOID) {return m_Rand100.GetRand();};


	VOID			SetMapName(const CHAR *pszMapName);
	CHAR*			GetMapName(){ return m_szMapName ; }
	//�жϵ�ǰ�����Ƿ�Ϊ�������� ��ͨ��Ϸ���� ���г���, enum SCENE_TYPE
	VOID			SetSceneType( INT type ){ m_SceneType = type ; } ;
	INT				GetSceneType( )const{ return m_SceneType ; } ;

	VOID			BeginSceneTimer( UINT uTerm, UINT uNowTime ) ;
	VOID			EndSceneTimer( ) ;

	// ������ͨ�ʼ�
	VOID			SendNormalMail(const Obj_Human* pHuman, const CHAR* szReceiverName, const CHAR* szContent);

	// ���Ϳ�ִ���ʼ�
	VOID			SendScriptMail(const CHAR* szReceiverName, UINT uParam0, UINT uParam1, UINT uParam2, UINT uParam3 );

	//ִ��һ��monster.ini�ļ�������һȺԤ����õĹ�
	BOOL			ExecuteMonsterIni( const CHAR* szFileName ) ;

	//�ⲿ��ô˳����ĳ�����Ϣ
	CITYSCENE_DATA* GetCityData(){return &m_CityData;}

	const MsgMediatorFactory* GetMsgMediatorFactory() {return &m_oMsgMediatorFacotry;}
/////////////////////////////////////////////////////////////////////////////////
//����Ϊ�����������г�Ա��������������
/////////////////////////////////////////////////////////////////////////////////
protected :
	//��ǰ������״̬����SCENE_STATUS
	INT						m_nSceneStatus ;
	//��ǰ�����ĵ�ͼ��Ϣ
	Map*					m_pMap ;

	//��ǰ��ͼ�е�������Ϣ
	Zone*					m_pZone ;
	_ZONE_INFO				m_ZoneInfo ;

	//��ǰ�����ĳ���ID
	SceneID_t				m_SceneID ;

	//��ǰ�������û�Ⱥ����ģ��ָ��
	ScenePlayerManager*		m_pScenePlayerManager ;

	//��ǰ�����ȴ����յ��û�ָ��
	RecyclePlayerManager*	m_pRecyclePlayerManager;

	//OBJ Manager�����й���ǰ�����������OBJ����Ϣ��
	//����ͨ��ObjID��ѯ����ǰOBJ��ָ������
	//OBJ Manager�����𴴽�������OBJ����������
	ObjManager*				m_pObjManager ;

	//Obj_Human Manager������ǰ����������н�ɫ���߼�����
	HumanManager*			m_pHumanManager ;

	//Obj_Monster Manager������ǰ����������еĹ�����߼�����
	MonsterManager*			m_pMonsterManager ;

	//Obj_Pet Manager������ǰ����������е�Obj_Pet���߼�����
	PetManager*				m_pPetManager ;

	//Obj_Horse Manager������ǰ����������е�Obj_Pet���߼�����
	HorseManager*			m_pHorseManager;
	//Obj_ItemBox Manager ����ǰ������������Ʒ�е��߼�
	ItemBoxManager*			m_pItemBoxManager;

	Obj_ItemManager*		m_pObjItemManager;
	//Obj_Platform Manager ����ǰ���������еĲ���̨
	PlatformManager*		m_pPlatformManager;

	//StallInfoManager		����ǰ�����а�̯���շѼ۸�
	StallInfoManager*		m_pStallInfoManager;

	SceneDropPosManager*    m_pDropPosManager;
	//Obj_Special Manager ����ǰ���������еķ���OBJ
	SpecialManager*			m_pSpecialManager;

	StoreManager*			m_pStoreManager;

	// ����OBJ
	BusManager*				m_pBusManager;
	
	// Event Zone Manager
	AreaManager*			m_pAreaManager;

	TimerDoingManager*		m_pTimerDoingManager;

	// Combat event ������
	EventCore_T				m_EventCore;

	GrowPointManager		m_GrowPointGroup;
	LuaInterface*           m_pLuaInterface;
	CFileDataMgr* 			m_pScriptFileMgr;
	SceneTimer*				m_pSceneTimers;
	PatrolPathMgr*			m_pPatrolPathMgr;
	ChatPipe*				m_pChatPipe;
	CMyTimer				m_QuitTimer ;
	CMyTimer				m_SceneTimer ;
	INT						m_SceneType ;//enum SCENE_TYPE
	CMyTimer				m_CopySceneQuitTimer ;

	MsgMediatorFactory		m_oMsgMediatorFacotry;
private :
	//��ǰ��������Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
	_100_PER_RANDOM_TABLE	m_Rand100 ; //ÿ���߳�Ψһ�������������
public:
	// �������Ĺ��ñ���
	Packet					*m_pPacket_NewPlayer;
	Packet					*m_pPacket_NewPlayer_Move;
	Packet					*m_pPacket_NewPlayer_Death;
	Packet					*m_pPacket_NewMonster;
	Packet					*m_pPacket_NewMonster_Move;
	Packet					*m_pPacket_NewMonster_Death;
	Packet					*m_pPacket_NewPet;
	Packet					*m_pPacket_NewPet_Move;
	Packet					*m_pPacket_NewPet_Death;
	Packet					*m_pPacket_NewPlatform;
	Packet					*m_pPacket_NewSpecial;
	Packet					*m_pPacket_NewItemBox;
	Packet					*m_nPacket_NewObj_Item;
	Packet					*m_pPacket_DelObject;
	Packet					*m_pPacket_NewBus_Move;
	Packet					*m_pPacket_NewBus;
	MisBuf					mMisBuf;
	_ITEM_LIST				mItemList;
	TID						m_ThreadID ;//�������̺߳�
	MISSION_DATA			*m_pMissionData;
	SCENE_LOAD				m_SceneLoad ;//�����еĳ�ʼ����Ϣ
	SCENE_INIT_DATA			m_SceneInitData ;//�����еĸ�����Ϣ
	CHAR					m_szMapName[_MAX_PATH] ;//��ͼ����
	ID_t					m_ClientRes ;//�ͻ���ʹ�õ���Դ��
	COPYSCENE_DATA			m_CopyData ;
	CITYSCENE_DATA			m_CityData;//���е���Ϣ
	SCENE_PERFOR			m_Perfor ;//�������
	X_PARAM					m_XParam ;//UICommand����
	HUMANLIST				m_aHumanList ;//���ڽű�����ɨ������
	_ObjID_List				m_ObjIDList ;//�û��ű�����ɨ������
};

#include "Scene.inl"


#endif
