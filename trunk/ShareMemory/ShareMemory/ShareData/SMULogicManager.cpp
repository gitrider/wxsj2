#include "stdafx.h"
#include "SMULogicManager.h"
#include "TimeManager.h"
#include "DBCharFullData.h"
#include "Log.h"
#include "ShareDBManager.h"
#include "PlayerShopDefine.h"
#include "GameStruct_Pet.h"
#include "BBS.h"
#include "Config.h"
#include "DBItemSerial.h"
#include "DBMailInfo.h"
#include "DBGuildInfo.h"
//#include "DBShopInfo.h"
//#include "DBPetItemInfo.h"
//#include "DBCityInfo.h"
//#include "DBCharSaveLock.h"
#include "DBChardbVersion.h"
//#include "DBCrcInfo.h"
//#include "DBGlobalData.h"
//#include "DBCommitShopInfo.h"

using namespace PLAYER_SHOP;

GLOBAL_CONFIG	g_SMConfig;
extern BOOL		g_LockTimeOutEnable;
extern BOOL		g_NeedSaveLogout;
#define			SAVE_LOCK_INTERVAL		60000

BOOL			g_CleanBattleGuild = FALSE;

//HumanSMU 的心跳
template<>
BOOL	SMULogicManager<HumanSMU>::DoSaveAll()
{
	__ENTER_FUNCTION
		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		//做预先统计
		INT		iHoldDataCount	=	0;		//已经占据位置的SMU个数
		INT		iCanSaveCount	=	0;		//服务器通知下线等待存盘档案数量
		INT		iHaveSaveCount	=	0;		//已经存盘等待服务器回收个数

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);

			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				iHoldDataCount++;
			}

			if(UseStatus == SM_USE_READYFREE)
			{
				iCanSaveCount ++;
			}

		}

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UINT    uTime		=	g_pTimeManager->RunTime();
			INT		UseStatus	=	pSMU->GetUseStatusFast();


			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				GUID_t	guid=INVALID_ID ;
				FULLUSERDATA	FullData;
				_MY_TRY
				{
					if(pSMU->GetLockValueFast() != SM_FREE)
					{
						
						if(g_LockTimeOutEnable)
						{
							g_pLog->FastSaveLog(LOG_FILE_14,"ERROR: User SMU Locked Human GUID=%X",pSMU->m_HumanSM.m_GUID);
						}
						continue;
					}

					pSMU->Copy2FullData(SM_C_WRITE,&FullData);		//将ShareMemory 中对应的角色数据copy出来

					FullData.m_Human.m_LastLogoutTime = g_pTimeManager->CurrentDate();		//下线时间
					guid = FullData.m_Human.m_GUID;

					ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
					Assert(pInterface);

					// 存盘时间锁检查去除[12/12/2006]
					//DBCharSaveLock	CharSaveLockObject(pInterface);
					//CharSaveLockObject.SetCharGuid(guid);
					//
					//BOOL bRet = CharSaveLockObject.Load();
					//if(!bRet)
					//{
					//	g_pLog->FastSaveLog(LOG_FILE_14,"ERROR: CharSaveLock get db errors GUID=%X",guid);			
					//}
					//else if(CharSaveLockObject.GetIsValidTime()!=DBCL_SAVE_FREE)
					//{
					//	g_pLog->FastSaveLog(LOG_FILE_14,"ERROR: CharSaveLock get lock errors code = %d, GUID=%X",
					//		CharSaveLockObject.GetIsValidTime(),guid);
					//	pSMU->SetTime2Save(uTime+SAVE_LOCK_INTERVAL,SM_C_WRITE);
					//	if(g_LockTimeOutEnable == FALSE)
					//	{
					//		continue;
					//	}
					//}

					// 数据库版本检查添加[12/29/2006]
					DBChardbVersion ChardbVersionObject(pInterface);
					ChardbVersionObject.SetCharGuid(guid);
					ChardbVersionObject.ForceCheckConnect();
					BOOL bNeedSave = ChardbVersionObject.Load();
					if(bNeedSave)
					{
						BOOL bIsValid	  = ChardbVersionObject.IsValid();
						if(FALSE == bIsValid)
						{
							bNeedSave = FALSE;
							g_pLog->FastSaveLog(LOG_FILE_14,"IsValid==0放弃本次存盘   PlayerPool Index=%d,Player Guid=%X",
								iIndex,guid);
						}

						INT nCurDBVersion = ChardbVersionObject.GetdbVersion();
						if(nCurDBVersion >= 0 && nCurDBVersion > (INT)(FullData.m_Human.m_DBVersion))
						{
							bNeedSave = FALSE;
							g_pLog->FastSaveLog(LOG_FILE_14,"版本低放弃本次存盘   PlayerPool Index=%d,Player Guid=%X,cdbv=%d,odbv=%u",
								iIndex,guid,nCurDBVersion,FullData.m_Human.m_DBVersion);
						}
					}
					else
					{
						g_pLog->FastSaveLog(LOG_FILE_14,"ChardbVersionObject.Save()...CharGUID=%X,Get Errors: %s ",
							ChardbVersionObject.GetCharGuid(),
							ChardbVersionObject.GetErrorMessage());
						continue;
					}

					// 需要存盘的才进行存储[12/29/2006]
					BOOL bRet = TRUE;
					DBCharFullData	CharFullDataObject(pInterface);
					if(bNeedSave)
					{
						CharFullDataObject.SetCharGuid(FullData.m_Human.m_GUID);
						bRet = CharFullDataObject.Save(&FullData); //保存基本档案
						if(bRet)
						{
							bRet  =	CharFullDataObject.ParseResult(&FullData);//Parse 档案内容
						}
					}

					//修改下次保存时间
					pSMU->SetTime2Save(uTime+g_Config.m_ShareMemInfo.DATAInterval,SM_C_WRITE);
					if(!bRet)
					{
						g_pLog->FastSaveLog(LOG_FILE_14,"CharFullDataObject.Save()...CharGUID=%X,DebugStep=%u,Get Errors: %s ", 
							CharFullDataObject.GetCharGuid(),
							CharFullDataObject.GetDebugStep(),
							CharFullDataObject.GetErrorMessage());
					}
					else
					{
						if(UseStatus==SM_USE_READYFREE)
						{
							pSMU->SetUseStatus(SM_USE_FREEED,SM_C_WRITE);
							// 必须清除保存时间，这样分配给新的User使用时才能及时保存[7/21/2007]
							pSMU->SetTime2Save(0,SM_C_WRITE);
							iHaveSaveCount++;
							g_pLog->FastSaveLog(LOG_FILE_14,"离线存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
						}
						else
						{
							g_pLog->FastSaveLog(LOG_FILE_14,"普通存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
						}
						g_pLog->FastSaveLog(LOG_FILE_14,"系统信息: 档案总数 = %d,总等待存盘数量 =%d,已经存储数量 = %d",iHoldDataCount,iCanSaveCount,iHaveSaveCount);
					}
					
					//CharSaveLockObject.Save(NULL);

				}
				_MY_CATCH
				{
					CHAR filename[_MAX_PATH]={0} ;
					sprintf( filename, "./Users/%X_%d.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
						guid, iIndex,
						g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
						g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
					FullData.OutputToDisk( filename ) ;
					g_pLog->FastSaveLog(LOG_FILE_14,"系统信息: 存盘失败	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
				}
			}
		}
		return TRUE;
	__LEAVE_FUNCTION
		return FALSE;

}


template<>
BOOL SMULogicManager<HumanSMU>::DoNormalSave()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		//做预先统计
		INT		iHoldDataCount	=	0;		//已经占据位置的SMU个数
		INT		iCanSaveCount	=	0;		//服务器通知下线等待存盘档案数量
		INT		iHaveSaveCount	=	0;		//已经存盘等待服务器回收个数

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);
			
			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				iHoldDataCount++;
			}

			if(UseStatus == SM_USE_READYFREE)
			{
				iCanSaveCount ++;
			}
			
		}

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			
			UINT    uTime		=	g_pTimeManager->RunTime();				//取当前时间
			INT		UseStatus	=	pSMU->GetUseStatusFast();			//取当前内存块状态
			
			
			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据 holddata 或者准备回收
			{
				//检查是否到达存盘时间
				UINT Time2Save	= pSMU->GetTime2SaveFast();
				if((uTime>Time2Save)||UseStatus ==SM_USE_READYFREE)
				{
					GUID_t	guid=INVALID_ID ;
					FULLUSERDATA	FullData;
					_MY_TRY
					{
						if(!pSMU->TryCopy2FullData(SM_C_WRITE,&FullData))		//将ShareMemory 中对应的角色数据copy出来
						{
							continue;
						}


						FullData.m_Human.m_LastLogoutTime = g_pTimeManager->CurrentDate();		//下线时间
						guid = FullData.m_Human.m_GUID;
						ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
						Assert(pInterface);

						// 存盘时间锁去除[12/12/2006]
						//DBCharSaveLock	CharSaveLockObject(pInterface);
						//CharSaveLockObject.SetCharGuid(guid);

						//BOOL bRet = CharSaveLockObject.Load();
						//if(!bRet)
						//{
						//	g_pLog->FastSaveLog(LOG_FILE_14,"ERROR: CharSaveLock get db errors GUID=%X",guid);			
						//}
						//else if(CharSaveLockObject.GetIsValidTime()!=DBCL_SAVE_FREE)
						//{
						//	g_pLog->FastSaveLog(LOG_FILE_14,"ERROR: CharSaveLock get lock errors code = %d, GUID=%X",
						//		CharSaveLockObject.GetIsValidTime(),guid);
						//	pSMU->SetTime2Save(uTime+SAVE_LOCK_INTERVAL,SM_C_WRITE);
						//	continue;
						//}

						// 数据库版本检查添加[12/29/2006]
						DBChardbVersion ChardbVersionObject(pInterface);
						ChardbVersionObject.SetCharGuid(guid);
						BOOL bNeedSave = ChardbVersionObject.Load();
						if(bNeedSave)
						{
							BOOL bIsValid	  = ChardbVersionObject.IsValid();
							if(FALSE == bIsValid)
							{
								bNeedSave = FALSE;
								g_pLog->FastSaveLog(LOG_FILE_14,"IsValid==0放弃本次存盘   PlayerPool Index=%d,Player Guid=%X",
									iIndex,guid);
							}

							INT nCurDBVersion = ChardbVersionObject.GetdbVersion();
							if(nCurDBVersion >= 0 && nCurDBVersion > (INT)(FullData.m_Human.m_DBVersion))
							{
								bNeedSave = FALSE;
								g_pLog->FastSaveLog(LOG_FILE_14,"版本低放弃本次存盘   PlayerPool Index=%d,Player Guid=%X,cdbv=%d,odbv=%u",
									iIndex,guid,nCurDBVersion,FullData.m_Human.m_DBVersion);
							}
						}
						else
						{
							g_pLog->FastSaveLog(LOG_FILE_14,"ChardbVersionObject.Save()...CharGUID=%X,Get Errors: %s ",
								ChardbVersionObject.GetCharGuid(),
								ChardbVersionObject.GetErrorMessage());
							continue;
						}

						// 需要存盘的才进行存储[12/29/2006]
						BOOL bRet = TRUE;
						DBCharFullData	CharFullDataObject(pInterface);
						if(bNeedSave)
						{
							CharFullDataObject.SetCharGuid(FullData.m_Human.m_GUID);
							bRet = CharFullDataObject.Save(&FullData); //保存基本档案
							if(bRet)
							{
								bRet  =	CharFullDataObject.ParseResult(&FullData);//Parse 档案内容
							}
						}

						//修改下次保存时间
						pSMU->SetTime2Save(uTime+g_Config.m_ShareMemInfo.DATAInterval,SM_C_WRITE);
						if(!bRet)
						{
							g_pLog->FastSaveLog(LOG_FILE_14,"CharFullDataObject.Save()...CharGUID=%X,DebugStep=%u,Get Errors: %s ", 
								CharFullDataObject.GetCharGuid(),
								CharFullDataObject.GetDebugStep(),
								CharFullDataObject.GetErrorMessage());
						}
						else
						{
							if(UseStatus==SM_USE_READYFREE)
							{
								pSMU->SetUseStatus(SM_USE_FREEED,SM_C_WRITE);
								// 必须清除保存时间，这样分配给新的User使用时才能及时保存[7/21/2007]
								pSMU->SetTime2Save(0,SM_C_WRITE);
								iHaveSaveCount++;
								g_pLog->FastSaveLog(LOG_FILE_14,"离线存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);

								//保存文件格式的档案
								if(bNeedSave && g_NeedSaveLogout)
								{
									CHAR filename[_MAX_PATH]={0} ;
									sprintf( filename, "./errUsr/%Xs_%u.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
										guid, CharFullDataObject.GetCRC(),
										g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
										g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
									FullData.OutputToDisk( filename ) ;

									//保存到数据库
#ifdef CRC32_ERROR_LOGTO_DB
									DBCrcInfo CrcInfoObject(pInterface);
									CrcInfoObject.SetCharGuid(guid);
									CrcInfoObject.SetCrc32(CharFullDataObject.GetCRC());
									CrcInfoObject.SetServerId(1);
									if(CrcInfoObject.Save(&FullData))
									{
										CrcInfoObject.ParseResult(&FullData);
									}
#endif
								}
							}
							else
							{
								g_pLog->FastSaveLog(LOG_FILE_14,"普通存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
							}
							g_pLog->FastSaveLog(LOG_FILE_14,"系统信息: 档案总数 = %d,总等待存盘数量 =%d,已经存储数量 = %d",iHoldDataCount,iCanSaveCount,iHaveSaveCount);
						}

						//CharSaveLockObject.Save(NULL);
					}
					_MY_CATCH
					{
						CHAR filename[_MAX_PATH]={0} ;
						sprintf( filename, "./Users/%X_%d.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
							guid, iIndex,
							g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
							g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
						FullData.OutputToDisk( filename ) ;
						g_pLog->FastSaveLog(LOG_FILE_14,"系统信息: 存盘失败	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
					}
				}
			}
		}
		
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<HumanSMU>::DoClear()
{
	__ENTER_FUNCTION
	
	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
	{
		HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
		if(!pSMU)
		{
			Assert(pSMU);
			return FALSE;
		}
		INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);
		pSMU->SetUseStatus(SM_USE_FREE,SM_C_WRITE);
	}
	return TRUE;
	

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<HumanSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}
		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);
			if(g_NeedManagerDoPosInit) pSMU->SetUseStatus(SM_USE_FREE,SM_C_WRITE);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<HumanSMU>::DoDefault(SM_COMMANDS cmd)
{
	return FALSE;
}

template<>
BOOL	SMULogicManager<GuildSMU>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	INT		SMUCount			=		0;
	INT		TotalSMUSize		=		0;
	UINT    uTime		=	g_pTimeManager->RunTime();

	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}

	SM_KEY key	= m_PoolSharePtr->GetKey();

	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);

	DBGuildInfo	GuildInfoObject(pInterface);
	BOOL bRet	= GuildInfoObject.Save(m_PoolSharePtr);
	if(bRet)
	{
		bRet  =	GuildInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	}

	g_pLog->FastSaveLog(LOG_FILE_14,"End GuildSMU_%d SaveAll...OK!",key);
	g_pLog->FastSaveLog(LOG_FILE_14,"SMUCount = %d",SMUCount);
	g_pLog->FastSaveLog(LOG_FILE_14,"TotalSMUSize = %d",TotalSMUSize);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<GuildSMU>::DoNormalSave()
{
	__ENTER_FUNCTION
		
	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
		return TRUE;

	__LEAVE_FUNCTION
		
	return FALSE;

}

template<>
BOOL SMULogicManager<GuildSMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<GuildSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{	
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{

			GuildSMU* pGuildSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pGuildSMU)
			{
				Assert(pGuildSMU);
				return FALSE;
			}

			if(g_NeedManagerDoPosInit)	pGuildSMU->Init();
		}

		SM_KEY key  = m_PoolSharePtr->GetKey();

		BOOL bRet = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		if(g_NeedManagerDoPosInit)
		{
			//DBGuildInfo	GuildInfoObject(pInterface);
			//bRet = GuildInfoObject.Load();

			//if(bRet)
			//{
			//	bRet  =	GuildInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
			//}
			//if(!bRet)
			//{
			//	m_bReady = FALSE;
			//	g_pLog->FastSaveLog(LOG_FILE_14,"PostInit GuildSMU_%d from database fails",key);
			//	return FALSE;
			//}
			//if(bRet)
			//{
			//	m_bReady = TRUE;
			//	g_pLog->FastSaveLog(LOG_FILE_14,"PostInit GuildSMU_%d from database  Ok!",key);
			//}
		}
		else
		{
			m_bReady = TRUE;
			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit GuildSMU_%d from database  Skip!",key);
		}

		return TRUE;



	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<GuildSMU>::DoDefault(SM_COMMANDS cmd)
{
	return FALSE;
}

template<>
BOOL	SMULogicManager<MailSMU>::DoSaveAll()
{
	__ENTER_FUNCTION
		
	///存盘统计数据 
	INT		SMUCount			=		0;
	INT		TotalSMUSize		=		0;
	UINT    uTime		=	g_pTimeManager->RunTime();
	
	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}

	SM_KEY key	= m_PoolSharePtr->GetKey();
	
	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);

	DBMailInfo	MailInfoObject(pInterface);
	BOOL bRet	= MailInfoObject.Save(m_PoolSharePtr);
	if(bRet)
	{
		bRet  =	MailInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	}

	g_pLog->FastSaveLog(LOG_FILE_14,"End MailSMU_%d SaveAll...OK!",key);
	g_pLog->FastSaveLog(LOG_FILE_14,"SMUCount = %d",SMUCount);
	g_pLog->FastSaveLog(LOG_FILE_14,"TotalSMUSize = %d",TotalSMUSize);
	
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<MailSMU>::DoNormalSave()
{
	__ENTER_FUNCTION

		/*
		 *	临时存文件方法
		 */
		UINT    uTime	=	g_pTimeManager->RunTime();
		if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
		{
			if(m_bReady)
			{
				DoSaveAll();
			}
			m_FinalSaveTime = uTime;
		}
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<MailSMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<MailSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{	Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{

			MailSMU* pMailSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pMailSMU)
			{
				Assert(pMailSMU);
				return FALSE;
			}

			if(g_NeedManagerDoPosInit) pMailSMU->Init();
		}
		
		SM_KEY key  = m_PoolSharePtr->GetKey();
	

		BOOL bRet = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		if(g_NeedManagerDoPosInit)
		{
			DBMailInfo	MailInfoObject(pInterface);
			bRet = MailInfoObject.Load();

			if(bRet)
			{
				bRet  =	MailInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
			}
			if(!bRet)
			{
				m_bReady = FALSE;
				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit MailSMU_%d from database fails",key);
				return FALSE;
			}
			if(bRet)
			{
				m_bReady = TRUE;
				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit MailSMU_%d from database  Ok!",key);
			}
		}
		else
		{
			m_bReady = TRUE;
			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit MailSMU_%d from database  Skip!",key);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<MailSMU>::DoDefault(SM_COMMANDS cmd)
{
	return FALSE;
}

template<>
BOOL	SMULogicManager<PlayerShopSM>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	INT		SMUCount			=		0;
	INT		TotalSMUSize		=		0;
	UINT    uTime		=	g_pTimeManager->RunTime();

	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}

	SM_KEY	key					= m_PoolSharePtr->GetKey();
	ID_t	ServerID			= g_Config.Key2ServerID(key);
	if(ServerID == INVALID_ID)
	{
		AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
	}

	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);
	
	//DBShopInfo	ShopInfoObject(pInterface);
	//ShopInfoObject.SetServerID(ServerID);

	//BOOL bRet = ShopInfoObject.Save(m_PoolSharePtr);
	//if(bRet)
	//{
	//	bRet  =	ShopInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	//}

	//if(bRet)
	//{
	//	g_pLog->FastSaveLog(LOG_FILE_14,"End PlayerShopSM_%d SaveAll...OK!",key);
	//	g_pLog->FastSaveLog(LOG_FILE_14,"SMUCount = %d",SMUCount);
	//	g_pLog->FastSaveLog(LOG_FILE_14,"TotalSMUSize = %d",TotalSMUSize);
	//}
	//else
	//{
	//	g_pLog->FastSaveLog(LOG_FILE_14,"End PlayerShopSM_%d SaveAll...Get Errors!",key);
	//}
	

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoNormalSave()
{
	__ENTER_FUNCTION
		
	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoPostInit()
{
	__ENTER_FUNCTION
		//////////////////////////////////////////////////////////////////////////
		if(!m_PoolSharePtr)
		{	
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{

			PlayerShopSM* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			if(g_NeedManagerDoPosInit)	pSMU->Init();
		}

		SM_KEY key  = m_PoolSharePtr->GetKey();
		ID_t	ServerID			= g_Config.Key2ServerID(key);
		
		if(ServerID == INVALID_ID)
		{
			AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
		}

		BOOL bRet = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		if(g_NeedManagerDoPosInit)
		{
			//DBShopInfo	ShopInfoObject(pInterface);
			//ShopInfoObject.SetServerID(ServerID);

			//bRet = ShopInfoObject.Load();
			//if(bRet)
			//{
			//	bRet  =	ShopInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
			//}
			//if(!bRet)
			//{
			//	m_bReady = FALSE;
			//	g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PlayerShopSM_%d from database fails",key);
			//	return FALSE;
			//}
			//if(bRet)
			//{
			//	m_bReady = TRUE;
			//	g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PlayerShopSM_%d from database  Ok!",key);
			//}
		}
		else
		{
			m_bReady = TRUE;
			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PlayerShopSM_%d from database  Skip!",key);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoDefault(SM_COMMANDS cmd)
{
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////


template<>
BOOL	SMULogicManager<ItemSerialKeySMU>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	
	UINT	Serial = 0;
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}
	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
	Assert(MaxPoolSize == 1);
	ItemSerialKeySMU*	pSMU = m_PoolSharePtr->GetPoolObj(0);
	if(!pSMU)
	{
		Assert(pSMU);
		return FALSE;
	}
	Serial = pSMU->GetSerial(SM_C_READ);

	SM_KEY	key					= m_PoolSharePtr->GetKey();
	ID_t	ServerID			= g_Config.Key2ServerID(key);

	if(ServerID == INVALID_ID)
	{
		AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
	}
	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);
	DBItemSerial	ItemSerialObject(pInterface);
	ItemSerialObject.SetSID(ServerID);

	INT ErrorCode ;
	if(ItemSerialObject.Save(&Serial))
	{
		ItemSerialObject.ParseResult(&ErrorCode);
	}
	else
	{
		g_pLog->FastSaveLog(LOG_FILE_14,"ItemSerial Save...Error! Serial = %d ,ServerID = %d",Serial,ServerID);
		Assert(FALSE);
	}
	
	g_pLog->FastSaveLog(LOG_FILE_14,"ItemSerial Save...OK! Serial = %d ,ServerID = %d",Serial,ServerID);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

#define SERIALKEYTIME 30000

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoNormalSave()
{
	__ENTER_FUNCTION

	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+SERIALKEYTIME)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoPostInit()
{
	__ENTER_FUNCTION

#define SERIAL_GROWUP 10000

		if(!m_PoolSharePtr)
		{	Assert(m_PoolSharePtr);
			return FALSE;
		}
		
		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();
		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		SM_KEY key  = m_PoolSharePtr->GetKey();
		
		ItemSerialKeySMU* pSMU = m_PoolSharePtr->GetPoolObj(0);
		if(!pSMU)
		{
			Assert(pSMU);
			return FALSE;
		}
		
		ID_t	ServerID			= g_Config.Key2ServerID(key);

		if(ServerID == INVALID_ID)
		{
			AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
		}

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);
		UINT Serial	= 0;
		DBItemSerial	ItemSerialObject(pInterface);
		ItemSerialObject.SetSID(ServerID);

		
		if(ItemSerialObject.Load())
		{
			ItemSerialObject.ParseResult(&Serial);
		}
		if(Serial>0)
		{
			pSMU->SetSerial(SM_C_WRITE,Serial+SERIAL_GROWUP);
			Serial = pSMU->GetSerial(SM_C_READ);
		}
		else
		{
			Assert(FALSE);
		}
		m_bReady = TRUE;
		
		g_pLog->FastSaveLog(LOG_FILE_14,"PostInit ItemSerialSMU=%d from database Ok!  Serial = %d",key,Serial);

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoDefault(SM_COMMANDS cmd)
{
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////


//template<>
//BOOL	SMULogicManager<PetProcreateItemSM>::DoSaveAll()
//{
//	__ENTER_FUNCTION
//
//	///存盘统计数据 
//	INT		SMUCount			=		0;
//	INT		TotalSMUSize		=		0;
//	UINT    uTime		=	g_pTimeManager->RunTime();
//
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	DBPetItemInfo	PetItemInfoObject(pInterface);
//	BOOL bRet	= PetItemInfoObject.Save(m_PoolSharePtr);
//	if(bRet)
//	{
//		bRet  =	PetItemInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//	}
//	
//	if(bRet)
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"End PetProcreateItemSM_%d SaveAll...OK!",key);
//		g_pLog->FastSaveLog(LOG_FILE_14,"SMUCount = %d",SMUCount);
//		g_pLog->FastSaveLog(LOG_FILE_14,"TotalSMUSize = %d",TotalSMUSize);
//	}
//	else
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"End PetProcreateItemSM_%d SaveAll...Get Errors!",key);
//	}
//	
//
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoNormalSave()
//{
//	__ENTER_FUNCTION
//
//	/*
//	*	临时存文件方法
//	*/
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoClear()
//{
//	__ENTER_FUNCTION
//
//		return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoPostInit()
//{
//	__ENTER_FUNCTION
//		//////////////////////////////////////////////////////////////////////////
//		if(!m_PoolSharePtr)
//		{	
//			Assert(m_PoolSharePtr);
//			return FALSE;
//		}
//
//		if(g_CmdArgv == CMD_MODE_CLEARALL)
//		{
//			return TRUE;
//		}
//
//		m_FinalSaveTime	=	g_pTimeManager->RunTime();
//
//		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//		{
//
//			PetProcreateItemSM* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//			if(!pSMU)
//			{
//				Assert(pSMU);
//				return FALSE;
//			}
//			if(g_NeedManagerDoPosInit)	pSMU->Init();
//		}
//
//		SM_KEY	key  = m_PoolSharePtr->GetKey();
//		BOOL	bRet = FALSE;
//		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//		Assert(pInterface);
//		
//		if(g_NeedManagerDoPosInit)
//		{
//			DBPetItemInfo	PetItemObject(pInterface);
//			
//			bRet = PetItemObject.Load();
//			if(bRet)
//			{
//				bRet  =	PetItemObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//			}
//			if(!bRet)
//			{
//				m_bReady = FALSE;
//				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PetProcreateItemSM_%d from database fails",key);
//				return FALSE;
//			}
//			if(bRet)
//			{
//				m_bReady = TRUE;
//				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PetProcreateItemSM%d from database  Ok!",key);
//			}
//		}
//		else
//		{
//			m_bReady = TRUE;
//			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit PetProcreateItemSM%d from database  Skip!",key);
//		}
//
//		return TRUE;
//
//		__LEAVE_FUNCTION
//
//		return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoDefault(SM_COMMANDS cmd)
//{
//	return FALSE;
//}
//
////////////////////////////////////////////////////////////////////////////
//template<>
//BOOL	SMULogicManager<CitySMU>::DoSaveAll()
//{
//	__ENTER_FUNCTION
//
//		///存盘统计数据 
//	INT		SMUCount			=		0;
//	INT		TotalSMUSize		=		0;
//	UINT    uTime		=	g_pTimeManager->RunTime();
//
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//	
//	DBCityInfo	CityInfoObject(pInterface);
//	BOOL bRet	= CityInfoObject.Save(m_PoolSharePtr);
//	if(bRet)
//	{
//		bRet  =	CityInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//	}
//
//	if(bRet)
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"End CityInfoSM_%d SaveAll...OK!",key);
//		g_pLog->FastSaveLog(LOG_FILE_14,"SMUCount = %d",SMUCount);
//		g_pLog->FastSaveLog(LOG_FILE_14,"TotalSMUSize = %d",TotalSMUSize);
//	}
//	else
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"End CityInfoSM_%d SaveAll...Get Errors!",key);
//		return FALSE;
//	}
//	
//
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoNormalSave()
//{
//	__ENTER_FUNCTION
//
//	/*
//	*	临时存文件方法
//	*/
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoClear()
//{
//	__ENTER_FUNCTION
//
//		return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoPostInit()
//{
//	__ENTER_FUNCTION
//	//////////////////////////////////////////////////////////////////////////
//	if(!m_PoolSharePtr)
//	{	
//			Assert(m_PoolSharePtr);
//			return FALSE;
//	}
//
//	if(g_CmdArgv == CMD_MODE_CLEARALL)
//	{
//			return TRUE;
//	}
//
//	m_FinalSaveTime	=	g_pTimeManager->RunTime();
//
//	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//	{
//		CitySMU* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//		if(!pSMU)
//		{
//			Assert(pSMU);
//			return FALSE;
//		}
//		if(g_NeedManagerDoPosInit)	pSMU->Init();
//	}
//
//	SM_KEY	key  = m_PoolSharePtr->GetKey();
//	BOOL	bRet = FALSE;
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	if(g_NeedManagerDoPosInit)
//	{
//		DBCityInfo	CityInfoObject(pInterface);
//
//		bRet = CityInfoObject.Load();
//		if(bRet)
//		{
//				bRet  =	CityInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//		}
//		if(!bRet)
//		{
//				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CityInfoSM_%d from database fails",key);
//				return FALSE;
//		}
//		if(bRet)
//		{
//				m_bReady = TRUE;
//				g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CityInfoSM%d from database  Ok!",key);
//		}
//	}
//	else
//	{
//		m_bReady = TRUE;
//		g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CityInfoSM%d from database  Skip!",key);
//	}
//	
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoDefault(SM_COMMANDS cmd)
//{
//__ENTER_FUNCTION
//	BOOL bRet = FALSE;
//
//	if(cmd == CMD_CLEAN_ALL_CITY_BATTLEGUILD)
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"0.Begin Clean All City BattleGuild.");
//
//		if(!m_PoolSharePtr)
//		{	
//			g_pLog->FastSaveLog(LOG_FILE_14,"FAIL:m_PoolSharePtr is NULL");
//			return FALSE;
//		}
//
//		if(!m_bReady)
//		{
//			g_pLog->FastSaveLog(LOG_FILE_14,"FAIL:m_bReady is FALSE");
//			return FALSE;
//		}
//
//		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//		{
//			CitySMU* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//			if(!pSMU)
//			{
//				Assert(pSMU);
//				return FALSE;
//			}
//			
//			pSMU->m_CitySMU.CleanUpBattleGuild();
//		}
//
//		g_pLog->FastSaveLog(LOG_FILE_14,"1.CitySMU BattleGuild Clean In Memory.");
//
//		bRet = DoSaveAll();
//		if(!bRet)
//		{
//			g_pLog->FastSaveLog(LOG_FILE_14,"FAIL:SMULogicManager<CitySMU>::DoSaveAll() return FALSE");
//			return FALSE;
//		}
//		else
//		{
//			g_pLog->FastSaveLog(LOG_FILE_14,"2.SMULogicManager<CitySMU>::DoSaveAll() OK.");
//		}
//
//		g_pLog->FastSaveLog(LOG_FILE_14,"3.End Clean All City BattleGuild.");
//	}
//	return bRet;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//////////////////////////////////////////////////////////////////////////

//
//template<>
//BOOL	SMULogicManager<GlobalDataSMU>::DoSaveAll()
//{
//__ENTER_FUNCTION
//
//	//存盘统计数据 
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//	DBGlobalData	GlobalDataObject(pInterface);
//
//	if(GlobalDataObject.Save(m_PoolSharePtr))
//	{
//		GlobalDataObject.ParseResult(m_PoolSharePtr);
//	}
//	else
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"GlobalData Save...Error!");
//		Assert(FALSE);
//	}
//
//	g_pLog->FastSaveLog(LOG_FILE_14,"GlobalData Save...OK!");
//
//	return TRUE;
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<GlobalDataSMU>::DoNormalSave()
//{
//__ENTER_FUNCTION
//
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+SERIALKEYTIME)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	return TRUE;
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<GlobalDataSMU>::DoClear()
//{
//__ENTER_FUNCTION
//
//	return TRUE;
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}

//template<>
//BOOL SMULogicManager<GlobalDataSMU>::DoPostInit()
//{
//__ENTER_FUNCTION
//
//	if(!m_PoolSharePtr)
//	{	Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	if(g_CmdArgv == CMD_MODE_CLEARALL)
//	{
//		return TRUE;
//	}
//
//	m_FinalSaveTime	=	g_pTimeManager->RunTime();
//	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//	{
//
//		GlobalDataSMU* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//		if(!pSMU)
//		{
//			Assert(pSMU);
//			return FALSE;
//		}
//		pSMU->Init();
//	}
//
//	SM_KEY key  = m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	BOOL	bRet = FALSE;
//	DBGlobalData	GlobalDataObject(pInterface);
//	bRet = GlobalDataObject.Load();
//	if(bRet)
//	{
//		bRet = GlobalDataObject.ParseResult(m_PoolSharePtr);
//	}
//	if(!bRet)
//	{
//		m_bReady = FALSE;
//		g_pLog->FastSaveLog(LOG_FILE_14,"PostInit GlobalDataSMU=%d from database fails!",key);
//		return FALSE;
//	}
//	if(bRet)
//	{
//		m_bReady = TRUE;
//		g_pLog->FastSaveLog(LOG_FILE_14,"PostInit GlobalDataSMU=%d from database Ok!",key);
//	}
//
//	return TRUE;
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<GlobalDataSMU>::DoDefault(SM_COMMANDS cmd)
//{
//	return FALSE;
//}
////////////////////////////////////////////////////////////////////////////
//
//
//template<>
//BOOL	SMULogicManager<CommisionShopSMU>::DoSaveAll()
//{
//__ENTER_FUNCTION
//	//存盘统计数据 
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ID_t	ServerID			= g_Config.Key2ServerID(key);
//	if(ServerID == INVALID_ID)
//	{
//		AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
//	}
//
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//	DBCommitShopInfo	CommisionShopObject(pInterface);
//	CommisionShopObject.SetWorldID(g_Config.WorldID());
//	CommisionShopObject.SetServerID(ServerID);
//
//	if(CommisionShopObject.Save(m_PoolSharePtr))
//	{
//		CommisionShopObject.ParseResult(m_PoolSharePtr);
//	}
//	else
//	{
//		g_pLog->FastSaveLog(LOG_FILE_14,"CommisionShop Save...Error!");
//		Assert(FALSE);
//	}
//
//	g_pLog->FastSaveLog(LOG_FILE_14,"CommisionShop Save...OK!");
//
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CommisionShopSMU>::DoNormalSave()
//{
//__ENTER_FUNCTION
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+SERIALKEYTIME)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CommisionShopSMU>::DoClear()
//{
//__ENTER_FUNCTION
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CommisionShopSMU>::DoPostInit()
//{
//__ENTER_FUNCTION
//	if(!m_PoolSharePtr)
//	{	Assert(m_PoolSharePtr);
//	return FALSE;
//	}
//
//	if(g_CmdArgv == CMD_MODE_CLEARALL)
//	{
//		return TRUE;
//	}
//
//	m_FinalSaveTime	=	g_pTimeManager->RunTime();
//	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//	{
//
//		CommisionShopSMU* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//		if(!pSMU)
//		{
//			Assert(pSMU);
//			return FALSE;
//		}
//		if(g_NeedManagerDoPosInit)	pSMU->Init();
//	}
//
//	SM_KEY key  = m_PoolSharePtr->GetKey();
//	ID_t	ServerID			= g_Config.Key2ServerID(key);
//	if(ServerID == INVALID_ID)
//	{
//		AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
//	}
//
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	if(g_NeedManagerDoPosInit)
//	{
//		BOOL	bRet = FALSE;
//		DBCommitShopInfo	CommisionShopObject(pInterface);
//		CommisionShopObject.SetWorldID(g_Config.WorldID());
//		CommisionShopObject.SetServerID(ServerID);
//
//		bRet = CommisionShopObject.Load();
//		if(bRet)
//		{
//			bRet = CommisionShopObject.ParseResult(m_PoolSharePtr);
//		}
//		if(!bRet)
//		{
//			m_bReady = FALSE;
//			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CommisionShopSMU=%d from database fails!",key);
//			return FALSE;
//		}
//		if(bRet)
//		{
//			m_bReady = TRUE;
//			g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CommisionShopSMU=%d from database Ok!",key);
//		}
//	}
//	else
//	{
//		m_bReady = TRUE;
//		g_pLog->FastSaveLog(LOG_FILE_14,"PostInit CommisionShopSMU=%d from database  Skip!",key);
//	}
//
//	return TRUE;
//__LEAVE_FUNCTION
//	return FALSE;
//}

//template<>
//BOOL SMULogicManager<CommisionShopSMU>::DoDefault(SM_COMMANDS cmd)
//{
//	return FALSE;
//}
//////////////////////////////////////////////////////////////////////////
