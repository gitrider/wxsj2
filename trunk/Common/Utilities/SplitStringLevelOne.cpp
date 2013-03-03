/********************************************************************
	created:	2007/12/25
	created:	25:12:2007   16:24
	filename: 	MapServer\Server\Utilities\SplitStringLevelOne.cpp
	file path:	MapServer\Server\Utilities
	file base:	SplitStringLevelOne
	file ext:	cpp
	author:		Richard
	
	purpose:	�ο�ͷ�ļ��е�˵��
*********************************************************************/

#include "stdafx.h"
#include "SplitStringLevelOne.h"

BOOL SplitStringLevelOne::DoSplit( const CHAR* strSplit )
{
	__ENTER_FUNCTION

	Assert( strSplit!=NULL && m_SplitChar!=0 && "SplitStringLevelOne::DoSplit" );
	Assert( strlen(strSplit) <= MAX_INPUT_STRING_LENGTH && "SplitStringLevelOne::DoSplit"  ); 

	//��ȡ�ָ������ַ����е�һ�γ��ֵ�λ��
	size_t iPos = strcspn( strSplit, m_SplitChar );
	
	//���û���ҵ��򷵻�
	if( iPos <= 0 )
		return FALSE;

	INT iIndex = m_iLevelOneLineCount;
	while ( iPos )
	{
		Assert( iIndex < MAX_LEVEL_ONE_LENGTH && "SplitStringLevelOne::DoSplit" );
		Assert( iPos < MAX_SPLIT_STRING_LENGTH && "SplitStringLevelOne::DoSplit" );
		
		if( iIndex>=MAX_LEVEL_ONE_LENGTH || iPos>=MAX_SPLIT_STRING_LENGTH )
			return FALSE;

		strncpy( m_strSplitLevelOne[iIndex], strSplit, iPos );
		++iIndex;
		
		if( *(strSplit+iPos) == 0 )
			break;

		strSplit += iPos + 1;

		iPos = strcspn( strSplit, m_SplitChar );
	}
	m_iLevelOneLineCount = iIndex;
	DoSubSplit();

	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}