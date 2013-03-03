#include "StdAfx.h"
#include "UpdateSystem.h"
#include "LaunchUtil.h"
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
//#include <zzip/zzip.h>
#include "Resource.h"
#include "LaunchSystem.h"


UpdateSystem* UpdateSystem::s_pMe = 0;
int UpdateSystem::s_Temp = 100;

unsigned char g_byPubKey[] = 
{
"\x06\x02\x00\x00\x00\x24\x00\x00\x52\x53\x41\x31\x00\x04\x00\x00"
"\x01\x00\x01\x00\xD9\xE6\x2E\x38\x62\x35\xD7\x06\x6F\x8D\x7A\x25"
"\x17\xE5\x7A\xA7\x68\xE1\x32\x52\x50\x8C\xE0\x1F\x2F\x1B\x2C\x85"
"\xA5\xCF\x6C\x2A\x8C\x14\x38\x67\xAE\xCD\x2E\x1F\xA3\xA9\xEF\xD9"
"\x90\xDF\xAE\x26\x85\x86\xF0\xC4\x5A\xA2\x54\x63\x36\x26\xEE\x42"
"\xEA\x9A\x87\x0D\x37\x90\x0C\x19\xDB\x76\x3B\xC4\xD3\x34\xEF\xFA"
"\x43\x7F\xED\xC0\xB0\xB2\x04\xF7\xE4\x16\xEB\x4E\xDE\x8A\xFA\x39"
"\x8E\xAD\x87\x6A\xFE\xC3\x39\x61\xAF\xEB\xF9\x3E\x97\xC7\xBF\x64"
"\xE0\x70\x9F\x61\x0B\x62\x26\x68\xE0\xD7\x30\x2B\x1F\x1E\xD7\x18"
"\x22\xE1\x65\xE2"
};

unsigned int g_nPubKeySize = sizeof(g_byPubKey)/sizeof(unsigned char);

UpdateSystem::UpdateSystem(void)
{
	s_pMe = this;
	m_bInit = FALSE;
	m_bUpdate = FALSE;
	m_bUndo = FALSE;
	m_bFullCheck = FALSE;
	m_nStep = 0;
	m_hNotifyWnd = NULL;
	m_lpThisPackageHead = NULL;
	m_lpPackageSize = NULL;
	m_dwTotalSize = 0;
	m_dwCurrent = 0;
	m_dwLastVersionFilesCount = 0;
	m_LogFile = fopen( "Launch.log", "wb" );
	m_DeleteFile.clear();
	m_ReplaceFile.clear();
	m_NewFile.clear();
}

UpdateSystem::~UpdateSystem(void)
{
	if( m_LogFile )
		fclose( m_LogFile );

	m_LogFile = 0;

	if( m_lpThisPackageHead ) 
	{ 
		delete [] m_lpThisPackageHead; 
		m_lpThisPackageHead = 0; 
	}

	if( m_lpPackageSize ) 
	{ 
		delete [] m_lpPackageSize; 
		m_lpPackageSize = 0; 
	}
}

BOOL UpdateSystem::Init(HWND hWnd, int nOwner, LPCSTR szUpdateIniFile, LPCSTR szResourcesIniFile, CString& strErrInfo)
{
	if( szUpdateIniFile == NULL )
	{
		strErrInfo = "PatchInfo�ļ�Ϊ�ա�";
		return( FALSE );
	}

	if( szResourcesIniFile == NULL )
	{
		strErrInfo = "Resources�ļ�Ϊ�ա�";
		return( FALSE );
	}

	m_hNotifyWnd = hWnd;
	m_nOwner = nOwner;

	// 2. �鿴��ǰ�ͻ��˰汾
	if( _access( "(version)", 4 ) != 0 ) 
	{
		strErrInfo = "���ؿͻ���version�ļ������ڡ�";
		return( FALSE );
	}

	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	CHAR szTempFile[MAX_PATH] = {0};
	memcpy(szTempFile,"(version)",sizeof("(version)"));

	FILE* fp = fopen( szTempFile, "rb" );
	if( fp == NULL ) 
	{
		strErrInfo.Format("���ļ�%sʧ�ܡ�",szTempFile);
		return( FALSE );
	}

	fseek(fp, 0, SEEK_END);
	INT nFileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	CHAR* szTemp = new CHAR[nFileLen];
	if(szTemp == NULL) 
	{
		strErrInfo = "��version�ļ�ʱ�ռ����ʧ�ܡ�";
		return ( FALSE );
	}

	CHAR* p = &szTemp[0];

	int read = (int)fread( szTemp, 1, nFileLen, fp );

	if( fclose( fp ) != 0 )
		strErrInfo = "�ر�version�ļ�����";

	if( read != nFileLen )
	{
		strErrInfo ="��version�ļ�ʱ�����д�";
		return( FALSE ); //�汾�ļ�����
	}

	//DeleteFile( szTempFile );
	if( ( p = strchr( szTemp, '|' ) ) == 0 ) 
	{
		strErrInfo.LoadString( IDS_US_ERR_VERSION );
		return( FALSE ); //�汾�ļ�����
	}
	*p = 0;
	m_strThisVersion = szTemp;

	// 3. ����������Ϣ�ļ�
	if( ReadPatchInfo( szUpdateIniFile, strErrInfo ) == FALSE ) 
	{
		Log(strErrInfo,strErrInfo.GetLength());
		return( FALSE );
	}

	// 4.��ȡResources.cfg
	m_strResourceCfg = szResourcesIniFile;
	_splitpath( szResourcesIniFile, 0, szTemp, 0, 0 );
	m_strResourceCfgBasePath = szTemp;
	if( ReadPackages(strErrInfo) == FALSE ) 
	{
		//strErrInfo.LoadString( IDS_US_ERR_CFG );
		return( FALSE ); //�������ļ�����
	}

	// 5.��ʼ����ȷ
	m_bInit = TRUE;
	return( TRUE );
}

BOOL UpdateSystem::ReadPackages(CString& strErrInfo)
{
	char buf[256];
	::GetCurrentDirectory(256,buf);
	m_PackageInfo.clear();

	CHAR szTemp[1024*20] = {0};
	CHAR* p = &szTemp[0];
	vector< string > vSections;

	if( _access( m_strResourceCfg.c_str(), 4 ) != 0 ) 
	{
		strErrInfo.Format("%s�ļ�������",m_strResourceCfg.c_str());
		return( FALSE ); //resources.cfg�ļ��޷���ȡ
	}

	if( GetPrivateProfileSectionNames( szTemp, 1024*20, m_strResourceCfg.c_str() ) == 0 )
	{
		strErrInfo.Format("��ȡ�����ļ�%sʱ����",m_strResourceCfg.c_str());
		return( FALSE ); //��ȡcfg�ļ�����
	}

	while( *p )
	{
		vSections.push_back( p );
		p += strlen( p ) + 1;
	}

	for( int n = 0; n < (int)vSections.size(); n ++ ) 
	{
		p = &szTemp[0]; *p = 0;
		GetPrivateProfileSection( vSections[n].c_str(), szTemp, 1024*20, m_strResourceCfg.c_str() );
		while( *p ) 
		{
			if( strncmp( p, "PakFileSystem=", 14 ) == 0 ) 
			{
				p += 14;
				if( !p ) 
					continue;

				_mbslwr( (byte*)p );
				CHAR* q = strrchr( p, '/' );
				if( !q ) 
					q = strrchr( p, '\\' );
				if( !q ) 
					q = p;
				else 
					q++;
				PackageInfo pi;
				pi.strPath = m_strResourceCfgBasePath + p;
				pi.strName = q;
				m_PackageInfo.insert( m_PackageInfoPair( pi.strName, pi ) );
			}
			p += strlen( p ) + 1;
		}
	}

	return( TRUE );
}

BOOL UpdateSystem::RereadPackages(LPSTR lpData, DWORD dwSize)
{
	if( lpData == NULL || dwSize == 0 ) return( FALSE );

	CHAR* p = lpData;
	CHAR* q;
	while( ( q = strstr( p, "PakFileSystem=" ) ) != NULL ) {
		p = strchr( q, '\r' );
		if( p ) *p = 0;
		_mbslwr( (byte*)q );
		CHAR* r = strrchr( q, '/' );
		if( !r ) r = strrchr( q, '\\' );
		if( !r ) r = q;
		else r++;
		map<string,PackageInfo >::iterator it = m_PackageInfo.find( r );
		if( it == m_PackageInfo.end() ) { //���µ�PackFileSystem
			PackageInfo pi;
			pi.strPath = m_strResourceCfgBasePath + q;
			pi.strName = r;
			m_PackageInfo.insert( m_PackageInfoPair( pi.strName, pi ) );

			// ��������µİ��ļ�
			FILE* fp = fopen( ( pi.strPath + ".nam" ).c_str(), "w" );
			if( fp ) {
				if( fwrite( "\0", 4, 1, fp ) != 4 )
				{
					fclose( fp );
					return( FALSE );
				}
				fclose( fp );
			}
			else
				return( FALSE );
			fp = fopen( ( pi.strPath + ".p01" ).c_str(), "w" );
			if( fp ) {
				if( fwrite( "\0", 4, 1, fp ) != 4 )
				{
					fclose( fp );
					return( FALSE );
				}
				fclose( fp );
			}
			else
				return( FALSE );
		}

		if( p ) p += 2;
		else break;
	}

	return( TRUE );
}

BOOL UpdateSystem::PushCommand( LPCSTR cmd, LPCSTR name, LPCSTR from, LPCSTR pak, DWORD dwCRC, LPCSTR zip,CString& strErrInfo )
{
	PackageOper po;
	string TempName = name;
	po.strFName = name;
	po.strZip = zip;

	string::size_type pos = TempName.find("from");
	if ( pos != string::npos )		//�ҵ�������һ��MPQ������
	{
		po.strFName = TempName.substr(0,pos-1);
		po.strMPQName = TempName.substr(pos+5);
	}

	po.dwCRC = dwCRC;
	map< string, PackageInfo >::iterator itInfo;
	map< string, PackageOper >::iterator itOper;

	if( strcmp( cmd, "pdel" ) == 0 )		//ɾ����
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��pdel����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			m_NewFile.erase( itOper );//ǰ�治�����ˣ�����Ҳ��ɾ����
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				m_ReplaceFile.erase( itOper );//ǰ�治�޸��ˣ�����ֱ��ɾ��
			}
			itOper = m_DeleteFile.find( name );
			if( itOper == m_DeleteFile.end() ) 
			{
				m_DeleteFile.insert( m_FilePair( name, po ) );
			}
			else 
			{
				strErrInfo = "command�ļ��߼���������ȶԹ���";
				return( FALSE ); //command�ļ��߼�����
			}
		}
	}
	else if( strcmp( cmd, "prpl" ) == 0 )	//�滻��
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��prpl����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			PackageOper* ppo = (PackageOper*)&itOper->second;
			ppo->strZip = zip;
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				PackageOper* ppo = (PackageOper*)&itOper->second;
				ppo->strZip = zip;
			}
			else 
			{
				itOper = m_DeleteFile.find( name );
				if( itOper == m_DeleteFile.end() ) 
				{
					m_ReplaceFile.insert( m_FilePair( name, po ) );
				}
				else
				{
					strErrInfo = "command�ļ��߼���������ȶԹ���";
					return( FALSE ); //command�ļ��߼�����
				}
			}
		}
	}
	else if( strcmp( cmd, "pnew" ) == 0 )	//���һ���°�
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��pnew����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			strErrInfo = "command�ļ��߼���������ȶԹ���";
			return( FALSE ); //command�ļ��߼�����
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				strErrInfo = "command�ļ��߼���������ȶԹ���";
				return( FALSE ); //command�ļ��߼�����
			}
			else 
			{
				itOper = m_DeleteFile.find( name );
				if( itOper != m_DeleteFile.end() ) 
				{
					m_DeleteFile.erase( itOper );
					m_ReplaceFile.insert( m_PackageOperPair( name, po ) );
				}
				else 
				{
					m_NewFile.insert( m_PackageOperPair( name, po ) );
				}
			}
		}
	}
	else if( strcmp( cmd, "frpl" ) == 0 ) 
	{
		if( name[0] == 0 )
			return( FALSE );

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			PackageOper* ppo = (PackageOper*)&itOper->second;
			ppo->strZip = zip;
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				PackageOper* ppo = (PackageOper*)&itOper->second;
				ppo->strZip = zip;
			}
			else 
			{
				itOper = m_DeleteFile.find( name );
				if( itOper == m_DeleteFile.end() ) 
				{
					m_ReplaceFile.insert( m_FilePair( name, po ) );
				}
				else
				{
					strErrInfo = "command�ļ��߼���������ȶԹ���";	
					return( FALSE ); //command�ļ��߼�����
				}
			}
		}
	}
	else if( strcmp( cmd, "fpdel" ) == 0 )
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��fpdel����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			m_NewFile.erase( itOper );//ǰ�治�����ˣ�����Ҳ��ɾ����
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				m_ReplaceFile.erase( itOper );//ǰ�治�޸��ˣ�����ֱ��ɾ��
			}
			itOper = m_DeleteFile.find( name );
			if( itOper == m_DeleteFile.end() ) 
			{
				m_DeleteFile.insert( m_FilePair( name, po ) );
			}
			else 
			{
				strErrInfo = "command�ļ��߼���������ȶԹ���";	
				return( FALSE ); //command�ļ��߼�����
			}
		}
	}
	else if( strcmp( cmd, "fprpl" ) == 0 ) 
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��fprpl����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			PackageOper* ppo = (PackageOper*)&itOper->second;
			ppo->strZip = zip;
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				PackageOper* ppo = (PackageOper*)&itOper->second;
				ppo->strZip = zip;
			}
			else 
			{
				itOper = m_DeleteFile.find( name );
				if( itOper == m_DeleteFile.end() ) 
				{
					m_ReplaceFile.insert( m_FilePair( name, po ) );
				}
				else
				{
					strErrInfo = "command�ļ��߼���������ȶԹ���";	
					return( FALSE ); //command�ļ��߼�����
				}
			}
		}
	}
	else if( strcmp( cmd, "fpnew" ) == 0 ) 
	{
		if( name[0] == 0 ) 
		{
			strErrInfo = "ִ��fpnew����ʱ��Ҫ���µ��ļ�Ϊ��,�����������?";		
			return( FALSE );
		}

		itOper = m_NewFile.find( name );
		if( itOper != m_NewFile.end() ) 
		{
			strErrInfo = "command�ļ��߼���������ȶԹ���";	
			return( FALSE ); //command�ļ��߼�����
		}
		else 
		{
			itOper = m_ReplaceFile.find( name );
			if( itOper != m_ReplaceFile.end() ) 
			{
				strErrInfo = "command�ļ��߼���������ȶԹ���";	
				return( FALSE ); //command�ļ��߼�����
			}
			else 
			{
				itOper = m_DeleteFile.find( name );
				if( itOper != m_DeleteFile.end() ) 
				{
					m_DeleteFile.erase( itOper );
					m_ReplaceFile.insert( m_PackageOperPair( name, po ) );
				}
				else 
				{
					m_NewFile.insert( m_PackageOperPair( name, po ) );
				}
			}
		}
	}
	return( TRUE );
}

 //strDir��ԭ��ԴĿ¼   Ext����չ��
void UpdateSystem::ClearTempFile(string  &strDir,string Ext)
{
	//for( vector< BackupDelete >::iterator it = m_BackupDelFiles.begin(); it != m_BackupDelFiles.end(); ++ it ) {
	//	DeleteFile( ((*it).strBackFile).c_str() );
	//	m_dwCurrent++;
	//	m_strCurDealFile = "";
	//	if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_UPDATESYSTEM, m_nOwner, (LPARAM)0 );
	//}

   	intptr_t	ptrFile;  
	struct		_finddata_t file;
	string		strFindTemp = strDir + "/*.*";
	
	if( (ptrFile = _findfirst( strFindTemp.c_str(), &file )) != -1L )
	{	
		while( _findnext( ptrFile, &file ) == 0 )
		{	
			if (file.name[0] == '.')
				continue;					//����

			string FilePath = strDir + "/" + file.name;
			if( PathIsDirectory( FilePath.c_str() ) )
			{
				//�����Ŀ¼,�����Ŀ¼���еݹ�
				ClearTempFile(strDir+"/"+file.name,Ext);
			}
			else
			{
				//�õ��ļ���չ��
				string::size_type ExtPos = FilePath.find_last_of(".");

				if ( ExtPos != string::npos )
				{
					string TempExt = FilePath.substr(ExtPos,FilePath.size()-ExtPos);
					if ( TempExt == Ext || Ext == "*.*" )
					{
						DeleteFile( FilePath.c_str() );
					}
         		}			
			}
			
		}//while end

		_findclose( ptrFile );				//�رղ��Ҿ��

	}//if end
}

BOOL UpdateSystem::BackupAddFile( string& strFileName )
{
	m_BackupAddFiles.push_back( strFileName );
	return( TRUE );
}

BOOL UpdateSystem::BackupDeleteFile( string strFileName )
{
	m_dwCurrent++;
	m_strCurDealFile = PathFindFileName( strFileName.c_str() );
	if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_BACKUPFILE, m_nOwner, (LPARAM)0 );

	BackupDelete bi;

	CHAR szBuf[2048];
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	CHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, itoa( s_Temp++, szBuf, 10 ), MAX_PATH, szTempFile);

	//if( !MyCopyFile( szTempFile, strFileName ) ) return( FALSE );
	if( MyCopyFile( szTempFile, strFileName ) ) {
		bi.strFile = strFileName;
		bi.strBackFile = szTempFile;
		m_BackupDelFiles.push_back( bi );
	}
	else {
		sprintf( szBuf, "�����ļ�%s������!\n", strFileName.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		//return( FALSE ); ���ﲻ�˳�����Ȼ�ڽ�������ʾ???
	}

	return( TRUE );
}

BOOL UpdateSystem::PreparePackage( string strPackage, CString& strErrInfo )
{
	char szBuf[1024];
	m_dwCurrent++;

	//��ȡͷ�ļ�
	if( m_lpThisPackageHead ) { delete [] m_lpThisPackageHead; m_lpThisPackageHead = 0; }
	string strHead = strPackage + ".nam";
	FILE* fp;
	fp = fopen( strHead.c_str(), "rb" );
	if( !fp ) return( FALSE );
	if( fread( &m_dwHeadSize, 1, 4, fp ) != 4 || m_dwHeadSize > MAX_PACKAGE_SIZE )
	{
		fclose( fp );
		return( FALSE );
	}
	m_lpThisPackageHead = new CHAR[m_dwHeadSize];
	if( m_lpThisPackageHead == NULL )
	{
		fclose( fp );
		return( FALSE );
	}
	if( fread( m_lpThisPackageHead, 1, m_dwHeadSize, fp ) != m_dwHeadSize )
	{
		delete [] m_lpThisPackageHead;
		m_lpThisPackageHead = NULL;
		fclose( fp );
		return( FALSE );
	}
	fclose( fp );
	m_dwHeadSizeAfter = m_dwHeadSize;

	//��ÿһ���ļ�
	char* lpHead = m_lpThisPackageHead;
	BYTE Length;
	FileInfo fi;
	while( (DWORD)( lpHead - m_lpThisPackageHead ) < m_dwHeadSize ) {
		Length = *lpHead ++;
		fi.strName = string("data/pack/") + string( lpHead, Length ); lpHead += Length;
		CopyMemory( &fi.dwOffset, lpHead, 4 ); lpHead += 4;
		CopyMemory( &fi.dwSize, lpHead, 4 ); lpHead += 4;
		CopyMemory( &fi.dwCRC, lpHead, 4 ); lpHead += 4;
		m_PackFileMap.insert( m_PackFilePair( fi.strName, fi ) );
	}
	sprintf( szBuf, "��ȡ��%sͷ��������%d���ļ�\n", strPackage.c_str(), m_PackFileMap.size() );
	Log( szBuf, (DWORD)strlen( szBuf ) );
	return( TRUE );
}

BOOL UpdateSystem::BeginingPackage( string strPackage, CString& strErrInfo )
{
	//CHAR szBuf[100];
	CHAR szFileName[1024] = {0};
	//CHAR szTempPath[MAX_PATH] = {0};
	//::GetTempPath(MAX_PATH, szTempPath);
	CHAR szTempFile[MAX_PATH] = {0};
	//::GetTempFileName(szTempPath, itoa( s_Temp++, szBuf, 10 ), MAX_PATH, szTempFile);

	BackupPackageDelete bi;
	bi.bDirty = false;

	//���ݰ�ͷ
	if( sprintf( szFileName, "%s.nam", strPackage.c_str() ) == -1 )
	{
		strErrInfo = CString("ͷ�ļ���")+strPackage.c_str()+"����";
		return( FALSE );
	}
	m_dwCurrent++;
	m_strCurDealFile = PathFindFileName( szFileName );
	if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_BACKUPFILE, m_nOwner, (LPARAM)0 );
	if( sprintf( szTempFile, "%s.%s.bak", szFileName, m_strThisVersion.c_str() ) != -1 )
	{
		if( MyCopyFile( szTempFile, szFileName ) ) bi.strBackFile = szTempFile;
		else bi.strBackFile = "";
	}
	else bi.strBackFile = "";
	m_PackDeleteFile.insert( m_PackDeleteFilePair( bi.strPackName, bi ) );

	//���ݰ��ļ�
	for( int n = 1; n < 100; n ++ ) {
		if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), n ) == -1 ||
			sprintf( szTempFile, "%s.%s.bak", szFileName, m_strThisVersion.c_str() ) == -1 )
		{
			strErrInfo = CString("�ļ���")+strPackage.c_str()+"����";
			return( FALSE );
		}

		FILE* fp = fopen( szFileName, "rb" );
		if( !fp ) { m_nThisPackageNum = n - 1; break; }
		fclose( fp );

		m_dwCurrent++;
		m_strCurDealFile = PathFindFileName( szFileName );
		if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_BACKUPFILE, m_nOwner, (LPARAM)0 );

		//::GetTempFileName(szTempPath, itoa( s_Temp++, szBuf, 10 ), MAX_PATH, szTempFile);
		bi.strPackName = szFileName;
		if( MyCopyFile( szTempFile, szFileName ) ) bi.strBackFile = szTempFile;
		else bi.strBackFile = "";
		m_PackDeleteFile.insert( m_PackDeleteFilePair( bi.strPackName, bi ) );
	}

	//��ȡÿ�����ļ��ĳ���
	if( m_lpPackageSize ) { delete [] m_lpPackageSize; m_lpPackageSize = 0; }
	m_lpPackageSize = new pair<INT, INT>[m_nThisPackageNum];
	for( int n = 0; n < m_nThisPackageNum; n ++ ) {
		if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), n + 1 ) == -1 ) return( FALSE );
		FILE* fp = fopen( szFileName, "rb" );
		if( !fp ) { strErrInfo.Format( IDS_US_ERR_PACK_NOTFOUND, szFileName ); return( FALSE ); }
		if( fread( &m_lpPackageSize[n].first, 1, 4, fp ) != 4 ||
			fseek( fp, 0, SEEK_END ) != 0 ) return( FALSE );
		m_lpPackageSize[n].second = (INT)ftell( fp );
		fclose( fp );
	}

	return( TRUE );
}

BOOL UpdateSystem::AppendFilesFromPackage( string strPackage, vector<PackageOper>& vecFiles, CString& strErrInfo )
{
	AfxMessageBox("AppendFilesFromPackage");

/*	DWORD dwNeedSize = 0;
	CHAR szFileName[1024];
	CHAR szBuf[1024];
	for( int n = 0; n < (int)vecFiles.size(); n ++ ) {
		strcpy( szFileName, vecFiles[n].strFName.c_str() );
		CHAR* p = strstr( szFileName, "data\\pack" );
		if( !p ) p = strstr( szFileName, "data/pack" );
		if( p ) p += 10;
		else 
			continue;
		dwNeedSize += (DWORD)strlen( p ) + 13;
	}

	if( ( m_dwHeadSizeAfter + dwNeedSize ) > m_dwHeadSize ) {
		char* p = new char[m_dwHeadSizeAfter + dwNeedSize];
		CopyMemory( p, m_lpThisPackageHead, m_dwHeadSizeAfter );
		delete [] m_lpThisPackageHead;
		m_lpThisPackageHead = p;
	}
	m_dwHeadSize = m_dwHeadSizeAfter + dwNeedSize;

	for( int n = 0; n < (int)vecFiles.size(); n ++ ) {
		strcpy( szFileName, vecFiles[n].strFName.c_str() );
		CHAR* p = strstr( szFileName, "data\\pack" );
		if( !p ) p = strstr( szFileName, "data/pack" );
		if( p ) p += 10;
		else 
			continue;

		m_dwCurrent++;
		m_strCurDealFile = PathFindFileName( p );
		if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_APPENDPACKFILE, m_nOwner, (LPARAM)0 );

		zzip_error_t zzipError;
		ZZIP_DIR* mZzipDir = zzip_dir_open( vecFiles[n].strZip.c_str(), &zzipError );
		if (zzipError != ZZIP_NO_ERROR) {
			strErrInfo.Format( IDS_US_ERR_PATCH, vecFiles[n].strZip.c_str() );
			return( FALSE );//�޷���Patch�ļ�
		}
		ZZIP_FILE* zzipFile = zzip_file_open( mZzipDir, szFileName, ZZIP_ONLYZIP | ZZIP_CASELESS );
		if( zzipFile == 0 ) {
			strErrInfo.Format( IDS_DOWNLOAD_FILE_ERR, vecFiles[n].strZip.c_str(), szFileName );
			return( FALSE );//Patch�ļ���û������ļ�
		}

		ZZIP_STAT zstat;
		int zip_err = zzip_dir_stat( mZzipDir, szFileName, &zstat, ZZIP_CASEINSENSITIVE );
		if( zip_err != 0 )
		{
			zzip_file_close( zzipFile );
			zzip_dir_close( mZzipDir );
			strErrInfo = CString("�޷������ļ���")+szFileName;
			return( FALSE );
		}
		LPSTR lpFile = new CHAR[zstat.st_size];
		if( !lpFile ) 
		{
			zzip_file_close( zzipFile );
			zzip_dir_close( mZzipDir );
			strErrInfo = CString("�޷������ļ���")+szFileName;
			return( FALSE );
		}
		zip_err = zzip_file_read( zzipFile, lpFile, zstat.st_size );
		if( zip_err != zstat.st_size )
		{
			zzip_file_close( zzipFile );
			zzip_dir_close( mZzipDir );
			strErrInfo = CString("�޷������ļ���")+szFileName;
			return( FALSE );
		}
		zzip_file_close( zzipFile );
		zzip_dir_close( mZzipDir );

		int nPack = AppendFile( strPackage, m_lpPackageSize, m_nThisPackageNum, lpFile, (DWORD)zstat.st_size );
		if( nPack == -1 ) {
			strErrInfo.Format( IDS_US_ERR_PACK_ADD, p );
			return( FALSE );
		}
		if( AppendHead( m_lpPackageSize, m_nThisPackageNum, nPack, p, (DWORD)zstat.st_size, vecFiles[n].dwCRC, m_lpThisPackageHead, m_dwHeadSizeAfter ) == -1 ) {
			strErrInfo.LoadString( IDS_US_ERR_PACK_LOGIC );
			return( FALSE );
		}

		FileInfo fi;
		fi.strName = vecFiles[n].strFName;
		fi.dwOffset = 0;
		fi.dwSize = zstat.st_size;
		fi.dwCRC = vecFiles[n].dwCRC;

		if( nPack >= m_nThisPackageNum ) {
			pair<INT, INT>* pTemp = new pair<INT, INT>[nPack + 1];
			int i;
			for(i = 0; i < m_nThisPackageNum; i ++ ) {
				pTemp[i].first = m_lpPackageSize[i].first;
				pTemp[i].second = m_lpPackageSize[i].second;
				fi.dwOffset += m_lpPackageSize[i].first;
			}
			pTemp[i].first = zstat.st_size;
			pTemp[i].second = zstat.st_size + 4;

			delete [] m_lpPackageSize;
			m_lpPackageSize = pTemp;
			pTemp = NULL;
			m_nThisPackageNum = nPack + 1;
		}
		else {
			int i;
			for(i = 0; i < nPack + 1; i ++ ) {
				fi.dwOffset += m_lpPackageSize[i].first;
			}
			m_lpPackageSize[nPack].first += zstat.st_size;
		}
		delete [] lpFile;

		m_PackFileMap.insert( m_PackFilePair( fi.strName, fi ) );

		sprintf( szBuf, "��������%s�ļ�\n", m_strCurDealFile.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
	}
*/
	return( TRUE );
}

BOOL UpdateSystem::DeleteFilesFromPackage( string strPackage, vector<string>& vecFiles, CString& strErrInfo )
{
	CHAR szFileName[1024] = {0};
	char szBuf[1024];

	//����ÿ��ɾ��
	for( int n = 0; n < (int)vecFiles.size(); n ++ ) {
		strcpy( szFileName, vecFiles[n].c_str() );
		CHAR* p = strstr( szFileName, "data\\pack" );
		if( !p ) p = strstr( szFileName, "data/pack" );
		if( p ) p += 10;
		else 
			continue;

		m_dwCurrent++;
		m_strCurDealFile = PathFindFileName( p );
		if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_DELETEPACKFILE, m_nOwner, (LPARAM)0 );

		DWORD dwPackOffset;
		FileInfo fi; fi.dwOffset = 0;
		INT nOffset = FindFile( m_lpThisPackageHead, m_dwHeadSizeAfter, p, fi );
		INT nPack = CalPackOffset( m_lpPackageSize, m_nThisPackageNum, fi.dwOffset, dwPackOffset );
		if( nOffset >= 0 && nPack >= 0 ) {
			if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), nPack + 1 ) == -1 )
			{
				strErrInfo.Format( IDS_US_ERR_PACK_DEL, szFileName, p );
				return( FALSE );
			}
			map<string,BackupPackageDelete >::iterator it = m_PackDeleteFile.find( szFileName );
			if( it != m_PackDeleteFile.end() ) {
				string strPack = it->second.strBackFile;//���ȸ��±��ݵ��ļ�
				if( strPack.empty() ) strPack = szFileName;
				if( MyDeleteFile( strPack, (DWORD)( m_lpPackageSize[nPack].first + 4 ), dwPackOffset + 4, fi.dwSize ) ) {
					m_dwHeadSizeAfter = ReduceHead( m_lpThisPackageHead, m_dwHeadSizeAfter, (DWORD)nOffset );
					m_lpPackageSize[nPack].first -= fi.dwSize;
					if( it->second.strBackFile.empty() == false ) 
						it->second.bDirty = true;//�����ļ���
				}
				else {
					strErrInfo.Format( IDS_US_ERR_PACK_DEL, szFileName, p );
					Log( strErrInfo, strErrInfo.GetLength() );
					//delete [] nSizes;
					//return( FALSE );
				}
			}
		}
		map<string,FileInfo>::iterator InfoIterator = m_PackFileMap.find( vecFiles[n].c_str() );
		if( InfoIterator != m_PackFileMap.end() ) 
			m_PackFileMap.erase( InfoIterator );

		sprintf( szBuf, "�Ӱ���ɾ��%s�ļ�\n", m_strCurDealFile.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
	}

	return( TRUE );
}

BOOL UpdateSystem::MyDeleteFile( string strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize )
{
	if( MyDeleteFileM( strPackage, dwValueSize, dwOffset, dwSize ) ) return( TRUE );
	return( MyDeleteFileD( strPackage, dwValueSize, dwOffset, dwSize ) );
}

BOOL UpdateSystem::MyDeleteFileM( string& strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize )
{
	if( dwValueSize < ( dwOffset + dwSize ) ) return( FALSE );
	HANDLE hFile, hFileMapping;
	LPBYTE lpFile;
	DWORD dwTemp;
	hFile = CreateFile( strPackage.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) {
		SetFileAttributes( strPackage.c_str(), FILE_ATTRIBUTE_NORMAL );
		hFile = CreateFile( strPackage.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hFile == INVALID_HANDLE_VALUE ) {
			return( FALSE );
		}
	}
	if( dwValueSize > GetFileSize( hFile, NULL ) ) return( FALSE );
	hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0, dwValueSize, NULL );
	CloseHandle( hFile ); hFile = INVALID_HANDLE_VALUE;
	if( hFileMapping == NULL ) return( FALSE );
	lpFile = (LPBYTE)MapViewOfFile( hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwValueSize );
	if( lpFile == NULL ) { CloseHandle( hFileMapping ); return( FALSE ); }
	CopyMemory( lpFile + dwOffset, lpFile + dwOffset + dwSize, dwValueSize - dwOffset - dwSize );
	CopyMemory( &dwTemp, lpFile, 4 );
	dwTemp -= dwSize;
	CopyMemory( lpFile, &dwTemp, 4 );
	UnmapViewOfFile( lpFile );
	CloseHandle( hFileMapping );
	return( TRUE );
}

BOOL UpdateSystem::MyDeleteFileD( string& strPackage, DWORD dwValueSize, DWORD dwOffset, DWORD dwSize )
{
	if( dwValueSize < ( dwOffset + dwSize ) ) return( FALSE );
	FILE* fp;
	fp = fopen( strPackage.c_str(), "r+b" );
	if( fp == NULL ) {
		if( ( _chmod( strPackage.c_str(), _S_IREAD | _S_IWRITE ) != 0 ) && ( ( fp = fopen( strPackage.c_str(), "r+b" ) ) == 0 ) ) {
			return( FALSE );
		}
	}

	CHAR s[2] = { 0,0 };
	DWORD dwPoint = dwOffset;
	for( DWORD dw = 0; dw < ( dwValueSize - dwOffset - dwSize ); dw ++ ) {
		if( fseek( fp, dwPoint + dwSize, SEEK_SET ) != 0 ||
			fread( s, 1, 1, fp ) != 1 ||
			fseek( fp, dwPoint, SEEK_SET ) != 0 )
		{
			fclose( fp );
			return( FALSE );
		}
		if( fwrite( s, 1, 1, fp ) != 1 )
		{
			fclose( fp );
			return( FALSE );
		}
		dwPoint ++;
	}
	if( fseek( fp, 0, SEEK_SET ) != 0 ||
		fread( &dwPoint, 1, 4, fp ) != 4 || 
		fseek( fp, 0, SEEK_SET ) != 0 )
	{
		fclose( fp );
		return( FALSE );
	}		
	dwPoint -= dwSize;
	int read = (int)fwrite( &dwPoint, 1, 4, fp );
	fclose( fp );
	if( read != 4 ) return( FALSE );
	return( TRUE );
}

BOOL UpdateSystem::MyCopyFile( string strDest, string strSrc )
{
	if( MyCopyFileM( strDest, strSrc ) ) return( TRUE );
	return( MyCopyFileD( strDest, strSrc ) );
}

BOOL UpdateSystem::MyCopyFile1M( string strDest, LPCSTR lpData, DWORD dwSize )
{
	HANDLE hDst = CreateFile( strDest.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hDst == INVALID_HANDLE_VALUE ) {
		SetFileAttributes( strDest.c_str(), FILE_ATTRIBUTE_NORMAL );
		hDst = CreateFile( strDest.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hDst == INVALID_HANDLE_VALUE ) {
			return( FALSE );
		}
	}
	HANDLE hDstMapping = CreateFileMapping( hDst, NULL, PAGE_READWRITE, 0, dwSize + 4, NULL );
	CloseHandle( hDst ); hDst = INVALID_HANDLE_VALUE;
	if( hDstMapping == NULL ) { return( FALSE ); }
	LPBYTE lpDst = (LPBYTE)MapViewOfFile( hDstMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwSize + 4 );

	CopyMemory( lpDst, &dwSize, 4 );
	CopyMemory( lpDst + 4, lpData, dwSize );

	UnmapViewOfFile( lpDst );
	CloseHandle( hDstMapping );

	return( TRUE );
}

BOOL UpdateSystem::MyCopyFileM( string& strDest, string& strSrc )
{
	HANDLE hSrc = CreateFile( strSrc.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hSrc == INVALID_HANDLE_VALUE ) return( FALSE );
	DWORD dwSrc = GetFileSize( hSrc, NULL );
	HANDLE hSrcMapping = CreateFileMapping( hSrc, NULL, PAGE_READONLY, 0, dwSrc, NULL );
	CloseHandle( hSrc ); hSrc = INVALID_HANDLE_VALUE;
	if( hSrcMapping == NULL ) return( FALSE );
	LPBYTE lpSrc = (LPBYTE)MapViewOfFile( hSrcMapping, FILE_MAP_READ, 0, 0, dwSrc );
	if( lpSrc == NULL ) { CloseHandle( hSrcMapping ); return( FALSE ); }

	HANDLE hDst = CreateFile( strDest.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hDst == INVALID_HANDLE_VALUE ) {
		SetFileAttributes( strDest.c_str(), FILE_ATTRIBUTE_NORMAL );
		hDst = CreateFile( strDest.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hDst == INVALID_HANDLE_VALUE ) {
			UnmapViewOfFile( lpSrc );
			CloseHandle( hSrcMapping );
			return( FALSE );
		}
	}
	HANDLE hDstMapping = CreateFileMapping( hDst, NULL, PAGE_READWRITE, 0, dwSrc, NULL );
	CloseHandle( hDst ); hDst = INVALID_HANDLE_VALUE;
	if( hDstMapping == NULL ) { UnmapViewOfFile( lpSrc ); CloseHandle( hSrcMapping ); return( FALSE ); }
	LPBYTE lpDst = (LPBYTE)MapViewOfFile( hDstMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwSrc );
	if( lpDst == NULL ) { CloseHandle( hDstMapping ); UnmapViewOfFile( lpSrc ); CloseHandle( hSrcMapping ); return( FALSE ); }

	CopyMemory( lpDst, lpSrc, dwSrc );

	UnmapViewOfFile( lpSrc );
	CloseHandle( hSrcMapping );
	UnmapViewOfFile( lpDst );
	CloseHandle( hDstMapping );

	return( TRUE );
}

BOOL UpdateSystem::MyCopyFileD( string& strDest, string& strSrc )
{
	//FILE* f1;
	//FILE* f2;
	//f1 = fopen( strSrc.c_str(), "rb" );
	//if( !f1 ) return( FALSE );
	//fclose( f1 );
	//f2 = fopen( strDest.c_str(), "wb" );
	//if( !f2 ) {
	//	if( ( _chmod( strDest.c_str(), _S_IREAD | _S_IWRITE ) != 0 ) && ( ( f2 = fopen( strDest.c_str(), "wb" ) ) == 0 ) ) {
	//		return( FALSE );
	//	}
	//}
	//fclose( f2 );

	//fseek( f1, 0, SEEK_END );
	//LONG nLen = ftell( f1 );
	//fseek( f1, 0, SEEK_SET );


	//CHAR buf[512];
	//while( nLen > 512 ) {
	//	fread( buf, 1, 512, f1 );
	//	fwrite( buf, 1, 512, f2 );
	//	nLen -= 512;
	//}
	//if( nLen > 0 ) {
	//	fread( buf, 1, nLen, f1 );
	//	fwrite( buf, 1, nLen, f2 );
	//	nLen = 0;
	//}
	//fclose( f1 );
	//fclose( f2 );
	SetFileAttributes( strDest.c_str(), FILE_ATTRIBUTE_NORMAL );
	return( CopyFile( strSrc.c_str(), strDest.c_str(), FALSE ) );
}

int UpdateSystem::GetUpdateStep( void )
{
	if( m_bInit == FALSE ) return( 0 );

	return( m_nStep );
}

const PatchInfo* UpdateSystem::GetUpdateInfo( int nStep )
{
	if( m_bInit == FALSE ) return( NULL );
	if( nStep >= m_nStep ) return( NULL );

	return( &m_PatchInfo[nStep] );
}

//����CRC32��MD5���ж��ļ�������
BOOL UpdateSystem::IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen)
{
	md5_context md5_ctx;
	crc32_context crc32_ctx;

	MD5_Init(&md5_ctx);
	CRC32_Init(&crc32_ctx);

	CRC32_Update(&crc32_ctx,input,ilen );
	MD5_Update(&md5_ctx,input,ilen);

	TKPMCRC32*		pCrc32 = KPM_ALLOC(TKPMCRC32,0x4000 );	
	TKPMMD5	*		pMD5 = KPM_ALLOC(TKPMMD5, 0x4000);
	memset(pCrc32, 0, sizeof(TKPMCRC32) * 0x4000);
	memset(pMD5, 0, sizeof(TKPMMD5) * 0x4000);

	CRC32_Finish(&crc32_ctx, (unsigned long *)&pCrc32->dwValue);
	MD5_Finish(&md5_ctx, pMD5->Value);

	if ( pFile->pCrc32->dwValue == pCrc32->dwValue )
	{
		if ( _mbscmp(pFile->pMD5->Value,pMD5->Value) == 0 )
		{
			printf("MD5����CRC32�붼��ͬ��˵���ļ�δ��");
			return TRUE;	
		}
		else
		{
			//ȡ����һ�ν��бȽ�
			TKPMMD5 *temp = KPM_ALLOC(TKPMMD5, 0x4000); ;
			memset(temp, 0, sizeof(TKPMMD5) * 0x4000);
			memcpy(temp->Value,pFile->pMD5->Value,16);
			if ( _mbscmp(temp->Value,pMD5->Value) == 0 )
			{
				//MD5����CRC32�붼��ͬ��˵���ļ�δ��
				return TRUE;					
			}
		}
		return FALSE;		
	}

	//�ļ���
	return FALSE;

}//IsAllRightCRC32AndMD5 end

BOOL UpdateSystem::PreUpdate( int nStep, LPCSTR szLocalPatchFile, CString& strErrInfo )
{
	if( m_bInit == FALSE ) 
	{
		strErrInfo = "m_bInit��־ΪFALSE";	
		return( FALSE );
	}

	if( nStep >= m_nStep ) 
	{
		strErrInfo.LoadString( IDS_US_ERR_STEP );
		return( FALSE );
	}
	if( szLocalPatchFile == NULL ) 
	{
		strErrInfo.LoadString( IDS_US_ERR_PARAM );
		return( FALSE );//��������
	}

	DWORD R;
	TKPMArchive* pArchive = KPM_OpenArchive(szLocalPatchFile,R);
	if ( !pArchive )
	{
		strErrInfo.Format( IDS_US_ERR_PATCH, szLocalPatchFile );
		return( FALSE );	//�޷���Patch�ļ�
	}

	//************** ��ȡ������(version)�ļ�
	TKPMFile*	pFile = KPM_FileOpen(pArchive,"(version)",R);
	if ( !pFile )
	{
		//strErrInfo.LoadString( IDS_US_ERR_ACCESS_VERSION );
		strErrInfo = "���°���û�з���version�ļ�����û�и��ѽ��";
		return( FALSE );	//Patch�ļ���û��(version)�ļ�
	}

	DWORD FileSize = KPM_GetFileSize(pFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		strErrInfo = CString( "�����ڴ����" );
		return( FALSE );
	}
	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		strErrInfo.LoadString( IDS_US_ERR_ACCESS_VERSION );
		return( FALSE );		//Patch�ļ���û��(version)�ļ�
	}

	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		strErrInfo.Format("Version�ļ��𻵣����������أ�");
		return( FALSE );
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);

	//**************lpFile�м�Ϊ��version���ļ����ݣ�����Ҫ����д����ʱ�ļ�
	CHAR szTemp[100];
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	CHAR szTempFile[MAX_PATH] = {0};
	memcpy(szTempFile,"(version)",sizeof("(version)"));
	delete [] lpFile;

	//**********����ʱ�ļ����ݶ����ڴ棬ɾ����ʱ�ļ�
	map<string, DWORD> mCrc;
	FILE* fp = fopen( szTempFile, "rb" );
	if( fp == 0 ) 
	{
		strErrInfo.Format("������ʱ�ļ�%sʧ�ܡ�",szTempFile);
		return( FALSE );
	}
	if( fseek( fp, 0, SEEK_END ) != 0 )
	{
		fclose( fp );
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		return( FALSE );
	}
	DWORD dwSize = (DWORD)ftell( fp );
	lpFile = new CHAR[dwSize+1];
	if( lpFile == NULL )
	{
		fclose( fp );
		strErrInfo.Format("�����ڴ�ʧ�ܡ�");
		return( FALSE );
	}

	lpFile[dwSize] = 0;
	if( fseek( fp, 0, SEEK_SET ) != 0 || fread( lpFile, 1, dwSize, fp ) != dwSize )
	{
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		delete [] lpFile;
		fclose( fp );
		return( FALSE );
	}
	fclose( fp );
	//DeleteFile( szTempFile );

	//*********************����(version)�ļ�
	CHAR* token = strtok( lpFile, "\n" );
	token = strtok( NULL, "\n" );
	while( token ) 
	{
		_mbslwr( (byte*)token );
		char* p = token;
		while( *p++ ) 
		{ 
			if( *p == '\\' )
				*p = '/'; 
		}
		char* dot1 = strchr( token, '|' );
		if( !dot1 ) 
		{
			strErrInfo.LoadString( IDS_US_ERR_VERSION );
			return( FALSE );
		}

		char* dot2 = strchr( dot1 + 1, '|' );
		if( !dot2 ) 
		{
			strErrInfo.LoadString( IDS_US_ERR_VERSION );
			return( FALSE );
		}

		dot2 ++;
		DWORD dwCRC;
		sscanf( dot2, "%x", &dwCRC );
		mCrc.insert( std::make_pair( string( token, ( dot1 - token ) ), dwCRC ) );
		token = strtok( NULL, "\n" );
	}
	delete [] lpFile;
	m_dwLastVersionFilesCount = (DWORD)mCrc.size();

	//****************************** ��ȡ������(command)�ļ�
	{

	TKPMFile*	pFile = KPM_FileOpen(pArchive,"(command)",R);
	if ( !pFile )
	{
		strErrInfo = "���°���û�з���command�ļ��������Ҫɱ���ˣ�";
		return( FALSE );	//Patch�ļ���û��(command)�ļ�
	}

	DWORD FileSize = KPM_GetFileSize(pFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		strErrInfo = CString( "�����ڴ����" );
		return( FALSE );
	}
	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		strErrInfo.LoadString( IDS_US_ERR_ACCESS_COMMAND );
		return( FALSE );		//Patch�ļ���û��(version)�ļ�
	}

	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		strErrInfo.Format("Command�ļ��𻵣����������أ�");
		return( FALSE );
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);

	//**************lpFile�м�Ϊ��command���ļ����ݣ�����Ҫ����д����ʱ�ļ�
	::GetTempFileName( szTempPath, itoa( s_Temp++, szTemp, 10 ), MAX_PATH, szTempFile );

	//����ԭʼ�ļ�
	FILE* fp = fopen(szTempFile, "wb");
	if(fp == NULL)
	{
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		delete [] lpFile;
		return FALSE;
	}

	fwrite(lpFile, 1, FileSize, fp);
	fclose(fp);
	delete [] lpFile;

	//**********����ʱ�ļ����ݶ����ڴ棬ɾ����ʱ�ļ�
	fp = fopen( szTempFile, "rb" );
	if( fp == 0 )
	{
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		return( FALSE );
	}
	if( fseek( fp, 0, SEEK_END ) != 0 )
	{
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		fclose( fp );
		return( FALSE );
	}		
	dwSize = (DWORD)ftell( fp );
	lpFile = new CHAR[dwSize+1];
	if( lpFile == NULL )
	{
		strErrInfo = CString( "�����ڴ����" );
		return( FALSE );
	}
	lpFile[dwSize] = 0;
	if( fseek( fp, 0, SEEK_SET ) != 0 || fread( lpFile, 1, dwSize, fp ) != dwSize )
	{
		strErrInfo.Format("����ʱ�ļ�%sʧ�ܡ�",szTempFile);
		delete [] lpFile;
		fclose( fp );
		return( FALSE );
	}		
	fclose( fp );
	DeleteFile( szTempFile );

	//*********************����(command)�ļ�
	CHAR str1[1024], str2[1024], str3[1024], str4[1024];
	token = strtok( lpFile, "\r\n" );
	while( token ) 
	{
		_mbslwr( (byte*)token );
		char* p = token;
		while( *p++ )
		{
			if( *p == '\\' ) 
				*p = '/';
		}

		char* pDot = strchr( token, ' ' );
		if( !pDot )
		{
			delete [] lpFile;
			strErrInfo.LoadString( IDS_US_ERR_COMMAND );
			return( FALSE );
		}

		strcpy( str1, string( token, ( pDot - token ) ).c_str() );
		pDot ++;
		if( str1[0] == 'p' ) 
		{
			strcpy( str2, string( pDot).c_str() );
		}
		else
		{
			strcpy( str2, pDot );
			str3[0] = 0;
			str4[0] = 0;
		}
		//sscanf( token, "%s %s %s %s", str1, str2, str3, str4 );
		DWORD dwCRC;
		if( ( strcmp( str1, "pdel" ) == 0 ) || strcmp( str1, "fdel" ) == 0 || strcmp( str2, "(version)" ) == 0 ) 
		{
			dwCRC = 0;
		}
		else 
		{	
			dwCRC = 0;
		}
		if( PushCommand( str1, str2, str3, str4, dwCRC, szLocalPatchFile,strErrInfo ) == FALSE )
		{
			strErrInfo =  strErrInfo + ":\n" + str2 + "_" + str4 + ")" ;
			delete [] lpFile;
			return( FALSE );//(command)�ļ��߼�����
		}
		token = strtok( NULL, "\r\n" );
	}

	delete [] lpFile;
	}
	KPM_CloseArchive(pArchive);
	return( TRUE );

}//PreUpdate end

BOOL UpdateSystem::ReadPatchInfo( LPCSTR szUpdateIniFile, CString& strErrInfo )
{
	CHAR szTemp[1024] = {0};
	CHAR szIni[1024*20] = {0};
	CHAR* p = &szIni[0];
	int nMaxPath = 0;

	if( GetPrivateProfileSectionNames( szIni, 1024*20, szUpdateIniFile ) == 0 )
	{
		strErrInfo.LoadString( IDS_US_ERR_INI );
		strErrInfo = szUpdateIniFile + strErrInfo;
		return( FALSE ); // �����ļ�����
	}

	map< string, PatchInfo > vPatchInfo;
	while( *p )
	{
		if( strcmp( p, "Version" ) == 0 ) 
		{
			GetPrivateProfileString( p, "Latest", "", szTemp, 1024, szUpdateIniFile );
			m_strLastVersion = szTemp;
			if( m_strThisVersion == m_strLastVersion ) 
			{ //��ǰ�������°汾���˳�
				m_bInit = TRUE;
				m_bUpdate = TRUE;
				strErrInfo = "��ǰ�������°汾���˳�\n";
				return( TRUE );
			}
		}
		else if( strncmp( p, "Patch_", 6 ) == 0 ) 
		{
			PatchInfo pi;
			GetPrivateProfileString( p, "VersionFrom", "", szTemp, 1024, szUpdateIniFile );
			pi.strVerFrom = szTemp;
			GetPrivateProfileString( p, "VersionTo", "", szTemp, 1024, szUpdateIniFile );
			pi.strVerTo = szTemp;

			CHAR szUrl[1024];
			int nUrl = 1;
			sprintf( szUrl, "URL_%d", nUrl );
			while( GetPrivateProfileString( p, szUrl, "", szTemp, 1024, szUpdateIniFile ) > 0 ) 
			{
				CHAR* pDot = strchr( szTemp, ';' );
				if( pDot ) 
					*pDot ++ = 0;
				else 
					pDot = szTemp;
				pi.urlInfo.push_back( URLInfo( szTemp, pDot ) );
				sprintf( szUrl, "URL_%d", ++nUrl );
			}

			p += 6;
			int n = atoi( p );
			//vPatchInfo.insert( std::make_pair( pi.strVerFrom + pi.strVerTo,  pi ) );
			vPatchInfo[pi.strVerFrom+"."+pi.strVerTo] = pi;
		}
		p += strlen( p ) + 1;
	}

	nMaxPath = (int)vPatchInfo.size();

	// ����������Ҫ����
	//BOOL bFindThisVer = FALSE;
	//BOOL bFindEndVer = FALSE;
	//for( map< string, PatchInfo >::iterator it = vPatchInfo.begin(); it != vPatchInfo.end(); ++ it ) {
	//	PatchInfo& pi = it->second;
	//	if( m_strThisVersion == pi.strVerFrom ) bFindThisVer = TRUE;
	//	if( m_strLastVersion == pi.strVerTo ) bFindEndVer = TRUE;
	//}
	//if( bFindThisVer == FALSE || bFindEndVer == FALSE ) {
	//	strErrInfo.LoadString( IDS_US_ERR_UPDATE );
	//	return( FALSE ); // ���ܴӸð汾���������°汾
	//}

	string strFrom = m_strThisVersion;
	string strTo;
	string strKey;
	map< string, PatchInfo > bResult;
	while( 1 )
	{
		strKey = FindMaxTo( vPatchInfo, strFrom, strTo );
		if( strTo.empty() || strKey.empty() ) 
		{
			strErrInfo.Format("���ܴ�%s������%s�����鱾�ذ汾��PatchInfo�ļ�",strFrom.c_str(),strTo.c_str());
			return( FALSE );// ���ܴӸð汾���������°汾
		}
		bResult[strKey] = vPatchInfo[strKey];
		if( strTo == m_strLastVersion ) 
			break;
		strFrom = strTo; strTo.clear(); strKey.clear();
	}
	if( bResult.size() == 0 )
	{
		strErrInfo.LoadString( IDS_US_ERR_UPDATE );
		return( FALSE );
	}
	for( map< string, PatchInfo >::iterator it = bResult.begin(); it != bResult.end(); ++ it )
	{
		m_PatchInfo.push_back( it->second );
	}

	m_nStep = (int)m_PatchInfo.size();
	strErrInfo.Format("\n����%d�����°�������",m_nStep);
	Log(strErrInfo,strErrInfo.GetLength());
	return( TRUE );
}

string UpdateSystem::FindMaxTo( map< string, PatchInfo >& vPatchInfo, string strFrom, string& strTo )
{
	string strKey;
	for( map< string, PatchInfo >::iterator it = vPatchInfo.begin(); it != vPatchInfo.end(); ++ it ) {
		if( it->second.strVerFrom == strFrom ) {
			string strComTo, strComVerTo;
			ConvertVer( strTo, strComTo );
			ConvertVer( it->second.strVerTo, strComVerTo );
			//if( strTo < it->second.strVerTo ) {
			if( strComTo < strComVerTo ) {
				strTo  = it->second.strVerTo;
				strKey = it->first;
			}
		}
	}
	return strKey;
}

BOOL UpdateSystem::IfInit( void )
{
	return( m_bInit );
}

BOOL UpdateSystem::IfUpdateOK( void )
{
	return( m_bUpdate );
}

BOOL UpdateSystem::UndoUpdate( void )
{
	Sleep( 3000 );
	m_bUndo = TRUE;
	DWORD dwCurTotalSize = (DWORD)m_BackupAddFiles.size() + (DWORD)m_BackupDelFiles.size()*2;
	DWORD dwCurrent = dwCurTotalSize;
	DWORD dwThisPoint = 0;
	if( m_dwTotalSize > 0 ) dwThisPoint = ( m_dwCurrent * 100 ) / m_dwTotalSize;
	for( int n = 0; n < (int)m_BackupAddFiles.size(); n ++ ) {
		DeleteFile( m_BackupAddFiles[n].c_str() );
		m_dwPoint = dwCurrent-- * dwThisPoint / dwCurTotalSize; // dwCurTotalSize������0
		m_strCurDealFile = PathFindFileName( m_BackupAddFiles[n].c_str() );
		if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_ROLLBACK, m_nOwner, (LPARAM)0 );
	}
	for( int n = 0; n < (int)m_BackupDelFiles.size(); n ++ ) {
		DeleteFile( m_BackupDelFiles[n].strFile.c_str() );
		MyCopyFile( m_BackupDelFiles[n].strFile.c_str(), m_BackupDelFiles[n].strBackFile.c_str() );
		m_dwPoint = dwCurrent-- * dwThisPoint / dwCurTotalSize; // dwCurTotalSize������0
		m_strCurDealFile = PathFindFileName( m_BackupDelFiles[n].strFile.c_str() );
		if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_ROLLBACK, m_nOwner, (LPARAM)0 );
	}
	return( TRUE );
}

//�������ĸ������̣���װ���°汾��
BOOL UpdateSystem::DoUpdate( CString& strErrInfo )
{
	map<string,PackageInfo >::iterator itInfo;
	map<string,PackageOper >::iterator itOper;
	vector<PackageOper > vDeleteFiles;
	vector<PackageOper > vNewFiles;
	char szBuf[1024];

	//����������ܽ���
	m_dwCurrent = 0;
	m_dwTotalSize = (DWORD)m_DeleteFile.size()*2 + 
					(DWORD)m_ReplaceFile.size()*3 +
					(DWORD)m_NewFile.size();

	//����Ҫɾ�����ļ�
	vDeleteFiles.clear();
	vNewFiles.clear();
	for( itOper = m_DeleteFile.begin(); itOper != m_DeleteFile.end(); ++ itOper ) 
	{
		vDeleteFiles.push_back(itOper->second);
		//BackupDeleteFile( itOper->second.strFName );
	}
	if( vDeleteFiles.size() > 0 ) 
	{
		DeleteFiles( vDeleteFiles ); 
		vDeleteFiles.clear();
	}

	//����Ҫ�滻���ļ�
	map<string,PackageOper>::iterator VersionIterator = m_ReplaceFile.end();
	for( itOper = m_ReplaceFile.begin(); itOper != m_ReplaceFile.end(); ++ itOper ) 
	{
		string name = itOper->second.strFName;
		if( itOper->second.strFName == "(version)" ) 
		{ 
			VersionIterator = itOper; 
			continue; 
		}

		vDeleteFiles.push_back( itOper->second );
		vNewFiles.push_back( itOper->second );
		//BackupDeleteFile( itOper->second.strFName );
	}
	if( vDeleteFiles.size() > 0 ) 
	{
		DeleteFiles( vDeleteFiles ); 
		vDeleteFiles.clear();
	}

	if( vNewFiles.size() > 0 ) 
	{
		if( AddFiles( vNewFiles ) == FALSE )
		{
			strErrInfo.LoadString( IDS_US_ERR_FILE_ADD );
			strErrInfo = "\n" + strErrInfo;
			return( FALSE );
		}
		vNewFiles.clear();
	}

	//����Ҫ��ӵ��ļ�
	for( itOper = m_NewFile.begin(); itOper != m_NewFile.end(); ++ itOper )
	{
		string name = itOper->second.strFName;
		if( itOper->second.strFName == "(version)" ) 
			continue;
		vNewFiles.push_back( itOper->second );
		//BackupAddFile( itOper->second.strFName );
	}
	if( vNewFiles.size() > 0 ) 
	{
		if( AddFiles( vNewFiles ) == FALSE ) 
		{
			strErrInfo.LoadString( IDS_US_ERR_FILE_ADD );
			strErrInfo = "\n" + strErrInfo;
			return( FALSE );
		}
		vNewFiles.clear();
	}

	if( VersionIterator != m_ReplaceFile.end() ) 
	{
		if( m_bFullCheck )
		{
			if( m_hNotifyWnd ) ::SendMessage( m_hNotifyWnd, US_CHECK_ALL, m_nOwner, (LPARAM)0 );
			int nErr = CheckALL( VersionIterator->second, strErrInfo );
			if( nErr == 1 )
			{
				strErrInfo += "���������°�װ��Ϸ��";
				return( FALSE );
			}
			else if( nErr == 2 )
			{
				strErrInfo += "���������°�װ��Ϸ��";
				return( FALSE );
			}
			else if( nErr == 3 )
			{
				strErrInfo += "�����ִ��󣬵��Կ�����������Ϸ��";
				//////return( FALSE );
			}
			else if( nErr == 4 )
			{
				strErrInfo += "���ļ����ʧ�ܣ�����������Launch��";
				return( FALSE );
			}
			else if( nErr == 5 )
			{
				strErrInfo += "���������°�װ��Ϸ��";
				return( FALSE );
			}
		}
		PackageOper pp;
		pp.strFName = "(version)";
		pp.strMPQName = "";
		//vDeleteFiles.push_back( "(version)" );
		vDeleteFiles.push_back(pp);
		DeleteFiles( vDeleteFiles );
		vDeleteFiles.clear();
		vNewFiles.push_back( VersionIterator->second );
		AddFiles( vNewFiles ); 
		vNewFiles.clear();
	}
	else
	{
		strErrInfo = CString( "û���ҵ�(version)�ļ���" );
		return( FALSE );
	}

	char Path[256];
	::GetCurrentDirectory(256,Path);
	string strPathPath = Path ;
	strPathPath += "/Patch";
	ClearTempFile(strPathPath,".mpq");	//����PatchĿ¼�е���ʱ�ļ�

	string strNewLaunch = Path ;
	strNewLaunch += "/newlaunch.mpq";
	DeleteFile( strNewLaunch.c_str() );	//����newlauch.mpq��

	m_dwCurrent = m_dwTotalSize;
	m_bUpdate = TRUE;
	return( TRUE );
}


//�Ӱ���ɾ��һ���ļ�
BOOL UpdateSystem::DelFileFromPackage(PackageOper& PackOper)
{
	DWORD R;
	TKPMArchive* pArchive = KPM_OpenArchive(PackOper.strMPQName.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "��Patch�ļ�%sʧ��!\n", PackOper.strMPQName.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	BOOL ret = KPM_RemoveFile(pArchive,PackOper.strFName.c_str());
	if ( !ret )
	{
		char szBuf[1024];
		sprintf( szBuf, "�Ӱ�%s��ɾ���ļ�%sʧ��!\n", PackOper.strMPQName.c_str(),PackOper.strFName.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	KPM_CloseArchive(pArchive);

	char szBuf[1024];
	sprintf( szBuf, "�Ӱ�%s��ɾ���ļ�%s�ɹ�!\n", PackOper.strMPQName.c_str(),PackOper.strFName.c_str() );
	Log( szBuf, (DWORD)strlen( szBuf ) );
	return TRUE;

}//DelFileFromPackage end

//ɾ��version�ļ���������
BOOL UpdateSystem::DelDiscreteFiles( PackageOper& PackOper)
{
	BOOL bFlag = TRUE;

	if( PathFileExists(PackOper.strFName.c_str() ) ) 
	{
		if( SetFileAttributes(PackOper.strFName.c_str(), FILE_ATTRIBUTE_NORMAL ) == FALSE 
				||	DeleteFile(PackOper.strFName.c_str() ) == FALSE )
			bFlag = FALSE;

		if( bFlag == FALSE )
		{
			char szBuf[1024];
			sprintf( szBuf, "û��ɾ�����ļ�%s\n",PackOper.strFName.c_str() );
			Log( szBuf, (DWORD)strlen( szBuf ) );
		}
	}
	else 
	{
		char szBuf[1024];
		sprintf( szBuf, "Ҫɾ�����ļ�%s������\n", PackOper.strFName.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
	}

	char szBuf[1024];
	sprintf( szBuf, "�ɹ�ɾ���ļ�%s\n", PackOper.strFName.c_str() );
	Log( szBuf, (DWORD)strlen( szBuf ) );

	return( TRUE );

}//DelDiscreteFiles end


//vector<string>& vecFiles
//��ʹɾ��ʧ�ܣ�Ҳ���ܷ���FALSE��������һ�������������FALSE���˳�
BOOL UpdateSystem::DeleteFiles( vector<PackageOper>& vecFiles )
{
	for( int n = 0; n < (int)vecFiles.size(); n ++ )
	{
		m_dwCurrent++;
		m_strCurDealFile = PathFindFileName( vecFiles[n].strFName.c_str() );
		if( m_hNotifyWnd )
			::SendMessage( m_hNotifyWnd, US_DELETEFILE, m_nOwner, (LPARAM)0 );


		if ( vecFiles[n].strFName == "(version)" )
		{
			//ɾ��version�ļ�
			DelDiscreteFiles(vecFiles[n]);
		}
		else if ( vecFiles[n].strMPQName == "" )	//ɾ��������
		{	
			//�����ɲ����ʱ��ɾ�����Ķ����Ѿ����ɾ�����������ļ���������ﲻ���汾�ж�
			DelDiscreteFiles(vecFiles[n]);
		}
		else			//�Ӱ���ɾ��һ���ļ�
		{
			if ( LaunchSystem::GetMe()->IsTestVersion() )
			{
				//�ڲ����԰棺ɾ������԰�������ɾ��Ŀ¼�µ���ɢ�ļ�
				TestUpdateSystem testSys;
				testSys.DelFileFromPackage(vecFiles[n].strFName);
			}
			else
			{
				DelFileFromPackage(vecFiles[n]);		
			}

		}

	}//foe end

	return( TRUE );
}

//�����ɢ�ĵ����ļ���Ŀǰֻ��(version)
BOOL UpdateSystem::AddDiscreteFiles( PackageOper& PackOper)
{
	char szFileName[1024];
	strcpy( szFileName, PackOper.strFName.c_str() );
	CreateDirectorys( szFileName );
	HANDLE hFile = CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( hFile == INVALID_HANDLE_VALUE )
	{
		SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL );
		hFile = CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if( hFile == INVALID_HANDLE_VALUE ) 
		{
			char szBuf[1024];
			sprintf( szBuf, "�����ļ�%sʧ��!\n", szFileName );
			Log( szBuf, (DWORD)strlen( szBuf ) );
			return( FALSE );
		}
	}

	DWORD R;
	string  MPQPack = PackOper.strZip;
	TKPMArchive* pArchive = KPM_OpenArchive(PackOper.strZip.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "��Patch�ļ�%sʧ��!\n", PackOper.strZip.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,szFileName,R);
	if ( !pFile )
	{
		char szBuf[1024];
		sprintf( szBuf, "��Patch�ļ�%sʧ��!\n", PackOper.strZip.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}
	
	DWORD FileSize = KPM_GetFileSize(pFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		char szBuf[1024];
		sprintf( szBuf, "�����ڴ����!\n" );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );
	}

	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		char szBuf[1024];
		sprintf( szBuf, "���ļ�����!\n" );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );		
	}
	
	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		char szBuf[1024];
		sprintf( szBuf, "�ļ�%s�𻵣�����������!\n", szFileName );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		return( FALSE );
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	DWORD dwSize = 0;
	BOOL bWrite = WriteFile( hFile, lpFile, FileSize, &dwSize, NULL );
	CloseHandle( hFile );
	delete [] lpFile;
	if( bWrite == FALSE )
	{
		char szBuf[1024];
		sprintf( szBuf, "д�ļ�����!\n" );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );
	}

	char szBuf[1024];
	sprintf( szBuf, "����ļ�%s�ɹ�!\n", szFileName );
	Log( szBuf, (DWORD)strlen( szBuf ) );
	return( TRUE );

}//AddDiscreteFiles end


//��������ĵ�����
BOOL UpdateSystem::AddPackage(PackageOper& PackOper)
{
	//�ɽ���������һ����ɢ���ļ�
	return AddDiscreteFiles(PackOper);

}//AddPackage end


//���һ���ļ�������
UpdateSystem::AddFileToPackage(PackageOper& PackOper)
{
	char szFileName[1024];
	strcpy( szFileName, PackOper.strFName.c_str() );

	CHAR szTemp[1024] = {0};
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	CHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName( szTempPath, itoa( s_Temp++, szTemp, 10 ), MAX_PATH, szTempFile );

	DWORD R;
	string  MPQPack = PackOper.strZip;
	TKPMArchive* pArchive = KPM_OpenArchive(PackOper.strZip.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "��Patch��%sʧ��!\n", PackOper.strZip.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,szFileName,R);
	if ( !pFile )
	{
		char szBuf[1024];
		sprintf( szBuf, "�Ӱ���%s���ļ�%sʧ��!\n", PackOper.strZip.c_str(),szFileName );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	DWORD FileSize = KPM_GetFileSize(pFile);
	
	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		char szBuf[1024];
		sprintf( szBuf, "�����ڴ����!\n" );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );
	}

	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		char szBuf[1024];
		sprintf( szBuf, "�Ӱ���%s���ļ�%sʧ��!\n", PackOper.strZip.c_str(),szFileName );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );		
	}

	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		char szBuf[1024];
		sprintf( szBuf, "����%s�ļ�%sʧ��!\n", PackOper.strZip.c_str(),szFileName );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);
		return( FALSE );
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	FILE* fp = fopen( szTempFile, "wb" );
	if( fp == NULL ) 
	{
		char szBuf[1024];
		sprintf( szBuf, "����ʱ�ļ�%sʧ��!\n",szTempFile );
		Log( szBuf, (DWORD)strlen( szBuf ) );		
		return( FALSE );
	}

	int Write = fwrite(lpFile, 1, FileSize, fp );
	if (Write!=FileSize)
	{
		char szBuf[1024];
		sprintf( szBuf, "д��ʱ�ļ�%sʧ��!\n",szTempFile );
		Log( szBuf, (DWORD)strlen( szBuf ) );	
		return( FALSE );
	}

	if( fclose( fp ) != 0 )
	{
		char szBuf[1024];
		sprintf( szBuf, "�ر���ʱ�ļ�%sʧ��!\n",szTempFile );
		Log( szBuf, (DWORD)strlen( szBuf ) );		
	}

	//����ʱ�ļ�д�����
	pArchive = 	KPM_OpenArchive(PackOper.strMPQName.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "��MQP�ļ�%sʧ��!\n", PackOper.strMPQName.c_str());
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}
	bool bRet = KPM_AddFile(pArchive,szTempFile,szFileName,KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE|KPM_FILE_REPLACEEXISTING,0,FILE_TYPE_DATA);
	if ( bRet )
	{
		char szBuf[1024];
		sprintf( szBuf, "��%s������ļ�%s�ɹ�!\n", PackOper.strMPQName.c_str(),szTempFile);
		Log( szBuf, (DWORD)strlen( szBuf ) );		
	}
	else
	{
		char szBuf[1024];
		sprintf( szBuf, "��%s������ļ�%s���!\n", PackOper.strMPQName.c_str(),szTempFile);
		Log( szBuf, (DWORD)strlen( szBuf ) );		
	}

	KPM_CloseArchive(pArchive);
	DeleteFile( szTempFile );

	return TRUE;

}//AddFileToPackage end

BOOL UpdateSystem::AddFiles( vector<PackageOper>& vecFiles )
{
	char szFileName[1024];

	for( int n = 0; n < (int)vecFiles.size(); n ++ ) 
	{
		PackageOper pp = vecFiles[n];
		m_dwCurrent++;
		m_strCurDealFile = PathFindFileName( vecFiles[n].strFName.c_str() );
		if( m_hNotifyWnd ) 
			::SendMessage( m_hNotifyWnd, US_APPENDFILE, m_nOwner, (LPARAM)0 );

		if ( vecFiles[n].strMPQName == "" )		
		{
			if ( vecFiles[n].strFName == "(version)" )
			{
				if ( AddDiscreteFiles(vecFiles[n]) == FALSE )
				{
					return FALSE;
				}
			}
			else		//����������Ĳ���������°�
			{
				//if ( vecFiles[n].strFName == "NewLaunch.mpq" || vecFiles[n].strFName == "newlaunch.mpq" )
				//{
				//	if ( AddPackage(vecFiles[n]) )		//newlaunch����������������һ��
				//	{
				//		//�����Լ�
				//		if ( UpdateMyself(vecFiles[n]) == FALSE )
				//		{					
				//			return FALSE;
				//		}
				//	}	
				//	else
				//	{	
				//		return FALSE;
				//	}

				//	continue;
				//}

				if ( LaunchSystem::GetMe()->IsTestVersion() )
				{
					//�ڲ����԰�
					TestUpdateSystem testSys;
					if ( testSys.AddPackage(vecFiles[n].strZip,vecFiles[n].strFName) == FALSE )
					{
						return FALSE;
					}
				}
				else
				{
					if ( AddPackage(vecFiles[n]) == FALSE )
					{
						return FALSE;					
					}
				}

			}
		}
		else		//���������ļ�
		{
			if ( LaunchSystem::GetMe()->IsTestVersion() )
			{
				//�ڲ����԰�
				TestUpdateSystem TestSys;
				if ( TestSys.AddFileToPackage(vecFiles[n].strZip,vecFiles[n].strFName) == FALSE )
				{
					return FALSE;
				}
			}
			else
			{
				if ( AddFileToPackage(vecFiles[n]) == FALSE )
				{
					return FALSE;
				}
			}

		}
	}

	return( TRUE );

}


BOOL UpdateSystem::UpdateMyself(PackageOper& PackOper)
{
	DWORD R;
	//��ʱ�ļ�����Ϊ����
	TKPMArchive* pArchive = KPM_OpenArchive(PackOper.strFName.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "��Patch�ļ�%sʧ��!\n", PackOper.strFName.c_str() );
		Log( szBuf, (DWORD)strlen( szBuf ) );
		return( FALSE );			//�޷���Patch�ļ�
	}

	
	FILE_FIND_DATA	sf;
	BOOL bFound		= TRUE;
	TKPMSearch*		hFind = KPM_FindFirstFile(pArchive,"*",&sf);
	
	while(hFind != NULL && bFound != FALSE)
	{
		if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && 
			strcmp(sf.cFileName, SIGNATURE_NAME) != 0 && 
			strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
		{
			char szFileName[1024] = {0};

			//ȥ��·����Ϣ
			string FilePath = sf.cFileName;
			string::size_type Pos = FilePath.find_last_of("/");
			if ( Pos != string::npos )
			{
				//ȥ��·����Ϣ���ʵ������
				FilePath = FilePath.substr(Pos+1,FilePath.size()-Pos);			
			}

			strcpy( szFileName, FilePath.c_str());
			CreateDirectorys( szFileName );
			HANDLE hFile = CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			
			if( hFile == INVALID_HANDLE_VALUE )
			{
				SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL );
				hFile = CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
				if( hFile == INVALID_HANDLE_VALUE ) 
				{
					char szBuf[1024];
					sprintf( szBuf, "�����ļ�%sʧ��!\n", szFileName );
					Log( szBuf, (DWORD)strlen( szBuf ) );
					return( FALSE );
				}
			}

			TKPMFile*	pFile = KPM_FileOpen(pArchive,sf.cFileName,R);
			if ( !pFile )
			{
				char szBuf[1024];
				sprintf( szBuf, "��Patch�ļ�%sʧ��!\n", PackOper.strFName.c_str() );
				Log( szBuf, (DWORD)strlen( szBuf ) );
				return( FALSE );			//�޷���Patch�ļ�
			}
			
			LPSTR lpFile = new CHAR[sf.dwFileSize];
			if( !lpFile ) 
			{
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive);
				char szBuf[1024];
				sprintf( szBuf, "�����ڴ����!\n" );
				Log( szBuf, (DWORD)strlen( szBuf ) );
				return( FALSE );
			}

			DWORD Read;
			KPM_ReadFile(pFile,lpFile,sf.dwFileSize,&Read);
			if( Read != sf.dwFileSize )
			{
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive);
				char szBuf[1024];
				sprintf( szBuf, "���ļ�����!\n" );
				Log( szBuf, (DWORD)strlen( szBuf ) );
				return( FALSE );		
			}

			KPM_FileEof(pFile);	
			KPM_FileClose(pFile);

			DWORD dwSize = 0;
			BOOL bWrite = WriteFile( hFile, lpFile, sf.dwFileSize, &dwSize, NULL );
			CloseHandle( hFile );
			delete [] lpFile;
			if( bWrite == FALSE )
			{
				char szBuf[1024];
				sprintf( szBuf, "д�ļ�����!\n" );
				Log( szBuf, (DWORD)strlen( szBuf ) );
				return( FALSE );
			}
		}

		bFound = KPM_FindNextFile(hFind,&sf);
	}

	char szBuf[1024];
	sprintf( szBuf, "Launch�Ը��³ɹ���\n" );
	Log( szBuf, (DWORD)strlen( szBuf ) );

	KPM_SearchClose(hFind);
	KPM_CloseArchive(pArchive);

	return TRUE;

}//UpdateMyself end


BOOL UpdateSystem::CreateDirectorys( LPSTR szPaths )
{
	char szPath[_MAX_PATH];
	_splitpath( szPaths, NULL, szPath, NULL, NULL );
	if( _access( szPath, 6 ) != 0 ) {
		char* p = &szPath[0];
		if( *p == '/' || *p == '\\' ) p ++;
		while( *p ) {
			if( *p == '/' || *p == '\\' ) {
				*p = 0;
				if( _access( szPath, 6 ) != 0 ) {
					if( errno == ENOENT ) {
						if( _mkdir( szPath ) != 0 ) 
							return( FALSE );
					}
					else if( errno == EACCES ) {
						if( _chmod( szPath, _S_IREAD | _S_IWRITE ) != 0 ) 
							return( FALSE );
					}
				}
				*p = '/';
			}
			p++;
		}
	}

	return( TRUE );
}

INT UpdateSystem::FindFile( LPCSTR szFileString, DWORD dwSize, LPCSTR szFileName, FileInfo& fi )
{
	if( szFileString == NULL || dwSize == 0 || szFileName == NULL ) return( FALSE );
	CHAR* p = (CHAR*)szFileString;
	BYTE Length;
	while( (DWORD)( p - szFileString ) < dwSize ) {
		Length = *p;
		if( strcmp( string( p + 1, Length ).c_str(), szFileName ) == 0 ) {
			CopyMemory( &fi.dwOffset, p + Length + 1, 4 );
			CopyMemory( &fi.dwSize, p + Length + 5, 4 );
			CopyMemory( &fi.dwCRC, p + Length + 9, 4 );
			return( (INT)( p - szFileString ) );
		}
		p += Length;
		p += 13;
	}
	return( -1 );
}

INT UpdateSystem::CalPackOffset( pair<INT, INT>* nValueSizes, INT dwPackCount, DWORD dwOffset, DWORD& dwPackOffset )
{
	if( nValueSizes == NULL || dwPackCount == 0 ) return( -1 );
	INT dwTemp = 0;
	for( INT dw = 0; dw < dwPackCount; dw ++ ) {
		if( dwOffset < (DWORD)( dwTemp + nValueSizes[dw].first ) ) {
			dwPackOffset = dwOffset - dwTemp;
			return( dw );
		}
		dwTemp += nValueSizes[dw].first;
	}
	return( -1 );
}

DWORD UpdateSystem::ReduceHead( LPSTR szHead, DWORD dwSize, DWORD dwOffset )
{
	if( szHead == NULL ) return( 0 );

	CHAR* p1 = szHead + dwOffset;
	BYTE Length = *p1;
	CHAR* p2 = p1 + Length + 13;
	DWORD dwFileSize;

	CopyMemory( &dwFileSize, p1 + Length + 5, 4 );
	CopyMemory( p1, p2, dwSize - ( p2 - szHead ) );
	dwSize = dwSize - Length - 13;
	while( (DWORD)( p1 - szHead ) < dwSize ) {
		Length = *p1 ++;
		p1 += Length;
		DWORD dwTemp;
		CopyMemory( &dwTemp, p1, 4 );
		dwTemp -= dwFileSize;
		CopyMemory( p1, &dwTemp, 4 );
		p1 += 12;
	}
	return( dwSize );
}

INT UpdateSystem::AppendHead( pair<INT, INT>* nSizes, INT dwPackCount, INT nPack, LPCSTR szFileName, 
							 DWORD dwSize, DWORD dwCRC, LPSTR lpHead, DWORD& dwHeadSize )
{
	DWORD dwOffset = 0;
	if( nPack >= dwPackCount ) {
		for( int n = 0; n < dwPackCount; n ++ ) {
			dwOffset += nSizes[n].first;
		}
		char* p = lpHead + dwHeadSize;
		*p = (char)strlen( szFileName ); p ++;
		CopyMemory( p, szFileName, strlen( szFileName ) ); p += strlen( szFileName );
		CopyMemory( p, &dwOffset, 4 ); p += 4;
		CopyMemory( p, &dwSize, 4 ); p += 4;
		CopyMemory( p, &dwCRC, 4 ); p += 4;
		dwHeadSize += (DWORD)strlen( szFileName ) + 13;
	}
	else {
		for( int n = 0; n < nPack + 1; n ++ ) {
			dwOffset += nSizes[n].first;
		}
		char* p = lpHead;
		BOOL bFind = FALSE;
		if( dwHeadSize == 0 ) {
			bFind = TRUE;
			*p = (char)strlen( szFileName ); p ++;
			CopyMemory( p, szFileName, strlen( szFileName ) ); p += strlen( szFileName );
			CopyMemory( p, &dwOffset, 4 ); p += 4;
			CopyMemory( p, &dwSize, 4 ); p += 4;
			CopyMemory( p, &dwCRC, 4 ); p += 4;
			dwHeadSize += (DWORD)strlen( szFileName ) + 13;
		}
		else {
			while( (DWORD)( p - lpHead ) < dwHeadSize ) {
				if( bFind ) {
					char* q = p + *p + 1;
					DWORD dwTemp;
					CopyMemory( &dwTemp, q, 4 );
					dwTemp += dwSize;
					CopyMemory( q, &dwTemp, 4 );
				}
				byte Len = *p ++; // NameLen
				p += Len; // FileName
				//p += ( *p + 1 );
				DWORD dw1, dw2;
				CopyMemory( &dw1, p, 4 ); p += 4; // Offset
				CopyMemory( &dw2, p, 4 ); p += 4; // Size
				p += 4; // CRC
				if( ( dw1 + dw2 ) == dwOffset && bFind == FALSE ) {
					bFind = TRUE;
					DWORD dwBackupSize = dwHeadSize - (DWORD)( p - lpHead );
					char* pTempHead = new char[dwBackupSize];
					CopyMemory( pTempHead, p, dwBackupSize );
					//CopyMemory( p + strlen( szFileName ) + 13, p, (DWORD)( dwHeadSize - ( p - lpHead ) ) );
					*p = (char)strlen( szFileName ); p ++;
					CopyMemory( p, szFileName, strlen( szFileName ) ); p += strlen( szFileName );
					CopyMemory( p, &dwOffset, 4 ); p += 4;
					CopyMemory( p, &dwSize, 4 ); p += 4;
					CopyMemory( p, &dwCRC, 4 ); p += 4;
					CopyMemory( p, pTempHead, dwBackupSize );// p += dwBackupSize;
					dwHeadSize += (DWORD)strlen( szFileName ) + 13;
					delete [] pTempHead;
				}
			}
		}
		if( bFind == FALSE ) {
			return( -1 );
		}
	}
	
	return( nPack );
}

INT UpdateSystem::AppendFile( string strPackage, pair<INT, INT>* nSizes, INT dwPackCount, LPCSTR lpFile, DWORD dwSize )
{
	CHAR szFileName[1024];
	int n;
	for( n = 0; n < dwPackCount; n ++ ) {
		if( (DWORD)( nSizes[n].second - nSizes[n].first ) > ( dwSize + 4 ) ) {
			if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), n + 1 ) == -1 ) return( -1 );
			map<string,BackupPackageDelete >::iterator it = m_PackDeleteFile.find( szFileName );
			if( it != m_PackDeleteFile.end() ) {
				string strPack = it->second.strBackFile;
				if( strPack.empty() ) strPack = szFileName;
				if( MyAppendFile( strPack, lpFile, dwSize ) ) {
					if( it->second.strBackFile.empty() == false ) it->second.bDirty = true;
					return( n );
				}
				else {
					return( -1 );
				}
			}
			else {			
				return( -1 );
			}
		}
		else if( (DWORD)( nSizes[n].first + 4 + dwSize ) < MAX_PACKAGE_SIZE ) {
			if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), n + 1 ) == -1 ) return( -1 );
			map<string,BackupPackageDelete >::iterator it = m_PackDeleteFile.find( szFileName );
			if( it != m_PackDeleteFile.end() ) {
				string strPack = it->second.strBackFile;
				if( strPack.empty() ) strPack = szFileName;
				if( MyAppendFile( strPack, lpFile, dwSize, (DWORD)( ( nSizes[n].first + 4 + dwSize ) - nSizes[n].second ) ) ) {
					if( it->second.strBackFile.empty() == false ) 
						it->second.bDirty = true;
				}
				else {
					return( -1 );
				}
			}
			else {
				return( -1 );
			}
			nSizes[n].second += ( nSizes[n].first + 4 + dwSize ) - nSizes[n].second;
			return( n );
		}
	}


	CHAR szTempFile[MAX_PATH] = {0};
	if( sprintf( szFileName, "%s.p%02d", strPackage.c_str(), n + 1 ) == -1 ||
		sprintf( szTempFile, "%s.%s.bak", szFileName, m_strThisVersion.c_str() ) == -1 )
	{
		return( -1 );
	}

	BackupPackageDelete bi;
	bi.bDirty = true;
	bi.strPackName = szFileName;

	DeleteFile( szTempFile );
	FILE* fp;
	fp = fopen( szTempFile, "a" );
	if( fp == NULL ) {
		if( ( _chmod( strPackage.c_str(), _S_IREAD | _S_IWRITE ) != 0 ) && ( ( fp = fopen( szTempFile, "a" ) ) == 0 ) ) {
			return( -1 );
		}
	}
	char szWrite[4] = {0};
	if( fwrite( szWrite, 1, 4, fp ) != 4 )
	{
		fclose( fp );
		return( -1 );
	}
	fclose( fp );

	if( MyAppendFile( szTempFile, lpFile, dwSize, dwSize ) ) {
		bi.strBackFile = szTempFile;
		m_PackDeleteFile.insert( m_PackDeleteFilePair( bi.strPackName, bi ) );
		m_BackupAddFiles.push_back( bi.strPackName );
	}
	else {
		return( -1 );
	}
	return( n );
}

BOOL UpdateSystem::MyAppendFile( string strPackage, LPCSTR lpFile, DWORD dwSize, DWORD dwAppend )
{
	if( MyAppendFileM( strPackage, lpFile, dwSize, dwAppend ) ) return( TRUE );
	return( MyAppendFileD( strPackage, lpFile, dwSize, dwAppend ) );
}

BOOL UpdateSystem::MyAppendFileM( string& strPackage, LPCSTR lpData, DWORD dwSize, DWORD dwAppend )
{
	if( lpData == NULL || dwSize == 0 ) return( FALSE );

	HANDLE hFile, hFileMapping;
	LPBYTE lpFile;
	hFile = CreateFile( strPackage.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) {
		SetFileAttributes( strPackage.c_str(), FILE_ATTRIBUTE_NORMAL );
		hFile = CreateFile( strPackage.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		if( hFile == INVALID_HANDLE_VALUE ) {
			return( FALSE );
		}
	}
	DWORD dwPakSize = GetFileSize( hFile, NULL );
	DWORD dwValueSize = dwPakSize + dwAppend;
	hFileMapping = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0, dwValueSize, NULL );
	CloseHandle( hFile ); hFile = INVALID_HANDLE_VALUE;
	if( hFileMapping == NULL ) return( FALSE );
	lpFile = (LPBYTE)MapViewOfFile( hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, dwValueSize );
	if( lpFile == NULL ) { CloseHandle( hFileMapping ); return( FALSE ); }

	DWORD dwAvailed = 0;
	if( dwPakSize > 0 ) {
		CopyMemory( &dwAvailed, lpFile, 4 );
	}
	CopyMemory( lpFile + dwAvailed + 4, lpData, dwSize );
	dwAvailed += dwSize;
	CopyMemory( lpFile, &dwAvailed, 4 );

	UnmapViewOfFile( lpFile );
	CloseHandle( hFileMapping );
	return( TRUE );
}

BOOL UpdateSystem::MyAppendFileD( string& strPackage, LPCSTR lpFile, DWORD dwSize, DWORD dwAppend )
{
	if( lpFile == NULL || dwSize == 0 ) return( FALSE );

	FILE* fp;
	fp = fopen( strPackage.c_str(), "a" );
	if( fp == NULL ) {
		if( ( _chmod( strPackage.c_str(), _S_IREAD | _S_IWRITE ) != 0 ) && ( ( fp = fopen( strPackage.c_str(), "a" ) ) == 0 ) ) {
			return( FALSE );
		}
	}
	if( dwAppend > 0 ) 
	{
		if( fwrite( lpFile, 1, dwAppend, fp ) != dwAppend )
		{
			fclose( fp );
			return( FALSE );
		}
	}
	fclose( fp );

	fp = fopen( strPackage.c_str(), "r+b" );
	if( fp == NULL ) {
		if( ( _chmod( strPackage.c_str(), _S_IREAD | _S_IWRITE ) != 0 ) && ( ( fp = fopen( strPackage.c_str(), "r+b" ) ) == 0 ) ) {
			return( FALSE );
		}
	}
	DWORD dwAvailed;

	if( fread( &dwAvailed, 1, 4, fp ) != 4 ||
		fseek( fp, dwAvailed + 4, SEEK_SET ) != 0 ||
		fwrite( lpFile, 1, dwSize, fp ) != dwSize )
	{
		fclose( fp );
		return( FALSE );
	}		
	dwAvailed += dwSize;
	if( fseek( fp, 0, SEEK_SET ) != 0 ||
		fwrite( &dwAvailed, 1, 4, fp ) != 4 )
	{
		fclose( fp );
		return( FALSE );
	}		
	fclose( fp );

	return( TRUE );
}

DWORD UpdateSystem::GetProcess( void )
{
	if( m_dwTotalSize == 0 ) return( 0 );
	if( m_bUndo ) return( m_dwPoint );
	return( ( m_dwCurrent * 100 ) / m_dwTotalSize );
}

LPCSTR UpdateSystem::GetCurrentFileName( void )
{
	return( m_strCurDealFile.c_str() );
}

void UpdateSystem::Log( LPCSTR pData, DWORD dwSize )
{
	char text[2048] = {0};
	sprintf( text,"%s\n",pData);
	
	int len = strlen( text );
	
	if( m_LogFile ) {
		fwrite( text, 1, len, m_LogFile );
	}
}

BOOL UpdateSystem::EndingPackage( string strPackage )
{
	char szBuf[1024];
	sprintf( szBuf, "\t���°�ͷ�ļ�..." );
	Log( szBuf, (DWORD)strlen( szBuf ) );
	string strHead = strPackage + ".nam";
	if( MyCopyFile1M( strHead.c_str(), m_lpThisPackageHead, m_dwHeadSize ) == FALSE ) return( FALSE );
	sprintf( szBuf, "�ɹ�!\n" );
	Log( szBuf, (DWORD)strlen( szBuf ) );

	//�������˵İ��ļ�
	for( map<string,BackupPackageDelete >::iterator it = m_PackDeleteFile.begin(); it != m_PackDeleteFile.end(); ++ it ) {
		if( it->second.strBackFile.empty() == false && it->second.bDirty == true ) {
			sprintf( szBuf, "\t���°��ļ�%s...", PathFindFileName( it->second.strPackName.c_str() ) );
			Log( szBuf, (DWORD)strlen( szBuf ) );
			DeleteFile( it->second.strPackName.c_str() );
			if( rename( it->second.strBackFile.c_str(), it->second.strPackName.c_str() ) != 0 ) {
			//if( MyCopyFile( it->second.strPackName, it->second.strBackFile ) == FALSE ) {
				char szBuf[1024];
				sprintf( szBuf, "�������ļ�%s��%sʧ��!\n", it->second.strBackFile, it->second.strPackName );
				Log( szBuf, (DWORD)strlen( szBuf ) );
			}
			sprintf( szBuf, "�ɹ�!\n" );
			Log( szBuf, (DWORD)strlen( szBuf ) );
		}
		else if( it->second.strBackFile.empty() == false ) {
			DeleteFile( it->second.strBackFile.c_str() );
		}
		m_dwCurrent ++;
	}

	//��������ļ�
	for( map<string,BackupPackageDelete >::iterator it = m_PackDeleteFile.begin(); it != m_PackDeleteFile.end(); ++ it ) {
		if( it->second.strBackFile.empty() == false ) {
			DeleteFile( it->second.strBackFile.c_str() );
		}
		m_dwCurrent ++;
	}

	m_PackDeleteFile.clear();
	m_PackFileMap.clear();

	delete [] m_lpPackageSize; m_lpPackageSize = 0;
	delete [] m_lpThisPackageHead; m_lpThisPackageHead = 0;

	return( TRUE );
}

void UpdateSystem::ConvertVer(const string& strStringVer, string& strComVer)
{
	INT nVer[4];
	char *pStart = NULL, *pDot = NULL;
	string strExt;

	TCHAR szTemp[MAX_PATH] = {0};
	_snprintf(szTemp, MAX_PATH, "%s", strStringVer.c_str());
	nVer[0] = nVer[1] = nVer[2] = nVer[3] = -1;

	pStart = szTemp;
	int nSecNum = 0;

	int i = 0;
	for(i=0; i<4; i++)
	{
		if(*pStart == 0)  break;
		if(i==3)
		{
			nVer[3] = atoi(pStart);
			nSecNum++;
			break;
		}
		pDot = strchr(pStart, '.');
		if(pDot == NULL) 
		{ 
			nVer[i] = atoi(pStart); 
			nSecNum++;
			break; 
		}
		*pDot = 0;
		nVer[i] = atoi(pStart);
		nSecNum++;
		pStart = pDot+1;
	}

	string strRet;
	CHAR szNode[MAX_PATH] = {0};
	for(i=0; i<4; i++)
	{
		if(i<4-nSecNum)
		{
			_snprintf(szNode, MAX_PATH, "%010d", 0);
		}
		else
		{
			_snprintf(szNode, MAX_PATH, "%010d", nVer[i-(4-nSecNum)]);
		}
		strRet += szNode;
	}
	if( strStringVer.size() > 0 )
	{
		if( strStringVer[strStringVer.size()-1] >= 'a' &&
			strStringVer[strStringVer.size()-1] <= 'z' )
			strExt = strStringVer[strStringVer.size()-1];
		if( strStringVer[strStringVer.size()-1] >= 'A' &&
			strStringVer[strStringVer.size()-1] <= 'Z' )
			strExt = strStringVer[strStringVer.size()-1];
	}
	if( strExt.size() > 0 ) strRet += strExt;

	strComVer = strRet;	
}

//1.(version)�ļ�����ֻ�����°�װ��
//2.zip�ļ��������
//3.һЩ�ļ�У����󣬵��ǲ�Ӱ����Ϸ
//4.һЩ�ļ�У�����Ӱ��������Ϸ
//5.��������
int UpdateSystem::CheckALL(PackageOper& Version, CString& strErrInfo)
{
	AfxMessageBox("CheckALL");
	return( 0 );
}

void UpdateSystem::ClearMyself()
{
	m_DeleteFile.clear();
	m_ReplaceFile.clear();
	m_NewFile.clear();
	m_bUpdate = FALSE;
}


