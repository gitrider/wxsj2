#include "stdafx.h" 
#include "DBCharVisualGem.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBCharVisualGem::DBCharVisualGem(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBCharVisualGem::Load()
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

	pQuery->Parse(LoadCharVisualGem,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBCharVisualGem::Save(VOID* pSource)
{
	__ENTER_FUNCTION
	
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存CharVisualGem数据
	do
	{
		CHAR	VisualGemInfo[2048];
		memset(VisualGemInfo,0,2048);
	
		//获取所有装备的物品显示信息
		DB_CHAR_EQUIP_LIST eqlist;
		for(INT i = 0; i < HEQUIP_NUMBER; ++i)
		{
			_ITEM* pItem = pCharFullData->GetItem(i+DB_EQUIP_POS_START);
			if(pItem->IsNullType()) continue;

			eqlist.m_Equip[i] = pItem->m_ItemIndex;
			//eqlist.m_EquipVisual[i] = pItem->GetEquipData()->m_Visual;
			//eqlist.m_EquipGemID[i] = pItem->GetEquipmentMaxLevelGemID();
		}

		Binary2String((CHAR*)(&eqlist),
					  sizeof(DB_CHAR_EQUIP_LIST),
					  VisualGemInfo);

		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharVisualGem,
					  CHAR_TABLE,
					  VisualGemInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCharVisualGem::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCharVisualGem::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			DB_CHAR_EQUIP_LIST* pCharEquipList = static_cast<DB_CHAR_EQUIP_LIST*>(pResult);
			Assert(pCharEquipList);

			enum 
			{
				DB_CharGuid	=	1,
				DB_VisulaGem,
			};

			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				mInterface->GetField(DB_VisulaGem,
					(CHAR*)(pCharEquipList),
					sizeof(DB_CHAR_EQUIP_LIST),
					ErrorCode) ;
			}
			
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
	
		}
		break;
	case DB_SAVE:
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

GUID_t	DBCharVisualGem::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCharVisualGem::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBCharVisualGem::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBCharVisualGem::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}