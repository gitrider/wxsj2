#include "DBPetItemInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "SMUManager.h"
#include "ShopInfoLoad.h"
#include "GameStruct_Pet.h"

DBPetItemInfo::DBPetItemInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBPetItemInfo::Load()
{
	__ENTER_FUNCTION

	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	if(!pQuery)
	{
			Assert(FALSE);
	}
	pQuery->Clear();
	pQuery->Parse(LoadPetItemInfo,PETITEM_TABLE);

	return ODBCBase::LongLoad();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPetItemInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_PetPairData	=	1,
	};

	INT Result;

	SMUPool<PetProcreateItemSM>* pPoolPtr = static_cast<SMUPool<PetProcreateItemSM>*>(pSource);
	Assert(pPoolPtr);


	PetProcreateItem PetProItem;

	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();
		
		if(SmuCount>MAX_PET_PROCREATE_ITEM_NUM)
			Assert(FALSE);

		INT UseStats;

		for(INT i = 0 ;i<SmuCount;i++)
		{
			PetProcreateItemSM*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UseStats =pSMU->GetUseStatus(SM_C_READ);
			pSMU->Lock(SM_C_READ);
			PetProcreateItem* pPetItem = &pSMU->m_PetProcreateItem;
			Assert(pPetItem);
			if(pPetItem == NULL)
			{
				pSMU->UnLock(SM_C_READ);
				Assert(FALSE);
			}

			if(pPetItem->IsInvalid() == TRUE && UseStats != PET_DELETE)
			{
				pSMU->UnLock(SM_C_READ);
				continue;
			}
			memcpy(&PetProItem,pPetItem,sizeof(PetProcreateItem));
			pSMU->m_SMUHead.UseStatus = PET_SAVED;
			pSMU->UnLock(SM_C_READ);
			
			//继续存储
			mInterface->Clear();
			CheckDBConnect("DBPetItemInfo");

			LONG_DB_QUERY* pQuery = GetLongInterQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();

			CHAR PairDataList[20480] = {0};
			Binary2String(((CHAR*)pPetItem),sizeof(PetProcreateItem),
						   PairDataList);

			pQuery->Parse(SavePetItemInfo,
						  PairDataList,
						  i,
						  ((UseStats!=PET_DELETE)?1:0));

			ODBCBase::LongSave(&Result);

			//保存数据版本
			pQuery->Clear();
			pQuery->Parse(SavePetItemDataVersion,PETITEM_TABLE,PETPROCREATE_CURRENT_ITEM_VERSION,i);
			ODBCBase::LongSave(&Result);
		}

	}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPetItemInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}





BOOL DBPetItemInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			SMUPool<PetProcreateItemSM>* pPoolPtr = static_cast<SMUPool<PetProcreateItemSM>*>(pResult);
			Assert(pPoolPtr);
			enum 
			{
				DB_PetPairIndex	=	1,
				DB_DataVersion	=	2,
				DB_PetPairData	=	3,
			};
			
			//加载玩家商店属性
			Assert(mInterface);
			INT ErrorCode;
			
			INT		SmuCount	=	pPoolPtr->GetPoolMaxSize();

			for(INT i =0;i<MAX_PET_PROCREATE_ITEM_NUM;i++)
			{
				if(!mInterface->LongFetch())
					break;
				if(i>= SmuCount)
					Assert(FALSE);

				INT iIndex			= mInterface->GetInt(DB_PetPairIndex,ErrorCode);
				PetProcreateItemSM*	pSMU = pPoolPtr->GetPoolObj(iIndex);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				INT nVersion = mInterface->GetInt(DB_DataVersion,ErrorCode);

				if(PETPROCREATE_ITEM_VERSION_0 == nVersion)
				{
					PetProcreateItem_VO	petPro_V0;

					mInterface->GetLongField(DB_PetPairData,
						(CHAR*)(&(petPro_V0)),sizeof(PetProcreateItem_VO),ErrorCode);

					PetProcreateInfoLoad::LoadFrom_V0(pSMU->m_PetProcreateItem,petPro_V0);
				}
				else if(PETPROCREATE_ITEM_VERSION_1 == nVersion)
				{
					PetProcreateItem_V1	petPro_V1;

					mInterface->GetLongField(DB_PetPairData,
						(CHAR*)(&(petPro_V1)),sizeof(PetProcreateItem_V1),ErrorCode);

					PetProcreateInfoLoad::LoadFrom_V1(pSMU->m_PetProcreateItem,petPro_V1);
				}
				else if(PETPROCREATE_CURRENT_ITEM_VERSION == nVersion)
				{
					mInterface->GetLongField(DB_PetPairData,
						(CHAR*)(&(pSMU->m_PetProcreateItem)),sizeof(PetProcreateItem),ErrorCode);
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
