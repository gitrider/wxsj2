
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

//���һ�����а�
BOOL TopListManager::AddTopList( TOP_LIST_ATTR attr )
{
__ENTER_FUNCTION

	TopListID_t id = attr.m_TopListID - 1;

	if( id < 0 || id >= TOP_LIST_NUM )
	{
		//���а񳬹���Χ
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


//ɾ��һ�����а�
BOOL TopListManager::DelTopList( TopListID_t TopListID )
{
__ENTER_FUNCTION
	
	if( TopListID < 0 || TopListID >= TOP_LIST_NUM )
	{
		//���а��Ѵ�����
		return FALSE;
	}

	m_pTopList[TopListID]->CleanUp();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ����һ�����а�
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





