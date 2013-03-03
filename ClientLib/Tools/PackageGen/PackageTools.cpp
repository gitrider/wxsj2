#ifndef __PACKAGETOOLS_H__
#include "PackageTools.h"
#endif

#ifndef _INC_IO
#include <io.h>
#endif

#ifndef _INC_SHLWAPI
#include "shlwapi.h"
#endif

#include <algorithm>
#include <mbstring.h>

CPackageTools::CPackageTools()
{
	m_dwFileSize = 0;
	m_dwHeadSize = 0;
	m_dwSameFiles = 0;
	m_lpFileBaseAddress = 0;
	m_lpHeadBaseAddress = 0;
	m_lpHeadAddress = 0;
	m_lpFileAddress = 0;
	m_bSpecialLua = 0;
	m_bSpecialTxt = 1;
	m_lpLua = 0;
	m_lpTxt = 0;
	m_LogFile = fopen( "PackageGen.log", "w" );
}

CPackageTools::~CPackageTools()
{
	if( m_lpLua ) { delete [] m_lpLua; m_lpLua = 0; }
	if( m_lpTxt ) { delete [] m_lpTxt; m_lpTxt = 0; }
	if( m_LogFile ) { fclose( m_LogFile ); }
}

bool CPackageTools::ResourcePackageGen( vector<DUMPPATH_DESC>& vecResPaths, 
	const string& strPackFileName, const vector<string>& vecGroupBasePathNames )
{
	if( vecResPaths.size() == 0 || strPackFileName.empty() ) return( false );

	m_dwFileSize = 0;
	m_dwHeadSize = 0;
	m_lpFileBaseAddress = 0;
	m_lpHeadBaseAddress = 0;
	m_lpHeadAddress = 0;
	m_lpFileAddress = 0;
	m_dwFarOffset = 0;
	m_hFileMapping = 0;
	m_bSpecialLua = 0;
	m_bSpecialTxt = 1;
	m_dwLuaLen = 0;
	if( m_lpLua ) { delete [] m_lpLua; m_lpLua = 0; }
	if( m_lpTxt ) { delete [] m_lpTxt; m_lpTxt = 0; }
	m_strPakFileName = strPackFileName;

	char szPathFile[1024];
	::GetCurrentDirectory(1024, szPathFile);
	m_strBasePath = szPathFile;

	m_dwSameFiles = 0;
	printf( "%s: Parseing Path...\n", strPackFileName.c_str() );
	if( m_LogFile ) fprintf( m_LogFile, "%s: Parseing Path...\n", strPackFileName.c_str() );
	unsigned long filesnum = 0;
	m_mapFileNames.clear();
	for( size_t i = 0; i < vecResPaths.size(); i ++ )
	{
		DUMPPATH_DESC* lpDesc = &vecResPaths[i];
		m_strRelativePath = lpDesc->szPath;
		m_strRecursiveBasePath.resize( 0 );
		m_strAppendBasePath.resize( 0 );
		if( lpDesc->bRecursive )
		{
			for( size_t j = 0; j < vecGroupBasePathNames.size(); j ++ )
			{
				string strGroupName = vecGroupBasePathNames[j];
				if( m_strRelativePath.find( vecGroupBasePathNames[j] ) != -1 )
				{
					if( m_strRelativePath.find( vecGroupBasePathNames[j] ) != m_strRelativePath.length() - vecGroupBasePathNames[j].length() )
					{
						m_strRecursiveBasePath = m_strRelativePath.substr( m_strRelativePath.find( vecGroupBasePathNames[j] ) + vecGroupBasePathNames[j].length() + 1 );
					}
					else if( strGroupName == "../Interface" )
					{
						m_strRecursiveBasePath = "Interface";
					}
					else if( strGroupName == "../Sound" )
					{
						m_strRecursiveBasePath = "Sound";
					}
					else if( strGroupName == "../Brushes" )
					{
						m_strRecursiveBasePath = "Brushes";
					}
					else if( strGroupName == "../config" )
					{
						m_strRecursiveBasePath = "config";
						m_strAppendBasePath = "config/";
					}
					else if( strGroupName == "../../public/config" )
					{
						m_strRecursiveBasePath = "config";
						m_strAppendBasePath = "data/";
					}
					else if( strGroupName == "../../public/data" )
					{
						m_strRecursiveBasePath = "data/script";
						m_strAppendBasePath = "data/";
						m_bSpecialLua = true;
					}
					break;
				}
			}
		}
		FindDump( false, lpDesc->bRecursive, lpDesc->vecExt, vecGroupBasePathNames, filesnum );
	}

	if( m_dwSameFiles != 0 ) {
		Sleep( 3000 );
		return( false );
	}

	HANDLE hName = CreateFileA( ( strPackFileName + ".nam" ).c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hName == INVALID_HANDLE_VALUE ) return( false );
	HANDLE hNameMapping = CreateFileMapping( hName, NULL, PAGE_READWRITE, 0, m_dwHeadSize + 4, NULL );
	if( hNameMapping == NULL ) return( false );
	CloseHandle( hName );
	m_lpHeadBaseAddress = (LPBYTE)MapViewOfFile( hNameMapping, FILE_MAP_ALL_ACCESS, 0, 0, m_dwHeadSize + 4 );
	if( m_lpHeadBaseAddress == NULL ) return( false );
	CopyMemory( m_lpHeadBaseAddress, &m_dwHeadSize, 4 );
	m_lpHeadAddress = m_lpHeadBaseAddress + 4;

	string strFileName;
	m_dwLeaveSize = m_dwFileSize;
	if( m_dwFileSize < MAX_PACKAGE_SIZE ) {
		strFileName = strPackFileName + ".p01";
		m_dwThisFile = 0;
		m_dwThisFileSize = m_dwFileSize + 4;
	}
	else {
		strFileName = strPackFileName + ".p01";
		m_dwThisFile = 1;
		m_dwThisFileSize = MAX_PACKAGE_SIZE;
	}

	HANDLE hFile = CreateFileA( strFileName.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) return( false );
	m_hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0, m_dwThisFileSize, NULL );
	if( m_hFileMapping == NULL ) return( false );
	CloseHandle( hFile );
	m_lpFileBaseAddress = (LPBYTE)MapViewOfFile( m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, m_dwThisFileSize );
	if( m_lpFileBaseAddress == NULL ) return( false );
	m_lpFileAddress = m_lpFileBaseAddress + 4;

	filesnum = 0;
	for( size_t i = 0; i < vecResPaths.size(); i ++ )
	{
		DUMPPATH_DESC* lpDesc = &vecResPaths[i];
		m_strRelativePath = lpDesc->szPath;
		m_strRecursiveBasePath.resize( 0 );
		m_strAppendBasePath.resize( 0 );
		if( lpDesc->bRecursive )
		{
			for( size_t j = 0; j < vecGroupBasePathNames.size(); j ++ )
			{
				string strGroupName = vecGroupBasePathNames[j];
				if( m_strRelativePath.find( vecGroupBasePathNames[j] ) != -1 )
				{
					if( m_strRelativePath.find( vecGroupBasePathNames[j] ) != m_strRelativePath.length() - vecGroupBasePathNames[j].length() )
					{
						m_strRecursiveBasePath = m_strRelativePath.substr( m_strRelativePath.find( vecGroupBasePathNames[j] ) + vecGroupBasePathNames[j].length() + 1 );
					}
					else if( strGroupName == "../Interface" )
					{
						m_strRecursiveBasePath = "Interface";
					}
					else if( strGroupName == "../Sound" )
					{
						m_strRecursiveBasePath = "Sound";
					}
					else if( strGroupName == "../Brushes" )
					{
						m_strRecursiveBasePath = "Brushes";
					}
					else if( strGroupName == "../config" )
					{
						m_strRecursiveBasePath = "config";
						m_strAppendBasePath = "config/";
					}
					else if( strGroupName == "../../public/config" )
					{
						m_strRecursiveBasePath = "config";
						m_strAppendBasePath = "data/";
					}
					else if( strGroupName == "../../public/data" )
					{
						m_strRecursiveBasePath = "data/script";
						m_strAppendBasePath = "data/";
						m_bSpecialLua = true;
					}
					break;
				}
			}
		}
		FindDump( true, lpDesc->bRecursive, lpDesc->vecExt, vecGroupBasePathNames, filesnum );
	}
	CopyMemory( m_lpFileBaseAddress, &m_dwLeaveSize, 4 );
	UnmapViewOfFile( m_lpFileBaseAddress );
	CloseHandle( m_hFileMapping );

	return( true );
}

void CPackageTools::FindDump( bool bDump, bool bRecursive, vector<string>& exts, const vector<string>& basepaths, unsigned long& filesnum )
{
	string strFindTemp = m_strBasePath + "/" + m_strRelativePath + "/*.*";
	struct _finddata_t c_file;
	intptr_t ptrFile;
	if( (ptrFile = _findfirst( strFindTemp.c_str(), &c_file )) != -1L )
	{
		char* p = &c_file.name[0];
		_mbslwr( (byte*)p );
		int nHandle = CheckFileName( c_file.name, exts );
		if( nHandle == 0 )
		{
			filesnum ++;
			string strTemp( c_file.name );
			if( bDump )
			{
				if( m_strRecursiveBasePath.empty() == false )
				{
					if( ( strTemp.find( ".skeleton" ) != -1 ) && ( strTemp.find( ".skeleton" ) == ( strTemp.length() - 9 ) ) )
					{
					}
					else if( ( m_strRelativePath.find( m_strRecursiveBasePath ) != -1 ) && ( m_strRelativePath.find( m_strRecursiveBasePath ) != m_strRelativePath.length() - m_strRecursiveBasePath.length() ) )
					{
						strTemp = m_strRelativePath.substr( m_strRelativePath.find( m_strRecursiveBasePath ) + m_strRecursiveBasePath.length() + 1 ) + "/" + c_file.name;
					}
					//if( m_strAppendBasePath.empty() == false )
					//{
					//	strTemp = m_strAppendBasePath + strTemp;
					//}
				}
				if( FileDump( m_strBasePath + "/" + m_strRelativePath + "/" + c_file.name, strTemp, c_file.size ) )
				{
					printf( "Dump:%s, size=%d Success.\n", strTemp.c_str(), c_file.size );
					if( m_LogFile ) fprintf( m_LogFile, "Dump:%s, size=%d Success.\n", strTemp.c_str(), c_file.size );
				}
				else
				{
					printf( "Dump:%s, Failed!!.\n", strTemp.c_str() );
					if( m_LogFile ) fprintf( m_LogFile, "Dump:%s, Failed!!.\n", strTemp.c_str() );
					Beep( 100, 10 );
				}
			}
			else
			{
				m_dwFileSize = m_dwFileSize + c_file.size;
				if( m_strRecursiveBasePath.empty() == false )
				{
					if( ( strTemp.find( ".skeleton" ) != -1 ) && ( strTemp.find( ".skeleton" ) == ( strTemp.length() - 9 ) ) )
					{
					}
					else if( ( m_strRelativePath.find( m_strRecursiveBasePath ) != -1 ) && ( m_strRelativePath.find( m_strRecursiveBasePath ) != m_strRelativePath.length() - m_strRecursiveBasePath.length() )  )
					{
						strTemp = m_strRelativePath.substr( m_strRelativePath.find( m_strRecursiveBasePath ) + m_strRecursiveBasePath.length() + 1 ) + "/" + c_file.name;
					}
					//if( m_strAppendBasePath.empty() == false )
					//{
					//	strTemp = m_strAppendBasePath + strTemp;
					//}
				}
				m_dwHeadSize = m_dwHeadSize + strTemp.length() + 13;
				map<string, string>::iterator it;
				it =  m_mapFileNames.find( strTemp );
				if( it == m_mapFileNames.end() )
				{
					m_mapFileNames.insert( m_FileNamePair( strTemp, strTemp ) );
				}
				else
				{
					m_dwSameFiles ++;
					printf( "File:%s, not only one!!.\n", strTemp.c_str() );
					if( m_LogFile ) fprintf( m_LogFile, "File:%s, not only one!!.\n", strTemp.c_str() );
					system("pause");
				}
			}
		}
		else if( nHandle == 1 )
		{
			if( bRecursive )
			{
				m_strRelativePath = m_strRelativePath + "/" + c_file.name;
				FindDump( bDump, true, exts, basepaths, filesnum );
				m_strRelativePath = m_strRelativePath.substr( 0, m_strRelativePath.rfind( "/" ) );
			}
		}
		while( _findnext( ptrFile, &c_file ) == 0 )
		{
			if( m_lpLua ) { delete [] m_lpLua; m_lpLua = 0; }
			if( m_lpTxt ) { delete [] m_lpTxt; m_lpTxt = 0; }
			char* p = &c_file.name[0];
			_mbslwr( (byte*)p );
			nHandle = CheckFileName( c_file.name, exts );
			if( nHandle == 0 )
			{
				if( c_file.size > 0 )
				{
					string strTemp( c_file.name );
					filesnum ++;
					if( bDump )
					{
						if( m_strRecursiveBasePath.empty() == false )
						{
							if( ( strTemp.find( ".skeleton" ) != -1 ) && ( strTemp.find( ".skeleton" ) == ( strTemp.length() - 9 ) ) )
							{
							}
							else if( ( m_strRelativePath.find( m_strRecursiveBasePath ) != -1 ) && ( m_strRelativePath.find( m_strRecursiveBasePath ) != m_strRelativePath.length() - m_strRecursiveBasePath.length() )  )
							{
								strTemp = m_strRelativePath.substr( m_strRelativePath.find( m_strRecursiveBasePath ) + m_strRecursiveBasePath.length() + 1 ) + "/" + c_file.name;
							}
							//if( m_strAppendBasePath.empty() == false )
							//{
							//	strTemp = m_strAppendBasePath + strTemp;
							//}
						}
						if( FileDump( m_strBasePath + "/" + m_strRelativePath + "/" + c_file.name, strTemp, c_file.size ) )
						{
							printf( "Dump:%s, size=%d Success.\n", strTemp.c_str(), c_file.size );
							if( m_LogFile ) fprintf( m_LogFile, "Dump:%s, size=%d Success.\n", strTemp.c_str(), c_file.size );
						}
						else
						{
							printf( "Dump:%s, Failed!!.\n", strTemp.c_str() );
							if( m_LogFile ) fprintf( m_LogFile, "Dump:%s, Failed!!.\n", strTemp.c_str() );
							Beep( 100, 10 );
						}
					}
					else
					{
						if( m_bSpecialLua && m_lpLua && m_dwLuaLen > 0 )
						{
							m_dwFileSize = m_dwFileSize + m_dwLuaLen;
						}
						else if( m_bSpecialTxt && m_lpTxt && m_dwTxtLen > 0 )
						{
							m_dwFileSize = m_dwFileSize + m_dwTxtLen;
						}
						else
						{
							m_dwFileSize = m_dwFileSize + c_file.size;
						}
						if( m_strRecursiveBasePath.empty() == false )
						{
							if( ( strTemp.find( ".skeleton" ) != -1 ) && ( strTemp.find( ".skeleton" ) == ( strTemp.length() - 9 ) ) )
							{
							}
							else if( ( m_strRelativePath.find( m_strRecursiveBasePath ) != -1 ) && ( m_strRelativePath.find( m_strRecursiveBasePath ) != m_strRelativePath.length() - m_strRecursiveBasePath.length() )  )
							{
								strTemp = m_strRelativePath.substr( m_strRelativePath.find( m_strRecursiveBasePath ) + m_strRecursiveBasePath.length() + 1 ) + "/" + c_file.name;
							}
							//if( m_strAppendBasePath.empty() == false )
							//{
							//	strTemp = m_strAppendBasePath + strTemp;
							//}
						}
						m_dwHeadSize = m_dwHeadSize + strTemp.length() + 13;
						map<string, string>::iterator it;
						it =  m_mapFileNames.find( strTemp );
						if( it == m_mapFileNames.end() )
						{
							m_mapFileNames.insert( m_FileNamePair( strTemp, strTemp ) );
						}
						else
						{
							m_dwSameFiles ++;
							printf( "File:%s, not only one!!.\n", strTemp.c_str() );
							if( m_LogFile ) fprintf( m_LogFile, "File:%s, not only one!!.\n", strTemp.c_str() );
							system("pause");
						}
					}
				}
			}
			else if( nHandle == 1 )
			{
				if( bRecursive )
				{
					m_strRelativePath = m_strRelativePath + "/" + c_file.name;
					FindDump( bDump, true, exts, basepaths, filesnum );
					m_strRelativePath = m_strRelativePath.substr( 0, m_strRelativePath.rfind( "/" ) );
				}
			}
		}
		_findclose( ptrFile );
	}
}

int CPackageTools::CheckFileName( string strFileName, vector<string>& exts )
{
	if( stricmp( strFileName.c_str(), "." ) == 0 ) return( -1 );
	if( stricmp( strFileName.c_str(), ".." ) == 0 ) return( -1 );
	if( stricmp( strFileName.c_str(), "cvs" ) == 0 ) return( -1 );
	if( stricmp( strFileName.c_str(), "vssver.scc" ) == 0 ) return( -1 );
	if( ( strFileName.find( ".zip" ) != -1 ) && ( strFileName.find( ".zip" ) == ( strFileName.length() - 4 ) ) ) return( -1 );
	string strTemp = m_strBasePath + "/" + m_strRelativePath + "/" + strFileName;
	if( m_bSpecialLua && ( strFileName.find( ".lua" ) != -1 ) && ( strFileName.find( ".lua" ) == ( strFileName.length() - 4 ) ) )
	{
		FILE* fp;
		fp=fopen( strTemp.c_str(), "rb" );
		if(!fp) return -1;
		fseek(fp, 0, SEEK_END);
		m_dwLuaLen = ftell( fp );
		if( m_dwLuaLen == 0 ) { fclose(fp); return -1; }
		if( m_lpLua ) { delete [] m_lpLua; m_lpLua = 0; }
		m_lpLua = new char[m_dwLuaLen+1];
		fseek(fp, 0, SEEK_SET);
		fread( m_lpLua, 1, m_dwLuaLen, fp );
		fclose( fp );
		m_lpLua[m_dwLuaLen] = 0;
		if( strstr( m_lpLua, "--MisDescBegin" ) == NULL )
		{
			delete [] m_lpLua; m_lpLua = 0;
			return( -1 );
		}
		if( strstr( m_lpLua, "--MisDescEnd" ) == NULL )
		{
			delete [] m_lpLua; m_lpLua = 0;
			return( -1 );
		}
		m_dwLuaLen = strstr( m_lpLua, "--MisDescEnd" ) - strstr( m_lpLua, "--MisDescBegin" ) + 12;
		return 0;
	}
	if( m_bSpecialTxt && ( strFileName.find( ".txt" ) != -1 ) && ( strFileName.find( ".txt" ) == ( strFileName.length() - 4 ) ) )
	{
		FILE* fp;
		fp=fopen( strTemp.c_str(), "rb" );
		if(!fp) return -1;
		fseek(fp, 0, SEEK_END);
		m_dwTxtLen = ftell( fp );
		if( m_dwTxtLen == 0 ) { fclose(fp); return -1; }
		if( m_lpTxt ) { delete [] m_lpTxt; m_lpTxt = 0; }
		char* fl = new char[m_dwTxtLen+1];
		fseek(fp, 0, SEEK_SET);
		fread( fl, 1, m_dwTxtLen, fp );
		fclose( fp );
		fl[m_dwTxtLen] = 0;

		m_dwTxtLen = OpenDBC( fl, fl+m_dwTxtLen );
		delete [] fl;
		return m_dwTxtLen == 0 ? -1 : 0;
	}
	if( PathIsDirectory( strTemp.c_str() ) ) return( 1 );
	for( vector<string>::const_iterator it = exts.begin(); it != exts.end(); ++ it )
	{
		if( (*it) == "*" ) return( 0 );
		int n1 = strFileName.find( *it );
		int n2 = strFileName.length();
		int n3 = (*it).length();
		if( n1 != -1 && ( n1 == ( n2 - n3 ) ) ) return( 0 );
		//if( strFileName.find( *it ) == ( strFileName.length() - (*it).length() ) ) return( 0 );
	}
	return( -1 );
}

void CPackageTools::HeadDump( string& strFileName, unsigned long dwOffset, unsigned long dwSize, unsigned long dwCRC )
{
	byte NameLen = (byte)strFileName.length();
	CopyMemory( m_lpHeadAddress, &NameLen, 1 ); m_lpHeadAddress += 1;
	CopyMemory( m_lpHeadAddress, strFileName.c_str(), NameLen ); m_lpHeadAddress += NameLen;
	CopyMemory( m_lpHeadAddress, &dwOffset, 4 ); m_lpHeadAddress += 4;
	CopyMemory( m_lpHeadAddress, &dwSize, 4 ); m_lpHeadAddress += 4;
	CopyMemory( m_lpHeadAddress, &dwCRC, 4 ); m_lpHeadAddress += 4;
}

bool CPackageTools::FileDump( string& strFileName, string strRelativeName, unsigned long dwFileSize )
{
	if( m_bSpecialLua && m_lpLua && m_dwLuaLen > 0 )
	{
		DWORD dwCrc32, dwResult;
		dwResult = Memory32Win32( strstr( m_lpLua, "--MisDescBegin" ), m_dwLuaLen, dwCrc32 );
		CopyMemory( m_lpFileAddress, strstr( m_lpLua, "--MisDescBegin" ), m_dwLuaLen );

		HeadDump( strRelativeName, (unsigned long)( m_lpFileAddress - m_lpFileBaseAddress - 4 + m_dwFarOffset ), m_dwLuaLen, dwCrc32 );
		m_lpFileAddress += m_dwLuaLen;

		delete [] m_lpLua;
		m_lpLua = 0;

		return true;
	}
	if( m_bSpecialTxt && m_lpTxt && m_dwTxtLen > 0 )
	{
		DWORD dwCrc32, dwResult;
		dwResult = Memory32Win32( m_lpTxt, m_dwTxtLen, dwCrc32 );
		CopyMemory( m_lpFileAddress, m_lpTxt, m_dwTxtLen );
	
		HeadDump( strRelativeName, (unsigned long)( m_lpFileAddress - m_lpFileBaseAddress - 4 + m_dwFarOffset ), m_dwTxtLen, dwCrc32 );
		m_lpFileAddress += m_dwTxtLen;

		delete [] m_lpTxt;
		m_lpTxt = 0;

		return true;
	}

	HANDLE hFile = CreateFileA( strFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) {
		printf( "file:%s open error\n", strFileName.c_str() );
		if( m_LogFile ) fprintf( m_LogFile, "file:%s open error\n", strFileName.c_str() );
		system("pause");
		return false;
	}
	HANDLE hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
	if( hFileMapping == NULL ) {
		printf( "file:%s open error\n", strFileName.c_str() );
		if( m_LogFile ) fprintf( m_LogFile, "file:%s open error\n", strFileName.c_str() );
		system("pause");
		return false;
	}
	CloseHandle( hFile );
	LPBYTE hBaseAddress = (LPBYTE)MapViewOfFile( hFileMapping, FILE_MAP_READ, 0, 0, dwFileSize );
	if( hBaseAddress == NULL ) {
		printf( "file:%s open error\n", strFileName.c_str() );
		if( m_LogFile ) fprintf( m_LogFile, "file:%s open error\n", strFileName.c_str() );
		system("pause");
		return false;
	}

	DWORD dwCrc32, dwResult;
	dwResult = Memory32Win32( hBaseAddress, dwFileSize, dwCrc32 );

	if( ( m_dwThisFileSize - ( m_lpFileAddress - m_lpFileBaseAddress ) ) < dwFileSize ) {
		DWORD dwTemp = m_lpFileAddress - m_lpFileBaseAddress - 4;
		CopyMemory( m_lpFileBaseAddress, &dwTemp, 4 );
		UnmapViewOfFile( m_lpFileBaseAddress );
		CloseHandle( m_hFileMapping );
		m_dwLeaveSize -= dwTemp;
		m_dwFarOffset += dwTemp;

		char szFileName[260] ={0};
		sprintf( szFileName, "%s.p%02d", m_strPakFileName.c_str(), ++m_dwThisFile );
		if( m_dwLeaveSize < MAX_PACKAGE_SIZE ) {
			m_dwThisFileSize = m_dwLeaveSize + 4;
		}
		else {
			m_dwThisFileSize = MAX_PACKAGE_SIZE;
		}
		HANDLE hFile = CreateFileA( szFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hFile == INVALID_HANDLE_VALUE ) return( false );
		m_hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0, m_dwThisFileSize, NULL );
		if( m_hFileMapping == NULL ) return( false );
		CloseHandle( hFile );
		m_lpFileBaseAddress = (LPBYTE)MapViewOfFile( m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, m_dwThisFileSize );
		if( m_lpFileBaseAddress == NULL ) return( false );
		m_lpFileAddress = m_lpFileBaseAddress + 4;
	}

	CopyMemory( m_lpFileAddress, hBaseAddress, dwFileSize );
	HeadDump( strRelativeName, (unsigned long)( m_lpFileAddress - m_lpFileBaseAddress - 4 + m_dwFarOffset ), dwFileSize, dwCrc32 );
	m_lpFileAddress += dwFileSize;

	UnmapViewOfFile( hBaseAddress );
	CloseHandle( hFileMapping );

	return true;
}

long CPackageTools::FindOptiMatch( const string& strPackFileName, unsigned long dwNeed )
{
	FILE* fp;
	fp = fopen( strPackFileName.c_str(), "rb" );
	if( !fp ) return( -1 );

	DWORD dwHeadSize, dwFileSize, dwTotalSize;
	fseek( fp, 0, SEEK_END );
	dwTotalSize = (DWORD)ftell( fp );
	fseek( fp, 0, SEEK_SET );
	fread( &dwHeadSize, 1, 4, fp );
	fread( &dwFileSize, 1, 4, fp );
	fseek( fp, dwFileSize + 8, SEEK_SET );
	LPBYTE szTemp = new unsigned char[dwHeadSize];
	fread( szTemp, 1, dwHeadSize, fp );
	fclose( fp );

	list<FILE_DESC> lfd;
	FILE_DESC desc;
	BYTE Length;
	DWORD dwSize = dwHeadSize;
	LPBYTE lpHead = szTemp;
	while( dwSize > 0 )
	{
		Length = *lpHead++; dwSize --;
		ZeroMemory( &desc, sizeof( FILE_DESC ) );
		CopyMemory( desc.szFileName, lpHead, Length ); lpHead += Length; dwSize-= Length;
		CopyMemory( &desc.dwOffset, lpHead, 4 ); lpHead += 4; dwSize -= 4;
		CopyMemory( &desc.dwSize, lpHead, 4 ); lpHead += 4; dwSize -= 4;
		CopyMemory( &desc.dwCRC, lpHead, 4 ); lpHead += 4; dwSize -= 4;
		list<FILE_DESC>::iterator it;
		for( it = lfd.begin(); it != lfd.end(); ++ it )
		{
			if( desc.dwOffset > (*it).dwOffset ) 
			{
				lfd.insert( it, desc );
				break;
			}
		}
		if( it == lfd.end() ) lfd.push_back( desc );
	}
	delete [] szTemp;

	long nBestOffset = -1;
	if( lfd.size() == 0 ) return( nBestOffset );

	DWORD dwOffset = dwTotalSize;
	list<FILE_DESC>::iterator next = lfd.begin();
	list<FILE_DESC>::iterator first = next ++;
	for( ; next != lfd.end(); ++ next, ++ first )
	{
		DWORD dwTemp = (*first).dwOffset - (*next).dwOffset - (*next).dwSize;
		if( dwTemp > dwNeed )
		{
			dwTemp = dwTemp - dwNeed;
			if( dwOffset > dwTemp )
			{
				dwOffset = dwTemp;
				nBestOffset = (long)( (*next).dwOffset + (*next).dwSize );
			}
		}
		else if( dwTemp == dwNeed )
		{
			nBestOffset = (long)( (*next).dwOffset + (*next).dwSize );
			return( nBestOffset );
		}
	}
	if( nBestOffset > 0 ) return( nBestOffset );

	if( ( (*first).dwOffset - 8 ) >= dwNeed ) nBestOffset = 8;

	return( nBestOffset );
}

#define MAX_BUFFER_SIZE		(4096)

// Static CRC table
DWORD g_arrdwCrc32Table[256] =
{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
};


void CPackageTools::CalcCrc32(const BYTE byte, DWORD &dwCrc32)
{
	dwCrc32 = ((dwCrc32) >> 8) ^ g_arrdwCrc32Table[(byte) ^ ((dwCrc32) & 0x000000FF)];
}

DWORD CPackageTools::Memory32Win32(LPVOID pMemoryBuf, DWORD dwBufSize, DWORD &dwCrc32)
{
	DWORD dwErrorCode = NO_ERROR;
	if(pMemoryBuf == NULL) return ERROR_CRC;

	dwCrc32 = 0xFFFFFFFF;
	
	try
	{
		DWORD dwLoop;
		for(dwLoop = 0; dwLoop < dwBufSize; dwLoop++)
			CalcCrc32(((BYTE*)pMemoryBuf)[dwLoop], dwCrc32);
	}
	catch(...)
	{
		// An unknown exception happened
		dwErrorCode = ERROR_CRC;
	}

	dwCrc32 = ~dwCrc32;
	return dwErrorCode;
}

DWORD CPackageTools::OpenDBC( char* pMemory, char* pDeadEnd )
{
	DATA_BUF				m_vDataBuf;
	CHAR*					m_pStringBuf;

	CHAR szLine[1024*10] = {0};

	register const char* pMem = pMemory;
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return 0;

	vector< std::string > vRet;
	_ConvertStringToVector(szLine, vRet, "\t", TRUE, TRUE);
	if(vRet.empty()) return 0;
	//生成Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());

	for(INT i=0; i<(INT)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") vFieldsType[i] = T_INT;
		else if(vRet[i] == "FLOAT") vFieldsType[i] = T_FLOAT;
		else if(vRet[i] == "STRING") vFieldsType[i] = T_STRING;
		else
		{
			return 0;
		}
	}

	INT nRecordsNum	= 0;
	INT nFieldsNum	= (INT)vFieldsType.size();

	//临时字符串区
	vector< std::pair< std::string, INT > >	vStringBuf;
	//检索表
	std::map< std::string, INT >					mapStringBuf;

	//空读一行
	pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return 0;

	INT nStringBufSize = 0;
	do
	{
		//读取一行
		pMem = _GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
		if(!pMem) break;;

		//是否是注释行
		if(szLine[0] == '#') continue;

		//分解
		_ConvertStringToVector(szLine, vRet, "\t", TRUE, FALSE);

		//列数不对
		if(vRet.empty()) continue;
        if(vRet.size() != nFieldsNum) 
		{
			//补上空格
			if((INT)vRet.size() < nFieldsNum)
			{
				INT nSubNum = nFieldsNum-(INT)vRet.size();
				for(INT i=0; i<nSubNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//第一列不能为空
		if(vRet[0].empty()) continue;

		for(register INT i=0; i<nFieldsNum; i++)
		{
			FIELD newField;
			switch(vFieldsType[i])
			{
			case T_INT:
				newField.iValue = atoi(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_FLOAT:
				newField.fValue = (FLOAT)atof(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_STRING:
				if(vRet[i].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					const CHAR * p = vRet[i].c_str();
					std::map< std::string, INT >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (INT)vStringBuf.size()-1));
						newField.iValue = nStringBufSize + 1; // first CHAR is '\0' for blank string
	
						nStringBufSize += (INT)strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	}while(TRUE);

	//--------------------------------------------------------
	//生成正式数据库
	nStringBufSize = nStringBufSize+1;

	//Create String Blok
	m_pStringBuf = new CHAR[nStringBufSize];
	if(!m_pStringBuf) return 0;

	//------------------------------------------------------
	// Create String Block
	UCHAR byBlank = '\0';
	m_pStringBuf[0] = '\0';

	register CHAR* p = m_pStringBuf + 1;
	for(INT i=0; i<(INT)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();

		*(p++) = '\0';
	}

	INT nFlag = 0xDDBBCC00;

	DWORD dwRet = 0;
	dwRet += 4; // Flag;
	dwRet += 4; //record
	dwRet += 4; //field
	dwRet += 4; // bufsize
	dwRet += (nFieldsNum*4); //types
	dwRet += (nFieldsNum*nRecordsNum*4); //value
	dwRet += nStringBufSize;

	m_lpTxt = new char[dwRet];
	char* pp = m_lpTxt;
	memcpy( pp, &nFlag, 4 ); pp += 4;
	memcpy( pp, &nFieldsNum, 4 ); pp += 4;
	memcpy( pp, &nRecordsNum, 4 ); pp += 4;
	memcpy( pp, &nStringBufSize, 4 ); pp += 4;
	for(register INT i=0; i<nFieldsNum; i++)
	{
		memcpy( pp, &vFieldsType[i], 4 ); pp += 4;
	}
	memcpy(pp, &(m_vDataBuf[0]), sizeof(FIELD)*nFieldsNum*nRecordsNum); 
	pp+=sizeof(FIELD)*nFieldsNum*nRecordsNum;
	memcpy( pp, m_pStringBuf, nStringBufSize-1 );
	m_lpTxt[dwRet-1] = 0;

	delete [] m_pStringBuf;

	return dwRet;
}

const CHAR* CPackageTools::_GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd)
{
	//寻找下一个换行符
	register const char* pMem = pMemory;
	if(pMem >= pDeadEnd || *pMem==0) return 0;

	while(pMem < pDeadEnd && pMem-pMemory+1<nBufSize && 
			*pMem != 0 && *pMem != '\r' && *pMem != '\n') *(pStringBuf++) = *(pMem++);
	//add 'null' end
	*pStringBuf = 0;

	//skip all next \r and \n
	while(pMem < pDeadEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) pMem++;

	return pMem;
}

INT	CPackageTools::_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty)
{
	vRet.clear();
	
	std::string strSrc = strStrINTgSource;
	if(strSrc.empty())
	{
		return 0;
	}

	std::string::size_type nLeft = 0;
	std::string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == std::string::npos)
	{
		nRight = strSrc.length();
	}
	while(TRUE)
	{
		std::string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : strlen(szKey));
		
		if(bOneOfKey)
		{
			std::string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != std::string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == std::string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (INT)vRet.size();
}


