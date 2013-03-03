#ifndef _SMU_LOGIC_MANAGER_H_
#define _SMU_LOGIC_MANAGER_H_
#include "Type.h"
#include "SMUManager.h"
#include "Log.h"
#include "TimeManager.h"
#include "CommandThread.h"

#define MAX_SERVER_IDLE_TIME	5000  //������ֹͣ��Ӧʱ��(����)

extern GLOBAL_CONFIG	g_SMConfig;
extern BOOL				g_CleanBattleGuild;		//������г��еİ����ս��Ϣ

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
				
				//�����Ӧ�����ݱ�־
				//����Server������û��崻����µ�ShareMemory������󱨣���
				//����İ����н�ɫ�ı������ݱ�ʶ��գ�����ˣ�����ȥ����DoClear�ĵ���[2008/03/07 YangJun]
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
	BOOL				DoSaveAll();			//ͣ��ʱ�������ݿ��¼
	BOOL				DoNormalSave();			//�������и������ݿ��¼
	BOOL				DoClear();
	BOOL				DoPostInit();			//����������ʱ�����ݿ��������

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