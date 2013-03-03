#include "DBCityInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "SMUManager.h"

DBCityInfo::DBCityInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBCityInfo::Load()
{
	__ENTER_FUNCTION

	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	if(!pQuery)
	{
			Assert(FALSE);
	}
	pQuery->Clear();
	pQuery->Parse(LoadCityInfo,CITYINFO_TABLE);

	return ODBCBase::LongLoad();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCityInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_PetPairData	=	1,
	};

	INT Result;

	SMUPool<CitySMU>* pPoolPtr = static_cast<SMUPool<CitySMU>*>(pSource);
	Assert(pPoolPtr);


	CityDB_t	CityInfo;

	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();
		
		if(SmuCount>MAX_CITY_PER_WORLD)
			Assert(FALSE);

		INT UseStats;

		for(INT i = 0 ;i<SmuCount;i++)
		{
			CitySMU*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UseStats =pSMU->GetUseStatus(SM_C_READ);
			pSMU->Lock(SM_C_READ);
			CityDB_t* pCityInfo = &pSMU->m_CitySMU;
			Assert(pCityInfo);
			if(pCityInfo == NULL)
			{
				pSMU->UnLock(SM_C_READ);
				Assert(FALSE);
			}

			if(pCityInfo->m_GuildID==INVALID_ID)
			{
				pSMU->UnLock(SM_C_READ);
				continue;
			}
			memcpy(&CityInfo,pCityInfo,sizeof(CityDB_t));
			pSMU->m_SMUHead.UseStatus = CITY_SAVED;
			if(UseStats ==CITY_DELETE)
			{
				pCityInfo->CleanUp();
			}
			pSMU->UnLock(SM_C_READ);

			//继续存储
			mInterface->Clear();
			CheckDBConnect("DBCityInfo");

			LONG_DB_QUERY* pQuery = GetLongInterQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();

			CHAR CityList[153600] = {0};
			Binary2String(((CHAR*)pCityInfo),sizeof(CityDB_t),
						   CityList);

			pQuery->Parse(SaveCityInfo,
						  CityList,
						  i,
						  ((UseStats!=CITY_DELETE)?1:0));

			ODBCBase::LongSave(&Result);

		}

	}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCityInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}





BOOL DBCityInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			SMUPool<CitySMU>* pPoolPtr = static_cast<SMUPool<CitySMU>*>(pResult);
			Assert(pPoolPtr);
			enum 
			{
				DB_CityIndex =  1,
				DB_CityData	 =	2,
			};
			
			//加载玩家商店属性
			Assert(mInterface);
			INT ErrorCode;
			
			INT		SmuCount	=	pPoolPtr->GetPoolMaxSize();

			for(INT i =0;i<MAX_CITY_PER_WORLD;i++)
			{
				if(!mInterface->LongFetch())
					break;
				if(i>= SmuCount)
					Assert(FALSE);

				CityDB_t CityInfo;
				CityInfo.CleanUp();

				INT iIndex = mInterface->GetInt(DB_CityIndex,ErrorCode);
				
				mInterface->GetLongField(DB_CityData,
					(CHAR*)(&CityInfo),sizeof(CityDB_t),ErrorCode);

				if(iIndex>=0&& iIndex<MAX_CITY_PER_WORLD)
				{
					CitySMU*	pSMU = pPoolPtr->GetPoolObj(iIndex);
					if(!pSMU)
					{
						Assert(pSMU);
						return FALSE;
					}

					pSMU->m_CitySMU = CityInfo;
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
