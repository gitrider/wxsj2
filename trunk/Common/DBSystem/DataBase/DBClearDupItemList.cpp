#include "stdafx.h" 
#include "DBClearDupItemList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBClearDupItemList::DBClearDupItemList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		=	0;
	Assert(pInterface);
	mInterface		=	pInterface;

//	for(INT i = 0; i < DB_BANK_POS_END; ++i)	m_DupList[i].CleanUp();
}

BOOL DBClearDupItemList::Save(VOID* pSource)
{
__ENTER_FUNCTION

	//DB_QUERY* pQuery = GetInternalQuery();

	//if(!pQuery)
	//{
	//	Assert(FALSE);
	//}

	//if(m_CharGuid==INVALID_ID)
	//{
	//	return FALSE;
	//}

	//enum
	//{
	//	DB_CharGUID = 1,
	//	DB_ItemPos,
	//	DB_ItemType,
	//	DB_dbVersion,
	//};

	//FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	//Assert(pCharFullData);
	//INT	   ErrorCode;

	//for(INT ItemPos =0;ItemPos<=DB_BANK_POS_END;ItemPos++)
	//{//��ÿһ����Ʒ������ɾ���Ƽ��
	//	_ITEM* pItem = pCharFullData->GetItem(ItemPos);
	//	if(pItem->IsNullType()) continue;

	//	pQuery->Clear();
	//	mInterface->Clear();
	//	//�������д�Ψһ�ŵ���Ʒ
	//	pQuery->Parse(LoadDupItemList,ITEM_TABLE,pItem->m_ItemGUID.m_Serial,pItem->m_ItemGUID.m_World,pItem->m_ItemGUID.m_Server);
	//	if(!ODBCBase::Load())
	//		return FALSE;

		////��¼�����ظ�����Ʒ���
		//DupItemList& dList = m_DupList[ItemPos];
		//dList.SetItemGUID(&(pItem->m_ItemGUID));

	//	for(INT DupPos = 0; DupPos<MAX_DUPITEM_NUM; ++DupPos)
	//	{
	//		if(!mInterface->Fetch())
	//			break;

	//		DupItemUnit dUnit;

	//		dUnit.m_CharGUID		= mInterface->GetInt(DB_CharGUID,ErrorCode);
	//		dUnit.m_itemPos			= mInterface->GetInt(DB_ItemPos,ErrorCode);
	//		dUnit.m_itemType		= mInterface->GetInt(DB_ItemType,ErrorCode);
	//		dUnit.m_itemdbVersion	= mInterface->GetInt(DB_dbVersion,ErrorCode);

	//		//�����DBItemList�ոձ�����Ǹ���Ʒ��һ�����Ǹ�����Ʒ
	//		//���̫���ӣ��޷����ǵ�����Щ����û���ã���Щ�Ǹ�����Ʒ��Ŀǰû�кõĸ��η���
	//		//if( dUnit.m_CharGUID == m_CharGuid && 
	//		//	dUnit.m_itemType == pItem->m_ItemIndex &&
	//		//	dUnit.m_itemdbVersion >= (INT)m_DBVersion)
	//		//	continue;
	//		//if(dUnit.m_CharGUID)
	//		//dList.AddDupUnit(&dUnit);
	//	}
	//}

	//pQuery->Parse(DeleteCharItemSingle,ITEM_TABLE,m_DBVersion,m_CharGuid,m_DBVersion);

	//return ODBCBase::Save(pCharFullData);
__LEAVE_FUNCTION
	return FALSE;
}

BOOL DBClearDupItemList::ParseResult(VOID* pResult)
{
__ENTER_FUNCTION

	Assert(mInterface);
	mInterface->Clear();

	return TRUE;

__LEAVE_FUNCTION
	return FALSE;
}

GUID_t	DBClearDupItemList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBClearDupItemList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBClearDupItemList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBClearDupItemList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}
