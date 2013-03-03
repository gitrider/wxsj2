#include "DBCommitShopInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "GameStruct.h"
#include "SMUManager.h"

DBCommitShopInfo::DBCommitShopInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;

	mWorldID = mServerID = INVALID_ID;
}

VOID	DBCommitShopInfo::SetServerID(ID_t sid)
{
	mServerID = sid;
}

ID_t	DBCommitShopInfo::GetServerID()
{
	return mServerID;
}

VOID	DBCommitShopInfo::SetWorldID(ID_t sid)
{
	mWorldID = sid;
}

ID_t	DBCommitShopInfo::GetWorldID()
{
	return mWorldID;
}

BOOL DBCommitShopInfo::Load()
{
__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();
	if(!pQuery)
	{
		Assert(FALSE);
	}
	
	if(mWorldID<0 || mServerID<0)	return FALSE;

	pQuery->Clear();

	pQuery->Parse(LoadCommitShop,CSHOP_TABLE,mWorldID,mServerID);

	return ODBCBase::Load();

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBCommitShopInfo::Save(VOID* pSource)
{
__ENTER_FUNCTION

	if(mWorldID<0 || mServerID<0)	return FALSE;

	INT Result;
	
	SMUPool<CommisionShopSMU>* pPoolPtr = static_cast<SMUPool<CommisionShopSMU>*>(pSource);
	Assert(pPoolPtr);
	//保存Mail数据
	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();
		if(SmuCount>MAX_COMMISIONSHOP)
		Assert(FALSE);

		_COMMISION_SHOP	mCommisionShopInfo;
		CommisionShopID_t shopId;
		INT UseStats;

		for(INT i = 0 ;i<SmuCount;i++)
		{
			CommisionShopSMU*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UseStats =pSMU->GetUseStatus(SM_C_READ);
			if(UseStats == COMMISIONSHOP_CHANGED)
			{
				pSMU->Lock(SM_C_READ);
				_COMMISION_SHOP* pCommisionShop = &(pSMU->m_CommisionShopSMU);
				
				if(pCommisionShop == NULL)
				{
					pSMU->UnLock(SM_C_READ);
					Assert(FALSE);
				}
				
				mCommisionShopInfo = *pCommisionShop;
				pSMU->m_SMUHead.UseStatus = COMMISIONSHOP_SAVED;
				pSMU->UnLock(SM_C_READ);
				
				shopId = mCommisionShopInfo.m_ShopID;
			}
			else
			{
				continue;
			}

			if(shopId<0)	//寄售商店属于NPC挂接，不需考虑删除的问题
			{
				continue;
			}
		
			//继续存储
			mInterface->Clear();
			CheckDBConnect("DBCommitShopInfo");

			DB_QUERY* pQuery = GetInternalQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();
			pQuery->Parse(SaveCommitShop,mWorldID,mServerID,i,mCommisionShopInfo.m_ShopID);

			if(!ODBCBase::Save(&Result))
			{
				pSMU->SetUseStatus(COMMISIONSHOP_CHANGED,SM_C_WRITE);
				continue;
			}

			//保存寄售商店的寄售信息
			for(INT k=0; k<MAX_COMMISIONSHOP_ITEM; k++)
			{
				_COMMISIONSHOP_ITEM* pItem = &(mCommisionShopInfo.m_aCommisionItem[k]);				
				Assert(pItem);
				CommisionShopID_t shopId = mCommisionShopInfo.m_ShopID;

				mInterface->Clear();
				CheckDBConnect("DBCommitShopInfoItem");

				pQuery->Clear();
				if(pItem->IsEmpty())
				{
					pQuery->Parse(DeleteCommitShopItem,CSHOPITEM_TABLE,shopId,k);
					
					if(!ODBCBase::Delete())
					{
						pSMU->SetUseStatus(COMMISIONSHOP_CHANGED,SM_C_WRITE);
						continue;
					}
				}
				else
				{
					if(!StrSafeCheck(pItem->m_szSellerName,MAX_CHARACTER_NAME))
						Assert(FALSE);

					pQuery->Parse(SaveCommitShopItem,
						shopId,
						k,
						pItem->m_uSerialNumber,
						pItem->m_SellerGUID,
						pItem->m_szSellerName,
						pItem->m_eItemType,
						pItem->m_nValue,
						pItem->m_nPrice,
						pItem->m_uStartTime,
						pItem->m_uSaleTime);

					if(!ODBCBase::Save(&Result))
					{
						pSMU->SetUseStatus(COMMISIONSHOP_CHANGED,SM_C_WRITE);
						continue;
					}
				}// end of if(pItem->IsEmpty())
			}// end of for(INT k=0; k<MAX_COMMISIONSHOP_ITEM; k++)
		}//end of for(INT i = 0 ;i<SmuCount;i++)

	}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCommitShopInfo::Delete()
{
__ENTER_FUNCTION

	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}


BOOL DBCommitShopInfo::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			SMUPool<CommisionShopSMU>* pPoolPtr = static_cast<SMUPool<CommisionShopSMU>*>(pResult);
			Assert(pPoolPtr);
			enum 
			{
				DB_PoolID	=	1,
				DB_CShopID,
			};

			//加载寄售商店属性
			Assert(mInterface);
			INT ErrorCode;
			INT	SmuCount = pPoolPtr->GetPoolMaxSize();
			INT pIndex;
			for(INT i =0;i<MAX_COMMISIONSHOP;i++)
			{
				if(!mInterface->Fetch())
					break;

				if(i>= SmuCount)
					Assert(FALSE);

				pIndex = mInterface->GetUInt(DB_PoolID,ErrorCode);

				if(pIndex>= SmuCount)
					Assert(FALSE);

				CommisionShopSMU*	pSMU = pPoolPtr->GetPoolObj(pIndex);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				_COMMISION_SHOP* pCommitShop = &pSMU->m_CommisionShopSMU;
				Assert(pCommitShop);
				
				pCommitShop->m_ShopID = mInterface->GetUInt(DB_CShopID,ErrorCode);
			}

			mInterface->Clear();

			//加载寄售商店的寄售信息
			enum 
			{
				DB_ShopPos	=	1,
				DB_Serial,
				DB_CharGUID,
				DB_CharName,
				DB_GTYPE,
				DB_GVALUE,
				DB_SVALUE,
				DB_CTIME,
				DB_COSTCTIME,
			};

			for(INT i =0;i<MAX_COMMISIONSHOP;i++)
			{
				CommisionShopSMU*	pSMU = pPoolPtr->GetPoolObj(i);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				_COMMISION_SHOP* pCommitShop = &pSMU->m_CommisionShopSMU;
				Assert(pCommitShop);
				CommisionShopID_t CommisionShopId = pCommitShop->m_ShopID;

				if(CommisionShopId<0)	continue;

				DB_QUERY* pQuery = GetInternalQuery();
				Assert(pQuery);
				pQuery->Clear();

				pQuery->Parse(LoadCommitShopItem,CSHOPITEM_TABLE,CommisionShopId);
				mInterface->Clear();
				
				if(!ODBCBase::Load())	return FALSE;

				//按照物品的位置来设置寄售信息
				for(INT k=0; k<MAX_COMMISIONSHOP_ITEM;k++)
				{
					if(!mInterface->Fetch())
						break;

					INT nPos = mInterface->GetInt(DB_ShopPos,ErrorCode);

					if(nPos<0||nPos>=MAX_COMMISIONSHOP_ITEM)
						Assert(FALSE);

					_COMMISIONSHOP_ITEM* pItem = &(pCommitShop->m_aCommisionItem[nPos]);
					Assert(pItem);

					pItem->m_uSerialNumber	= mInterface->GetUInt(DB_Serial,ErrorCode);
					pItem->m_SellerGUID		= mInterface->GetUInt(DB_CharGUID,ErrorCode);
					mInterface->GetString(DB_CharName,pItem->m_szSellerName,MAX_CHARACTER_NAME,ErrorCode);
					pItem->m_eItemType		= (ECSIType_T)mInterface->GetShort(DB_GTYPE,ErrorCode);
					pItem->m_nValue			= mInterface->GetInt(DB_GVALUE,ErrorCode);
					pItem->m_nPrice			= mInterface->GetInt(DB_SVALUE,ErrorCode);
					pItem->m_uStartTime		= mInterface->GetUInt(DB_CTIME,ErrorCode);
					pItem->m_uSaleTime		= mInterface->GetUInt(DB_COSTCTIME,ErrorCode);
				}
			}

			mInterface->Clear();
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
