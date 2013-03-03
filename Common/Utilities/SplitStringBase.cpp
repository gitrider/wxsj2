/********************************************************************
	created:	2007/12/25
	created:	25:12:2007   16:08
	filename: 	MapServer\Server\Utilities\SplitStringBase.cpp
	file path:	MapServer\Server\Utilities
	file base:	SplitStringBase
	file ext:	cpp
	author:		Richard
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include "SplitStringBase.h"

SplitStringBase::SplitStringBase( void )
{
	Reset();
}

SplitStringBase::~SplitStringBase(void)
{
}

const SplitStringBase* SplitStringBase::GetChild()
{
	return m_pChild;
}

VOID SplitStringBase::Reset()
{
	__ENTER_FUNCTION

	m_pChild				= NULL;
	m_iLevelOneLineCount	= 0;

	memset( (VOID*)m_SplitChar, 0, sizeof(m_SplitChar) );

	memset( (VOID*)m_strSplitLevelOne,  0, sizeof(m_strSplitLevelOne) );

	__LEAVE_FUNCTION
}

VOID SplitStringBase::Init( CHAR cSplit, SplitStringBase* pChild )
{
	Reset();
	m_SplitChar[0]	= cSplit;
	m_pChild		= pChild;
}

INT SplitStringBase::GetResultLineCount()
{
	return m_iLevelOneLineCount;
}

const CHAR* SplitStringBase::GetResultLine( INT iLine )
{
	__ENTER_FUNCTION

	if( iLine >= m_iLevelOneLineCount )
	{
		return NULL;
	}
	return m_strSplitLevelOne[iLine];

	__LEAVE_FUNCTION
		return NULL;
}

BOOL SplitStringBase::DoSubSplit()
{
	__ENTER_FUNCTION

		if( m_pChild != NULL )
		{
			for( INT i=0; i<m_iLevelOneLineCount; ++i )
			{
				m_pChild->DoSplit( m_strSplitLevelOne[i] );
			}
		}
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
}