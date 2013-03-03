#ifndef _SMU_LOGIC_MANAGER_H_
#define _SMU_LOGIC_MANAGER_H_
#include "Type.h"
#include "SMUManager.h"
#include "Log.h"
#include "TimeManager.h"
#include "CommandThread.h"

#define MAX_SERVER_IDLE_TIME	5000  //服务器停止响应时间(毫秒)

extern GLOBAL_CONFIG	g_SMConfig;
extern BOOL				g_CleanBattleGuild;		//清除所有城市的帮会宣战信息

template<typename T>
class SMULogicManager
{
public:
	SMULogicManager():
	  m_PoolSharePtr(0),
	  m_pSMU(0),
	  m_FinalSaveTime(0)
	{}
	~SMULogicManager()
	{}

public:
	BOOL		Init(SMUPool<T>* pPool)
	{
		if(!pPool)
			return FALSE;
		m_PoolSharePtr = pPool;
		m_PoolSharePtr->SetHeadVer(0);
		m_OldCheckTime = g_pTimeManager->RunTime();
		m_OldVer = 0;

		return DoPostInit();
		
	}

public:

	BOOL				HeartBeat()
	{
		
		UINT uTime  = g_pTimeManager->RunTime();
		
		if((uTime-m_OldCheckTime)>MAX_SERVER_IDLE_TIME)
		{
			m_OldCheckTime = uTime;
			UINT ver = m_PoolSharePtr->GetHeadVer();
			if((ver == m_OldVer) && m_OldVer>0)
			{
				m_OldVer = 0;
				
				g_pLog->FastSaveLog(LOG_FILE_14,"Receive Server Crash command..");
				BOOL bRet =  DoSaveAll();
				
				//清理对应的数据标志
				//由于Server程序本身没有宕机导致的ShareMemory程序的误报，会
				//错误的把所有角色的保有数据标识清空！！因此，必须去掉对DoClear的调用[2008/03/07 YangJun]
				//DoClear();		

				m_PoolSharePtr->SetHeadVer(0);
				return bRet;
			}
			m_OldVer = ver;
		}
			
			
		

		switch(g_SMConfig.Commands.cmdType) 
		{
		case CMD_SAVE_ALL:
			{
				return DoSaveAll();
				break;
			}
		case CMD_CLEAR_ALL:
			{
				return DoClear();
				break;
			}
		case CMD_UNKNOW:
			{
				return DoNormalSave();
				break;
			}
		default:
			{
				return DoDefault(g_SMConfig.Commands.cmdType);
			}
		}
		
	
			
	}
	BOOL				DoSaveAll();			//停机时更新数据库记录
	BOOL				DoNormalSave();			//在心跳中更新数据库记录
	BOOL				DoClear();
	BOOL				DoPostInit();			//服务器开机时从数据库加载数据

	BOOL				DoDefault(SM_COMMANDS cmd);
private:

	SMUPool<T>*		m_PoolSharePtr;
	T*				m_pSMU;
	UINT			m_FinalSaveTime;
	BOOL			m_bReady;
	UINT			m_OldVer;
	UINT			m_OldCheckTime;

};


#endif