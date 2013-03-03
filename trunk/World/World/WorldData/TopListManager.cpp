
#include "stdafx.h"
#include "TopListManager.h"
#include "WorldTable.h"

TopListManager* g_pTopListManager = 0;

TopListManager::TopListManager()
{
__ENTER_FUNCTION

	for( INT i=0; i<TOP_LIST_NUM; i++ )
	{
		m_pTopList[i] = 0;
	}

	CleanUp( );

__LEAVE_FUNCTION
}


TopListManager::~TopListManager()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}


BOOL TopListManager::Init( )
{
__ENTER_FUNCTION
		
	for(UINT i=0;i<g_TopListAttrTbl.m_Count;i++)
	{
		AddTopList( g_TopListAttrTbl.m_TableExt[i]);
	}
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID TopListManager::CleanUp( )
{
__ENTER_FUNCTION

	for( INT i=0; i<TOP_LIST_NUM; i++ )
	{
		if ( m_pTopList[i] )
		{
			m_pTopList[i]->CleanUp();
		}

		m_pTopList[i] = 0;
	}

__LEAVE_FUNCTION
}

//添加一个排行榜
BOOL TopListManager::AddTopList( TOP_LIST_ATTR attr )
{
__ENTER_FUNCTION

	TopListID_t id = attr.m_TopListID - 1;

	if( id < 0 || id >= TOP_LIST_NUM )
	{
		//排行榜超过范围
		return FALSE;
	}

	switch(id)
	{
	case LEVEL_TOP_LIST:
		m_pTopList[id] = &m_LevelTopList;
		break;
	case MONEY_TOP_LIST:
		m_pTopList[id] = &m_MoenyTopList;
		break;
	default:
		return TRUE;;
		break;
	}

	m_pTopList[id]->SetTopListID( id );
	m_pTopList[id]->SetTopListName( attr.m_Name );
	m_pTopList[id]->SetMaxListSize( attr.m_MemberCount );
	m_pTopList[id]->SetEnterMinKey(attr.m_EnterMinKey);
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


//删除一个排行榜
BOOL TopListManager::DelTopList( TopListID_t TopListID )
{
__ENTER_FUNCTION
	
	if( TopListID < 0 || TopListID >= TOP_LIST_NUM )
	{
		//排行榜已达上限
		return FALSE;
	}

	m_pTopList[TopListID]->CleanUp();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// 查找一个排行榜
TopList* TopListManager::GetTopList( TopListID_t TopListID )
{
__ENTER_FUNCTION
	
	if( TopListID >= 0 && TopListID < TOP_LIST_NUM )
	{
		return m_pTopList[TopListID];
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}





