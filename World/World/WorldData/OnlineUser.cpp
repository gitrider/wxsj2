
#include "stdafx.h"
#include "OnlineUser.h"
#include "WGRetUserData.h"

#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Team.h"
#include "Ini.h"

#include "GuildManager.h"

#include "WGTeamResult.h"
#include "WGTeamFollowList.h"
#include "WGTeamError.h"
#include "WLNotifyUser.h"
#include "WGNotifyUser.h"
#include "WLBillingEnd.h"


#include "SceneInfo.h"
#include "AllUser.h"
#include "WorldTable.h"
#include "TeamRecruitManager.h"
#include "GameStruct_Query.h"

#include "PlayerPool.h"

OnlineUser*	g_pOnlineUser = NULL;

OnlineUser::OnlineUser( )
{
__ENTER_FUNCTION

	for( int i=0; i<MAX_USER_DATA; i++ )
	{
		m_pUserData[i] = NULL;
	}
	m_iLeftUserDataCount = 0;
	m_iUserDataOffset = 0;
	m_UserCount = 0 ;
	m_HeartBeatTimer.CleanUp() ;

__LEAVE_FUNCTION
}

OnlineUser::~OnlineUser( )
{
__ENTER_FUNCTION

	for( int i=0; i<MAX_USER_DATA; i++ )
	{
		if( m_pUserData[i]==NULL )
		{
			Assert(FALSE);
		}
		else
		{
			SAFE_DELETE(m_pUserData[i]);
		}
	}

	m_iLeftUserDataCount = 0;
	m_iUserDataOffset = 0;

__LEAVE_FUNCTION
}

BOOL OnlineUser::Init( )
{
__ENTER_FUNCTION

	for( int i=0; i<MAX_WORLD_USER; i++ )
	{
		m_aUser[i].SetUserID( (ID_t)i );
	}

	for(INT i=0; i<MAX_USER_DATA; i++ )
	{
		m_pUserData[i] = new FULLUSERDATA;

		if( m_pUserData[i]==NULL )
		{
			Assert(FALSE);
			return FALSE;
		}
	}

	m_iLeftUserDataCount = MAX_USER_DATA;
	m_iUserDataOffset = 0;

	m_GUIDTable.InitTable( g_Config.m_ConfigInfo.m_nHashOnlineUserCount );
	m_NameTable.InitTable( g_Config.m_ConfigInfo.m_nHashOnlineUserCount, MAX_CHARACTER_NAME );
	m_AccTable.InitTable(MAX_USER_DATA*2,MAX_ACCOUNT_LENGTH);
	m_UserCount = 0 ;

	m_HeartBeatTimer.BeginTimer( 10000, 0 ) ;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

//此函数对性能要求很高，目前只是临时使用
USER* OnlineUser::FindUser( GUID_t guid )
{
__ENTER_FUNCTION

	USER* pUser = (USER*)(m_GUIDTable.Get(guid));
	//for( int i=0; i<MAX_WORLD_USER; i++ )
	//{
	//	if( m_aUser[i].UserStatus()==US_NONE )
	//		continue;

	//	if( m_aUser[i].m_GUID == guid )
	//		return &(m_aUser[i]);
	//}

	return pUser;

__LEAVE_FUNCTION

	return NULL;
}
//此函数对性能要求很高，目前只是临时使用
USER* OnlineUser::FindUser( const CHAR* szName )
{
__ENTER_FUNCTION

	USER* pUser = (USER*)(m_NameTable.Get((CHAR*)szName));
	//CHAR szTemp[MAX_CHARACTER_NAME];
	//memset( szTemp, 0, MAX_CHARACTER_NAME );
	//strncpy( szTemp, szName, MAX_CHARACTER_NAME-1 );

	//for( int i=0; i<MAX_WORLD_USER; i++ )
	//{
	//	if( m_aUser[i].UserStatus()==US_NONE )
	//		continue;

	//	if( strncmp(m_aUser[i].m_Name,szTemp,MAX_CHARACTER_NAME-1)==0 )
	//		return &(m_aUser[i]);
	//}

	return pUser;

__LEAVE_FUNCTION

	return NULL;
}

USER* OnlineUser::LoadUser( GUID_t guid, INT nCountry, INT nMenPai ,CHAR* szAccount)
{
__ENTER_FUNCTION

	USER* pUser = NewUser();
	if( pUser==NULL )
	{
		Assert(FALSE);
		return NULL;
	}

	FULLUSERDATA* pFullUserData = NewData();
	if( pFullUserData==NULL )
	{
		Assert(FALSE);
		return NULL;
	}


	pUser->CleanUp();
	pUser->SetFullUserData( pFullUserData );
	pUser->SetUserStatus( US_HOLDUSERDATA );
	pUser->ResetKickTime() ;
	pUser->SetGUID( guid );

	
	char szFileName[_MAX_PATH];
	sprintf( szFileName, "%sUSER-%d-%s.usr", g_Config.m_ConfigInfo.m_UserPath, (int)guid ,szAccount);

	FILE* f = fopen( szFileName, "rb" );
	if( f==NULL )
	{//没有数据文件，创建一个角色文件
		InitUserData( pFullUserData, guid , nCountry, nMenPai);		

		return pUser;
	}

	uint Ret;
	char buf[104800];
	setvbuf( f, buf, _IOFBF, sizeof( buf ) );

	Ret = (uint)fread( &(pFullUserData->m_Human), sizeof(_HUMAN_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Equip), sizeof(_EQUIP_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Bag), sizeof(_BAG_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Bank), sizeof(_BANK_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Skill), sizeof(_SKILL_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Cooldown), sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_XinFa), sizeof(_XINFA_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Impact), sizeof(_IMPACT_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Ability), sizeof(_ABILITY_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Mission), sizeof(_MISSION_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Setting), sizeof(_SETTING_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_PetList), sizeof(_PET_DB_LIST_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Relation), sizeof(_RELATION_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}
	Ret = (uint)fread( &(pFullUserData->m_PrivateInfo), sizeof(_PRIVATE_INFO_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_SouXia), sizeof(_SOUXIA_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	Ret = (uint)fread( &(pFullUserData->m_Mount), sizeof(_MOUNT_DB_LOAD), 1, f );
	if( Ret==0 )
	{
		Assert(FALSE);
		pUser->CleanUp();
		DelData(pFullUserData);
		fclose(f);
		return NULL;
	}

	fclose(f);

	return pUser;

__LEAVE_FUNCTION

	return NULL;
}

void OnlineUser::InitUserData( FULLUSERDATA* pData, GUID_t guid  ,  INT nCountry, INT nMenPai)
{
__ENTER_FUNCTION

	pData->CleanUp();
	pData->m_Human.m_GUID = guid;

	pData->m_Human.m_StartScene = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_ENTERSCENCE);
	pData->m_Human.m_Position.m_fX = (FLOAT)(g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_BORNX));
	pData->m_Human.m_Position.m_fZ = (FLOAT)(g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_BORNY));
	pData->m_Human.m_CampData.m_nCampID = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_CAMP);
	
	pData->m_Human.m_HP = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIHP);
	pData->m_Human.m_Rage = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIRAGE);
	pData->m_Human.m_nPneuma = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIPNEUMA);
	pData->m_Human.m_Level = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INILEVEL);
	
	pData->m_Human.m_MaxVigor = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_MAX_VIGOR);
	pData->m_Human.m_Vigor = pData->m_Human.m_MaxVigor;
	pData->m_Human.m_Exp = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIEXP);
	pData->m_Human.m_MenPai = nMenPai;
	pData->m_Human.m_Money = g_DefaultCharDataTbl.Get(nCountry, nMenPai, CHAR_INIMONEY);

	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_STR, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_CON, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_INT, 0);
	pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_DEX, 0);

	INT nSkillID = -1;
	for(int i=CHAR_INISKILL1; i<=CHAR_INISKILL1+MAX_INITSKILL_COUNT; ++i)
	{

		nSkillID = g_DefaultCharDataTbl.Get(nCountry, nMenPai, i);

		if(nSkillID == -1)
			break;
		else
		{
			pData->m_Skill.m_aSkill[i-CHAR_INISKILL1].m_nSkillID = nSkillID;
			++pData->m_Skill.m_Count;
		}
	}
    //add by gh for common skill 2010/08/02
	for(int i=CHAR_INICOMMON_SKILL1; i<=CHAR_INICOMMON_SKILL1+MAX_INIT_COMMON_SKILL_COUNT; ++i)
	{

		nSkillID = g_DefaultCharDataTbl.Get(nCountry, nMenPai, i);

		if(nSkillID == -1)
			break;
		else
		{
			pData->m_Skill.m_aSkill[pData->m_Skill.m_Count].m_nSkillID = nSkillID;
			++pData->m_Skill.m_Count;
		}
	}
		//pData->m_Ability.m_aABility[iSkillAbility].m_Level=f.ReadInt("ability", szLevel);
		//pData->m_Ability.m_aABility[iSkillAbility].m_Exp = f.ReadInt("ability", szExp);
	
	pData->m_bIsPasswdUnlock = FALSE;
	pData->m_Human.m_PostCode.m_PostCode = 1000010;

	//pData->m_Human.m_StartScene = f.ReadInt( "baseinfo", "scene" );
	////	pData->m_Human.m_StartScene = 4;

	//f.ReadText( "baseinfo", "pos_x", szTmp, 32 );
	//pData->m_Human.m_Position.m_fX = (FLOAT)(atof(szTmp));
	////	pData->m_Human.m_Possiton.m_fX = 138.3f;

	//f.ReadText( "baseinfo", "pos_z", szTmp, 32 );
	//pData->m_Human.m_Position.m_fZ = (FLOAT)(atof(szTmp));
	////	pData->m_Human.m_Possiton.m_fZ = 100.0f;

	//pData->m_Human.m_Sex = f.ReadInt( "baseinfo", "sex" );
	////	pData->m_Human.m_Sex = rand()%2;

	//pData->m_Human.m_HP = f.ReadInt( "baseinfo", "hp" );
	////	pData->m_Human.m_HP = 100000;

	//pData->m_Human.m_MP = f.ReadInt( "baseinfo", "mp" );
	////	pData->m_Human.m_MP = 100000;

	//pData->m_Human.m_Level = f.ReadInt( "baseinfo", "level" );
	////	pData->m_Human.m_Level = 1;

	//pData->m_Human.m_HairColor = 0;
	//pData->m_Human.m_PortraitID = pData->m_Human.m_Sex ? 7 : 0;

	//pData->m_Human.m_Vigor				= f.ReadInt( "baseinfo", "vigor" );				//活力
	//pData->m_Human.m_MaxVigor			= f.ReadInt( "baseinfo", "max_vigor" );			//活力上限
	//pData->m_Human.m_VigorRegeneRate	= f.ReadInt( "baseinfo", "vigor_regenerate" );	//活力恢复速度
	//pData->m_Human.m_Energy				= f.ReadInt( "baseinfo", "energy" );			//精力
	//pData->m_Human.m_MaxEnergy			= f.ReadInt( "baseinfo", "max_energy" );		//精力上限
	//pData->m_Human.m_EnergyRegeneRate	= f.ReadInt( "baseinfo", "energy_regenerate" );	//精力恢复速度

	//pData->m_Human.m_Exp = f.ReadInt( "baseinfo", "exp" );
	////	pData->m_Human.m_Exp = 44000000;

	//pData->m_Human.m_CampData.m_nCampID = f.ReadInt( "baseinfo", "camp" );
	////	pData->m_Human.m_Camp = 0;

	//pData->m_Human.m_MenPai = f.ReadInt( "baseinfo", "menpai" );
	////	pData->m_Human.m_MenPai = MATTRIBUTE_WUMENPAI;

	//pData->m_Human.m_Money = f.ReadInt( "baseinfo", "money" );

	//pData->m_Human.m_nGoodBadValue = 0;


	//int iTmp;

	//iTmp = f.ReadInt( "attribute", "str" );
	//pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_STR, iTmp );
	///*iTmp = f.ReadInt( "attribute", "spr" );
	//pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_SPR, iTmp );*/
	//iTmp = f.ReadInt( "attribute", "con" );
	//pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_CON, iTmp );
	//iTmp = f.ReadInt( "attribute", "int" );
	//pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_INT, iTmp );
	//iTmp = f.ReadInt( "attribute", "dex" );
	//pData->m_Human.m_BaseAttrLevel1.Set( CATTR_LEVEL1_DEX, iTmp );

	//iTmp = f.ReadInt( "skill", "number" );
	//pData->m_Skill.m_Count = iTmp;
	//iTmp>MAX_CHAR_SKILL_NUM?iTmp=MAX_CHAR_SKILL_NUM:NULL;

	//for( int i=0; i<pData->m_Skill.m_Count; i++ )
	//{
	//	char szKeyID[32];
	//	sprintf( szKeyID, "id%d", i );
	//	pData->m_Skill.m_aSkill[i].m_nSkillID = f.ReadInt( "skill", szKeyID );
	//}

	//iTmp = f.ReadInt( "xinfa", "number" );
	//iTmp>MAX_CHAR_XINFA_NUM?iTmp=MAX_CHAR_XINFA_NUM:NULL;
	//pData->m_XinFa.m_Count = iTmp;
	//for( int i=0; i<pData->m_XinFa.m_Count; i++ )
	//{
	//	char szKeyID[32], szKeyLevelTime[32];

	//	sprintf( szKeyID, "id%d", i );
	//	pData->m_XinFa.m_aXinFa[i].m_nXinFaID = f.ReadInt( "xinfa", szKeyID );

	//	sprintf( szKeyLevelTime, "level%d", i );
	//	pData->m_XinFa.m_aXinFa[i].m_nLevel = f.ReadInt( "xinfa", szKeyLevelTime );
	//}

	//strncpy( pData->m_Relation.m_szMood, "今天天气不错！", MOOD_DATA_SIZE-1 );
	//
	//INT iAbliCount = f.ReadInt( "ability", "count" );
	//for( INT i=0; i<iAbliCount; i++ )
	//{
	//	char szAbility[64] ;
	//	sprintf( szAbility, "abilityskill%d", i ) ;
	//	INT iSkillAbility = f.ReadInt("ability", szAbility ) ;
	//	char szLevel[64], szExp[64] ;
	//	sprintf( szLevel, "level%d", i ) ;
	//	sprintf( szExp, "exp%d", i ) ;
	//	pData->m_Ability.m_aABility[iSkillAbility].m_Level=f.ReadInt("ability", szLevel);
	//	pData->m_Ability.m_aABility[iSkillAbility].m_Exp = f.ReadInt("ability", szExp);
	//}

	////pData->m_Ability.m_aABility[1].m_Level=f.ReadInt("ability", "level1");
	////pData->m_Ability.m_aABility[1].m_Exp = f.ReadInt("ability", "exp1");
	////pData->m_Ability.m_aABility[2].m_Level=f.ReadInt("ability", "level2");
	////pData->m_Ability.m_aABility[2].m_Exp = f.ReadInt("ability", "exp2");
	////pData->m_Ability.m_aABility[3].m_Level=f.ReadInt("ability", "level3");
	////pData->m_Ability.m_aABility[3].m_Exp = f.ReadInt("ability", "exp3");
	////pData->m_Ability.m_aABility[4].m_Level=f.ReadInt("ability", "level4");
	////pData->m_Ability.m_aABility[4].m_Exp = f.ReadInt("ability", "exp4");
	////pData->m_Ability.m_aABility[5].m_Level=f.ReadInt("ability", "level5");
	////pData->m_Ability.m_aABility[5].m_Exp = f.ReadInt("ability", "exp5");
	////pData->m_Ability.m_aABility[6].m_Level=f.ReadInt("ability", "level6");
	////pData->m_Ability.m_aABility[6].m_Exp = f.ReadInt("ability", "exp6");
	////pData->m_Ability.m_aABility[7].m_Level=f.ReadInt("ability", "level7");
	////pData->m_Ability.m_aABility[7].m_Exp = f.ReadInt("ability", "exp7");
	////pData->m_Ability.m_aABility[8].m_Level=f.ReadInt("ability", "level8");
	////pData->m_Ability.m_aABility[8].m_Exp = f.ReadInt("ability", "exp8");
	////pData->m_Ability.m_aABility[9].m_Level=f.ReadInt("ability", "level9");
	////pData->m_Ability.m_aABility[9].m_Exp = f.ReadInt("ability", "exp9");
	////pData->m_Ability.m_aABility[10].m_Level=f.ReadInt("ability", "level10");
	////pData->m_Ability.m_aABility[10].m_Exp = f.ReadInt("ability", "exp10");
	////pData->m_Ability.m_aABility[11].m_Level=f.ReadInt("ability", "level11");
	////pData->m_Ability.m_aABility[11].m_Exp = f.ReadInt("ability", "exp11");
	////pData->m_Ability.m_aABility[12].m_Level=f.ReadInt("ability", "level12");
	////pData->m_Ability.m_aABility[12].m_Exp = f.ReadInt("ability", "exp12");

	//iTmp = f.ReadInt( "table", "number" );

	//for(INT i=0; i<iTmp; i++ )
	//{
	//	char szTmp[32];
	//	sprintf( szTmp, "peifang%d", i );
	//	int iSet = f.ReadInt( "table", szTmp );

	//	int iByte = iSet/8;
	//	iSet = iSet%8;

	//	pData->m_Ability.m_aPrescr[iByte] |= (1<<iSet);
	//}

	//iTmp = f.ReadInt( "setting", "count" );
	//for(INT i=0; i<iTmp; i++ )
	//{
	//	char szTmp0[32],szTmp1[32],szTmp2[32];
	//	sprintf( szTmp0, "settingindex%d", i );
	//	sprintf( szTmp1, "settingtype%d", i );
	//	sprintf( szTmp2, "settingdata%d", i );

	//	INT iIndex=f.ReadInt( "setting", szTmp0 );
	//	INT iType =f.ReadInt( "setting", szTmp1 ); ;
	//	INT iData =f.ReadInt( "setting", szTmp2 ); ;

	//	pData->m_Setting.m_aSetting[iIndex].m_SettingType=iType ;
	//	pData->m_Setting.m_aSetting[iIndex].m_SettingData=iData ;
	//}


	//pData->m_bIsPasswdUnlock = FALSE;


	////	pData->m_Ability.m_aPrescr[0] = 0xf;

__LEAVE_FUNCTION

}


BOOL OnlineUser::HeartBeat( DWORD dwTime )
{
	if( !m_HeartBeatTimer.CountingTimer(dwTime) )
	{
		return TRUE ;
	}

	for( INT i=0; i<MAX_WORLD_USER; i++ )
	{
		if( m_aUser[i].UserStatus() == US_NONE )
			continue ;

		m_aUser[i].HeartBeat( dwTime ) ;
	}

	return TRUE;
}

FULLUSERDATA* OnlineUser::NewData()
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_USER_DATA; i++ )
	{
		if( m_pUserData[m_iUserDataOffset]==NULL )
		{
			m_iUserDataOffset++;
			if( m_iUserDataOffset>=MAX_USER_DATA )
			{
				m_iUserDataOffset = 0;
			}
			continue;
		}


		FULLUSERDATA* pData = m_pUserData[m_iUserDataOffset];
		m_pUserData[m_iUserDataOffset] = NULL;
		m_iLeftUserDataCount --;


		m_iUserDataOffset++;
		if( m_iUserDataOffset>=MAX_USER_DATA )
		{
			m_iUserDataOffset = 0;
		}

		pData->CleanUp();

		return pData;
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

void OnlineUser::DelData(FULLUSERDATA* pData)
{
__ENTER_FUNCTION

	BOOL bDo = FALSE;
	for( INT i=0; i<MAX_USER_DATA; i++ )
	{
		m_iUserDataOffset --;
		if( m_iUserDataOffset<0 )
		{
			m_iUserDataOffset = MAX_USER_DATA-1;
		}

		if( m_pUserData[m_iUserDataOffset] != NULL )
		{
			continue;
		}

		m_pUserData[m_iUserDataOffset] = pData;
		bDo = TRUE;

		break;
	}

	Assert(bDo);

__LEAVE_FUNCTION
}

BOOL OnlineUser::Register( USER* pUser )
{
__ENTER_FUNCTION

	m_GUIDTable.Add( pUser->GetGUID(), pUser );
	m_NameTable.Add( pUser->GetName(), pUser );
	m_AccTable.Add(pUser->GetAccount(),pUser);
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL OnlineUser::UnRegister( USER* pUser )
{
__ENTER_FUNCTION

	m_GUIDTable.Remove( pUser->GetGUID() );
	m_NameTable.Remove( pUser->GetName() );
	m_AccTable.Remove(pUser->GetAccount());
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT OnlineUser::Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition )
{
__ENTER_FUNCTION

	Assert(szName);
	Assert(pPlayerList);

	USER* pUser;

	if ( bPreciseFlag )
	{ // 在线精确查找
		pUser = FindUser( szName );
		if ( pUser != NULL )
		{

			pPlayerList->AddPlayerInfo( pUser->GetUserSimpleData() );
			FINGER_PLAYER_INFO* pPlayerInfo;
			pPlayerInfo = pPlayerList->GetLastPlayerInfo();
			Assert( pPlayerInfo );
			pPlayerInfo->SetOnlineFlag( TRUE );
			pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );

			return pPlayerList->GetPlayerCount();
		}
	}
	else
	{
		INT i;
		for( i=nPosition; i<MAX_WORLD_USER; ++i )
		{
			if ( strstr( m_aUser[i].GetName(), szName ) != NULL )
			{
				pPlayerList->AddPlayerInfo( m_aUser[i].GetUserSimpleData() );
				FINGER_PLAYER_INFO* pPlayerInfo;
				pPlayerInfo = pPlayerList->GetLastPlayerInfo();
				Assert( pPlayerInfo );
				pPlayerInfo->SetOnlineFlag( TRUE );
				pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );
			}

			if ( pPlayerList->GetPlayerCount() >= FINGER_PLAYER_COUNT )
			{
				break;
			}
		}

		if ( i >= MAX_WORLD_USER )
		{ // 爆了
			pPlayerList->SetFingerFlag( FALSE );
		}
		else
		{
			pPlayerList->SetFingerFlag( TRUE );
			pPlayerList->SetPosition( i );
		}

		return pPlayerList->GetPlayerCount();
	}

__LEAVE_FUNCTION

	return 0;
}

UINT OnlineUser::Select( INT nMenPai, GuildID_t GuildID, CHAR Sex, INT m_nBottomLevel, INT m_nTopLevel,
					   RETURN_FINGER_PLAYERLIST* pPlayerList, INT nPosition )
{
__ENTER_FUNCTION

	Assert(pPlayerList);

	INT i;
	for( i=nPosition; i<MAX_WORLD_USER; ++i )
	{
		if ( nMenPai != INVALID_ID && nMenPai != m_aUser[i].GetMenpai() )
		{
			continue;
		}

		if ( GuildID != INVALID_ID && GuildID != m_aUser[i].GetGuildID() )
		{
			continue;
		}

		if ( Sex != INVALID_ID && Sex != m_aUser[i].GetSex() )
		{
			continue;
		}

		if ( m_nBottomLevel != INVALID_ID && m_nBottomLevel > m_aUser[i].GetLevel() )
		{
			continue;
		}

		if ( m_nTopLevel != INVALID_ID && m_nTopLevel < m_aUser[i].GetLevel() )
		{
			continue;
		}

		pPlayerList->AddPlayerInfo( m_aUser[i].GetUserSimpleData() );
		FINGER_PLAYER_INFO* pPlayerInfo;
		pPlayerInfo = pPlayerList->GetLastPlayerInfo();
		Assert( pPlayerInfo );
		pPlayerInfo->SetOnlineFlag( TRUE );
		pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );

		if ( pPlayerList->GetPlayerCount() >= FINGER_PLAYER_COUNT )
		{
			break;
		}
	}

	if ( i >= MAX_WORLD_USER )
	{ // 爆了
		pPlayerList->SetFingerFlag( FALSE );
	}
	else
	{
		pPlayerList->SetFingerFlag( TRUE );
		pPlayerList->SetPosition( i );
	}

	return pPlayerList->GetPlayerCount();

__LEAVE_FUNCTION

	return 0;
}

BOOL	OnlineUser::Select(INT iPostCode, 
						   PUSERVector& returnUSERVector, 
						   BOOL& bContinue, 
						   INT& iReturnPos, 
						   INT iPostions/* = 0*/)
{
__ENTER_FUNCTION
	INT i = 0;
	for(i=iPostions; i<m_UserCount; ++i){
		if(m_aUser[iPostions].GetFullUserData()->m_Human.m_PostCode.m_PostCode == iPostCode){
			returnUSERVector.Add(&m_aUser[i]);
		}
		if(returnUSERVector.GetSize() >= OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER){
			break;
		}
	}

	if(i >= m_UserCount)
		bContinue = FALSE;
	else
		bContinue = TRUE;

	iReturnPos = i;

	return (returnUSERVector.GetSize() > 0);
__LEAVE_FUNCTION
	return 0;

	// 这样的查询操作以后可能需要优化，例如按iPostCode分组的方式
	//INT iPushbackCount = 0;
	//INT iConditionCount = 0;
	//INT iSkipCount = iPageCount * OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER;
	//for(INT i=0; i<MAX_WORLD_USER; ++i){
	//	if(m_aUser[i].GetFullUserData()->m_Human.m_PostCode.m_PostCode == iPostCode){
	//		++iConditionCount;

	//		if(iConditionCount > iSkipCount){
	//			returnUSERVector.push_back(&m_aUser[i]);
	//			++iPushbackCount;
	//			if(iPushbackCount == OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER)
	//				break;
	//		}
	//	
	//	}
	//}

	//UINT uBeginIndex = iPageCount * OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER;
	//UserGroupByPostCode::iterator it = m_UserGroups.find(iPostCode);
	//if(it != m_UserGroups.end()){
	//	UINT uCount = 0;
	//	UINT uSize = (UINT)(it->second.size());
	//	if(uSize - uBeginIndex > OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER)
	//		bContinue = TRUE;
	//	else
	//		bContinue = FALSE;

	//	for(UINT i=uBeginIndex; i<uSize; ++i){
	//		returnUSERVector.push_back(it->second[i]);
	//		if(++uCount == OnlineUser_SameCity_PerPage::MAX_ONLINEUSER_NUMBER)
	//			break;
	//	}
	//}

	//return (!returnUSERVector.empty());
}

USER* OnlineUser::NewUser( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_WORLD_USER; i++ )
	{
		if( m_aUser[i].UserStatus()==US_NONE )
		{
			return &(m_aUser[i]);
		}
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

void OnlineUser::DelUser( USER* pUser )
{
__ENTER_FUNCTION

	if( pUser->GetFullUserData() )
	{
		//RemoveUserFromGroups(pUser);
		DelData( pUser->GetFullUserData() );	//modified by 王勇鸷
	}

	UnRegister( pUser );

	pUser->CleanUp();

__LEAVE_FUNCTION
}

void OnlineUser::RemoveUser( ID_t ServerID )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_WORLD_USER; i++ )
	{
		if( m_aUser[i].GetServerID() == ServerID )
		{
			DelUser( &(m_aUser[i]) );
		}
	}

__LEAVE_FUNCTION
}

void	OnlineUser::RemoveUser(ID_t ServerID, USER_STATUS UserUS )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_WORLD_USER; i++ )
	{
		if( m_aUser[i].GetServerID() == ServerID)
		{
			if(m_aUser[i].UserStatus() == UserUS)
			{
				DelUser( &(m_aUser[i]) );
			}	

		}
	}

__LEAVE_FUNCTION
}


void	OnlineUser::ChangeUserStatus(ID_t ServerID, USER_STATUS UserUS )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_WORLD_USER; i++ )
	{
		if( m_aUser[i].GetServerID() == ServerID )
		{
			m_aUser[i].SetUserStatus( UserUS );
		}
	}

__LEAVE_FUNCTION
}

BOOL OnlineUser::OnUserChangeScene( USER* pUser, SceneID_t OldSceneID, SceneID_t NewSceneID )
{
__ENTER_FUNCTION

	if( pUser==NULL )
	{
		Assert(FALSE) ;
		return FALSE ;
	}
	if( NewSceneID==INVALID_ID )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	//
	g_pSceneInfo->DecScenePlayerCount( OldSceneID ) ;
	g_pSceneInfo->IncScenePlayerCount( NewSceneID ) ;

	//此用户属于某个队伍,需要通知
	if( pUser->GetTeamID() != INVALID_ID )
	{
		Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
		if( pTeam )
		{
			WGTeamResult Msg;
			Msg.SetReturn( TEAM_RESULT_STARTCHANGESCENE );
			Msg.SetGUID( pUser->GetGUID() );
			Msg.SetTeamID( pUser->GetTeamID() );
			Msg.SetSceneID( INVALID_ID );
			Msg.SetFamily( pUser->GetMenpai() );
			Msg.SetDataID( pUser->GetSex() );
			Msg.SetIcon( pUser->GetPortrait() );

			//通知所有组内的玩家
			for( INT i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i );
				if( pMember==NULL )
				{
					Assert(FALSE);
					continue;
				}

				if( pMember->m_Member == pUser->GetGUID() )
					continue;

				USER* pTMUser = FindUser( pMember->m_Member );
				if( pTMUser==NULL )
				{
					//AssertEx(FALSE, "离线队员。");
					continue;
				}

				ID_t ServerID = pTMUser->GetServerID();
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
				if( pServerPlayer==NULL )
				{
					Assert(FALSE);
					continue;
				}

				Msg.SetPlayerID( pTMUser->GetServerID() );

				pServerPlayer->SendPacket( &Msg );
			}//end for
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 玩家断线处理接口，fancy
VOID OnlineUser::OnUserOffLine( USER* pUser )
{
__ENTER_FUNCTION

	Assert( pUser );

	//通知和提升队长的动作稍后在UserMomentLeave中做了

	//if( pUser->GetTeamID() != INVALID_ID )
	//{ // 此玩家属于某个队伍，需要通知

	//	Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
	//	if( pTeam )
	//	{
	//		BOOL bLeaderFlag = FALSE;
	//		USER* pNewLeader = NULL;

	//		if( pTeam->Leader()->m_Member == pUser->GetGUID() )
	//		{ // 如果是队长，则顺序找一个不断线的玩家
	//			bLeaderFlag = TRUE;
	//		}

	//		WGTeamResult Msg;
	//		Msg.SetReturn( TEAM_RESULT_MEMBEROFFLINE );
	//		Msg.SetGUID( pUser->GetGUID() );
	//		Msg.SetTeamID( pUser->GetTeamID() );
	//		Msg.SetFamily( pUser->GetMenpai() );
	//		Msg.SetDataID( pUser->GetSex() );
	//		Msg.SetIcon( pUser->GetPortrait() );

	//		//通知所有组内的玩家
	//		for( INT i=0; i<pTeam->MemberCount(); i++ )
	//		{
	//			TEAMMEMBER* pMember = pTeam->Member( i );
	//			if( pMember==NULL )
	//			{
	//				Assert(FALSE);
	//				continue;
	//			}

	//			if( pMember->m_Member == pUser->GetGUID() )
	//				continue;

	//			if ( pMember->m_IsMomentLeave )
	//			{
	//				//队员暂时离队
	//				continue ;
	//			}

	//			USER* pTMUser = FindUser( pMember->m_Member );
	//			if( pTMUser==NULL )
	//			{
	//				AssertEx(FALSE, "离线队员。");
	//				continue;
	//			}

	//			if( pTMUser->UserStatus() != US_NORMAL )
	//			{
	//				continue;
	//			}

	//			if( bLeaderFlag && pNewLeader == NULL )
	//			{ // 新队长位置
	//				pNewLeader = pTMUser;
	//			}

	//			ID_t ServerID = pTMUser->GetServerID();
	//			ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
	//			if( pServerPlayer==NULL )
	//			{
	//				Assert(FALSE);
	//				continue;
	//			}

	//			Msg.SetPlayerID( pTMUser->GetPlayerID() );

	//			pServerPlayer->SendPacket( &Msg );
	//		}//end for

	//		if( bLeaderFlag && pNewLeader != NULL )
	//		{ // 提升新队长
	//			AppointTeamLeader(pUser, pNewLeader);
	//		}
	//	}
	//}

__LEAVE_FUNCTION
}

// 玩家退出处理接口，fancy
VOID OnlineUser::OnUserRemove( USER* pUser )
{
__ENTER_FUNCTION

	if( pUser==NULL )
	{
		Assert(FALSE) ;
		return ;
	}

	//场景人数变化设置
	g_pSceneInfo->DecScenePlayerCount( pUser->GetSceneID() );
	m_UserCount -- ;
	
	//--------------------------------------
	// add by wuwenbin
	//RemoveUserFromGroups(pUser);
	//--------------------------------------

	//此玩家属于某个队伍，需要通知
	if( pUser->GetTeamID() != INVALID_ID )
	{ 
		Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
		if( pTeam )
		{
			// 离开队伍
			//UserLeaveTeam( pUser );	//玩家退出 不离开队伍

			UserMomentLeave(pUser);		//玩家暂时离开队伍

			INT nFollowedMemberCount = pTeam->GetFollowedmemberCount();
			if( nFollowedMemberCount > 0 )
			{ // 可能处于组队跟随状态
				GUID_t* pMembers = pTeam->GetFollowedMembers();
				USER* pMember;
				ServerPlayer* pServerPlayer;

				if( pTeam->Leader()->m_Member == pUser->GetGUID() )
				{ // 队长
					for( INT i=1; i<nFollowedMemberCount; ++i )
					{ // 队长掉线了就不发了
						pMember = FindUser( pMembers[i] );
						if( pMember!=NULL && pMember->UserStatus()==US_NORMAL )
						{
							pServerPlayer = g_pServerManager->GetServerPlayer( pMember->GetServerID() );
							if( pServerPlayer==NULL )
							{
								Assert(FALSE);
								continue;
							}

							WGTeamFollowList FollowListMsg;
							FollowListMsg.SetPlayerID( pMember->GetPlayerID() );
							pServerPlayer->SendPacket( &FollowListMsg );
						}
					}

					pTeam->ClearFollowedMembers();
				}
				else
				{
					WGTeamFollowList FollowListMsg;

					pTeam->DelFollowedMember( pUser->GetGUID() );
					GUID_t* pMembers = pTeam->GetFollowedMembers();

					for( INT i=0; i<nFollowedMemberCount; ++i )
					{
						FollowListMsg.AddFollowMember(pMembers[i]);
					}

					for( INT i=0; i<nFollowedMemberCount; ++i )
					{
						pMember = FindUser( pMembers[i] );
						if( pMember!=NULL && pMember->UserStatus()==US_NORMAL )
						{
							pServerPlayer = g_pServerManager->GetServerPlayer( pMember->GetServerID() );
							if( pServerPlayer==NULL )
							{
								Assert(FALSE);
								continue;
							}

							FollowListMsg.SetPlayerID( pMember->GetPlayerID() );
							pServerPlayer->SendPacket( &FollowListMsg );
						}
					}
				}
			}
		}
	}

	GuildID_t guildID = pUser->GetGuildID();
	if( guildID != INVALID_ID )
	{
		Guild* pGuild = g_pGuildManager->GetGuild( guildID );
		pGuild->OnUserLogoff( pUser->GetGUID() );
	}

	//从组队征兵列表中删除该玩家对应数据
	g_pTeamRecruitManager->Remove( pUser->GetGUID(),MEMBER_RECRUIT );

	//////////////////////////////////////////////////////////////////////////
	//通知自己所有好友下线信息
	//////////////////////////////////////////////////////////////////////////
	pUser->InformOfflineMsg();

__LEAVE_FUNCTION
}

BOOL OnlineUser::OnUserLogin( USER* pUser )
{
__ENTER_FUNCTION

	_HUMAN_DB_LOAD* pDBHuman = &(pUser->GetFullUserData()->m_Human);
	_BANK_DB_LOAD*	pDBBank	 = &(pUser->GetFullUserData()->m_Bank);			

	//设置用户角色名称
	pUser->SetName( pDBHuman->m_Name );

	//将用户所在的场景号记录到用户数据里
	pUser->SetSceneID( pDBHuman->m_StartScene );

	pUser->SetCountry( pDBHuman->m_nCountry);
	pUser->SetMenpai( pDBHuman->m_MenPai );
	pUser->SetPortrait( pDBHuman->m_PortraitID );
	pUser->SetFaceModel( pDBHuman->m_FaceModel );
	pUser->SetHairModel( pDBHuman->m_HairModel );
	pUser->SetHairColor( pDBHuman->m_HairColor );
	pUser->SetLevel( pDBHuman->m_Level );
	pUser->SetSex( pDBHuman->m_Sex );

	pUser->SetTitle( pDBHuman->m_Title );
	pUser->SetMoney( pDBHuman->m_Money );
	pUser->SetBankMoney( pDBBank->m_Money );

	//所有登陆过游戏的人都会被添加进全用户信息库，用于查询
	g_pAllUser->AddUser( pUser->GetUserSimpleData() ) ;

	Register( pUser );

	/////////////////////////////////////////////////////////////////////////////////
	g_pSceneInfo->IncScenePlayerCount(pUser->GetSceneID());
	m_UserCount++;

	//--------------------------------------
	// add by wuwenbin
	//AddUserToGroups(pUser);
	//--------------------------------------

	/////////////////////////////////////////////////////////////////////////////////
	if( pDBHuman->m_GuildID != INVALID_ID )
	{ // 判断一下玩家是否属于一个帮会
		Guild* pGuild = g_pGuildManager->GetGuild( pDBHuman->m_GuildID );

		if( pGuild == NULL )
		{
			pDBHuman->m_GuildID = INVALID_ID;
		}
		else
		{
			const GUILDUSER* pGuildUser = pGuild->GetGuildUser( pUser->GetGUID() );

			if( pGuildUser == NULL )
			{ // 该玩家的帮会 ID 号无效
				pDBHuman->m_GuildID = INVALID_ID;
			}
			else
			{
				pUser->SetGuildID( pDBHuman->m_GuildID );
			}
		}
	}


	/////////////////////////////////////////////////////////////////////////////////
	GuildID_t guildID = pUser->GetGuildID();
	if( guildID != INVALID_ID )
	{
		Guild* pGuild = g_pGuildManager->GetGuild( guildID );

		Assert( pGuild ); // 这种情况应该已经被处理过了，不应该出现

		pGuild->OnUserLogin( pUser->GetGUID() );
	}

	//////////////////////////////////////////////////////////////////////////
	// 友好度大于一定值的玩家存储下来，给亲密好友信息通知
	//////////////////////////////////////////////////////////////////////////
	_RELATION_DB_LOAD* pRelation = &(pUser->GetFullUserData()->m_Relation);
	GUID_t RelationGUID;

	// 玩家有可能是断线重上，所以这里需要清楚一下，否则一些增加操作执行有问题，特别是 IDTable
	pUser->CleanUpRelationData();

	pUser->SetSettings( pUser->GetFullUserData()->m_Setting.m_aSetting[SETTING_TYPE_GAME].m_SettingData & 0xffff );

	// OnlineUser 的玩家心情赋值，以后改成从数据库中读取
	pUser->SetMood( pRelation->m_szMood );

	for( INT i = RELATION_FRIEND_OFFSET; i < MAX_RELATION_SIZE; ++i )
	{ // 将名单填好
		RelationGUID = pRelation->m_aRelation[i].m_Member.m_MemberGUID;
		if( RelationGUID != INVALID_ID )
		{
			_RELATION_USER RUser;
			RUser.CleanUp();
			RUser.m_UserGUID = RelationGUID;
			RUser.m_RelationType = (RELATION_TYPE)pRelation->m_aRelation[i].m_Type;
			RUser.m_nFriendPoint = pRelation->m_aRelation[i].m_FriendPoint;
			memcpy(RUser.m_szGroupName,pRelation->m_aRelation[i].m_GroupName,sizeof(RUser.m_szGroupName));
			pUser->AddRelation( RelationGUID, &RUser );
		}
	}

	pUser->InformOnlineMsg();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

//玩家暂时离开队伍
BOOL OnlineUser::UserMomentLeave(USER* pUser)
{
__ENTER_FUNCTION

	Assert( pUser );

	TeamID_t tid = pUser->GetTeamID();
	GUID_t guid = pUser->GetGUID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	if( pTeam==NULL )
	{
		return FALSE;
	}

	//将玩家暂时离开队伍
	TEAMMEMBER Member;
	Member.m_Member = guid;
	pTeam->SetIsMomentLeave(&Member,TRUE);

	//设置玩家队伍数据
	ServerPlayer* pServerPlayer;
	pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );

	BOOL TeamEmpty = TRUE;
	for( int i=0; i<pTeam->MemberCount(); i++ )
	{
		if ( pTeam->GetIsMomentLeave(i) == FALSE)	
		{
			TeamEmpty = FALSE;
		}
	}

	if( TeamEmpty == TRUE )
	{
		//当前队伍中的人都离开队伍了
		WGTeamResult Msg;
		Msg.SetPlayerID( pUser->GetPlayerID() );
		Msg.SetReturn( TEAM_RESULT_TEAMDISMISS );
		Msg.SetTeamID( tid );
		Msg.SetFamily( pUser->GetMenpai() );
		Msg.SetDataID( pUser->GetSex() );
		Msg.SetIcon( pUser->GetPortrait() );

		pServerPlayer->SendPacket( &Msg );

		//清除队伍信息
		g_pTeamList->DestoryTeam( tid );

		return TRUE;
	}

	BOOL bLeaderFlag = FALSE;
	USER* pNewLeader = NULL;

	if( pTeam->Leader()->m_Member == pUser->GetGUID() )
	{ // 如果是队长，则顺序找一个不断线的玩家
		bLeaderFlag = TRUE;
	}

	WGTeamResult Msg;
	Msg.SetReturn( TEAM_RESULT_MEMBEROFFLINE );
	Msg.SetGUID( pUser->GetGUID() );
	Msg.SetTeamID( pUser->GetTeamID() );
	Msg.SetFamily( pUser->GetMenpai() );
	Msg.SetDataID( pUser->GetSex() );
	Msg.SetIcon( pUser->GetPortrait() );

	//通知所有组内的玩家
	for( INT i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		if( pMember->m_Member == pUser->GetGUID() )
			continue;

		if ( pMember->m_IsMomentLeave )
		{
			//队员暂时离队
			continue ;
		}

		USER* pTMUser = FindUser( pMember->m_Member );
		if( pTMUser==NULL )
		{
			AssertEx(FALSE, "离线队员。");
			continue;
		}

		if( pTMUser->UserStatus() != US_NORMAL )
		{
			continue;
		}

		if( bLeaderFlag && pNewLeader == NULL )
		{ // 新队长位置
			pNewLeader = pTMUser;
		}

		ID_t ServerID = pTMUser->GetServerID();
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		if( pServerPlayer==NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pTMUser->GetPlayerID() );

		pServerPlayer->SendPacket( &Msg );
	}//end for

	if( bLeaderFlag && pNewLeader != NULL )
	{ // 提升新队长
		AppointTeamLeader(pUser, pNewLeader);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;

}//UserMomentLeave end


// 玩家离开队伍接口，fancy
BOOL OnlineUser::UserLeaveTeam( USER* pUser )
{
__ENTER_FUNCTION

	Assert( pUser );

	TeamID_t tid = pUser->GetTeamID();
	GUID_t guid = pUser->GetGUID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	if( pTeam==NULL )
	{
		return FALSE;
	}

	GUID_t LeaderGuid = pTeam->Leader()->m_Member;

	//将玩家脱离队伍
	TEAMMEMBER Member;
	Member.m_Member = guid;
	pTeam->DelMember( &Member );

	//设置玩家队伍数据
	pUser->SetTeamID( INVALID_ID );

	ServerPlayer* pServerPlayer;

	pServerPlayer = g_pServerManager->GetServerPlayer( pUser->GetServerID() );

	if( pTeam->MemberCount()==0 )
	{//当前队伍中的人都离开队伍了
		WGTeamResult Msg;
		Msg.SetPlayerID( pUser->GetPlayerID() );
		Msg.SetReturn( TEAM_RESULT_TEAMDISMISS );
		Msg.SetTeamID( tid );
		Msg.SetFamily( pUser->GetMenpai() );
		Msg.SetDataID( pUser->GetSex() );
		Msg.SetIcon( pUser->GetPortrait() );

		pServerPlayer->SendPacket( &Msg );

		//清除队伍信息
		g_pTeamList->DestoryTeam( tid );

		return TRUE;
	}

	WGTeamResult Msg;
	if( LeaderGuid == guid )
	{//离开的人是队长
		Msg.SetReturn( TEAM_RESULT_LEADERLEAVETEAM );
		Msg.SetGUID( guid );							//离开队伍的人
		Msg.SetGUIDEx( pTeam->Leader()->m_Member );	//新队长
		Msg.SetTeamID( tid );
	}
	else
	{//离开的人是普通成员
		Msg.SetReturn( TEAM_RESULT_MEMBERLEAVETEAM );
		Msg.SetGUID( guid );							//离开队伍的人
		Msg.SetTeamID( tid );
	}

	Msg.SetFamily( pUser->GetMenpai() );
	Msg.SetDataID( pUser->GetSex() );
	Msg.SetIcon( pUser->GetPortrait() );
	Msg.SetPlayerID( pUser->GetPlayerID() );

	pServerPlayer->SendPacket( &Msg ); // 通知已经离开队伍的玩家

	//通知所有组内的玩家
	for( INT i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pUser==NULL )
		{//如果队员离线,则用户数据是空
			continue;
		}

		ID_t ServerID = pUser->GetServerID();
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		if( pServerPlayer==NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pUser->GetPlayerID() );

		pServerPlayer->SendPacket( &Msg );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 提升新队长，fancy
BOOL OnlineUser::AppointTeamLeader( USER* pOldLeader, USER* pNewLeader )
{
__ENTER_FUNCTION

	Assert(pOldLeader);
	Assert(pNewLeader);

	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( pOldLeader->GetServerID() );

	TeamID_t tid = pOldLeader->GetTeamID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	Assert( pTeam );

	if( pTeam->Leader()->m_Member != pOldLeader->GetGUID() )
	{//旧队长不是队长了
		if( pServerPlayer!=NULL )
		{
			WGTeamError Msg;
			Msg.SetPlayerID( pOldLeader->GetPlayerID() );
			Msg.SetErrorCode(TEAM_ERROR_APPOINTSOURNOLEADER);
			pServerPlayer->SendPacket( &Msg );
		}

		return FALSE;
	}

	TEAMMEMBER Member;
	Member.m_Member = pNewLeader->GetGUID();
	pTeam->Appoint( &Member );

	WGTeamResult Msg;
	Msg.SetReturn( TEAM_RESULT_TEAMAPPOINT );
	Msg.SetGUID( pOldLeader->GetGUID() );
	Msg.SetGUIDEx( pNewLeader->GetGUID() );

	//通知所有组内的玩家
	for( INT i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}
		
		if ( pMember->m_IsMomentLeave == TRUE )
		{
			//玩家暂时离队
			continue;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pUser==NULL )
		{
			Assert(FALSE);
			continue;
		}

		// 如果玩家是离线状态，则不发送数据包
		if ( pUser->UserStatus() == US_OFFLINE )
		{
			continue;
		}

		ID_t ServerID = pUser->GetServerID();
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		if( pServerPlayer==NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pUser->GetPlayerID() );

		pServerPlayer->SendPacket( &Msg );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
//
//BOOL OnlineUser::FindUserFromGroups(USER* pUser)
//{
//__ENTER_FUNCTION
//
//	if(pUser){
//		const FULLUSERDATA* pFullUserData = pUser->GetFullUserData();
//
//		Assert(pFullUserData);
//
//		PostCode iPostCode = pFullUserData->m_Human.m_PostCode.m_PostCode;
//		UserGroupByPostCode::iterator it = m_UserGroups.find(iPostCode);
//		if(it != m_UserGroups.end())
//			return TRUE;
//	}
//	return FALSE;
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//
//}

//BOOL OnlineUser::AddUserToGroups(USER* pUser)
//{
//	if(pUser){
//		const FULLUSERDATA* pFullUserData = pUser->GetFullUserData();
//		Assert(pFullUserData);
//
//		PostCode iPostCode = pFullUserData->m_Human.m_PostCode.m_PostCode;
//		UserGroupByPostCode::iterator it = m_UserGroups.find(iPostCode);
//		if(it != m_UserGroups.end()){
//			it->second.push_back(pUser);
//		}
//		else{
//			PUSERVector UserVector;
//			UserVector.push_back(pUser);
//			std::pair<PostCode, PUSERVector> value(pFullUserData->m_Human.m_PostCode.m_PostCode, UserVector);
//			m_UserGroups.insert(value);
//		}
//		return TRUE;
//	}
//	return FALSE;
//}

//BOOL OnlineUser::RemoveUserFromGroups(USER* pUser)
//{
//	if(pUser){
//		const FULLUSERDATA* pFullUserData = pUser->GetFullUserData();
//		Assert(pFullUserData);
//
//		UserGroupByPostCode::iterator it = m_UserGroups.find(pFullUserData->m_Human.m_PostCode.m_PostCode);
//		if(it != m_UserGroups.end()){
//			PUSERVector::iterator it2 =  it->second.begin();
//			for(; it2 != it->second.end(); ++it2){
//				if(*it2 == pUser){
//					it->second.erase(it2);
//					return TRUE;
//				}
//			}
//		}
//	}
//	return FALSE;
//}


USER*	OnlineUser::FindUserByAccount(const CHAR* szAccount)
{
	__ENTER_FUNCTION

		USER* pUser = (USER*)(m_AccTable.Get((CHAR*)szAccount));

	return pUser;

	__LEAVE_FUNCTION

		return NULL;
}


//////////////////////////////////////////////////////////////////////////
//踢人功能相关
//////////////////////////////////////////////////////////////////////////
BOOL OnlineUser::BeginKickUser(USER* pUser)
{
	__ENTER_FUNCTION
		Assert(pUser);
	if(pUser->UserStatus() == US_WAIT_KICK) return FALSE;
	if(pUser->IsKickFinish() == FALSE) return FALSE;	//冗余检查

	pUser->ClearKick();
	pUser->ResetSMU_KickTime();
	//结束计费包
	SendBillingEnd(pUser);
	//丢弃抽奖信息
	//AbandonPrize(pUser);

	//发送踢人信息包
	PlayerID_t* pPlayers = g_pServerManager->GetPlayers();
	for(UINT i = 0; i < g_pServerManager->GetPlayerNumber(); ++i)
	{
		if(INVALID_ID == pPlayers[i]) continue;
		ServerPlayer* pServerPlayer = g_pPlayerPool->GetPlayer(pPlayers[i]);

		if(pServerPlayer && pServerPlayer->GetServerData())
		{
			if(pServerPlayer->GetServerData()->m_Type == SERVER_LOGIN)
			{
				WLNotifyUser nLMsg;
				nLMsg.SetAccName(pUser->GetAccount());
				nLMsg.SetNotifyStatus(WNOTIFY_KICK_REQUEST);
				nLMsg.SetUserKey(0);

				pServerPlayer->SendPacket(&nLMsg);
				//添加为待踢服务器
				pUser->AddKick(pServerPlayer->PlayerID(), SK_KICK_SEND);

				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:OnlineUser::BeginKickUser Login id=%d Kick...User GUID=%X send", 
					pServerPlayer->PlayerID(), pUser->GetGUID());
			}
			else if(pServerPlayer->GetServerData()->m_Type == SERVER_GAME)
			{
				WGNotifyUser nGMsg;
				nGMsg.SetGUID(pUser->GetGUID());
				nGMsg.SetStatus(WGNotifyUser::NUS_WORLD_KICK_REQUEST);

				pServerPlayer->SendPacket(&nGMsg);
				//添加为待踢服务器
				pUser->AddKick(pServerPlayer->PlayerID(), SK_KICK_SEND);

				g_pLog->FastSaveLog(LOG_FILE_4, "KICK:OnlineUser::BeginKickUser GameServer id=%d Kick...User GUID=%X send", 
					pServerPlayer->PlayerID(), pUser->GetGUID());
			}
		}// end of if(pServerPlayer && pServerPlayer->GetServerData())
	}

	//设置为等待踢人
	pUser->SetUserStatus(US_WAIT_KICK);

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::IsKickUser(USER* pUser)
{
	__ENTER_FUNCTION
		Assert(pUser);
	if(pUser->UserStatus() == US_WAIT_KICK) return TRUE;
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::EndKickUser(USER* pUser, ID_t serverId, BYTE ret)
{
	__ENTER_FUNCTION
		Assert(pUser);
	if(pUser->UserStatus() != US_WAIT_KICK) return FALSE;

	//修改服务器的踢人状态
	pUser->ChangeKick((PlayerID_t)serverId, ret);
	if(FALSE == pUser->IsKickFinish()) return FALSE;

	//成功踢人后的处理
	ForceEndKickUser(pUser);

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::ForceEndKickUser(USER* pUser)
{
	__ENTER_FUNCTION
		Assert(pUser);

	g_pLog->FastSaveLog(LOG_FILE_4, "KICK:OnlineUser::ForceEndKickUser...User GUID=%X kick finish", 
		pUser->GetGUID());

	OnUserRemove(pUser);
	DelUser(pUser);

	g_nLeaveUserLastMinite ++ ;

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::SendBillingEnd(USER* pUser, BOOL bForce)
{
	__ENTER_FUNCTION
		Assert(pUser);

	if(FALSE == bForce)
	{
		return SendBillingEnd(pUser->GetAccount(),pUser->GetLevel());
	}
	else
	{
		return SendForceBillingEnd(pUser->GetAccount(),pUser->GetLevel());
	}
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::SendForceBillingEnd(const CHAR* pAccName, INT acclv)
{
	__ENTER_FUNCTION
		Assert(pAccName);

	WLBillingEnd	BillEndMsg;
	BillEndMsg.SetAccount(pAccName);
	BillEndMsg.SetCharLevel(acclv);
	BillEndMsg.SetEndTime((UINT)g_pTimeManager->GetANSITime());
	BillEndMsg.SetStartTime((UINT)g_pTimeManager->GetANSITime());
	BillEndMsg.SetFeeType(BET_FORCE);//强制结束

	ServerPlayer* pLoginPlayer = g_pServerManager->GetLoginPlayer();
	if(pLoginPlayer)
	{
		pLoginPlayer->SendPacket(&BillEndMsg);
	}

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::SendBillingEnd(const CHAR* pAccName,INT acclv)
{
	__ENTER_FUNCTION
		Assert(pAccName);

	WLBillingEnd	BillEndMsg;
	BillEndMsg.SetAccount(pAccName);
	BillEndMsg.SetCharLevel(acclv);
	BillEndMsg.SetEndTime((UINT)g_pTimeManager->GetANSITime());
	BillEndMsg.SetStartTime((UINT)g_pTimeManager->GetANSITime());
	BillEndMsg.SetFeeType(BET_NORMAL);//普通结束

	ServerPlayer* pLoginPlayer = g_pServerManager->GetLoginPlayer();
	if(pLoginPlayer)
	{
		pLoginPlayer->SendPacket(&BillEndMsg);
	}

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL OnlineUser::ReSendKickUser_LockByShareMem(USER* pUser)
{
	__ENTER_FUNCTION
		Assert(pUser);
	if(pUser->UserStatus() != US_WAIT_KICK) return FALSE;
	//是否已经踢人成功
	if(pUser->IsKickFinish())
	{
		SendBillingEnd(pUser);
		ForceEndKickUser(pUser);
		return FALSE;
	}

	//发送给所有GameServer踢人信息
	for(INT i = 0; i < OVER_MAX_SERVER; ++i)
	{
		SERVER_KICK* pKick = pUser->GetKick(i);
		Assert(pKick);
		if(pKick->m_ServerID == INVALID_ID) break;

		ServerPlayer* pServerPlayer = g_pPlayerPool->GetPlayer(pKick->m_ServerID);

		if(NULL == pServerPlayer)
		{//当机的服务器，忽略状态
			pKick->m_KickStatus = SK_KICK_SRV_CRUSH;
			continue;
		}
		if(pServerPlayer->GetServerData() && pServerPlayer->GetServerData()->m_Type != SERVER_GAME) continue;

		if(pKick->m_KickStatus == SK_KICK_WAIT_SHM)
		{
			WGNotifyUser nGMsg;
			nGMsg.SetGUID(pUser->GetGUID());
			nGMsg.SetStatus(WGNotifyUser::NUS_WORLD_KICK_REQUEST);

			pServerPlayer->SendPacket(&nGMsg);

			pKick->m_KickStatus = SK_KICK_SEND;
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}
//////////////////////////////////////////////////////////////////////////