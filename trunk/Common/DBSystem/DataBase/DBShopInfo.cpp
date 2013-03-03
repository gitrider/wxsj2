#include "DBShopInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "GameStruct.h"
#include "SMUManager.h"
#include "ShopInfoLoad.h"
#include "PlayerShopDefine.h"

using namespace PLAYER_SHOP;

DBShopInfo::DBShopInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

VOID	DBShopInfo::SetServerID(ID_t sid)
{
	mServerID = sid;
}

ID_t	DBShopInfo::GetServerID()
{
	return mServerID;
}

BOOL DBShopInfo::Load()
{
	__ENTER_FUNCTION

	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	if(!pQuery)
	{
			Assert(FALSE);
	}
	pQuery->Clear();
	pQuery->Parse(LoadShopInfo,SHOP_TABLE,mServerID);

	return ODBCBase::LongLoad();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBShopInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_ShopGUID	=	1,
		DB_Type,
		DB_Stat,
		DB_MaxbMoney,
		DB_BaseMoney,
		DB_Profit,
		DB_CreateTime,
		DB_ShopName,
		DB_ShopDesc,
		DB_OwnerName,
		DB_OwnerGuid,
		DB_IsOpen,
		DB_Sale,
		DB_SalePrice,
		DB_Partner,
		DB_PartNum,
		DB_SubType,
	};

	INT Result;

	SMUPool<PlayerShopSM>* pPoolPtr = static_cast<SMUPool<PlayerShopSM>*>(pSource);
	Assert(pPoolPtr);

	//保存商店数据

	PlayerShopDB_t	ShopDB;
	PlayerShopBuyShopDB_t	ShopBuyShopDB;

	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();
		
		if(SmuCount>MAX_PLAYER_SHOP_POOL_PER_SERVER)
			Assert(FALSE);

		INT UseStats;

		for(INT i = 0 ;i<SmuCount;i++)
		{
			PlayerShopSM*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UseStats =pSMU->GetUseStatus(SM_C_READ);
			pSMU->Lock(SM_C_READ);
			PlayerShopDB_t* pShop = &pSMU->m_PlayerShop;
			PlayerShopBuyShopDB_t* pShopBuyShop = &pSMU->m_PlayerShopBuyShop;
			Assert(pShop);
			Assert(pShopBuyShop);
			if(pShop == NULL)
			{
					pSMU->UnLock(SM_C_READ);
					Assert(FALSE);
			}

			if(pShop->m_ShopGUID.isNull())
			{
				pSMU->UnLock(SM_C_READ);
				continue;
			}
			memcpy(&ShopDB,pShop,sizeof(PlayerShopDB_t));
			memcpy(&ShopBuyShopDB,pShopBuyShop,sizeof(PlayerShopBuyShopDB_t));
			pSMU->m_SMUHead.UseStatus = SHOP_SAVED;
			pSMU->UnLock(SM_C_READ);
			
			//继续存储
			mInterface->Clear();
			CheckDBConnect("DBShopInfo");

			// 名称含有转义字符的直接跳过[6/4/2007]
			if(!StrSafeCheck(ShopDB.m_szShopDesc,MAX_SHOP_DESC_SIZE_DB))
			{
				ShopDB.m_szShopDesc[0]	= '\0';
			}

			LONG_DB_QUERY* pQuery = GetLongInterQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();

			CHAR ShopGuid[200] ={0};
			Binary2String((CHAR*)&ShopDB.m_ShopGUID,sizeof(_PLAYERSHOP_GUID),ShopGuid);

			CHAR PartList[2048] = {0};
			Binary2String(((CHAR*)ShopDB.m_PartnerList),sizeof(Partner_t)*MAX_PARTNER_PER_SHOP,
						   PartList);

			pQuery->Parse(SaveShopInfo,
						  mServerID,
						  i,
						  ShopGuid,
						  ShopDB.m_ShopType,
						  ShopDB.m_ShopStatus,
						  ShopDB.m_MaxBaseMoney,
						  ShopDB.m_BaseMoney,
						  ShopDB.m_ProfitMoney,
						  ShopDB.m_FoundedTime,
						  ShopDB.m_szShopName,
						  ShopDB.m_szShopDesc,
						  ShopDB.m_szOwnerName,
						  ShopDB.m_OwnerGuid,	
						  ShopDB.m_NumStallOpened, 
						  ShopDB.m_NumStallOnSale,
						  ShopDB.m_uSaleOutPrice,
						  PartList,
						  ((UseStats!=SHOP_DELETE)?1:0),
						  ShopDB.m_PartnerNum,
						  ShopDB.m_ShopSubType);

			ODBCBase::LongSave(&Result);

			//保存数据版本
			pQuery->Clear();
			pQuery->Parse(SaveShopDataVersion,SHOP_TABLE,SHOP_CURRENT_STALL_VERSION,mServerID,i);
			ODBCBase::LongSave(&Result);

			//商店买卖记录保存
			pQuery->Clear();
		
			CHAR	ShopRecorder[MAX_LONG_COLUMN_BUFFER] = {0};
				
			PlayerShopDB_t::RecordText Rt;
			ShopDB.GetRecord(Rt);

			Binary2String((CHAR*)&Rt,
						  sizeof(Rt),
						  ShopRecorder);
			pQuery->Parse(SaveShopRecord,
						  SHOP_TABLE,
						  ShopRecorder,
						  mServerID,
						  i);

			ODBCBase::LongSave(&Result);

			//商店柜台详细信息保存
			pQuery->Clear();

			memset(ShopRecorder,0,sizeof(CHAR)*MAX_LONG_COLUMN_BUFFER);

			
			
			Binary2String((CHAR*)(ShopDB.m_StallListData),
						sizeof(StallBoxDB_t)*MAX_STALL_NUM_PER_SHOP,
						ShopRecorder);
			pQuery->Parse(SaveShopStall,
						 SHOP_TABLE,
						 ShopRecorder,
						 mServerID,
						 i);
			ODBCBase::LongSave(&Result);

			//商店材料收购信息保存
			pQuery->Clear();
			memset(ShopRecorder,0,sizeof(CHAR)*MAX_LONG_COLUMN_BUFFER);
			Binary2String((CHAR*)(&ShopBuyShopDB),
				sizeof(PlayerShopBuyShopDB_t),
				ShopRecorder);
			pQuery->Parse(SaveShopBuyInfo,
				SHOP_TABLE,
				ShopRecorder,
				mServerID,
				i);
			ODBCBase::LongSave(&Result);	
		}

	}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBShopInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}





BOOL DBShopInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			SMUPool<PlayerShopSM>* pPoolPtr = static_cast<SMUPool<PlayerShopSM>*>(pResult);
			Assert(pPoolPtr);
			
			enum 
			{
				DB_ShopGUID	=	1,
				DB_Type,
				DB_Stat,
				DB_MaxbMoney,
				DB_BaseMoney,
				DB_Profit,
				DB_CreateTime,
				DB_ShopName,
				DB_ShopDesc,
				DB_OwnerName,
				DB_OwnerGuid,
				DB_IsOpen,
				DB_Sale,
				DB_SalePrice,
				DB_Partner,
				DB_PartNum,
				DB_SubType,
				DB_DataVersion,
			};
			
			//加载玩家商店属性
			
			Assert(mInterface);
			INT ErrorCode;
			
			INT		SmuCount	=	pPoolPtr->GetPoolMaxSize();
			
			_PLAYERSHOP_GUID	ShopGuid;
			
			INT		nDataVersion[MAX_PLAYER_SHOP_POOL_PER_SERVER];
			memset(nDataVersion,0,sizeof(INT)*MAX_PLAYER_SHOP_POOL_PER_SERVER);

			for(INT i =0;i<MAX_PLAYER_SHOP_POOL_PER_SERVER;i++)
			{
				if(!mInterface->Fetch())
					break;
				if(i>= SmuCount)
					Assert(FALSE);

				mInterface->GetField(DB_ShopGUID,(CHAR*)(&ShopGuid),sizeof(ShopGuid),ErrorCode);
			

				if(ShopGuid.m_PoolPos>= SmuCount||ShopGuid.m_PoolPos<0)
					Assert(FALSE);

				PlayerShopSM*	pSMU = pPoolPtr->GetPoolObj(ShopGuid.m_PoolPos);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				PlayerShopDB_t* pShop = &pSMU->m_PlayerShop;
				Assert(pShop);
				pShop->m_ShopGUID	= ShopGuid;
				pShop->m_ShopType		= mInterface->GetInt(DB_Type,ErrorCode);
				pShop->m_ShopStatus		= mInterface->GetInt(DB_Stat,ErrorCode);
				pShop->m_MaxBaseMoney	= mInterface->GetInt(DB_MaxbMoney,ErrorCode);
				pShop->m_BaseMoney		= mInterface->GetInt(DB_BaseMoney,ErrorCode);
				pShop->m_ProfitMoney	= mInterface->GetInt(DB_Profit,ErrorCode);
				pShop->m_FoundedTime	= mInterface->GetUInt(DB_CreateTime,ErrorCode);
				
				mInterface->GetString(DB_ShopName,
									  pShop->m_szShopName,
									  MAX_SHOP_NAME_SIZE_DB,
									  ErrorCode);
				mInterface->GetString(DB_ShopDesc,
									  pShop->m_szShopDesc,
									  MAX_SHOP_DESC_SIZE_DB,
									  ErrorCode);
				mInterface->GetString(DB_OwnerName,
									  pShop->m_szOwnerName,
									  MAX_CHARACTER_NAME,
									  ErrorCode);
				pShop->m_OwnerGuid	= mInterface->GetUInt(DB_OwnerGuid,ErrorCode);
				pShop->m_NumStallOpened =	mInterface->GetUInt(DB_IsOpen,ErrorCode);
				pShop->m_NumStallOnSale	=	mInterface->GetUInt(DB_Sale,ErrorCode);
				pShop->m_uSaleOutPrice	=	mInterface->GetUInt(DB_SalePrice,ErrorCode);
				mInterface->GetField(DB_Partner,
									((CHAR*)(pShop->m_PartnerList)),
									 sizeof(Partner_t)*MAX_PARTNER_PER_SHOP,
									 ErrorCode);
				pShop->m_PartnerNum		=	mInterface->GetUInt(DB_PartNum,ErrorCode);
				pShop->m_ShopSubType	=	mInterface->GetInt(DB_SubType,ErrorCode);
				
				Assert(0<=ShopGuid.m_PoolPos && MAX_PLAYER_SHOP_POOL_PER_SERVER>ShopGuid.m_PoolPos);
				nDataVersion[ShopGuid.m_PoolPos]			=	mInterface->GetInt(DB_DataVersion,ErrorCode);
			}
			mInterface->Clear();
			

			enum 
			{
				DB_ShopRecord	= 1,
			};

			enum 
			{
				DB_ShopStall	= 1,
			};

			enum
			{
				DB_ShopBuyInfo	= 1,
			};
			
			for(INT i =0;i<MAX_PLAYER_SHOP_POOL_PER_SERVER;i++)
			{
				PlayerShopSM*	pSMU = pPoolPtr->GetPoolObj(i);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				
				PlayerShopDB_t* pShop = &pSMU->m_PlayerShop;
				PlayerShopBuyShopDB_t* pShopBuyShop = &pSMU->m_PlayerShopBuyShop;
				Assert(pShop);
				Assert(pShopBuyShop);

				if(pShop->m_ShopGUID.isNull())
					continue;

				LONG_DB_QUERY* pQuery = GetLongInterQuery();
				if(!pQuery)
				{
					Assert(FALSE);
				}

				//商店买卖信息加载
				pQuery->Clear();
				pQuery->Parse(LoadShopRecord,SHOP_TABLE,
					pShop->m_ShopGUID.m_Server,pShop->m_ShopGUID.m_PoolPos);

				ODBCBase::LongLoad();

				if(!mInterface->LongFetch())
					continue;

				PlayerShopDB_t::RecordText Rt;
			
				mInterface->GetLongField(DB_ShopRecord,
									(CHAR*)&Rt,
									sizeof(Rt),
									ErrorCode);
				mInterface->Clear();

				pShop->SetRecord(Rt);

				//商店柜台详细信息加载
				pQuery->Clear();
				pQuery->Parse(LoadShopStall,SHOP_TABLE,
							  pShop->m_ShopGUID.m_Server,pShop->m_ShopGUID.m_PoolPos);

				ODBCBase::LongLoad();
				
				if(!mInterface->LongFetch())
					continue;

				Assert(0<=pShop->m_ShopGUID.m_PoolPos && MAX_PLAYER_SHOP_POOL_PER_SERVER>pShop->m_ShopGUID.m_PoolPos);
				if(SHOP_STALL_VERSION_0 == nDataVersion[pShop->m_ShopGUID.m_PoolPos])
				{
					StallBoxDB_t_V0 stBox[MAX_STALL_NUM_PER_SHOP];

					mInterface->GetLongField(DB_ShopStall,
						(CHAR*)(stBox),
						sizeof(StallBoxDB_t_V0)*MAX_STALL_NUM_PER_SHOP,
						ErrorCode);

					for(INT k=0;k<MAX_STALL_NUM_PER_SHOP;++k)
					{
						ShopInfoLoad::LoadFrom_V0(pShop->m_StallListData[k],stBox[k]);
					}
				}
				else if(SHOP_STALL_VERSION_1 == nDataVersion[pShop->m_ShopGUID.m_PoolPos])
				{
					StallBoxDB_t_V1 stBox[MAX_STALL_NUM_PER_SHOP];

					mInterface->GetLongField(DB_ShopStall,
						(CHAR*)(stBox),
						sizeof(StallBoxDB_t_V1)*MAX_STALL_NUM_PER_SHOP,
						ErrorCode);

					for(INT k=0;k<MAX_STALL_NUM_PER_SHOP;++k)
					{
						ShopInfoLoad::LoadFrom_V1(pShop->m_StallListData[k],stBox[k]);
					}
				}
				else if(SHOP_CURRENT_STALL_VERSION == nDataVersion[pShop->m_ShopGUID.m_PoolPos])
				{
					mInterface->GetLongField(DB_ShopStall,
										 (CHAR*)(pShop->m_StallListData),
										 sizeof(StallBoxDB_t)*MAX_STALL_NUM_PER_SHOP,
										 ErrorCode);
				}

				mInterface->Clear();

				//商店材料收购信息加载
				pQuery->Clear();
				pQuery->Parse(LoadShopBuyInfo,SHOP_TABLE,
					pShop->m_ShopGUID.m_Server,pShop->m_ShopGUID.m_PoolPos);

				ODBCBase::LongLoad();

				if(!mInterface->LongFetch())
					continue;

				if(SHOP_STALL_VERSION_0 == nDataVersion[pShop->m_ShopGUID.m_PoolPos] ||
					SHOP_STALL_VERSION_1 == nDataVersion[pShop->m_ShopGUID.m_PoolPos] )
				{
					PlayerShopBuyShopDB_t_V1	stShop;

					mInterface->GetLongField(DB_ShopBuyInfo,
						(CHAR*)(&stShop),
						sizeof(PlayerShopBuyShopDB_t_V1),
						ErrorCode);

					ShopInfoLoad::LoadFrom_BuyShop_V1(pSMU->m_PlayerShopBuyShop,stShop);
				}
				else if(SHOP_CURRENT_STALL_VERSION == nDataVersion[pShop->m_ShopGUID.m_PoolPos])
				{
					mInterface->GetLongField(DB_ShopBuyInfo,
						(CHAR*)(pShopBuyShop),
						sizeof(PlayerShopBuyShopDB_t),
						ErrorCode);
				}

				mInterface->Clear();
			}

		
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
