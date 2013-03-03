
#include "StdAfx.h"
#include "World\WorldManager.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_Enter.h"
#include "Procedure\GamePro_Login.h"
#include "Procedure\GamePro_ChangeScene.h"
#include "Network\NetManager.h"
#include "CGEnterScene.h"
#include "GIException.h"
#include "CGAskChangeScene.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_PlayerNpc.h"
#include "Object\Logic\tripperobj\GMTripperObj_Resource.h"
#include "Object\Logic\tripperobj\GMTripperObj_transport.h"
#include "DBC\GMDataBase.h"
#include "Global.h"
#include "GIUtil.h"
#include "Event\GMEventSystem.h"
#include "GIVariable.h"
#include "interface\gmgameinterface.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\gmuidatapool.h"
#include "Action\GMActionSystem.h"
#include "Sound\GMSoundSystem.h"
#include "ShortestPath.h"

//#include "GIDataBase.h"
//#include "GIDBC_Struct.h"



KL_IMPLEMENT_DYNAMIC(CWorldManager, GETCLASS(tWorldSystem));

CWorldManager* CWorldManager::s_pMe = NULL;
CActivePosManager* CActivePosManager::s_pMe = NULL;


CWorldManager::CWorldManager()
{
	s_pMe = this;
	m_pActiveScene = NULL;
	m_pActiveManager = NULL;
	m_Station = WS_NOT_ENTER;
	m_idNextScene = INVALID_UID;
	m_nSearchRange = 1;
	m_nSceneID =0;
	m_nResID = 0;
}

CWorldManager::~CWorldManager()
{
	if(m_pActiveScene) 
		delete m_pActiveScene;
	m_pActiveScene = NULL;

	if( m_pActiveManager )
	{
		m_pActiveManager->Release();
		delete m_pActiveManager;
		m_pActiveManager = 0;
	}
}


VOID CWorldManager::Initial(VOID*)
{
	// ע���¼�����
	CEventSystem::GetMe()->RegisterEventHandle("ON_SCENE_TRANS",	_OnSceneTransEvent);
	CEventSystem::GetMe()->RegisterEventHandle("ON_SERVER_TRANS",	_OnSceneTransEvent);

	// �ҽӱ���ϵͳ
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED",	_OnVariableChangedEvent);
	
	m_pActiveManager = new CActivePosManager;
	if( m_pActiveManager )
		m_pActiveManager->Initial();

	DBC_DEFINEHANDLE(s_scenePos, DBC_SCENE_POS_DEFINE);
	g_ShortestPath.Initailize(s_scenePos->GetRecordsNum()*2+1, MAX_SCENE);
	for(UINT i=0; i<s_scenePos->GetRecordsNum(); i++)
	{
		SceneTransferData data;
		const _DBC_SCENE_POS_DEFINE* pLine = (_DBC_SCENE_POS_DEFINE*)s_scenePos->Search_LineNum_EQU(i);
		data.nSceneID = pLine->nSceneID;
		data.xPos = pLine->nXPos;
		data.yPos = pLine->nZPos;
		data.nDestSceneID = pLine->nDestSceneID;
		g_ShortestPath.AddSceneTransData(data);
	}
}

//��ǰ������ServerID
INT CWorldManager::GetActiveSceneID(void) const
{
	return m_nResID;
	//if(m_pActiveScene) 
	//	return m_pActiveScene->GetSceneDefine()->nServerID;
	//else 
	//	return 0;
}

BOOL CWorldManager::EnterScene(INT nSceneID, INT nCityLevel, INT nResID)
{
	//�����ڽ��볡��������
	if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcEnter) 
	{
		//KLThrow("Must enter scene at ENTER procedure");
		return FALSE;
	}
	
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);

	const _DBC_SCENE_DEFINE* pSceneDef = 0;

	//�������ҳ���
	bool bUserCity = (nCityLevel>=0);
	if(bUserCity)
	{
		//���ҳ��г���
		for(int i=0; i<(int)s_pSceneDBC->GetRecordsNum(); i++)
		{
			const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU(i);

			//Id�͵ȼ���ͬ
			if(pTempSceneDef->nServerID==nSceneID && pTempSceneDef->nCityLevel==nCityLevel)
			{
				pSceneDef = pTempSceneDef;
				break;
			}
		}
	}
	else
	{
		//���ҳ�������
		pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_First_Column_Equ(
							_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, nResID);
	}

	if(!pSceneDef)
	{
		//�Ƿ��ĳ���ID
		KLThrow(_T("(CWorldManager::EnterScene) Invalid scene ID(%d,%d)"), nResID, nCityLevel);
		return FALSE;
	}

	//�����뿪���еĳ���
	if(GetActiveScene()) 
	{
		//��ͬ�ĳ���
		//if(GetActiveSceneID() == pSceneDef->nLocalID) 
		//	return FALSE;

		//ɾ����ǰ����
		m_pActiveScene->LeaveScene();
		delete m_pActiveScene;	m_pActiveScene = NULL;
	}

	//�����µĳ���
	m_pActiveScene = new CScene(pSceneDef, bUserCity);

	//�����µĳ����� ���ؾ�̬���嶨��
	m_pActiveScene->Initial();

	//���볡��
	m_pActiveScene->EnterScene();

	//�������볡�����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_SCENE_TRANSED, m_pActiveScene->GetSceneDefine()->szSceneMap);

	m_Station = WS_ENTER;

	// ��ȡ�������͵�
	const tDataBase* pdbcbase = CGameProcedure::s_pDataBaseSystem->GetDataBase( DBC_SCENE_POS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();

	m_ScenePoslistObj.clear();
	m_FlashlistObj.clear();
	m_ActivelistObj.clear();

	MAP_POS_DEFINE data;
	for( int i = 0; i < nNum; i ++ )
	{
		// scene_position.tab
		const _DBC_SCENE_POS_DEFINE* pLine = (const _DBC_SCENE_POS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine->nSceneID == pSceneDef->nLocalID )
		{
			data.pos.x = float( pLine->nXPos );
			data.pos.z = float( pLine->nZPos );
			data.dwSceneID = pLine->nSceneID;
			data.Set_Pos_Name(pLine->szDestSceneName );
			m_ScenePoslistObj.push_back( data );
		}
	}

	m_nSceneID =nSceneID;
	m_nResID =  nResID;

	return TRUE;
}

BOOL CWorldManager::UpdateUserCity(int nNewLevel)
{
	// �Ƿ��Ѿ�����һ������
	if(!m_pActiveScene) 
		return FALSE;
	// ������ҳ���
	if(!(m_pActiveScene->IsUserCity()))
		return FALSE;

	// ���³�������
	if(!(m_pActiveScene->UpdateUserCity(nNewLevel))) 
		return FALSE;

	const _DBC_SCENE_DEFINE* pSceneDefine = m_pActiveScene->GetSceneDefine();

	//�����������µ��¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SCENE_UPDATED, pSceneDefine->szMiniMap);

	// ��ȡ�������͵�
	const tDataBase* pdbcbase = CGameProcedure::s_pDataBaseSystem->GetDataBase( DBC_SCENE_POS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();

	m_ScenePoslistObj.clear();
	m_FlashlistObj.clear();
	m_ActivelistObj.clear();

	MAP_POS_DEFINE data;
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_POS_DEFINE* pLine = (const _DBC_SCENE_POS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine->nSceneID == pSceneDefine->nLocalID )
		{
			data.pos.x = float( pLine->nXPos );
			data.pos.z = float( pLine->nZPos );
			data.Set_Pos_Name(pLine->szDestSceneName );
			m_ScenePoslistObj.push_back( data );
		}
	}

	return TRUE;
}

VOID CWorldManager::Tick(VOID)
{
	if(m_pActiveScene)
		m_pActiveScene->Tick();
}

VOID CWorldManager::Release(VOID)
{
	//�����뿪���еĳ���
	if(m_pActiveScene) 
	{
		m_pActiveScene->LeaveScene();
		delete m_pActiveScene;	m_pActiveScene = NULL;
	}
	m_pActiveScene = NULL;
}

///��Ϸ�������л�����
VOID CWorldManager::ChangeScene(INT idTargetScene, INT idTargetSceneResID, const fVector2& fvPos, INT nDirection)
{
	//��������������
	if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcMain) 
	{
		KLThrow("Must change scene at MAIN procedure");
		return;
	}

	//����������Ϣ
	CGAskChangeScene msg;
	msg.SetSourSceneID(GetActiveSceneID());
	msg.SetDestSceneID(idTargetScene);
	CNetManager::GetMe()->SendPacket(&msg);
	//���Ϸ���
	CNetManager::GetMe()->ProcessOutput();

	//Ŀ�곡����
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = 
		(const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_First_Column_Equ(
		_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, idTargetSceneResID);

	// 20100517 AddCodeBegin
	DBC_DEFINEHANDLE(pMappicDBC,   DBC_MAPPIC_DATA);	// ������ת��ͼƬ��
	DBC_DEFINEHANDLE(pChatHelpDBC, DBC_CHATHELP_DATA);	// ������ת��������ʾ��

	_DBC_MAPPIC_DATA*   pMappicDef   = NULL;
	_DBC_CHATHELP_DATA* pChatHelpDef = NULL;

	// ���ҳ�����תͼƬ
	pMappicDef = (_DBC_MAPPIC_DATA*)pMappicDBC->Search_First_Column_Equ( _DBC_MAPPIC_DATA::MAPPIC_SCENEID_COLUMN, idTargetScene );

	// �������������ת������ʾ
	INT nTipRecordCount = 0;
	INT nTipID			= 0;

	nTipRecordCount = (INT)pChatHelpDBC->GetRecordsNum();
	nTipID = ( (double) rand() / (double) RAND_MAX ) * nTipRecordCount + 1;

	pChatHelpDef = (_DBC_CHATHELP_DATA*)pChatHelpDBC->Search_First_Column_Equ( _DBC_CHATHELP_DATA::CHATHELP_ID_COLUMN, nTipID );

	// 20100517 AddCodeEnd

	// 20100517 ModifyCodeBegin
	//CGameProcedure::s_pEventSystem->PushEvent(GE_PLAYER_LEAVE_WORLD, pSceneDef ? pSceneDef->szName : "");
	// ֪ͨUI,�ر���Ӧ����
	CGameProcedure::s_pEventSystem->PushEvent(GE_PLAYER_LEAVE_WORLD, pSceneDef ? pSceneDef->szName : "", 
		pMappicDef ? pMappicDef->szPicName: "", pChatHelpDef ? pChatHelpDef->szTip : "" );
	// 20100517 ModifyCodeEnd

	//---
	//����״̬
	m_idNextScene = idTargetScene;
	m_fvNextPos = fvPos;
	m_fNextDir = (FLOAT)((nDirection%36)*2.0f*KLU_PI/36.0f);
	INT idDefaultSkill = CActionSystem::GetMe()->GetDefaultAction() ? CActionSystem::GetMe()->GetDefaultAction()->GetDefineID() : 0;

	CGameProcedure::s_pVariableSystem->SetAs_Int("DefaultSkill",idDefaultSkill);
//	CActionSystem::GetMe()->SaveAction();

	m_Station = WS_ASK_CHANGE_SCANE;
}

VOID CWorldManager::_OnSceneTransEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	KLAssert(pEvent);

	switch(pEvent->pEventDef->idEvent)
	{
	case GE_ON_SCENE_TRANS:
		// ת����볡������
		{
			// ��ͬһ�����������л�����.
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
			CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FROM_OTHER);
			
			CGameProcedure::SetActiveProc(CGameProcedure::s_pProcEnter);
		}
		break;

	case GE_ON_SERVER_TRANS:
		{
			CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);
			
			// �л�������.
			CGameProcedure::SetActiveProc(CGameProcedure::s_pProcChangeScene);
		}
		break;
	}

	CGameProcedure::s_pProcLogIn->LoginForChangeScene();
}

bool WINAPI CWorldManager::_GetTerrainHeight(float fX, float fZ, float& fY)
{
	//�Ƿ���״̬
	if(WS_ENTER != GetMe()->m_Station || !(GetMe()->GetActiveScene())) return false;
	
	//����ת�� gfx->game
	fVector3 fvGame;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		tGfxSystem::AX_GFX, fVector3(fX, 0, fZ), tGfxSystem::AX_GAME, fvGame);

	//����
	if(GetMe()->m_pActiveScene->GetMapHeight(fVector2(fvGame.x, fvGame.z), fY) == TRUE)
    {
        fVector3 fvGfx;

        //����ת�� game->gfx
        fvGame.y = fY;
	    CGameProcedure::s_pGfxSystem->Axis_Trans(
	    	tGfxSystem::AX_GAME, fvGame, tGfxSystem::AX_GFX, fvGfx);

        fY = fvGfx.y;
        return true;
    }

    fY = 0.0f;
    return false;
}

bool WINAPI CWorldManager::GetTerrainHeight_GamePos(float fX, float fZ, float& fY)
{
	//�Ƿ���״̬
	if(WS_ENTER != GetMe()->m_Station || !(GetMe()->GetActiveScene())) return false;

	//����
	if(GetMe()->m_pActiveScene->GetMapHeight(fVector2(fX, fZ), fY) == TRUE)
    {
        return true;
    }

    fY = 0.0f;
    return false;
}

VOID WINAPI CWorldManager::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(!GetMe()->GetActiveScene()) return;

	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		if(strVariable == CSoundSystemFMod::szBKSoundVar) 
		{
			((CScene*)GetMe()->GetActiveScene())->ControlBkgSound(bEnable);
		}
		else if(strVariable == CSoundSystemFMod::sz3DSoundVar)
		{
			((CScene*)GetMe()->GetActiveScene())->ControlEnvSound(bEnable);
		}
		else if(strVariable == CSoundSystemFMod::szBKVolumeVar)  // ������Ч������
		{
			((CScene*)GetMe()->GetActiveScene())->ControlBkgSoundVol();
		}
		else if(strVariable == CSoundSystemFMod::sz3DVolumeVar)  
		{
			((CScene*)GetMe()->GetActiveScene())->ControlEnvSoundVol();
		}
		else if ( strVariable == CSoundSystemFMod::szAllSoundDisable ) // �ر�/����ȫ����Ч		20100714 BLL
		{
			((CScene*)GetMe()->GetActiveScene())->ControlBkgSound( !bEnable );
			((CScene*)GetMe()->GetActiveScene())->ControlEnvSound( !bEnable );
		}
	}
}
///////////////////////////////////////////////////////////////////////
bool CWorldManager::CheckIsFriend( INT id )
{
	for( int i = 0; i < CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount(); i ++ )
	{
		const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex( i );
		if( pInfo )
		{
			if( pInfo->m_SceneID != m_pActiveScene->GetSceneDefine()->nServerID )
				continue;
			if( pInfo->m_OjbID == id )
				return true;

		}
	}
	return false;
}
void CWorldManager::AddFriend( CObject_PlayerOther* pObj )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_FriendlistObj.begin(); it != m_FriendlistObj.end(); it ++ )
	{
		if( it->nServerID == pObj->GetServerID() ) // ����Ƕ���
		{
			it->pos.x = pObj->GetPosition().x;
			it->pos.z = pObj->GetPosition().z;
		}
	}
}

void CWorldManager::UpdateMinimapData()
{
	//		m_PlayerlistObj.clear();			// ������
	//		m_PKlistObj.clear();				// PK����
	//		m_PKKilllistObj.clear();			// PK���������

	if( m_bAnimyNeedFresh )
	{
		m_AnimylistObj.clear();		// �����б�
		m_FriendlistObj.clear();	// �����б�

		m_FamilylistObj.clear();			// �����б�
		m_GuildlistObj.clear();				// ����б�
		m_HomelandPlayerlistObj.clear();	// ����������
		m_NoHomelandPlayerlistObj.clear();	// �Ǳ���������

		m_ExpObjlistObj.clear();	// �������б�
		m_ScenePoslistObj.clear();	// �������͵��б�

		m_ExpNPClistObj.clear();	// NPC�б�
		m_MissCompleteNpcObj.clear();	// ����NPC�б�(���)
		m_MissNoCompleteNpcObj.clear();	// ����NPC�б�(δ���)
		m_MissionNpcObj.clear();	    // ����NPC�б�(�ɽ�)

		m_PetlistObj.clear();		// ������б�

		MAP_POS_DEFINE data;

		m_pActiveManager->update();
		m_pActiveManager->GetActiveList( GetActiveSceneID(), &m_ActivelistObj );	// ������б�
		m_pActiveManager->GetFlashList(  GetActiveSceneID(), &m_FlashlistObj );		// ������б�


		// ���¶����б�
		for( int i = 0; i < CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount(); i ++ )
		{
			const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex( i );
			if( pInfo )
			{
				/*if( pInfo->m_SceneID != m_pActiveScene->GetSceneDefine()->nID )
					continue;
				*/

				// ���Լ�
				if( pInfo->m_OjbID == CObjectManager::GetMe()->GetMySelf()->GetServerID() )
					continue; 

				data.pos.x = pInfo->m_WorldPos.m_fX;
				data.pos.z = pInfo->m_WorldPos.m_fZ;
				data.Set_Name( pInfo->m_szNick );
				data.Set_Pos_Utf8Name( pInfo->m_szUtf8Nick );
				data.nServerID = pInfo->m_OjbID;
				data.dwSceneID = pInfo->m_SceneID;
				
				// �����б�
				m_FriendlistObj.push_back( data );
			}
		}

		// pk������


		CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();

		// �������ݶ���
		const _DBC_SCENE_DEFINE* pSceneDef = GetActiveScene()->GetSceneDefine();
		CScene* pActiveScene = (CScene*)GetActiveScene();
		
		INT nZoneX, nZoneZ;
		int nTmp = 0;
		nZoneX = pActiveScene->GetZoneX( pCharObj->GetPosition().x );
		nZoneZ = pActiveScene->GetZoneZ( pCharObj->GetPosition().z );


		// ���໮��һ�������е�ע�����

		// ����x��Χ
		for( INT nX = nZoneX - m_nSearchRange; nX <= nZoneX + m_nSearchRange; nX ++ )
		{
			if( nX < 0 )
				continue;
			if( nX >= pActiveScene->GetSizeX() ) 
				break;

			// ����y��Χ
			for( INT nZ = nZoneZ - m_nSearchRange; nZ <= nZoneZ + m_nSearchRange; nZ ++ )
			{
				if( nZ < 0 )
					continue;
				if( nZ >= pActiveScene->GetSizeZ() )
					break;

				// �ߵ�����˵���ǺϷ���zone
				CZone* pZone = pActiveScene->GetZone( nX, nZ );
				
				std::set<INT>::iterator it;
				for( it = pZone->GetRigisterObject()->begin(); it != pZone->GetRigisterObject()->end(); it++ )
				{
					tObject* pCurObj = CObjectManager::GetMe()->FindObject( ( *it ) );
					if( !pCurObj )
						continue;

					// ������Լ�����
					if( pCurObj == CObjectManager::GetMe()->GetMySelf() ) 
						continue;

					nTmp ++;
					
					// ����ǽ�ɫ
					if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_Character) ) )
					{
						// ��ϵ
						ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType(pCharObj, (CObject*)pCurObj );
						
						// ����Ѿ����ˣ��Ͳ������б��ˡ�
						if( ((CObject_Character*)( pCurObj ))->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == 
							 CObject_Character::CHARACTER_LOGIC_DEAD )
							continue;

						// ����ģ��
						if( (( CObject* )pCurObj)->GetFakeObjectFlag() == 1 )
							continue;


						std::list< MAP_POS_DEFINE >::iterator  it;
						bool bNeedReturn = false;
						
						// �����
						for( it = m_FlashlistObj.begin(); it != m_FlashlistObj.end(); it ++ )
						{
							MAP_POS_DEFINE data = (*it);
							if( data.nServerID == -1 )
							{
								// �������һ�����Ͳ�����
								if( strcmp( data.name , ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() ) == 0 )
								{
									bNeedReturn = true;
									break;
								}
							}
						}

						// ������б�
						for( it = m_ActivelistObj.begin(); it != m_ActivelistObj.end(); it ++ )
						{
							MAP_POS_DEFINE data = (*it);
							if( data.nServerID == -1 )
							{
								// �������һ�����Ͳ�����
								if( strcmp( data.name , ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() ) == 0 )
								{
									bNeedReturn = true;
									break;
								}
							}
						}

						if( bNeedReturn )
							continue;



						/* PK����
						{
							m_PKlistObj;

							m_PKObj.dwSceneID = GetActiveSceneID();
							m_PKObj.Set_Pos_Name( "pk����" );
							m_PKObj.nServerID = -1;
							m_PKObj.pos.x = fX;
							m_PKObj.pos.z = fY;
						}*/


						// ��������
						if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) 
						{
							// �����ͬһ��Ӫ��
							if( sCamp == RELATION_FRIEND )
							{
								// ����Ƕ���,����������Ѿ����ڵ�λ��
								if( CheckIsFriend( pCurObj->GetServerID() ) )
								{
									AddFriend( (CObject_PlayerOther*)pCurObj );
									continue;
								}

								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								data.Set_Pos_Utf8Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Utf8Name() );
								data.Set_Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );

								//080423
//								m_PlayerlistObj.push_back( data );

								// pk������
								{
									m_PKKilllistObj;
								}

								// ���
								{
									INT nMyGuild = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
									if( INVALID_ID != nMyGuild )
									{
										INT nGuild = ((CObject_Character*)(pCurObj))->GetCharacterData()->Get_Guild();
										if( INVALID_ID != nGuild )
										{
											// �����᲻��ͬ�Ͳ�����
											if( nMyGuild == nGuild )
											{
												// ��ȡ�����Ϣ
												GuildMemberInfo_t* pMember = NULL;
												INT nMyFamily = -1;
												INT nFamily = -1;
												
												pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex( nMyGuild );
												if( pMember )
													nMyFamily = pMember->m_iFamilyID;	// ��������

												pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex( nGuild );
												if( pMember )
													nFamily = pMember->m_iFamilyID;		// ��������


												// ͨ�������ж�
												if( -1 != nMyFamily && -1 != nFamily )
												{
													if( nMyFamily == nFamily )
													{
														// ��ӵ������б�
														m_FamilylistObj.push_back( data );
														continue;
													}
												}

												// ��ӵ�����б�
												m_GuildlistObj.push_back( data );
												continue;
											}
										}
									}
								}

								// �жϹ���
								{
									INT nCountry   = ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Country();
									INT nMyCountry = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Country();

									// ������Ϸ�
									if( nCountry < 0 || nCountry >= 4 ) 
										continue ;

									// ������
									if( nMyCountry == nCountry )
										m_HomelandPlayerlistObj.push_back( data );		// ����������
									// �Ǳ���
									else
										m_NoHomelandPlayerlistObj.push_back( data );	// �Ǳ���������
								}

							}
							// ����
							else if( sCamp == RELATION_ENEMY )
							{
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								data.Set_Pos_Utf8Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Utf8Name() );
								data.Set_Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								
								m_AnimylistObj.push_back( data );	
							}

							//else if( sCamp == CAMP_TYPE_TEAM )
							//{
							//	OutputDebugString( "���ȶ���\n");
							//}
						}
						// �����npc
						else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_PlayerNPC) ) )
						{
							if( ((CObject_PlayerNPC*)pCurObj)->IsDisplayBoard() == FALSE )
							{
								continue;
							}

							
							// ����ǳ���
							if( ((CObject_PlayerNPC*)pCurObj)->GetNpcType() == NPC_TYPE_PET )
							{
								// 20100731 �߻�������ʾ����ͼ���ˣ�ע�͵�
								//// ��ﻹû������
								//if( ((CObject_PlayerNPC*)pCurObj)->GetOwnerID() == INVALID_ID ) 
								//{
								//	data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								//	data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								//	data.Set_Pos_Utf8Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Utf8Name() );
								//	data.Set_Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								//	
								//	m_PetlistObj.push_back( data );	
								//}

								continue;
							}

							// ͬ��Ӫ����npc
							if( sCamp == RELATION_FRIEND ) 
							{
								STRING name = ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name();

								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								data.Set_Pos_Utf8Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Utf8Name() );
								data.Set_Name( name.c_str() );
					
								
								// �ɽ�����
								// �ɽ�δ�������
								// �ɽ����������

								// �жϸ�npc�����Ƿ����Ѿ�����ȼ������Ŀɽӵ�����
								MISSION_STATE ms = ( (CObject_PlayerNPC*)(pCurObj) )->GetNpcMissionState();
								switch( ms )
								{
								case MS_MISS:
									{
										m_MissionNpcObj.push_back( data );
										break;
									}
								case MS_HAVE_MISS_NO_DONE:
									{
										m_MissNoCompleteNpcObj.push_back( data );
										break;
									}
								case MS_HAVE_MISS_DONE:
									{
										m_MissCompleteNpcObj.push_back( data );
										break;
									}

								default:
									// �ޱ�ǣ� ��ͨ״̬
									m_ExpNPClistObj.push_back( data );

								}

								continue;
							}
							// ����
							else if( sCamp == RELATION_ENEMY )
							{
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								data.Set_Pos_Utf8Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Utf8Name() );
								data.Set_Name(((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								
								m_AnimylistObj.push_back( data );	
							}
						}

					} 
					// end if g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_Character) ) ) 
					// ������ǽ�ɫ��������Դ��
					else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CTripperObject_Resource) ) )
					{
						data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
						data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
						data.Set_Pos_Name(((CTripperObject_Resource*)pCurObj )->GetLifeAbility()->szName );
						
						m_ExpObjlistObj.push_back( data );	
						
						/*_snprintf( szInfo, "ExpObj %s����λ�� x:%f, y%f\n", data.name, data.pos.x, data.pos.y );
						OutputDebugString( szInfo );*/
					}
					// ���͵�
					else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CTripperObject_Transport ) ) )
					{
						data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
						data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
						_snprintf( data.name, 128, "���͵�" );
						m_ScenePoslistObj.push_back( data );
					}
					
				} // end for ( it = pZone->GetRigisterObject()->begin(); it != pZone->GetRigisterObject()->end(); it++ )

			} // end for( INT nZ = nZoneZ - m_nSearchRange; nZ <= nZoneZ + m_nSearchRange; nZ ++ )
		} // end for( INT nX = nZoneX - m_nSearchRange; nX <= nZoneX + m_nSearchRange; nX ++ )

		m_bAnimyNeedFresh = false;

	} // end if m_bAnimyNeedFresh

}
void CWorldManager::MinimapNeedFresh()
{
	m_bAnimyNeedFresh = true;
	return;
}
std::list< MAP_POS_DEFINE >* CWorldManager::GetObjectListByClass( INT nType )
{
	//	case :	return &m_PKKilllistObj;			// pk����
	//	case :	return &m_PKlistObj;				// pk����
	//return &m_PlayerlistObj; // OtherPlayer ������

	switch( nType )
	{
	case MINIMAP_TYPE_ANIMY:  return &m_AnimylistObj;		// Animy ����
	case MINIMAP_TYPE_TEAM:	  return &m_FriendlistObj;		// Teamate ����

	case MINIMAP_TYPE_FAMILY:	   return &m_FamilylistObj;				// �����б�	
	case MINIMAP_TYPE_GUILD:	   return &m_GuildlistObj;				// ����б�	
	case MINIMAP_TYPE_HOMELAND:	   return &m_HomelandPlayerlistObj;	    // ����������			
	case MINIMAP_TYPE_NOHOMELAND:  return &m_NoHomelandPlayerlistObj;	// �Ǳ���������

	case MINIMAP_TYPE_EXPOBJ:	return &m_ExpObjlistObj;		// ExpObj ������
	case MINIMAP_TYPE_ACTIVE:	return &m_ActivelistObj;		// ���� ������
	case MINIMAP_TYPE_SCENEPOS: return &m_ScenePoslistObj;		// ������ת��
	case MINIMAP_TYPE_FLASH:	return &m_FlashlistObj;			// �����

	case MINIMAP_TYPE_EXPNPC:	    return &m_ExpNPClistObj;		// ExpNPC ��ͨnpc
	case MINIMAP_TYPE_MISCOMNPC:	return &m_MissCompleteNpcObj;	// ����NPC�б�(���)
	case MINIMAP_TYPE_MISNOCOMNPC:	return &m_MissNoCompleteNpcObj;	// ����NPC�б�(δ���)
	case MINIMAP_TYPE_ACCEPTNPC:	return &m_MissionNpcObj;	    // ����NPC�б�(�ɽ�)	

	case MINIMAP_TYPE_PET:return &m_PetlistObj;				// �����
	
	default:
		break;
	}

	return NULL;
}

fVector3 CWorldManager::GetMyselfPos()
{
	CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
	return pCharObj->GetPosition();
}
int CWorldManager::GetMyselfDir()
{
	CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
	return INT( pCharObj->GetFaceDir() * 180.f / KLU_PI );
}

const CHAR* CWorldManager::GetSceneName( INT nSceneID )
{
	// ���ҳ�������
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_Index_EQU(nSceneID);
	if(!pSceneDef)
	{
		return NULL;
	}

	return pSceneDef->szName;
}
CActivePosManager::CActivePosManager()
{
	s_pMe = this;
	m_listActiveObj.clear();
	m_listFlashObj.clear();
	m_nDistance = 25;
}

CActivePosManager::~CActivePosManager()
{
}

void   CActivePosManager::Initial()
{
}
void   CActivePosManager::Release()
{
}
void  CActivePosManager::AddFlashPos( bool bOnNPC,const char* pPosName, float fX, float fY, int nSceneID )
{
	MAP_POS_DEFINE data;
	data.dwSceneID = nSceneID;
	data.Set_Pos_Name(pPosName );

	if( bOnNPC )
		data.nServerID = -1;
	else
		data.nServerID = -2;

	data.pos.x = fX;
	data.pos.z = fY;
	m_listFlashObj.push_back( data );

	char szInfo[ 128 ];
	if( bOnNPC )
		_snprintf( szInfo, 128, "���FlashPos -->  %s,���ŷ�ʽnpc", pPosName );
	else
		_snprintf( szInfo, 128, "���FlashPos -->  %s,���ŷ�ʽλ��", pPosName );
	
	AxTrace( 0,0, szInfo );

	CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);
}
void  CActivePosManager::AddActivePos( bool bOnNPC,const char* pPosName, float fX, float fY, int nSceneID )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	char szInfo[ 128 ];
	
	for( it = m_listActiveObj.begin(); it != m_listActiveObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
		{
			_snprintf( szInfo, 128, "���ActivePos -->  %s", data.name );
			AxTrace( 0,0, szInfo );
			m_listActiveObj.erase( it );
			break;
		}
	}

	MAP_POS_DEFINE data;
	data.dwSceneID = nSceneID;
	data.Set_Pos_Name(pPosName );
	if( bOnNPC )
		data.nServerID = -1;
	else
		data.nServerID = -2;
	data.pos.x = fX;
	data.pos.z = fY;

	m_listActiveObj.push_back( data );
	

	_snprintf( szInfo, 128, "���ActivePos -->  %s", pPosName );
	AxTrace( 0,0, szInfo );
	
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);
}
void   CActivePosManager::update()
{
	update( &m_listFlashObj );
	update( &m_listActiveObj);
}
void   CActivePosManager::update( POS_LIST* pList )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	fVector3 fAimPos( 0,0,0 );
	for( it = pList->begin(); it != pList->end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == CWorldManager::GetMe()->GetActiveSceneID() )
		{
			if( data.nServerID == -1 ) // ����ǰ���npc���ϵ�
			{
				CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName( data.name );
				if( pObj )
				{
					fAimPos = pObj->GetPosition();
					(*it).pos = fAimPos;
				}
			}
			else
			{
				fAimPos = data.pos;
			}
			float fX = fAimPos.x - fMyselfPos.x;
			float fY = fAimPos.z - fMyselfPos.z;
			if( ( fX * fX + fY * fY ) < m_nDistance ) // �������ʧ
			{
				std::list< MAP_POS_DEFINE >::iterator it2 = it ++;
				pList->erase( it2 );
			}
		}
	}
}
void CActivePosManager::GetFlashList( int nSceneID, POS_LIST* pReturnList )
{
	pReturnList->clear();
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_listFlashObj.begin(); it != m_listFlashObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
			pReturnList->push_back( data );
	}
}
void CActivePosManager::GetActiveList( int nSceneID, POS_LIST* pReturnList )
{
	pReturnList->clear();
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_listActiveObj.begin(); it != m_listActiveObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
			pReturnList->push_back( data );
	}
}