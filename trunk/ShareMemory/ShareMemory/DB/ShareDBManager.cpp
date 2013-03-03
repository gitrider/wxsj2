#include "stdafx.h"
#include "ShareDBManager.h"
#include "Config.h"
#include "ODBCInterface.h"
#include "TimeManager.h"
#include "Log.h"
#include "FoxEncrypt.h"

DBManager*	g_pDBManager	=	NULL;


BOOL	ShareDBManager::Init()
{
__ENTER_FUNCTION

	//��Config ��ȡLogin DB ��ص�����
	CHAR	Host[HOST_STR_LEN];
	strncpy(Host,g_Config.m_ShareMemInfo.m_DBIP,HOST_STR_LEN);						//���ӶԶ�IP
	Host[HOST_STR_LEN-1] = '\0';

	UINT	Port							=	g_Config.m_ShareMemInfo.m_DBPort;	//���ӶԶ˶˿�
	CHAR	Database[DATABASE_STR_LEN];
	strncpy(Database,g_Config.m_ShareMemInfo.m_DBName,DATABASE_STR_LEN);			//���ݿ�����
	Database[DATABASE_STR_LEN-1] = '\0';

	CHAR	User[DB_USE_STR_LEN];												//�û�����
	strncpy(User,g_Config.m_ShareMemInfo.m_DBUser,DB_USE_STR_LEN);
	User[DB_USE_STR_LEN-1] = '\0';

	CHAR	Password[DB_PASSWORD_STR_LEN];										//����
	if(0 >= g_Config.m_ShareMemInfo.CryptPwd)
	{
	strncpy(Password,g_Config.m_ShareMemInfo.m_DBPassword,DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN-1] = '\0';
	}
	else
	{
		memset(Password,0,DB_PASSWORD_STR_LEN);
		string2pwd(g_Config.m_ShareMemInfo.m_DBPassword,Password);
	}

	Assert(mCharDBInterface);

	mCharDBInterface->Connect(Database,
		User,
		Password);

	if(!mCharDBInterface->IsConnected())
	{
		g_pLog->FastSaveLog(LOG_FILE_14,"mCharDBInterface->Connect()... Get Errors: %s ",mCharDBInterface->GetErrorMsg());
	}

	return mCharDBInterface->IsConnected();


__LEAVE_FUNCTION

	return FALSE;
}

VOID ShareDBManager::run()
{
	__ENTER_FUNCTION

		
	__LEAVE_FUNCTION
}