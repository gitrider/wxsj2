#pragma once

#include "stdfilet.h"
#include "fi_messagestruct.h"

inline
BOOL FI_LoadFileInBuffer( FI_AutoBuffer &autoBuffer, const char *sFilename )
{
	if( sFilename == NULL )
		return FALSE;
	if( strlen(sFilename) < 1 )
		return FALSE;
		
	CMyFile tmp;
	if( tmp.Open( sFilename, "rb") )
	{
		int nLength = tmp.GetLength();
		if( nLength > 0 )
		{
			autoBuffer.malloc( nLength );
			tmp.SeekToBegin();
			tmp.Read( (char *)autoBuffer.pBuffer, nLength );
			tmp.Close();
			return TRUE;
		}
		tmp.Close();
	}
	return FALSE;
}

inline
BOOL	FI_SaveBufferInFile( FI_AutoBuffer &autoBuffer, const char *sFilename )
{
	int nLength = autoBuffer.length();
	if( nLength > 0 )
	{
		CMyFile tmp;
		if( tmp.Open( sFilename, "w+b") )
		{
			tmp.SeekToBegin();
			tmp.Write( (char *)autoBuffer.pBuffer, nLength );
			tmp.Close();
			return TRUE;
		}
	}
	return FALSE;
}

inline
BOOL FI_LoadTextFileInString( std::string &sString, const char *sFilename )
{
	CMyFile tmp;
	if( tmp.Open( sFilename, "rb") )
	{
		int nLength = tmp.GetLength();
		if( nLength > 0 )
		{
			sString.resize( nLength );
			tmp.SeekToBegin();
			tmp.Read( &(*sString.begin()), nLength );
			tmp.Close();
			return TRUE;
		}
		tmp.Close();
	}
	return FALSE;
}

