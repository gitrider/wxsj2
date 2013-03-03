#include "stdafx.h"
#include "ShareMemory.h"
#include "Log.h"
#include "Config.h"
#include "TimeManager.h"
#include "SMUManager.h"
#include "SMULogicManager.h"
#include "ShareDBManager.h"
#include "ODBCInterface.h"
#include "CommandThread.h"
#include "PlayerShopDefine.h"
#include "GameStruct_Pet.h"
#include "GameUtil.h"

#if defined(__WINDOWS__)
#include "Minidump.h"
char * g_dump_exe_name = "ShareMemory";
#endif

#if __LINUX__
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>
#include <signal.h>
#elif __WINDOWS__
#include <WinBase.h>
#endif


ShareMemory			g_ShareMemory ;

using namespace PLAYER_SHOP;
INT					g_CmdInput;


BOOL				CheckSaveAllFile();
BOOL				CheckExitFile();
BOOL				CheckStartSaveLogout();
BOOL				CheckStopSaveLogout();

int main(int argc, char* argv[])
{
#if defined(__WINDOWS__)
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
	g_CmdInput = FALSE;

	if( argc>1 )
	{
		for( int i=1; i<argc; i++ )
		{
			if( strcmp(argv[i],"-clear")==0 )
			{
				g_CmdArgv = CMD_MODE_CLEARALL ;
				printf("Cmd enter clear mode!\r\n");
			}
			if(strcmp(argv[i],"-loaddump")==0)
			{
				g_CmdArgv = CMD_MODE_LOADDUMP;
				printf("Cmd enter loaddump mode!\r\n");
			}
			if(strcmp(argv[i],"-enablecmd")==0)
			{	
				g_CmdInput	= TRUE;

			}
			if( strcmp(argv[i],"-ignoreassert")==0 )
			{
				g_Command_Assert=1 ;
			}
			else if( strcmp(argv[i],"-retryassert")==0 )
			{
				g_Command_Assert=2 ;
			}

			if( strcmp(argv[i],"-ignoremessagebox")==0 )
			{
				g_Command_IgnoreMessageBox=TRUE ;
			}
			
			if( strcmp(argv[i],"-skiploaddb")==0 )
			{
				//g_NeedManagerDoPosInit=0;
			}

			if( strcmp(argv[i],"-cleanbattleguild")==0 )
			{
				g_CleanBattleGuild=TRUE;
			}
		}
	}
	
	
	g_pTimeManager = new TimeManager ;
	Assert( g_pTimeManager ) ;
	g_pTimeManager->Init( ) ;
	g_pLog = new Log ;
	Assert( g_pLog ) ;
	BOOL ret = g_pLog->Init( ) ;
	Assert(ret) ;
	
	BOOL bRet ;

	g_pLog->FastSaveLog( LOG_FILE_14, "(###) main...\r\n" ) ;
	g_pLog->FastSaveLog( LOG_FILE_14, "ShareMemory Start...\r\n" ) ;

	bRet = g_ShareMemory.Init();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}
	
	bRet = g_ShareMemory.Loop();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}

	bRet = g_ShareMemory.Exit();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}
	
	SAFE_DELETE(g_pTimeManager);

	return 0 ;
}

ShareMemory::ShareMemory( ) : m_bExited(FALSE)
{

	
}

ShareMemory::~ShareMemory( )
{
}

BOOL ShareMemory::Init( )
{

	BOOL bRet ;

	g_pLog->FastSaveLog( LOG_FILE_14, "Start Read Config Files..." ) ;
	bRet = g_Config.Init( ) ;
	Assert( bRet ) ;

	g_pLog->FastSaveLog( LOG_FILE_14, "Read Config Files...OK!" ) ;
	
	
	g_pLog->FastSaveLog( LOG_FILE_14, "Start New Managers..." ) ;
	bRet = NewStaticManager( ) ;
	Assert( bRet ) ;
	g_pLog->FastSaveLog( LOG_FILE_14, "New Managers...OK!" ) ;

	g_pLog->FastSaveLog( LOG_FILE_14, "Start Init Managers..." ) ;
	bRet = InitStaticManager( ) ;
	Assert( bRet ) ;
	g_pLog->FastSaveLog( LOG_FILE_14, "Init Managers...OK!" ) ;
	return TRUE ;
}

BOOL ShareMemory::Loop( )
{
	if(g_CmdInput)
	{
		g_CommandThread.start();
		g_pLog->FastSaveLog( LOG_FILE_14, "g_CommandThread.start()" ) ;
	}

	g_pLog->FastSaveLog( LOG_FILE_14, "Loop...Start" ) ;
	while(TRUE)
	{
		DoWork(m_bExited);
		if(m_bExited)	break;

		MySleep(1000);
	}
	g_pLog->FastSaveLog( LOG_FILE_14, "Loop...End" ) ;
	
	return TRUE ;
}

BOOL ShareMemory::Exit( )
{
	DelStaticManager();
	m_bExited= FALSE;

	g_pLog->FastSaveLog(LOG_FILE_14,"Exit ShareMemory Program");

	SAFE_DELETE( g_pLog ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pLog delete...OK" ) ;

	return TRUE ;
}

BOOL ShareMemory::DoWork(BOOL &bExitSign)
{
	
	_MY_TRY
	{
		UINT uDayTime = g_pTimeManager->Time2Day( ) ;
		if( g_FileNameFix != uDayTime )
		{//设置日志文件名后缀的变化
			g_FileNameFix = uDayTime ;
		}
		
		BOOL  bExit = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		if(!pInterface->IsConnected())
		{
			g_pLog->FastSaveLog(LOG_FILE_14,"connect database is fails");
			BOOL bRet =	pInterface->Connect();
			if(!bRet)
			{
				g_pLog->FastSaveLog(LOG_FILE_14,"Can't connect database");
				MySleep(5000);
				if(g_CommandThread.m_SMConfig.Commands.cmdType==CMD_UNKNOW)
				{
					return FALSE;
				}
			}
			else
			{
				g_pLog->FastSaveLog(LOG_FILE_14,"Reconnect database");
			}
		}
		if(CheckSaveAllFile())
		{
			g_CommandThread.m_SMConfig.Commands.cmdType = CMD_SAVE_ALL;
			g_pLog->FastSaveLog(LOG_FILE_14,"Get SaveAll Commands");
		}

		if(CheckExitFile())
		{
			g_CommandThread.m_SMConfig.Commands.cmdType = CMD_SAVE_ALL;
			bExit = TRUE;
			g_pLog->FastSaveLog(LOG_FILE_14,"Get Exit Commands");
		}
		if(CheckStartSaveLogout())
		{
			g_pLog->FastSaveLog(LOG_FILE_14,"Get StartSavLogout Commands");
		}
		if(CheckStopSaveLogout())
		{
			g_pLog->FastSaveLog(LOG_FILE_14,"Get StopSavLogout Commands");
		}
		//城市宣战列表清除命令[2007/08/15 YangJun]
		if(g_CleanBattleGuild)
		{
			//g_CommandThread.m_SMConfig.Commands.cmdType = CMD_CLEAN_ALL_CITY_BATTLEGUILD;
			bExit = TRUE;
			g_pLog->FastSaveLog(LOG_FILE_14,"Get Clean All City's BattleGuild Commands");
		}
		if(g_CommandThread.m_SMConfig.Commands.cmdType!=CMD_UNKNOW)
		{
			g_SMConfig = g_CommandThread.m_SMConfig ;
			g_CommandThread.m_SMConfig.Commands.cmdType=CMD_UNKNOW;
		}

		for(uint i = 0;i<MAX_SM_OBJ_NUM;i++)
		{
			if(m_SMULogicManager[i].m_SMULogicManager)
			{
				SHAREMEM_TYPE	Type = m_SMULogicManager[i].m_Type;

				switch(Type) 
				{
				case ST_HUMAN_SMU:
					{
						SMULogicManager<HumanSMU>* pHumanSMULogicMgr = static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pHumanSMULogicMgr)
							pHumanSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;
					}
				case ST_GUILD_SMU:
					{
						SMULogicManager<GuildSMU>* pGuildSMULogicMgr = static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pGuildSMULogicMgr)
							pGuildSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;
					}
				case ST_MAIL_SMU:
					{
						SMULogicManager<MailSMU>* pMailSMULogicMgr = static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pMailSMULogicMgr)
							pMailSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;	
					}
				//case ST_PSHOP_SMU:
				//	{
				//		SMULogicManager<PlayerShopSM>* pPShopSMULogicMgr = static_cast<SMULogicManager<PlayerShopSM>*>(m_SMULogicManager[i].m_SMULogicManager);
				//		if(pPShopSMULogicMgr)
				//			pPShopSMULogicMgr->HeartBeat();
				//		else
				//		{
				//			AssertEx(FALSE,"Runtime Type error" );
				//		}
				//		break;
				//	}

				case ST_ITEMSERIAL_SMU:
					{
						SMULogicManager<ItemSerialKeySMU>* pItemSerialSMULogicMgr = static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pItemSerialSMULogicMgr)
							pItemSerialSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;

					}
				/*case ST_PETCREATE_SMU:
					{
						SMULogicManager<PetProcreateItemSM>* pPetProSMULogicMgr = 
							static_cast<SMULogicManager<PetProcreateItemSM>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pPetProSMULogicMgr)
							pPetProSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;
					}
				case ST_CITY_SMU:
					{
						SMULogicManager<CitySMU>* pCitySMULogicMgr = 
							static_cast<SMULogicManager<CitySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pCitySMULogicMgr)
							pCitySMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;
					}
				case ST_GLOBALDATA_SMU:
					{
						SMULogicManager<GlobalDataSMU>* pGlobalDataSMULogicMgr = 
							static_cast<SMULogicManager<GlobalDataSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pGlobalDataSMULogicMgr)
							pGlobalDataSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;
					}
				case  ST_CSHOP_SMU:
					{
						SMULogicManager<CommisionShopSMU>* pCommisionShopSMULogicMgr = 
							static_cast<SMULogicManager<CommisionShopSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pCommisionShopSMULogicMgr)
							pCommisionShopSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;
					}*/
				default:
					{
						AssertEx(FALSE,"Unknow SMU Type");
					}
				}

			}
		}

		if(g_SMConfig.Commands.cmdType == CMD_CLEAR_ALL)
		{
			exit(0);
		}

		
		g_SMConfig.Commands.cmdType = CMD_UNKNOW;

		if(bExit)
		{
			bExitSign=TRUE;
		}

		return TRUE;

	}
	_MY_CATCH
	{
		g_pLog->FastSaveLog( LOG_FILE_14, "ShareMemory::DoWork() Get Error") ;
	
	}

	return FALSE;

}



BOOL ShareMemory::NewStaticManager( )
{
	__ENTER_FUNCTION

	g_pDBManager	=	 new	ShareDBManager();
	AssertEx(g_pDBManager,"分配数据库管理器失败!");
	g_pLog->FastSaveLog(LOG_FILE_14,"new ShareDBManager ...OK");

	
	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{
		m_SMUPool[i].m_Data = g_Config.m_ShareMemInfo.m_pShareData[i];

		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type) 
		{
		case ST_HUMAN_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<HumanSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<HumanSMU>();
				m_SMULogicManager[i].m_Type	=	ST_HUMAN_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<HumanSMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<HumanSMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<HumanSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<HumanSMU>()...OK" );
			}
			break;
		case ST_GUILD_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<GuildSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<GuildSMU>();
				m_SMULogicManager[i].m_Type	=	ST_GUILD_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<GuildSMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<GuildSMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<GuildSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<GuildSMU>()...OK" );
			}
			break;
		case ST_MAIL_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<MailSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<MailSMU>();
				m_SMULogicManager[i].m_Type	=	ST_MAIL_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<MailSMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<MailSMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<MailSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<MailSMU>()...OK" );
			}
			break;
		//case ST_PSHOP_SMU:
		//	{
		//		m_SMUPool[i].m_Pool = new SMUPool<PlayerShopSM>();
		//		m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<PlayerShopSM>();
		//		m_SMULogicManager[i].m_Type	=	ST_PSHOP_SMU;

		//		AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<PlayerShopSM>()...Fail!");
		//		AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<PlayerShopSM>()...Fail!");

		//		g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<PlayerShopSM>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<PlayerShopSM>()...OK" );
		//	}
		//	break;
		case ST_ITEMSERIAL_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<ItemSerialKeySMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<ItemSerialKeySMU>();
				m_SMULogicManager[i].m_Type	=	ST_ITEMSERIAL_SMU;	
				
				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<ItemSerialKeySMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<ItemSerialKeySMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<ItemSerialKeySMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<ItemSerialKeySMU>()...OK" );
			}
			break;
		/*case ST_PETCREATE_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<PetProcreateItemSM>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<PetProcreateItemSM>();
				m_SMULogicManager[i].m_Type	=	ST_PETCREATE_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<PetProcreateItemSM>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<PetProcreateItemSM>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<PetProcreateItemSM>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<PetProcreateItemSM>()...OK" );
			}
			break;
		case ST_CITY_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<CitySMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<CitySMU>();
				m_SMULogicManager[i].m_Type	=	ST_CITY_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<CitySMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<CitySMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<CitySMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<CitySMU>()...OK" );
			}
			break;
		case ST_GLOBALDATA_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<GlobalDataSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<GlobalDataSMU>();
				m_SMULogicManager[i].m_Type	=	ST_GLOBALDATA_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<GlobalDataSMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<GlobalDataSMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<GlobalDataSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<GlobalDataSMU>()...OK" );
			}
			break;
		case ST_CSHOP_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<CommisionShopSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<CommisionShopSMU>();
				m_SMULogicManager[i].m_Type	=	ST_CSHOP_SMU;

				AssertEx(m_SMUPool[i].m_Pool,"new SMUPool<CommisionShopSMU>()...Fail!");
				AssertEx(m_SMULogicManager[i].m_SMULogicManager,"new SMULogicManager<CommisionShopSMU>()...Fail!");

				g_pLog->FastSaveLog( LOG_FILE_14, "new SMUPool<CommisionShopSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "new SMULogicManager<CommisionShopSMU>()...OK" );
			}
			break;*/
		default:
			break;
		}
		
	}

	return TRUE ;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL ShareMemory::InitStaticManager( )
{

	__ENTER_FUNCTION

	BOOL bRet = FALSE;

	//DB 的初始化,连接数据库
	Assert(g_pDBManager);
	bRet = g_pDBManager->Init();
	if(!bRet)
	{
		g_pLog->FastSaveLog(LOG_FILE_14,"g_pDBManager->Init()...Fails");
		return FALSE;
	}
	else
		g_pLog->FastSaveLog(LOG_FILE_14,"g_pDBManager->Init()...OK");


			
	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{
		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type)
		{
		case ST_HUMAN_SMU:
			{
				SMUPool<HumanSMU>* pHumanSMUPool = static_cast<SMUPool<HumanSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pHumanSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pHumanSMUPool->Init(MAX_POOL_SIZE,key,SMPT_SHAREMEM,SHAREMEM_LOGFILE);
				Assert(bRet);
				SMULogicManager<HumanSMU>* pHumanSMULogicMgr = static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pHumanSMULogicMgr);
				bRet =	pHumanSMULogicMgr->Init(pHumanSMUPool);
				Assert(bRet);
			}
			break;
		case ST_GUILD_SMU:
			{
				SMUPool<GuildSMU>* pGuildSMUPool = static_cast<SMUPool<GuildSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pGuildSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pGuildSMUPool->Init(MAX_GUILD_SIZE,key,SMPT_SHAREMEM,SHAREMEM_LOGFILE);
				Assert(bRet);
				SMULogicManager<GuildSMU>* pGuildSMULogicMgr = static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pGuildSMULogicMgr);
				bRet =	pGuildSMULogicMgr->Init(pGuildSMUPool);
				Assert(bRet);
			}
			break;
		case ST_MAIL_SMU:
			{
				SMUPool<MailSMU>* pMailSMUPool = static_cast<SMUPool<MailSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pMailSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pMailSMUPool->Init(MAX_MAILNODE_SIZE,key,SMPT_SHAREMEM,SHAREMEM_LOGFILE);
				Assert(bRet);
				SMULogicManager<MailSMU>* pMailSMULogicMgr = static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pMailSMULogicMgr);
				bRet =	pMailSMULogicMgr->Init(pMailSMUPool);
				Assert(bRet);
			}
			break;
		//case ST_PSHOP_SMU:
		//	{
		//		SMUPool<PlayerShopSM>* pPlayerShopSMUPool = static_cast<SMUPool<PlayerShopSM>*>(m_SMUPool[i].m_Pool);
		//		Assert(pPlayerShopSMUPool);
		//		SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
		//		bRet = pPlayerShopSMUPool->Init(MAX_PLAYER_SHOP_POOL_PER_SERVER,key,SMPT_SHAREMEM,SHAREMEM_LOGFILE);
		//		Assert(bRet);
		//		SMULogicManager<PlayerShopSM>* pPlayerShopSMULogicMgr = static_cast<SMULogicManager<PlayerShopSM>*>(m_SMULogicManager[i].m_SMULogicManager);
		//		Assert(pPlayerShopSMULogicMgr);
		//		bRet =	pPlayerShopSMULogicMgr->Init(pPlayerShopSMUPool);
		//		Assert(bRet);
		//	}	
		//	break;

		case ST_ITEMSERIAL_SMU:
			{
				SMUPool<ItemSerialKeySMU>* pItemSerialSMUPool = static_cast<SMUPool<ItemSerialKeySMU>*>(m_SMUPool[i].m_Pool);
				Assert(pItemSerialSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pItemSerialSMUPool->Init(1,key,SMPT_SHAREMEM,SHAREMEM_LOGFILE);
				Assert(bRet);
				SMULogicManager<ItemSerialKeySMU>* pItemSerialSMULogicMgr = static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pItemSerialSMULogicMgr);
				bRet =	pItemSerialSMULogicMgr->Init(pItemSerialSMUPool);
				Assert(bRet);
			}
			break;
		/*case ST_PETCREATE_SMU:
			{
				SMUPool<PetProcreateItemSM>* pPetItemSMUPool = static_cast<SMUPool<PetProcreateItemSM>*>(m_SMUPool[i].m_Pool);
				Assert(pPetItemSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pPetItemSMUPool->Init(MAX_PET_PROCREATE_ITEM_NUM,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<PetProcreateItemSM>* pPetItemSMULogicMgr = 
					static_cast<SMULogicManager<PetProcreateItemSM>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pPetItemSMULogicMgr);
				bRet =	pPetItemSMULogicMgr->Init(pPetItemSMUPool);
				Assert(bRet);
			}
			break;
		case ST_CITY_SMU:
			{
				SMUPool<CitySMU>* pCitySMUPool = static_cast<SMUPool<CitySMU>*>(m_SMUPool[i].m_Pool);
				Assert(pCitySMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pCitySMUPool->Init(MAX_CITY_PER_WORLD,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<CitySMU>* pCitySMULogicMgr = 
					static_cast<SMULogicManager<CitySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pCitySMULogicMgr);
				bRet =	pCitySMULogicMgr->Init(pCitySMUPool);
				Assert(bRet);
			}
			break;
		case ST_GLOBALDATA_SMU:
			{
				SMUPool<GlobalDataSMU>* pGlobalDataSMUPool = static_cast<SMUPool<GlobalDataSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pGlobalDataSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pGlobalDataSMUPool->Init(MAX_WORLD_GLOBAL_DATA,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<GlobalDataSMU>* pGlobalDataSMULogicMgr = 
					static_cast<SMULogicManager<GlobalDataSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pGlobalDataSMULogicMgr);
				bRet =	pGlobalDataSMULogicMgr->Init(pGlobalDataSMUPool);
				Assert(bRet);
			}
			break;
		case ST_CSHOP_SMU:
			{
				SMUPool<CommisionShopSMU>* pCommisionShopSMUPool = static_cast<SMUPool<CommisionShopSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pCommisionShopSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pCommisionShopSMUPool->Init(MAX_COMMISIONSHOP,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<CommisionShopSMU>* pCommisionShopSMULogicMgr = 
					static_cast<SMULogicManager<CommisionShopSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pCommisionShopSMULogicMgr);
				bRet =	pCommisionShopSMULogicMgr->Init(pCommisionShopSMUPool);
				Assert(bRet);
			}
			break;*/
		default:
			AssertEx(bRet," ShareMemory::InitStaticManager fails");
			break;
		}
		
		
	}

	return bRet ;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL ShareMemory::DelStaticManager( )
{

	__ENTER_FUNCTION

	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{

		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type)
		{
		case ST_HUMAN_SMU:
			{

				SMUPool<HumanSMU>* pHumanSMUPool = 
					static_cast<SMUPool<HumanSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<HumanSMU>* pHumanSMULogicMgr = 
					static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pHumanSMUPool);
				SAFE_DELETE(pHumanSMULogicMgr);
                
				g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<HumanSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<HumanSMU>()...OK" );
			}
			break;
		case ST_GUILD_SMU:
			{
				SMUPool<GuildSMU>* pGuildSMUPool = 
					static_cast<SMUPool<GuildSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<GuildSMU>* pGuildSMULogicMgr = 
					static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pGuildSMUPool);
				SAFE_DELETE(pGuildSMULogicMgr);

				g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<GuildSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<GuildSMU>()...OK" );
			}
			break;
		case ST_MAIL_SMU:
			{
				SMUPool<MailSMU>* pMailSMUPool = 
					static_cast<SMUPool<MailSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<MailSMU>* pMailSMULogicMgr = 
					static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pMailSMUPool);
				SAFE_DELETE(pMailSMULogicMgr);

				g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<MailSMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<MailSMU>()...OK" );
			}
			break;
		case ST_ITEMSERIAL_SMU:
			{
				SMUPool<ItemSerialKeySMU>* pSMUPool = 
					static_cast<SMUPool<ItemSerialKeySMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<ItemSerialKeySMU>* pSMULogicMgr = 
					static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pSMUPool);
				SAFE_DELETE(pSMULogicMgr);

				g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<ItemSerialKeySMU>()...OK" );
				g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<ItemSerialKeySMU>()...OK" );
			}
			break;
		//case ST_PETCREATE_SMU:
		//	{
		//		SMUPool<PetProcreateItemSM>* pSMUPool = 
		//			static_cast<SMUPool<PetProcreateItemSM>*>(m_SMUPool[i].m_Pool);
		//		SMULogicManager<PetProcreateItemSM>* pSMULogicMgr = 
		//			static_cast<SMULogicManager<PetProcreateItemSM>*>(m_SMULogicManager[i].m_SMULogicManager);

		//		SAFE_DELETE(pSMUPool);
		//		SAFE_DELETE(pSMULogicMgr);

		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<PetProcreateItemSM>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<PetProcreateItemSM>()...OK" );
		//	}
		//	break;
		//case ST_PSHOP_SMU:
		//	{
		//		SMUPool<PlayerShopSM>* pSMUPool = 
		//			static_cast<SMUPool<PlayerShopSM>*>(m_SMUPool[i].m_Pool);
		//		SMULogicManager<PlayerShopSM>* pSMULogicMgr = 
		//			static_cast<SMULogicManager<PlayerShopSM>*>(m_SMULogicManager[i].m_SMULogicManager);

		//		SAFE_DELETE(pSMUPool);
		//		SAFE_DELETE(pSMULogicMgr);

		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<PlayerShopSM>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<PlayerShopSM>()...OK" );
		//	}
		//	break;
		
		//case ST_CITY_SMU:
		//	{
		//		SMUPool<CitySMU>* pSMUPool = 
		//			static_cast<SMUPool<CitySMU>*>(m_SMUPool[i].m_Pool);
		//		SMULogicManager<CitySMU>* pSMULogicMgr = 
		//			static_cast<SMULogicManager<CitySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
		//		SAFE_DELETE(pSMUPool);
		//		SAFE_DELETE(pSMULogicMgr);

		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<CitySMU>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<CitySMU>()...OK" );
		//	}
		//	break;
		//case ST_GLOBALDATA_SMU:
		//	{
		//		SMUPool<GlobalDataSMU>* pSMUPool = 
		//			static_cast<SMUPool<GlobalDataSMU>*>(m_SMUPool[i].m_Pool);
		//		SMULogicManager<GlobalDataSMU>* pSMULogicMgr = 
		//			static_cast<SMULogicManager<GlobalDataSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
		//		SAFE_DELETE(pSMUPool);
		//		SAFE_DELETE(pSMULogicMgr);

		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<GlobalDataSMU>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<GlobalDataSMU>()...OK" );
		//	}
		//	break;
		//case ST_CSHOP_SMU:
		//	{
		//		SMUPool<CommisionShopSMU>* pSMUPool = 
		//			static_cast<SMUPool<CommisionShopSMU>*>(m_SMUPool[i].m_Pool);
		//		SMULogicManager<CommisionShopSMU>* pSMULogicMgr = 
		//			static_cast<SMULogicManager<CommisionShopSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
		//		SAFE_DELETE(pSMUPool);
		//		SAFE_DELETE(pSMULogicMgr);

		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMUPool<CommisionShopSMU>()...OK" );
		//		g_pLog->FastSaveLog( LOG_FILE_14, "del SMULogicManager<CommisionShopSMU>()...OK" );
		//	}
		//	break;
		default:
			AssertEx(FALSE," ShareMemory::DelStaticManager fails");
			break;
		}
		
		
	}

	SAFE_DELETE(g_pDBManager);

	return TRUE ;

	__LEAVE_FUNCTION	
	return FALSE;
}

BOOL	CheckSaveAllFile()
{
	__ENTER_FUNCTION
	
	
	if( remove( "saveall.cmd" ) == -1 )
	{
				return FALSE;
	}
			
	return TRUE;


	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	CheckExitFile()
{
	__ENTER_FUNCTION


	if( remove( "exit.cmd" ) == -1 )
	{
			return FALSE;
	}
	extern	BOOL g_LockTimeOutEnable;
	g_LockTimeOutEnable = TRUE;
	return TRUE;


	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	CheckStartSaveLogout()
{
__ENTER_FUNCTION
	if( remove( "startsave.cmd" ) == -1 )
	{
		return FALSE;
	}
	extern	BOOL g_NeedSaveLogout;
	g_NeedSaveLogout = TRUE;

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL	CheckStopSaveLogout()
{
__ENTER_FUNCTION
	if( remove( "stopsave.cmd" ) == -1 )
	{
		return FALSE;
	}
	extern	BOOL g_NeedSaveLogout;
	g_NeedSaveLogout = FALSE;

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

ShmExceptionHandler::ShmExceptionHandler()
{
#ifdef __LINUX__
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, SEGHandler);
	signal(SIGFPE,  FPEHandler);
	signal(SIGILL,  ILLHandler);
	signal(SIGINT,  INTHandler);  
	signal(SIGTERM, TERMHandler); 
	signal(SIGABRT, ABORTHandler);
	signal(SIGXFSZ, XFSZHandler);
#endif
}

VOID ShmExceptionHandler::INTHandler(INT)
{
	DumpStack("INT exception:\r\n");
	exit(0);
}
VOID ShmExceptionHandler::TERMHandler(INT)
{
	DumpStack("TERM exception:\r\n");
	exit(0);

}
VOID ShmExceptionHandler::ABORTHandler(INT)
{
	DumpStack("ABORT exception:\r\n");
	exit(0);
}

VOID ShmExceptionHandler::ILLHandler(INT)
{
	DumpStack("ILL exception:\r\n");
	exit(0);
}

VOID ShmExceptionHandler::FPEHandler(INT)
{
	DumpStack("FPE exception:\r\n");
	exit(0);
}

VOID ShmExceptionHandler::SEGHandler(INT)
{
	DumpStack("SEG exception:\r\n");
	exit(0);
}
VOID ShmExceptionHandler::XFSZHandler(INT)
{
	DumpStack("XFSZ exception:\r\n");
	exit(0);
}

ShmExceptionHandler g_ShmExceptionHandler;
