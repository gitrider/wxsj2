

#include "stdafx.h"
#include "GamePlayer.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GUIDManager.h"
#include "TimeManager.h"
#include "Log.h"
#include "WGRetUserData.h"
#include "ServerManager.h"
#include "GWNotifyUser.h"

#include "GWTeamLeave.h"
#include "RecyclePlayerManager.h"

GamePlayer::GamePlayer( )
{
__ENTER_FUNCTION

	m_Status = PS_SERVER_EMPTY ;
//	m_GamePlayerSceneID = INVALID_ID ;
	m_KickTime = 0 ;
	m_HumanGUID = INVALID_ID ;

	m_LastSendTime	 = 0 ;
	m_KickTime		 = 0 ;
	m_CurrentTime	 = 0 ;
	m_LeftTimeToQuit = 0 ;

	m_pHuman = new Obj_Human ;
	m_pHuman->SetPlayer( this ) ;

	m_SaveTime = 0 ;

__LEAVE_FUNCTION
}

GamePlayer::~GamePlayer( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pHuman ) ;

__LEAVE_FUNCTION
}

VOID GamePlayer::CleanUp( )
{
__ENTER_FUNCTION

//	m_GamePlayerSceneID = INVALID_ID ;
	m_Status = PS_SERVER_EMPTY ;
	m_KickTime = 0 ;
	m_HumanGUID = INVALID_ID ;

	m_LastSendTime = 0 ;
	m_CurrentTime = 0 ;
	m_LeftTimeToQuit = 0 ;
	ResetKick( ) ;

	m_pHuman->CleanUp( ) ;

	return Player::CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL GamePlayer::ProcessCommand( BOOL Option )
{
__ENTER_FUNCTION

	return Player::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return Player::ProcessInput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return Player::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	return Player::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


BOOL GamePlayer::HeartBeat( UINT uTime, INT nFlag )
{
__ENTER_FUNCTION

	if( IsDisconnect() )
	{
		m_LeftTimeToQuit -= (uTime-m_CurrentTime) ;
		if( m_LeftTimeToQuit<0 )
		{//����ִ���˳�����
			return FALSE ;
		}
	}

	m_SaveTime -= (uTime-m_CurrentTime) ;
	if( m_SaveTime<0 && GetPlayerStatus()==PS_SERVER_NORMAL)
	{
		Save( ) ;
		m_SaveTime = 60000 ;
	}

	m_CurrentTime =uTime ;

	if( nFlag )
	{
		if(uTime>m_KickTime+MAX_INCOM_KICK_TIME )
		{
			Log::SaveLog( SERVER_LOGFILE, "ERROR: Incoming Player::HeartBeat Didn't recv message for too long time. Kicked!" ) ;
			return FALSE ;
		}
	}
	else
	{
		//���Player��һ��ʱ����û���յ��κ���Ϣ����Ͽ��ͻ�������
		//��Player������Ϣǰ��ִ��ResetKick��������m_KickTime��Ϣ
		if(uTime>m_KickTime+MAX_KICK_TIME )
		{
			Log::SaveLog( SERVER_LOGFILE, "ERROR: Player::HeartBeat Didn't recv message for too long time. Kicked!" ) ;
			return FALSE ;
		}
	}

	return Player::HeartBeat(uTime) ;

__LEAVE_FUNCTION

	return FALSE ;
}


BOOL GamePlayer::FreeOwn( )
{
__ENTER_FUNCTION

	ExitHuman( ) ;

	CleanUp( ) ;

	g_pPlayerPool->DelPlayer( PlayerID() ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	GamePlayer::ChooseFreeOwn(RecyclePlayerManager* pRecycler)
{
	__ENTER_FUNCTION
	
	Assert(pRecycler);
	
	if(g_pServerManager->IsEnableShareMem())
	{
		GUID_t		HumanGUID = GetHuman()->GetGUID();
		PlayerID_t	PlayerID  = this->PlayerID();
		if( pRecycler->AddPlayer(this) )
		{
			Log::SaveLog( RECYCLEPLAYER_LOGFILE, "Send FreePlayer CMD to SM,HumanGUID = %X,PlayerID = %d",HumanGUID,PlayerID);
			return	ReadyFreeOwn();
		}
		else
		{
			Log::SaveLog( RECYCLEPLAYER_LOGFILE, "ERROR Send FreePlayer CMD to SM, GUID=%X, PlayerID=%d",HumanGUID,PlayerID);
		}
	}
	else
	{
		return FreeOwn();
	}

	return FALSE ;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL GamePlayer::ReadyFreeOwn()
{
	__ENTER_FUNCTION
	
		if(m_pHuman)
		{
			m_pHuman->ValidateShareMem(TRUE);
			m_pHuman->GetDB()->SetFreeOwnFlag(SM_USE_READYFREE);
			return TRUE;
		}
		else
		{
			AssertEx(FALSE,"û�г�ʼ��Human����");
		}
	
		return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}

INT	GamePlayer::GetFreeOwnFlag()
{
	__ENTER_FUNCTION
		
		if(m_pHuman)
			return m_pHuman->GetDB()->GetFreeOwnFlag();
		else
			AssertEx(FALSE,"û�г�ʼ��Human����");

		return -1;

	__LEAVE_FUNCTION

		return -1;
}
/*
BOOL GamePlayer::PlayerConnectLogic( UINT uTime )
{
__ENTER_FUNCTION

	uint curtime =uTime ;
	if( curtime-m_KickTime> MAX_KICK_TIME )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR: GamePlayer::PlayerConnectLogic Kicked!" ) ;
		return FALSE ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::DefaultLogic( )
{
__ENTER_FUNCTION
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
*/

VOID GamePlayer::Init( )
{
__ENTER_FUNCTION

	SetDisconnect( FALSE ) ;
	ResetKick() ;
	SetDirty( TRUE ) ;

__LEAVE_FUNCTION
}


BOOL GamePlayer::CheckKey( uint key )
{
__ENTER_FUNCTION
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::InitHuman( FULLUSERDATA* pData ,INT Type)
{
__ENTER_FUNCTION


	if( Type==UDR_USERDATA )
	{//���������ݷ�����ȡ������
		//memcpy( GetHuman()->GetDB()->m_dbHuman, &(pData->m_Human), sizeof(_HUMAN_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbEquip, &(pData->m_Equip), sizeof(_EQUIP_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbBag, &(pData->m_Bag), sizeof(_BAG_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbBank, &(pData->m_Bank), sizeof(_BANK_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbSkill, &(pData->m_Skill), sizeof(_SKILL_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbXinFa, &(pData->m_XinFa), sizeof(_XINFA_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbImpact, &(pData->m_Impact), sizeof(_IMPACT_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbAbility, &(pData->m_Ability), sizeof(_ABILITY_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbMission, &(pData->m_Mission), sizeof(_MISSION_DB_LOAD) ) ;
		//memcpy( GetHuman()->GetDB()->m_dbSetting, &(pData->m_Setting), sizeof(_SETTING_DB_LOAD) ) ;
		
		_OBJ_HUMAN_INIT initHuman;
		initHuman.m_pUseData	= pData;
		m_pHuman->Init( &initHuman ) ;
		g_pGUIDManager->Add( m_HumanGUID, m_pHuman ) ;

		m_pHuman->ValidateShareMem(TRUE);//forceAll ValidateShareMen
		m_pHuman->ValidatePlayerID();
		m_pHuman->GetDB()->SetFreeOwnFlag(SM_USE_HOLDDATA);
	
	}
	else if(Type == UDR_USERSERVERCRASH)//�������Ŀǰ�������
	{
		Assert(FALSE);
	}

	
	

	SetDirty( FALSE ) ;
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::ExitHuman( )
{
__ENTER_FUNCTION

	if( m_HumanGUID == INVALID_ID )
	{
		return FALSE ;
	}

	g_pGUIDManager->Del( m_HumanGUID ) ;

	if( !IsDirty( ) )
	{
		GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
		pMsg->SetGUID( m_HumanGUID ) ;
		pMsg->SetStatus( GWNotifyUser::NUS_REMOVE ) ;
		g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;
	}

//���ԣ������û�����
	if( !IsDirty( ) )
	{
		Save( ) ;
	}
//����
	
	m_pHuman->GetDB()->SetFreeOwnFlag(SM_USE_FREE);
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GamePlayer::IsCanLogic( )
{
	__ENTER_FUNCTION

		if( this->GetPlayerStatus() != PS_SERVER_NORMAL )
			return FALSE ;

	if( this->GetHuman() == NULL )
		return FALSE ;

	if( this->GetHuman()->IsActiveObj()==FALSE )
		return FALSE ;

	if( this->GetHuman()->getScene()==NULL )
		return FALSE ;

	return TRUE ;

	__LEAVE_FUNCTION
		return FALSE ;
}

VOID GamePlayer::Disconnect( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		m_LeftTimeToQuit = g_Config.m_ConfigInfo.m_DisconnectTime ;
		SetDisconnect( TRUE ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	Player::Disconnect( ) ;

__LEAVE_FUNCTION
}

VOID GamePlayer::ResetKick( )
{
__ENTER_FUNCTION

	m_KickTime = g_pTimeManager->CurrentSavedTime() ;

	Player::ResetKick( ) ;

__LEAVE_FUNCTION
}

VOID GamePlayer::Save( )
{
__ENTER_FUNCTION

	if( m_HumanGUID==INVALID_ID || g_pServerManager->IsEnableShareMem() )
		return ;

	try
	{
		CHAR szFileName[_MAX_PATH] ;
		sprintf( szFileName, "%sUSER-%d-%s.usr", g_Config.m_ConfigInfo.m_UserPath, (INT)m_HumanGUID ,m_pHuman->GetDB()->GetHumanDB()->m_Name) ;

		FILE* f = fopen( szFileName, "wb" ) ;

		if (f)
		{
		fwrite( m_pHuman->GetDB()->GetHumanDB(), sizeof(_HUMAN_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetEquipDB(), sizeof(_EQUIP_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetBag(), sizeof(_BAG_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetBankDB(), sizeof(_BANK_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetSkillDB(), sizeof(_SKILL_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetCooldownDB(), sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetXinFaDB(), sizeof(_XINFA_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetImpactDB(), sizeof(_IMPACT_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetAbilityDB(), sizeof(_ABILITY_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetMissionDB(), sizeof(_MISSION_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetSettingDB(), sizeof(_SETTING_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->m_dbPetList, sizeof(_PET_DB_LIST_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetRelationDB(), sizeof(_RELATION_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetPrivateInfoDB(), sizeof(_PRIVATE_INFO_DB_LOAD), 1, f ) ;
		fwrite( m_pHuman->GetDB()->GetSouXiaDB(), sizeof(_SOUXIA_DB_LOAD), 1, f ) ;	// add by gh 2010/05/10
		fwrite( m_pHuman->GetDB()->GetMountDB(), sizeof(_MOUNT_DB_LOAD), 1, f ) ;   // [7/8/2010 �¾���]

		fclose(f) ;
	}
	}
	catch(...)
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR: ExitHuman() Save User Data Error!" ) ;
	}

__LEAVE_FUNCTION
}
