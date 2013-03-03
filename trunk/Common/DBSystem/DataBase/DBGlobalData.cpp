#include "DBGlobalData.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "SMUManager.h"

DBGlobalData::DBGlobalData(ODBCInterface* pInterface)
{
	mDBName			= CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBGlobalData::Load()
{
__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	pQuery->Parse(LoadGlobalData,GLOBALDATA_TABLE);

	return ODBCBase::Load();

__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBGlobalData::Save(VOID* pSource)
{
__ENTER_FUNCTION

	Assert(pSource);
	INT Result;

	SMUPool<GlobalDataSMU>* pPoolPtr = static_cast<SMUPool<GlobalDataSMU>*>(pSource);
	Assert(pPoolPtr);

	GlobalData	gdata;
	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();

		if(SmuCount>MAX_WORLD_GLOBAL_DATA)
			Assert(FALSE);

		for(INT i = 0 ;i<SmuCount;i++)
		{
			GlobalDataSMU*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			pSMU->Lock(SM_C_READ);
			gdata = pSMU->m_GlobalDataSMU;
			//pSMU->m_SMUHead.UseStatus = CITY_SAVED;
			pSMU->UnLock(SM_C_READ);

			if(0 >= gdata)
			{
				continue;
			}

			DB_QUERY* pQuery = GetInternalQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();

			pQuery->Parse(SaveGlobalData,
				i,
				gdata);

			ODBCBase::Save(&Result);
		}

	}while(0);
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBGlobalData::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBGlobalData::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			Assert(pResult);
			SMUPool<GlobalDataSMU>* pPoolPtr = static_cast<SMUPool<GlobalDataSMU>*>(pResult);
			Assert(pPoolPtr);
			enum 
			{
				DB_PoolID			=	1,
				DB_Data1,
			};

			Assert(mInterface);
			INT ErrorCode;

			INT		SmuCount	=	pPoolPtr->GetPoolMaxSize();
			for(INT i =0;i<MAX_WORLD_GLOBAL_DATA;i++)
			{
				if(!mInterface->Fetch())
					break;
				if(i>= SmuCount)
					Assert(FALSE);

				INT iIndex			= mInterface->GetInt(DB_PoolID,ErrorCode);
				GlobalData gdata	= mInterface->GetInt(DB_Data1,ErrorCode);
				
				if(iIndex>=0&& iIndex<MAX_WORLD_GLOBAL_DATA)
				{
					GlobalDataSMU*	pSMU = pPoolPtr->GetPoolObj(iIndex);
					if(!pSMU)
					{
						Assert(pSMU);
						return FALSE;
					}

					pSMU->m_GlobalDataSMU = gdata;
				}
			}
			
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
	
		}
		break;
	default:
		break;
	}
	
	Assert(mInterface);
	mInterface->Clear();
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
