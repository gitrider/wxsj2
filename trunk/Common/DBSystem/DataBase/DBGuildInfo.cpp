#include "stdafx.h"
#include "DBGuildInfo.h"
#include "DBGuildExtraInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "GameStruct.h"
#include "SMUManager.h"

DBGuildInfo::DBGuildInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBGuildInfo::Load()
{
	__ENTER_FUNCTION


	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	if(!pQuery)
	{
		Assert(FALSE);
	}
	pQuery->Clear();

	pQuery->Parse(LoadWorldGuildInfo,GUILD_TABLE);

	return ODBCBase::LongLoad();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBGuildInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	//INT Result;
	//
	//SMUPool<GuildSMU>* pPoolPtr = static_cast<SMUPool<GuildSMU>*>(pSource);
	//Assert(pPoolPtr);
	////保存工会数据

	//GUILD_DB	SaveGuild;
	//do
	//{
	//	INT SmuCount = pPoolPtr->GetPoolMaxSize();
	//	if(SmuCount>MAX_GUILD_SIZE)
	//		Assert(FALSE);

	//	INT UseStats;
	//	for(INT i = 0 ;i<SmuCount;i++)
	//	{
	//		GuildSMU*	pSMU = pPoolPtr->GetPoolObj(i);
	//		if(!pSMU)
	//		{
	//			Assert(pSMU);
	//			return FALSE;
	//		}

	//		UseStats =pSMU->GetUseStatus(SM_C_READ);
	//	
	//		pSMU->Lock(SM_C_READ);
	//			GUILD_DB* pGuild = &(pSMU->m_GuildSM);
	//			if(pGuild == NULL)
	//			{
	//				pSMU->UnLock(SM_C_READ);
	//				Assert(FALSE);
	//			}
	//			
	//		memcpy(&SaveGuild,pGuild,sizeof(GUILD_DB));
	//		pSMU->m_SMUHead.UseStatus	= GUILD_SAVED;
	//		if(UseStats ==GUILD_DELETE)
	//		{
	//			pGuild->CleanUp();
	//		}

	//		pSMU->UnLock(SM_C_READ);

	//		if(SaveGuild.m_GuildID == INVALID_ID)
	//			continue;

	//		//继续存储
	//		mInterface->Clear();
	//		CheckDBConnect("DBGuildInfo");

	//		// 名称含有转义字符的直接跳过[6/4/2007]
	//		if(!StrSafeCheck(SaveGuild.m_GuildDesc,MAX_GUILD_DESC_SIZE_DB))
	//		{
	//			SaveGuild.m_GuildDesc[0]	=	'\0';
	//		}
	//		if(!StrSafeCheck(SaveGuild.m_szGuildLeaveWord,MAX_CHAT_SIZE))	
	//		{
	//			SaveGuild.m_szGuildLeaveWord[0]	=	'\0';
	//		}
	//			
	//		CHAR	AdminInfo[4096] = {0};
	//		
	//		Binary2String((CHAR*)(&SaveGuild.m_AdminLayout),
	//						sizeof(GUILD_ADMIN_t),
	//						AdminInfo);
	//		
	//		CHAR    GuildUser[MAX_LONG_COLUMN_BUFFER] = {0};

	//		Binary2String((CHAR*)(SaveGuild.m_aGuildUser),
	//					  sizeof(GUILDUSER_DB)*USER_ARRAY_SIZE,
	//					  GuildUser);
	//		
	//		
	//		LONG_DB_QUERY* pQuery = GetLongInterQuery();
	//		if(!pQuery)
	//		{
	//			Assert(FALSE);
	//		}
	//		pQuery->Clear();
	//		pQuery->Parse(SaveWorldGuildInfo,
	//					  SaveGuild.m_GuildID,
	//					  SaveGuild.m_GuildName,
	//					  SaveGuild.m_Status,
	//					  SaveGuild.m_ChieftainGUID,
	//					  SaveGuild.m_nProposerCount,
	//					  SaveGuild.m_UserCount,
	//					  SaveGuild.m_MaxUserSize,
	//					  SaveGuild.m_GuildPoint,
	//					  SaveGuild.m_GuildMoney,
	//					  SaveGuild.m_CityID,
	//					  SaveGuild.m_nTime,
	//					  SaveGuild.m_Longevity,
	//					  SaveGuild.m_Contribute,
	//					  SaveGuild.m_Honor,
	//					  SaveGuild.m_nIndustryLevel,
	//					  SaveGuild.m_nAgrLevel,
	//					  SaveGuild.m_nComLevel,
	//					  SaveGuild.m_nDefLevel,
	//					  SaveGuild.m_nTechLevel,
	//					  SaveGuild.m_nAmbiLevel,
	//					  AdminInfo,
	//					  SaveGuild.m_GuildDesc,
	//					  SaveGuild.m_GuildChiefName,
	//					  SaveGuild.m_GuildCreatorName,
	//					  SaveGuild.m_GuildLevel,
	//					  GuildUser,
	//					  ((UseStats!=GUILD_DELETE)?1:0),
	//					  SaveGuild.m_szGuildLeaveWord);

	//		if(!ODBCBase::LongSave(&Result))
	//			continue;

	//		mInterface->Clear();
	//		if(UseStats!=GUILD_DELETE)
	//		{
	//			//保存帮会ExtraInfo
	//			DBGuildExtraInfo ExtraInfoObject(mInterface);
	//			ExtraInfoObject.SetGuildID(SaveGuild.m_GuildID);
	//			if(!ExtraInfoObject.Save(SaveGuild.m_aExtraUserData))
	//				continue;
	//			ExtraInfoObject.ParseResult(&Result);
	//		}
	//		else
	//		{
	//			//清除帮会ExtraInfo
	//			DBGuildExtraInfo ExtraInfoObject(mInterface);
	//			ExtraInfoObject.SetGuildID(SaveGuild.m_GuildID);
	//			if(!ExtraInfoObject.Delete())
	//				continue;
	//			ExtraInfoObject.ParseResult(&Result);
	//		}
	//	}

	//}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBGuildInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}





BOOL DBGuildInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	//switch(mOPType)
	//{
	//case DB_LOAD:
	//	{
	//		SMUPool<GuildSMU>* pPoolPtr = static_cast<SMUPool<GuildSMU>*>(pResult);
	//		Assert(pPoolPtr);
	//		enum 
	//		{
	//			DB_GuildID	=	1,
	//			DB_GuildName,
	//			DB_Guildstat,
	//			DB_Chiefguid,
	//			DB_PCount,
	//			DB_UCount,
	//			DB_MUCount,
	//			DB_GPoint,
	//			DB_GuildMoney,
	//			DB_CityID,
	//			DB_Time,
	//			DB_Logvity,
	//			DB_Contribu,
	//			DB_Honor,
	//			DB_Indlvl,
	//			DB_Agrlvl,
	//			DB_Comlvl,
	//			DB_Deflvl,
	//			DB_Techlvl,
	//			DB_Amblvl,
	//			DB_Admin,
	//			DB_GuildDesc,
	//			DB_Chiefname,
	//			DB_Cname,
	//			DB_Glvl,
	//			DB_GuildMsg,		//请看ODBCInterface::LongFetch()里的严重注意！！！
	//			DB_GuildUser,
	//		};
	//		//加载Guild属性
	//		Assert(mInterface);
	//		INT ErrorCode;
	//		INT	SmuCount = pPoolPtr->GetPoolMaxSize();
	//		INT GuildIndex;

	//		GuildID_t	guildInfo[MAX_GUILD_SIZE];
	//		for(INT i =0;i<MAX_GUILD_SIZE;i++)	guildInfo[i] = INVALID_ID;

	//		for(INT i =0;i<MAX_GUILD_SIZE;i++)
	//		{
	//			if(!mInterface->LongFetch())
	//				break;

	//			if(i>= SmuCount)
	//				Assert(FALSE);

	//			GuildIndex = mInterface->GetUInt(DB_GuildID,ErrorCode);

	//			if(GuildIndex>= SmuCount)
	//				Assert(FALSE);

	//			GuildSMU*	pSMU = pPoolPtr->GetPoolObj(GuildIndex);
	//			if(!pSMU)
	//			{
	//				Assert(pSMU);
	//				return FALSE;
	//			}
	//			GUILD_DB* pGuild = &pSMU->m_GuildSM;
	//			Assert(pGuild);
	//			
	//			pGuild->m_GuildID			= mInterface->GetInt(DB_GuildID,ErrorCode);
	//			guildInfo[i]				= pGuild->m_GuildID;

	//			mInterface->GetString(DB_GuildName,pGuild->m_GuildName,
	//				MAX_GUILD_NAME_SIZE_DB,ErrorCode);
	//			pGuild->m_Status			= (GUILD_STATUS)mInterface->GetInt(DB_Guildstat,ErrorCode);
	//			pGuild->m_ChieftainGUID		= mInterface->GetUInt(DB_Chiefguid,ErrorCode);
	//			pGuild->m_nProposerCount	= mInterface->GetInt(DB_PCount,ErrorCode);
	//			pGuild->m_UserCount			= mInterface->GetInt(DB_UCount,ErrorCode);
	//			pGuild->m_MaxUserSize		= mInterface->GetInt(DB_MUCount,ErrorCode);
	//			pGuild->m_GuildPoint		= mInterface->GetInt(DB_GPoint,ErrorCode);
	//			pGuild->m_GuildMoney		= mInterface->GetInt(DB_GuildMoney,ErrorCode);
	//			pGuild->m_CityID			= mInterface->GetInt(DB_CityID,ErrorCode);
	//			pGuild->m_nTime				= mInterface->GetInt(DB_Time,ErrorCode);
	//			pGuild->m_Longevity			= mInterface->GetInt(DB_Logvity,ErrorCode);
	//			pGuild->m_Contribute		= mInterface->GetInt(DB_Contribu,ErrorCode);
	//			pGuild->m_Honor				= mInterface->GetInt(DB_Honor,ErrorCode);
	//			pGuild->m_nIndustryLevel	= mInterface->GetInt(DB_Indlvl,ErrorCode);
	//			pGuild->m_nAgrLevel			= mInterface->GetInt(DB_Agrlvl,ErrorCode);
	//			pGuild->m_nComLevel			= mInterface->GetInt(DB_Comlvl,ErrorCode);
	//			pGuild->m_nDefLevel			= mInterface->GetInt(DB_Deflvl,ErrorCode);
	//			pGuild->m_nTechLevel		= mInterface->GetInt(DB_Techlvl,ErrorCode);
	//			pGuild->m_nAmbiLevel		= mInterface->GetInt(DB_Amblvl,ErrorCode);
	//			
	//			
	//			mInterface->GetField(DB_Admin,(CHAR*)(&pGuild->m_AdminLayout),sizeof(GUILD_ADMIN_t),ErrorCode);

	//			mInterface->GetString(DB_GuildDesc,pGuild->m_GuildDesc,
	//								 MAX_GUILD_DESC_SIZE_DB,ErrorCode);

	//			mInterface->GetString(DB_Chiefname,pGuild->m_GuildChiefName,
	//							MAX_CHARACTER_NAME,ErrorCode);
	//			
	//			mInterface->GetString(DB_Cname,pGuild->m_GuildCreatorName,
	//									MAX_CHARACTER_NAME,ErrorCode);
	//	
	//			pGuild->m_GuildLevel =	mInterface->GetInt(DB_Glvl,ErrorCode);	

	//			mInterface->GetLongField(DB_GuildUser,(CHAR*)(pGuild->m_aGuildUser),
	//									 sizeof(GUILDUSER_DB)*USER_ARRAY_SIZE,
	//									 ErrorCode);

	//			mInterface->GetString(DB_GuildMsg,pGuild->m_szGuildLeaveWord,MAX_CHAT_SIZE,ErrorCode);

	//			pSMU->SetUseStatus(GUILD_SAVED,SM_C_WRITE);
	//		}
	//		mInterface->Clear();
	//		
	//		//加载帮会ExtraInfo
	//		for(INT i =0;i<MAX_GUILD_SIZE;i++)
	//		{
	//			if(INVALID_ID == guildInfo[i])	break;

	//			GuildIndex = guildInfo[i];

	//			GuildSMU*	pSMU = pPoolPtr->GetPoolObj(GuildIndex);
	//			if(!pSMU)
	//			{
	//				Assert(pSMU);
	//				return FALSE;
	//			}
	//			GUILD_DB* pGuild = &pSMU->m_GuildSM;
	//			Assert(pGuild);

	//			DBGuildExtraInfo ExtraInfoObject(mInterface);
	//			ExtraInfoObject.SetGuildID(pGuild->m_GuildID);
	//			if(!ExtraInfoObject.Load())
	//				return FALSE;

	//			if(!ExtraInfoObject.ParseResult(pGuild->m_aExtraUserData))
	//				return FALSE;
	//		}
	//	}
	//	break;
	//case DB_DELETE:
	//	{
	//
	//	}
	//	break;
	//default:
	//	break;
	//}

	//Assert(mInterface);
	//mInterface->Clear();
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
