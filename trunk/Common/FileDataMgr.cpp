// FileDataMgr.cpp

#include "stdafx.h"
#include "FileDataMgr.h"

#include "WXSJ_DBC.h"

using namespace DBC;

SFileData::SFileData( VOID )
{
	m_uID			= INVALID_ID;
	memset( m_szFileName, 0, sizeof ( m_szFileName ) );
}

SFileData::~SFileData( VOID )
{
}

BOOL SFileData::Init( UINT uID, CHAR *pszFileName, FILE *fp )
{
	Assert( pszFileName != NULL );
	Assert( strlen(pszFileName) < FILE_DATA_NAME_LEN );
	m_uID			= uID;
	strcpy( m_szFileName, pszFileName );
	return TRUE;
}

VOID SFileData::Term( VOID )
{
	m_uID			= INVALID_ID;
	memset( m_szFileName, 0, sizeof ( m_szFileName ) );
}


CFileDataMgr::CFileDataMgr( VOID )
{
	m_bInit			= FALSE;
	m_uCount		= 0;
	m_apFileData	= NULL;
}

CFileDataMgr::~CFileDataMgr( VOID )
{
	Term() ;
}

VOID CFileDataMgr::Term( VOID )
{
	m_bInit  = FALSE;
	m_uCount = 0;
	SAFE_DELETE_ARRAY( m_apFileData );
}

INT CFileDataMgr::CompareOfFileDataID(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	INT	Serial1,Serial2;

	Serial1	= ((SFileData*)pArg1)->GetID();
	Serial2	= ((SFileData*)pArg2)->GetID();

	if(Serial1>Serial2)
		return 1;
	else if(Serial1<Serial2)
		return -1;
	else
		return 0;
	__LEAVE_FUNCTION	
	return 0 ;
}

SFileData* CFileDataMgr::GetFileData( UINT uID )
{
	__ENTER_FUNCTION

	SFileData tb;
	tb.Init( uID, "for bsearch", NULL );

	SFileData* pResult = (SFileData*)bsearch(&tb,
		m_apFileData,
		m_uCount,
		sizeof(SFileData),
		(INT (*)(const VOID*, const VOID*))CompareOfFileDataID);

	return pResult;

	__LEAVE_FUNCTION	
	return NULL;
}

const SFileData *CFileDataMgr::GetConstFileData( UINT uID )const
{
	return const_cast<CFileDataMgr*>(this)->GetFileData( uID );
}

BOOL CFileDataMgr::Init( const CHAR *pszPathFileName, BOOL bForceArrayMode )
{
	__ENTER_FUNCTION

	enum
	{
		FileData_Index = 0,
		FileData_Name
	};
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( pszPathFileName );
	if( ret != TRUE )
	{
		Assert( FALSE && "FileDataMgr::Init error, when open file" );
		return FALSE;
	}

	INT iTableCount = ThirdFile.GetRecordsNum();

	if( !ReadTabSizeCheck(iTableCount) )
	{
		return FALSE;
	}
	m_uCount = iTableCount;

	m_apFileData = new SFileData[m_uCount];

	for(INT i = 0; i<iTableCount; i++)
	{
		INT			iID	  = ThirdFile.Search_Posistion(i,FileData_Index)->iValue;
		const CHAR*	pName = ThirdFile.Search_Posistion(i,FileData_Name)->pString;
		m_apFileData[i].Init( iID, const_cast<CHAR*>(pName), NULL );
	}
	
	SortFileDataArray();

	m_bInit = TRUE;

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL CFileDataMgr::ReadTabSizeCheck( INT iTableCount )
{
	__ENTER_FUNCTION

	if( iTableCount <= 0 )
	{
		Assert( FALSE && "FileDataMgr::Init error, TableCount <= 0" );
		return FALSE;
	}

	//虽然是动态分配内存，也应该有一定的限制，如果大于此值很可能是表格解析出问题了
	if( iTableCount > AUTO_ARRAY_MODE_MAX_LEN )
	{
		Assert( FALSE && "FileDataMgr::Init error, iTableCount > AUTO_ARRAY_MODE_MAX_LEN" );
		return FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID CFileDataMgr::SortFileDataArray()
{
	__ENTER_FUNCTION

	qsort( m_apFileData,
		m_uCount,
		sizeof(SFileData),
		(INT (*)(const VOID*, const VOID*))CompareOfFileDataID);

	__LEAVE_FUNCTION

}