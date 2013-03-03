// FileDataMgr.h
//
///////////////////////////////////////////////////////

#pragma once

#include "Type.h"
#include <stdio.h>

#define FILE_DATA_NAME_LEN		(128)
#define AUTO_ARRAY_MODE_MAX_LEN	(2048)

struct SFileData
{
public:
	SFileData( VOID );
	virtual ~SFileData( VOID );

	virtual BOOL Init( UINT uID, CHAR *pszFileName, FILE *fp );
	virtual VOID Term( VOID );

public:
	INT GetID( VOID )const{
		return m_uID;
	}

	const CHAR *GetFileName( VOID )const{
		return m_szFileName;
	}

protected:
	INT			m_uID;
	CHAR		m_szFileName[FILE_DATA_NAME_LEN];
};


class CFileDataMgr
{
public:
	CFileDataMgr( VOID );
	virtual ~CFileDataMgr( VOID );

	BOOL Init( const CHAR *pszPathFileName, BOOL bForceArrayMode );

	VOID Term( VOID );

	SFileData *GetFileData( UINT uID );
	const SFileData *GetConstFileData( UINT uID )const;

	BOOL IsInit( ){ return m_bInit ; }

protected:
	static  INT  CompareOfFileDataID(const VOID* pArg1,const VOID* pArg2);
	BOOL ReadTabSizeCheck( INT iTableCount );
	VOID SortFileDataArray();
private:
	BOOL		m_bInit ;
	INT			m_uCount;
	SFileData*	m_apFileData;
};
