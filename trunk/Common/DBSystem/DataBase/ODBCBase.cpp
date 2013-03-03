#include "stdafx.h"
#include "ODBCBase.h"
#include "DBManager.h"
#include "DBInterface.h"
#include "Log.h"

ODBCBase::ODBCBase()
{
	mResult = 0;
	mResultCount = 0;
	mOPType = DB_INIT_EMPTY;
}

UINT ODBCBase::GetResultCount()
{
	return mResultCount;
}

DB_QUERY*	ODBCBase::GetInternalQuery()
{
	return &mInterface->GetQuery();
}
LONG_DB_QUERY*	ODBCBase::GetLongInterQuery()
{
	return &mInterface->GetLongQuery();
}

INT			ODBCBase::GetInternalAffectCount()
{
	return mInterface->GetAffectedRowCount();
}

BOOL	ODBCBase::IsPrepare()
{
	__ENTER_FUNCTION

	return mInterface->IsPrepare();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ODBCBase::CheckDBConnect(const CHAR* pInfo)
{
	__ENTER_FUNCTION
		Assert(mInterface);
	CHAR info[256] = {0};
	if(pInfo)	strncpy(info,pInfo,255);

	if(!mInterface->IsConnected())
	{
		//DiskLog(SHMEM_LOG_PATH,"ERROR:%s...connect database is fails",info);
		for(INT i = 0; i < 5; ++i)
		{
			MySleep(5000);
			//�ݲ�ʹ��g_DBConnectThread�������Ⲣ������[2007/12/03 YangJun]
			//����ӿ�ֻ��ShareMemory������ʹ��
			if(mInterface->Connect())
			{
				//DiskLog(SHMEM_LOG_PATH,"ERROR:%s...Reconnect database",info);
				return TRUE;
			}

			//DiskLog(SHMEM_LOG_PATH,"ERROR:%s...Can't connect database(%d)",info,i);
		}

		return FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	ODBCBase::Load()
{
	__ENTER_FUNCTION


	if(!IsPrepare())
		return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_LOAD;

	mResult = pInterface->Execute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ODBCBase::LongLoad()
{
	__ENTER_FUNCTION


		if(!IsPrepare())
			return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_LOAD;

	mResult = pInterface->LongExecute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	ODBCBase::AddNew()
{
	__ENTER_FUNCTION

	if(!IsPrepare())
		return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_ADDNEW;

	mResult = pInterface->Execute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ODBCBase::Delete()
{
	__ENTER_FUNCTION

    if(!IsPrepare())
		return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_DELETE;

	mResult = pInterface->Execute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL	ODBCBase::Save(VOID* pSource)
{

	__ENTER_FUNCTION

	if(!IsPrepare())
		return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_SAVE;

	mResult = pInterface->Execute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ODBCBase::LongSave(VOID* pSource)
{

	__ENTER_FUNCTION

		if(!IsPrepare())
			return FALSE;

	ODBCInterface* pInterface= 	mInterface;

	if(!pInterface)
		return FALSE;

	mOPType = DB_SAVE;

	mResult = pInterface->LongExecute();

	mResultCount	= pInterface->mAffectCount;

	return mResult;

	__LEAVE_FUNCTION

		return FALSE;
}



VOID ODBCBase::SetDBName(DB_NAMES dbName)
{
	mDBName = dbName;
}

INT	ODBCBase::GetErrorCode()
{
	return mInterface->GetError();
}

CHAR* ODBCBase::GetErrorMessage()
{
	return mInterface->GetErrorMsg();
}

