#include "stdafx.h" 
#include "DBThread.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "ODBCInterface.h"
#include "Config.h"
#include "Log.h"
#include "FoxEncrypt.h"

DBThread::DBThread()
{
	AutoLock_T autolock(m_Lock);
	m_Active = TRUE;
	mCharDBInterface = new ODBCInterface;
}
DBThread::~DBThread()
{
	AutoLock_T autolock(m_Lock);
	SAFE_DELETE(mCharDBInterface);
}

BOOL DBThread::Init()
{
	__ENTER_FUNCTION

	//从Config 读取Login DB 相关的数据
	CHAR	Host[HOST_STR_LEN];
	strncpy(Host,g_Config.m_LoginInfo.m_DBIP,HOST_STR_LEN);						//连接对端IP
	Host[HOST_STR_LEN-1] = '\0';

	UINT	Port							=	g_Config.m_LoginInfo.m_DBPort;	//连接对端端口
	CHAR	Database[DATABASE_STR_LEN];
	strncpy(Database,g_Config.m_LoginInfo.m_DBName,DATABASE_STR_LEN);			//数据库名称
	Database[DATABASE_STR_LEN-1] = '\0';

	CHAR	User[DB_USE_STR_LEN];												//用户名称
	strncpy(User,g_Config.m_LoginInfo.m_DBUser,DB_USE_STR_LEN);
	User[DB_USE_STR_LEN-1] = '\0';

	CHAR	Password[DB_PASSWORD_STR_LEN];										//密码
	if(0 >= g_Config.m_LoginInfo.CryptPwd)
	{
	strncpy(Password,g_Config.m_LoginInfo.m_DBPassword,DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN-1] = '\0';
	}
	else
	{
		memset(Password,0,DB_PASSWORD_STR_LEN);
		string2pwd(g_Config.m_LoginInfo.m_DBPassword,Password);
	}

	Assert(mCharDBInterface);

	mCharDBInterface->Connect(Database,
		User,
		Password);

	if(!mCharDBInterface->IsConnected())
	{
		g_pLog->FastSaveLog(LOG_FILE_0,"DBThread:mCharDBInterface->Connect()... Get Errors: %s ",
			mCharDBInterface->GetErrorMsg());
	}
	return mCharDBInterface->IsConnected();
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

ODBCInterface*		DBThread::GetInterface()
{
	return mCharDBInterface;
}

BOOL				DBThread::IsConnected()
{
	return mCharDBInterface->IsConnected();
}


VOID	DBThread::run()
{
	__ENTER_FUNCTION

			while (m_Active)
			{
				//Test connection
				Assert(mCharDBInterface);
				if(mCharDBInterface->IsConnected() == FALSE)
				{
					MySleep(5000);//保证不频繁连接数据库

					if(0 != g_DBConnectThread)
					{
						g_pLog->FastSaveLog(LOG_FILE_0,"DBThread g_DBConnectThread = 0 IGNORE reconnect database,TID=%d",getTID());
						continue;
					}
					
					//使用g_DBConnectThread尽量避免并发重连[2007/12/03 YangJun]
					//请在守护线程里定时设置g_DBConnectThread = 0
					g_DBConnectThread = (INT)getTID();
					BOOL bRet = mCharDBInterface->Connect();
					g_DBConnectThread = 0;

					if(!bRet)
					{
						g_pLog->FastSaveLog(LOG_FILE_0,"DBThread::mCharDBInterface->Connect() Get Errors: %s,TID = %d,ErrorCode =%d",
							mCharDBInterface->GetErrorMsg(),getTID(),mCharDBInterface->GetError());
						continue;	// 保证在重连失败的情况下不继续处理[4/16/2007]
					}
					else
						g_pLog->FastSaveLog(LOG_FILE_0,"DBThread::Reconnect database,TID=%d",getTID());
				}
				//execute Commands
				MySleep(1);
				BOOL bRet = ProcessCacheCommand();
				Assert(bRet);
			}

	__LEAVE_FUNCTION


}

BOOL	DBThread::ProcessCacheCommand()
{
	PlayerID_t PlayerID ;

	_MY_TRY
	{
		BOOL ret = FALSE ;
		
		Packet* pPacket = NULL ;
		

		ret = RecvPacket( pPacket, PlayerID);
		
	
		if( !ret )
			return TRUE ;
			
		Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
		if( pPlayer )
		{
			pPacket->Execute(pPlayer) ;
		}
		else
		{
			pPacket->Execute(NULL);
		}

		g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			
		return TRUE;
	}

	_MY_CATCH
	{
		g_pLog->FastSaveLog(LOG_FILE_0,"DBThread::ProcessCacheCommand Get Errors,TID=%d,PlayerID =%d",getTID(),PlayerID);
		return TRUE;
	}


}

BOOL	DBThread::SendPacket(Packet* pPacket,PlayerID_t pID)
{
	__ENTER_FUNCTION	

		AutoLock_T autolock(m_Lock);
		
		if(m_Input.m_pPacket == NULL)
		{
			m_Input.m_pPacket	= pPacket;
			m_Input.m_PlayerID  = pID;

			return TRUE;
		}
		else
		{
			return FALSE;
		}

	__LEAVE_FUNCTION	
	return FALSE;
}

BOOL	DBThread::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID)
{
	__ENTER_FUNCTION
	
		AutoLock_T autolock(m_Lock);
		
		if(m_Input.m_pPacket == NULL)
		{
			return FALSE;
		}
		else
		{
			pPacket		= m_Input.m_pPacket;
			PlayerID	= m_Input.m_PlayerID;
			m_Input.CleanUp();
			return TRUE;
		}

	__LEAVE_FUNCTION

		return FALSE;
}
