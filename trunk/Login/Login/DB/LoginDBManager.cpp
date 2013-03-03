#include "stdafx.h" 
#include "LoginDBManager.h"
#include "Config.h"
#include "ODBCInterface.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "Log.h"
#include "FoxEncrypt.h"

DBManager*	g_pDBManager	=	NULL;

BOOL	LoginDBManager::Init()
{
		__ENTER_FUNCTION

		//��Config ��ȡLogin DB ��ص�����
		CHAR	Host[HOST_STR_LEN];
		strncpy(Host,g_Config.m_LoginInfo.m_DBIP,HOST_STR_LEN);						//���ӶԶ�IP
		Host[HOST_STR_LEN-1] = '\0';

		UINT	Port							=	g_Config.m_LoginInfo.m_DBPort;	//���ӶԶ˶˿�
		CHAR	Database[DATABASE_STR_LEN];
		strncpy(Database,g_Config.m_LoginInfo.m_DBName,DATABASE_STR_LEN);			//���ݿ�����
		Database[DATABASE_STR_LEN-1] = '\0';

		CHAR	User[DB_USE_STR_LEN];												//�û�����
		strncpy(User,g_Config.m_LoginInfo.m_DBUser,DB_USE_STR_LEN);
		User[DB_USE_STR_LEN-1] = '\0';

		CHAR	Password[DB_PASSWORD_STR_LEN];										//����
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
			g_pLog->FastSaveLog(LOG_FILE_0,"mCharDBInterface->Connect()... Get Errors: %s ",mCharDBInterface->GetErrorMsg());
		}

		//return mCharDBInterface->IsConnected();

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
}





VOID LoginDBManager::run()
{
	__ENTER_FUNCTION
	
	  printf("run db Thread \r\n");
		
	   UINT uTime = g_pTimeManager->CurrentTime();
		
		g_DBLogicManager.m_ThreadID	= getTID();

		while (IsActive())
		{
			MySleep(100);

			if(!mCharDBInterface->IsConnected())
			{
				MySleep(5000);//��֤��Ƶ���������ݿ�

				if(0 != g_DBConnectThread)
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"LoginDBManager g_DBConnectThread = 0 IGNORE reconnect database,TID=%d",getTID());
					continue;
				}

				//ʹ��g_DBConnectThread�������Ⲣ������[2007/12/03 YangJun]
				//�����ػ��߳��ﶨʱ����g_DBConnectThread = 0
				g_DBConnectThread = (INT)getTID();
				BOOL bRet =	mCharDBInterface->Connect();
				g_DBConnectThread = 0;

				if(!bRet)
				{
					g_pLog->FastSaveLog(LOG_FILE_0,"mCharDBInterface->Connect() Get Errors: %s",mCharDBInterface->GetErrorMsg());
				}
				else
					g_pLog->FastSaveLog(LOG_FILE_0,"Reconnect database");
				
			}

			g_DBLogicManager.HeartBeat(uTime);
		}

		printf("end db Thread \r\n");

		__LEAVE_FUNCTION
}
