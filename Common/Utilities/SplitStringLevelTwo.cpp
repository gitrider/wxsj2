/********************************************************************
	created:	2007/12/25
	created:	25:12:2007   16:24
	filename: 	MapServer\Server\Utilities\SplitStringLevelTwo.cpp
	file path:	MapServer\Server\Utilities
	file base:	SplitStringLevelTwo
	file ext:	cpp
	author:		Richard
	
	purpose:	参考头文件中的说明
*********************************************************************/

#include "stdafx.h"
#include <stdlib.h>
#include "SplitStringLevelTwo.h"

BOOL SplitStringLevelTwo::DoSplit( const CHAR* strSplit )
{
	__ENTER_FUNCTION

	Assert( strSplit!=NULL && m_SplitChar!=0 && "SplitStringLevelTwo::DoSplit 1" );
	Assert( strlen(strSplit) < MAX_SPLIT_STRING_LENGTH && "SplitStringLevelTwo::DoSplit 2" );
	if( strlen(strSplit) >= MAX_SPLIT_STRING_LENGTH )
		return FALSE;
	//获取分隔符在字符串中第一次出现的位置
	INT iPos = (INT)strcspn( strSplit, m_SplitChar );

	if( iPos <= 0 )
		return FALSE;
	//如果不可拆分就缓存在一级,并返回
	if( strSplit[iPos] != m_SplitChar[0] )
	{
		Assert( strlen(strSplit) < MAX_FINAL_STRING_LENGTH && "SplitStringLevelTwo::DoSplit 3" );
		if( strlen(strSplit) >= MAX_FINAL_STRING_LENGTH )
			return FALSE;

		strcpy( m_strSplitLevelOne[m_iLevelOneLineCount++], strSplit );
		return FALSE;
	}
	
	DoSubLevelSplit( iPos, strSplit );

	DoSubSplit();
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID SplitStringLevelTwo::DoSubLevelSplit( INT iPos, const CHAR* strSplit )
{
	__ENTER_FUNCTION
	memset( m_strSplitLevelTwo, 0, sizeof(m_strSplitLevelTwo) );
	//拆分并缓存
	INT iIndex = 0;
	while ( iPos )
	{
		strncpy( m_strSplitLevelTwo[iIndex], strSplit, iPos );

		strSplit += iPos + 1;
		++iIndex;

		iPos = (INT)strcspn( strSplit, m_SplitChar );
	}
	Assert( iIndex%2 == 0 && "SplitStringLevelTwo::DoSubLevelSplit" );

	if( iIndex%2 != 0 )
		return;

	INT iStart;
	INT iEnd;
	CHAR strTmp[32];
	for( INT i=0; i<iIndex; i+=2 )
	{
		iStart	= atoi( m_strSplitLevelTwo[i] );
		iEnd	= atoi( m_strSplitLevelTwo[i+1] );
		if( iEnd-iStart >= MAX_LEVEL_ONE_LENGTH )
		{
			iEnd = iEnd - (iEnd - iStart - MAX_LEVEL_ONE_LENGTH + 1);
			//Assert( FALSE && "SplitStringLevelTwo > MAX_LEVEL_ONE_LENGTH" );
		}
		for( INT j=iStart; j<=iEnd; ++j )
		{
			sprintf(strTmp, "%d", j);
			//itoa( j, strTmp, 10 );
			if( m_iLevelOneLineCount >= MAX_LEVEL_ONE_LENGTH )
			{
				break;
			}
			strcpy( m_strSplitLevelOne[m_iLevelOneLineCount++], strTmp );
		}
	}
	__LEAVE_FUNCTION
}
