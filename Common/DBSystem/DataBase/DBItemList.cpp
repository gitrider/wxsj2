#include "stdafx.h"
#include "DBItemList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBItemList::DBItemList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		=	0;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBItemList::Load()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_CharGuid==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadCharItemList,ITEM_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemList::Save(VOID* pSource)
{
	__ENTER_FUNCTION
	
	//先删除旧有低版本道具
	//INT Result;
	//Delete();
	//ParseResult(&Result);

	//再保存所有的道具
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	for(INT ItemPos =0;ItemPos<=DB_BANK_POS_END;ItemPos++)
	{
		_MY_TRY
		{
			_ITEM* pItem = pCharFullData->GetItem(ItemPos);

			DB_QUERY* pQuery = GetInternalQuery();

			if(!pQuery)
			{
				Assert(FALSE);
			}

			pQuery->Clear();

			if(m_CharGuid==INVALID_ID)
			{
				return FALSE;
			}
			
			//不存在的物品进行删除
			if(pItem->IsNullType())
			{
				pQuery->Parse(DeleteCharItemSingle,ITEM_TABLE,m_DBVersion,m_CharGuid,ItemPos,m_DBVersion);

				if(!ODBCBase::Delete())
					return FALSE;

				mInterface->Clear();
				continue;
			}

			//物品参数
			INT OutLength;
			INT PArray[256] = {0};
			pItem->DBReadAttr((CHAR*)PArray,OutLength,sizeof(INT)*(255));

			Assert(OutLength / sizeof(INT) <= 40);

			// 装备宝石和显示外形[1/11/2007]
			//UINT uGemID = INVALID_ID;
			//USHORT uVisual = 0;
			//if(ICLASS_EQUIP == pItem->ItemClass())
			//{
			//	uGemID = pItem->GetEquipmentMaxLevelGemID();
			//	uVisual = pItem->GetEquipData()->m_Visual;
			//}

			pQuery->Parse(NewCharItem,
						m_CharGuid,	//charguid
						pItem->m_ItemGUID.m_Serial,
						pItem->m_ItemGUID.m_World,
						pItem->m_ItemGUID.m_Server,
						pItem->m_ItemIndex,
						ItemPos,
						0,
						-1,
						"",
						PArray[0],
						PArray[1],
						PArray[2],
						PArray[3],
						PArray[4],
						PArray[5],
						PArray[6],
						PArray[7],
						PArray[8],
						PArray[9],
						PArray[10],
						PArray[11],
						PArray[12],
						PArray[13],
						PArray[14],
						PArray[15],
						PArray[16],
						PArray[17],
						PArray[18],
						PArray[19],
						PArray[20],
						PArray[21],
						PArray[22],
						PArray[23],
						PArray[24],
						PArray[25],
						PArray[26],
						PArray[27],
						PArray[28],
						PArray[29],
						PArray[30],
						PArray[31],
						PArray[32],
						PArray[33],
						PArray[34],
						PArray[35],
						PArray[36],
						PArray[37],
						PArray[38],
						PArray[39], 
						1,
						m_DBVersion,
						pItem->m_Creator,
						"");

			if(!ODBCBase::Save(pCharFullData))
				return FALSE;
		}
		_MY_CATCH
		{
			return FALSE;
		}
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemList::Delete()
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBItemList::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType) 
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);
			enum
			{
				DB_CharGuid	=	1,
				DB_ItemWorld,
				DB_ItemServer,
				DB_ItemGuid,
				DB_ItemType,
				DB_ItemPos,
				DB_FixAttr,  //也可以去掉,但是Login 需要读装备表
				DB_P1,
				DB_P2,
				DB_P3,
				DB_P4,
				DB_P5,
				DB_P6,
				DB_P7,
				DB_P8,
				DB_P9,
				DB_P10,
				DB_P11,
				DB_P12,
				DB_P13,
				DB_P14,
				DB_P15,
				DB_P16,
				DB_P17,
				DB_P18,
				DB_P19,
				DB_P20,
				DB_P21,
				DB_P22,
				DB_P23,
				DB_P24,
				DB_P25,
				DB_P26,
				DB_P27,
				DB_P28,
				DB_P29,
				DB_P30,
				DB_P31,
				DB_P32,
				DB_P33,
				DB_P34,
				DB_P35,
				DB_P36,
				DB_P37,
				DB_P38,
				DB_P39,
				DB_P40,
				DB_Creator,
				DB_Var,
			};

			Assert(mResultCount<=(DB_BANK_POS_END+1));

			Assert(mInterface);
			INT	   ErrorCode;

			//加载物品属性
			for(INT i =0;i<(DB_BANK_POS_END+1);i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				USHORT	ItemPos  = mInterface->GetWORD(DB_ItemPos,ErrorCode);
				Assert(ItemPos<=DB_BANK_POS_END);

				_ITEM* pItem = pCharFullData->GetItem(ItemPos);

				Assert(pItem);

				pItem->m_ItemGUID.m_World	=	mInterface->GetBYTE(DB_ItemWorld,ErrorCode);
				pItem->m_ItemGUID.m_Server	=	mInterface->GetBYTE(DB_ItemServer,ErrorCode);
				pItem->m_ItemGUID.m_Serial	=	mInterface->GetUInt(DB_ItemGuid,ErrorCode);
				pItem->m_ItemIndex			=	mInterface->GetUInt(DB_ItemType,ErrorCode);
				
				
				INT	PArray[255] = {0};
				PArray[0]		=	mInterface->GetInt(DB_P1,ErrorCode);
				PArray[1]		=	mInterface->GetInt(DB_P2,ErrorCode);
				PArray[2]		=	mInterface->GetInt(DB_P3,ErrorCode);
				PArray[3]		=	mInterface->GetInt(DB_P4,ErrorCode);
				PArray[4]		=	mInterface->GetInt(DB_P5,ErrorCode);
				PArray[5]		=	mInterface->GetInt(DB_P6,ErrorCode);
				PArray[6]		=	mInterface->GetInt(DB_P7,ErrorCode);
				PArray[7]		=	mInterface->GetInt(DB_P8,ErrorCode);
				PArray[8]		=	mInterface->GetInt(DB_P9,ErrorCode);
				PArray[9]		=	mInterface->GetInt(DB_P10,ErrorCode);
				PArray[10]		=	mInterface->GetInt(DB_P11,ErrorCode);
				PArray[11]		=	mInterface->GetInt(DB_P12,ErrorCode);
				PArray[12]		=	mInterface->GetInt(DB_P13,ErrorCode);
				PArray[13]		=	mInterface->GetInt(DB_P14,ErrorCode);
				PArray[14]		=	mInterface->GetInt(DB_P15,ErrorCode);
				PArray[15]		=	mInterface->GetInt(DB_P16,ErrorCode);
				PArray[16]		=	mInterface->GetInt(DB_P17,ErrorCode);
				PArray[17]		=	mInterface->GetInt(DB_P18,ErrorCode);
				PArray[18]		=	mInterface->GetInt(DB_P19,ErrorCode);
				PArray[19]		=	mInterface->GetInt(DB_P20,ErrorCode);

				PArray[20]		=	mInterface->GetInt(DB_P21,ErrorCode);
				PArray[21]		=	mInterface->GetInt(DB_P22,ErrorCode);
				PArray[22]		=	mInterface->GetInt(DB_P23,ErrorCode);
				PArray[23]		=	mInterface->GetInt(DB_P24,ErrorCode);
				PArray[24]		=	mInterface->GetInt(DB_P25,ErrorCode);
				PArray[25]		=	mInterface->GetInt(DB_P26,ErrorCode);
				PArray[26]		=	mInterface->GetInt(DB_P27,ErrorCode);
				PArray[27]		=	mInterface->GetInt(DB_P28,ErrorCode);
				PArray[28]		=	mInterface->GetInt(DB_P29,ErrorCode);
				PArray[29]		=	mInterface->GetInt(DB_P30,ErrorCode);
				PArray[30]		=	mInterface->GetInt(DB_P31,ErrorCode);
				PArray[31]		=	mInterface->GetInt(DB_P32,ErrorCode);
				PArray[32]		=	mInterface->GetInt(DB_P33,ErrorCode);
				PArray[33]		=	mInterface->GetInt(DB_P34,ErrorCode);
				PArray[34]		=	mInterface->GetInt(DB_P35,ErrorCode);
				PArray[35]		=	mInterface->GetInt(DB_P36,ErrorCode);
				PArray[36]		=	mInterface->GetInt(DB_P37,ErrorCode);
				PArray[37]		=	mInterface->GetInt(DB_P38,ErrorCode);
				PArray[38]		=	mInterface->GetInt(DB_P39,ErrorCode);
				PArray[39]		=	mInterface->GetInt(DB_P40,ErrorCode);

				//序列化p1 ~ p40

				{
					pItem->DBWriteAttr((CHAR*)PArray,sizeof(PArray));
				}
				mInterface->GetString(DB_Creator,pItem->m_Creator,MAX_ITEM_CREATOR_NAME,ErrorCode);
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

GUID_t	DBItemList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBItemList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBItemList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBItemList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}