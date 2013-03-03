#include "StdAfx.h"
#include ".\filecollector.h"
#include "CRC.h"
#include "AxCryptoMath.h"
#include "XZip.h"
#include <algorithm>
#include <mbstring.h>

CFileCollector::CFileCollector(UINT nMeasureSize)
{
	m_hTerimateHandle = ::CreateEvent(0, FALSE, FALSE, 0);
	::ResetEvent(m_hTerimateHandle);
	m_vFileBuf.reserve(nMeasureSize);
}

CFileCollector::~CFileCollector(void)
{
	::CloseHandle(m_hTerimateHandle);
	m_hTerimateHandle = 0;
}

bool CFileCollector::CollectPath(const char* szPath, const char* szVersionFile, CString& strVersion, CAxCryptoMath* pKeyMath)
{
	assert(szVersionFile && pKeyMath);
	m_vFileBuf.clear();
	strVersion = "";

	//-----------------------------------------
	//生成临时文件名
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);

	CHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, "pw", MAX_PATH, szTempFile);

	//-----------------------------------------
	//分析文件是否合法
	if(S_OK != pKeyMath->UnSignFile(szVersionFile, szTempFile, FALSE, TRUE))
	{
		DeleteFile(szTempFile);
		return false;
	}

	//-----------------------------------------
	//读版本号
	FILE* fp = fopen(szTempFile, "r");
	if(fp == 0) return false;

	CHAR szLine[1024] = {0};
	fgets(szLine, 1024, fp);

	//版本号
	char* pDot = strchr(szLine, '|');
	if(!pDot) return false;
	*pDot = 0;
	strVersion = szLine;

	//节点个数
	int nNodeNumber = atoi(pDot+1);

	//-----------------------------------------
	//分析文件
	m_vFileBuf.reserve(nNodeNumber);
	for(int i=0; i<nNodeNumber; i++)
	{
		fgets(szLine, 1024, fp);

		FileNode newNode;
		
		char* pDot1 = strchr(szLine, '|');
		if(!pDot1) continue;
		*pDot1++ = 0;
		if(*pDot1 == 'x')
		{
			newNode.bFold = true;
			newNode.nCRC = newNode.nSize = 0;
			m_vFileBuf.push_back(newNode);
			continue;
		}

		char* pDot2 = strchr(pDot1, '|');
		if(!pDot2) continue; 
		*pDot2++ = 0;

		char* pDot3 = strchr(pDot2, '|');
		if(!pDot3) continue; 
		*pDot3++ = 0;
		if( strchr( pDot3, 10 ) ) *strchr( pDot3, 10 ) = 0;

		sscanf(pDot1, "%08x", &newNode.nSize);
		sscanf(pDot2, "%08x", &newNode.nCRC);
		newNode.strPackName = pDot3;
		newNode.bFold = false;

		if( newNode.strPackName.empty() ) {
			newNode.strPatchName = newNode.strFileName = szLine;
		}
		else {
			char szPName[1024];
			strcpy( szPName, szLine );
			_mbslwr( (byte*)&szPName[0] );
			char* pDot = strstr( szPName, "data\\pack" );
			if( pDot ) { pDot[9] = 0; pDot += 10; }
			else pDot = szPName;
			newNode.strPatchName = szPName;
			newNode.strFileName = pDot;
		}
		m_vFileBuf.push_back(newNode);
	}

	DeleteFile(szTempFile);

	m_strPath = szPath;
	m_strVersion = (LPCTSTR)strVersion;
	return true;
}

bool CFileCollector::CollectPath(const char* szPath)
{
	assert(szPath);
	m_vFileBuf.clear();

	::SendMessage(m_hProgressWnd, WM_PROGRESS_TITLE, (WPARAM)"分析目录...", 0);

	m_strPath = szPath;
	if(!_CollectPath(szPath, "", m_vFileBuf)) return false;

	return true;
}

bool CFileCollector::_CollectPath(const char* szPath, const char* szRelPath, std::vector< FileNode >& vFileBuf)
{
	bool PackPath = false;
	WIN32_FIND_DATA FindData;
	
	CHAR szFileName[MAX_PATH] = {0};
	strncpy(szFileName, szPath, MAX_PATH);
	if( stricmp( szRelPath, "data\\pack" ) == 0 ) {
		PathAppend(szFileName, "*.nam");
		PackPath = true;
	}
	else {
		PathAppend(szFileName, "*.*");
	}

	HANDLE hFind = FindFirstFile(szFileName, &FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return true;
	}
	BOOL bFind = FALSE;
	::SendMessage(m_hProgressWnd, WM_PROGRESS_COLLFILE, (WPARAM)szPath, 0);

	int nPathLen = (int)m_strPath.size();
	do
	{
		// check terminate flag
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hTerimateHandle, 0))
		{
			FindClose(hFind);
			return false;
		}

		CHAR szPathFileName[MAX_PATH];
		strncpy(szPathFileName, szPath, MAX_PATH);
		PathAppend(szPathFileName, FindData.cFileName);

		if(FindData.cFileName[0] != '.')
		{
			FileNode newNode;
			newNode.strPatchName = newNode.strFileName = szPathFileName+nPathLen+1;
			newNode.bFold = false;
			newNode.nCRC = 0;
			newNode.nSize = 0;

			//文件夹
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				newNode.bFold = true;
				if(!_CollectPath(szPathFileName, newNode.strPatchName.c_str(), vFileBuf))
				{
					FindClose(hFind);
					return false;
				}
			}
			//文件
			else
			{
				newNode.bFold = false;
				if( PackPath ) {
					char szPackName[260];
					char szPackPath[260];
					_splitpath( newNode.strFileName.c_str(), 0, szPackPath, szPackName, 0 );
					newNode.strPackName = std::string(szPackName);
					FILE* fp = fopen( szPathFileName, "rb" );
					if( !fp ) return false;
					DWORD dwHeadSize;
					fread( &dwHeadSize, 1, 4, fp );
					char* lpszHead = new char[dwHeadSize];
					fread( lpszHead, 1, dwHeadSize, fp );
					fclose( fp );
					char* p = lpszHead;
					BYTE Length;
					while( (DWORD)( p - lpszHead ) < dwHeadSize ) {
						Length = *p++;
						char szName[_MAX_PATH] = {0};
						CopyMemory( szName, p, Length ); p += Length; 
						newNode.strPatchName = szPackPath;
						newNode.strFileName = std::string( szPackPath ) + std::string( szName );
						p += 4; // Offset;
						CopyMemory( &newNode.nSize, p, 4 ); p += 4;
						CopyMemory( &newNode.nCRC, p, 4 ); p += 4;
						vFileBuf.push_back(newNode);
					}
					delete [] lpszHead;
				}
				else {
					DWORD dwCRC, dwFileSize;

CHECK_DATA:
					if(NO_ERROR != FileCrc32Win32(szPathFileName, dwCRC, dwFileSize))
					{
						char szMsg[MAX_PATH];
						_snprintf(szMsg, MAX_PATH, "无法打开文件:%s", szPathFileName);
						
						INT nSel = MessageBox(NULL, szMsg, "ERROR", MB_ABORTRETRYIGNORE|MB_ICONSTOP);

						if(IDABORT == nSel)
						{
							FindClose(hFind);
							return false;
						}
						else if(IDRETRY == nSel)
						{
							goto CHECK_DATA;
						}
						else
						{
							// Find Next file.
							bFind = FindNextFile(hFind, &FindData);
							continue;
						}
					}

					newNode.nCRC = dwCRC;
					newNode.nSize = dwFileSize;
					vFileBuf.push_back(newNode); //不push文件夹了
				}
			}
		}
		// Find Next file.
		bFind = FindNextFile(hFind, &FindData);
	}while(bFind);

	FindClose(hFind);
	return true;
}

void CFileCollector::OutputVersionFile(const char* szFileName, const char* szVersion, CAxCryptoMath* pKeyMath)
{
	assert(szFileName && szVersion && pKeyMath);

	//---------------------------------------------
	//生成临时文件名
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);

	CHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, "pw", MAX_PATH, szTempFile);

	//---------------------------------------------
	//首先保存到临时文件
	FILE* fp = fopen(szTempFile, "w");
	if(fp == 0) 
	{
		char szMsg[MAX_PATH];
		_snprintf(szMsg, MAX_PATH, "保存文件%s失败！", szTempFile);
		MessageBox(NULL, szMsg, "ERROR", MB_OK|MB_ICONSTOP);
		return;
	}

	fprintf(fp, "%s|%d\n", szVersion, (int)m_vFileBuf.size());

	//相对路径的长度
	for(int i=0; i<(int)m_vFileBuf.size(); i++)
	{
		const FileNode& theNode = m_vFileBuf[i];
		if(theNode.bFold)
		{
			fprintf(fp, "%s|x|x\n", theNode.strFileName.c_str(), theNode.nSize, theNode.nCRC);
		}
		else
		{
			fprintf(fp, "%s|%08x|%08x|%s\n", theNode.strFileName.c_str(), theNode.nSize, 
				theNode.nCRC, theNode.strPackName.c_str());
		}
	}
	fclose(fp);

	//---------------------------------------------
	//加密
	if(S_OK != pKeyMath->SignFile(szTempFile, szFileName, FALSE, TRUE))
	{
		char szMsg[MAX_PATH];
		_snprintf(szMsg, MAX_PATH, "保存文件%s失败！", szFileName);
		MessageBox(NULL, szMsg, "ERROR", MB_OK|MB_ICONSTOP);
	}

	m_strVersion = szVersion;
}

void CFileCollector::ClearWorlk(void)
{
	m_vFileBuf.clear();
	::ResetEvent(m_hTerimateHandle);
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
CPatchMaker::CPatchMaker(const CFileCollector& oldVerData, const CFileCollector& newVerData, const char* szPatchPath) : 
	m_oldVerData(oldVerData),
	m_newVerData(newVerData),
	m_strPatchPath(szPatchPath)
{
	m_hTerimateHandle = ::CreateEvent(0, FALSE, FALSE, 0);
	::ResetEvent(m_hTerimateHandle);

	m_hProgressWnd = 0;
}

CPatchMaker::~CPatchMaker()
{
	::CloseHandle(m_hTerimateHandle);
	m_hTerimateHandle = 0;
}

void CPatchMaker::GeneratePatchData(std::string strFilter)
{
	GenerateFilter( strFilter );

	//生成就版本比较数据map
	CompareMap oldVerMap;
	_generateCompareMap(m_oldVerData.GetFileNode(), oldVerMap);

	CompareMap newVerMap;
	_generateCompareMap(m_newVerData.GetFileNode(), newVerMap);

	m_vNewFile.clear();
	m_vDeleteFile.clear();

	//生成需要更新的文件

	for(int i=0; i<(int)m_newVerData.GetFileNode().size(); i++)
	{
		const FileNode& thisNode = m_newVerData.GetFileNode()[i];

		//转化文件名为小写
		char szTemp[1024] = {0};
		strncpy(szTemp, thisNode.strFileName.c_str(), 1024);
		_mbslwr((byte*)&szTemp[0]);

		if( FileNameMatch( szTemp ) ) {
			CompareMap::iterator itFind = oldVerMap.find(szTemp);
			if(itFind != oldVerMap.end()) {
				itFind->second.bMasked = true;
			}
			continue;//过滤掉
		}

		//在旧版本中寻找相同文件
		CompareMap::iterator itFind = oldVerMap.find(szTemp);
		if(itFind == oldVerMap.end())
		{
			//新添加的文件
			m_vNewFile.push_back(thisNode);
			continue;
		}

		//比较两个文件
		itFind->second.bMasked = true;
		const FileNode& theOldNode = *(itFind->second.pFileNode);
		if(theOldNode.nCRC != thisNode.nCRC || theOldNode.nSize != thisNode.nSize)
		{
			//需要更新的文件
			m_vReplaceFile.push_back(thisNode);

			//如果发生 文件<->目录 的变化，需要删除原文件
			if(theOldNode.nCRC==0 || thisNode.nCRC==0)
			{
				m_vDeleteFile.push_back(theOldNode);
			}
		}
	}

	//加入版本定义文件
	FileNode verNode;
	verNode.bFold = false;
	verNode.strPatchName = verNode.strFileName = "(version)";
	m_vReplaceFile.push_back(verNode);

	//生成需要删除的文件
	CompareMap::iterator itOld;
	for(itOld = oldVerMap.begin(); itOld != oldVerMap.end(); itOld++)
	{
		if(itOld->second.bMasked) continue;

		//尚未标记的文件，需要删除
		m_vDeleteFile.push_back(*(itOld->second.pFileNode));
	}

}

void CPatchMaker::_generateCompareMap(const std::vector< FileNode >& fileData, CompareMap& compareMap)
{
	for(int i=0; i<(int)fileData.size(); i++)
	{
		const FileNode& thisNode = fileData[i];
		CompareNode newNode;

		//转化文件名为小写
		char szTemp[1024] = {0};
		strncpy(szTemp, thisNode.strFileName.c_str(), 1024);
		_mbslwr((byte*)&szTemp[0]);

		newNode.pFileNode = &thisNode;
		newNode.bMasked = false;
		compareMap.insert(std::make_pair(szTemp, newNode));
	}
}

bool CPatchMaker::WritePatchFile(CAxCryptoMath* pKeyMath)
{
	assert(pKeyMath);

	::SendMessage(m_hProgressWnd, WM_PROGRESS_TITLE, (WPARAM)"压缩文件...", 0);

	//--------------------------------------------------------------
	//生成命令文件
	char szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);
	char szTempFileName[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, "pm", MAX_PATH, szTempFileName);

	FILE* fp = fopen(szTempFileName, "w");
	if(fp == 0)
	{
		char szMsg[MAX_PATH] = {0};
		_snprintf(szMsg, MAX_PATH, "写临时文件%s出错!", szTempFileName);
		AfxGetApp()->GetMainWnd()->MessageBox(szMsg, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	// add by tangpeng
	_dispatchFileName( m_vDeleteFile );
	_writeCommandFile( fp, "del" );
	_dispatchFileName( m_vReplaceFile );
	_writeCommandFile( fp, "replace" );
	_dispatchFileName( m_vNewFile );
	_writeCommandFile( fp, "new" );
	fprintf(fp, "frpl (version)\n");

	//输出需要删除的文件
	//for(int i=0; i<(int)m_vDeleteFile.size(); i++)
	//{
	//	fprintf(fp, "-%s\n", m_vDeleteFile[i].strFileName.c_str());
	//}

	//输出需要更改的文件
	//for(int i=0; i<(int)m_vNewFile.size(); i++)
	//{
	//	fprintf(fp, "+%s\n", m_vNewFile[i].strFileName.c_str());
	//}
	fclose(fp); fp=0;

	//--------------------------------------------------------------
	//加密命令文件
	char szTempCmdFile[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, "pmk", MAX_PATH, szTempCmdFile);
	pKeyMath->SignFile(szTempFileName, szTempCmdFile, FALSE, TRUE);
	DeleteFile(szTempFileName);
	
	//--------------------------------------------------------------
	//生成Patch文件名
	char szPatchFileName[MAX_PATH];
	_snprintf(szPatchFileName, MAX_PATH, "tlbb-%s-%s.zip", 
		m_oldVerData.GetVersion().c_str(), m_newVerData.GetVersion().c_str());

	char szPatchPathFileName[MAX_PATH] = {0};
	strncpy(szPatchPathFileName, m_strPatchPath.c_str(), MAX_PATH);
	PathAppend(szPatchPathFileName, szPatchFileName);
	
	m_strPatchFile = szPatchPathFileName;

	//创建PatchZip
	HZIP hz = CreateZip((void *)szPatchPathFileName, 0, ZIP_FILENAME);
	if(!hz) 
	{
		char szMsg[MAX_PATH] = {0};
		_snprintf(szMsg, MAX_PATH, "创建%s出错!", szTempFileName);
		AfxGetApp()->GetMainWnd()->MessageBox(szMsg, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	//加入命令文件
	ZipAdd(hz, "(command)", (void *)szTempCmdFile, 0, ZIP_FILENAME);

	CHAR szOrgFile[MAX_PATH] = {0};
	//加入需要更改的文件
	for(int i=0; i<(int)m_vReplaceFile.size(); i++)
	{
		// check terminate flag
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hTerimateHandle, 0))
		{
			CloseZip(hz);
			DeleteFile(szTempCmdFile);
			DeleteFile(szPatchPathFileName);
			return false;
		}

		const FileNode& thisNode = m_vReplaceFile[i];
		if(thisNode.bFold) continue;

		::SendMessage(m_hProgressWnd, WM_PROGRESS_ZIPFILE, (WPARAM)thisNode.strFileName.c_str(), 0);

		if( thisNode.strPackName.empty() ) {
			strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
			PathAppend(szOrgFile, thisNode.strFileName.c_str());
			ZipAdd(hz, thisNode.strPatchName.c_str(), (void*)szOrgFile, 0, ZIP_FILENAME);
		}
		else {
			DWORD dwSize = 0;
			strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
			PathAppend(szOrgFile, thisNode.strPatchName.c_str());
			PathAppend(szOrgFile, thisNode.strPackName.c_str());
			char szPackFileName[MAX_PATH] = {0};
			strncpy( szPackFileName, szOrgFile, MAX_PATH );
			strcat( szPackFileName, ".nam" );
			FILE* fp = fopen( szPackFileName, "rb" );
			if( !fp ) continue;
			fread( &dwSize, 1, 4, fp );
			char* pData = new char[dwSize];
			char* p = pData;
			fread( pData, 1, dwSize, fp );
			fclose( fp );
			int nOffset = -1;
			DWORD dwFileSize = 0;
			while( (DWORD)( p - pData ) < dwSize ) {
				if( ( *p == thisNode.strFileName.length() ) && ( strnicmp( p + 1, thisNode.strFileName.c_str(), *p ) == 0 ) ) {
					p += *p + 1;
					CopyMemory( &nOffset, p, 4 ); p += 4;
					CopyMemory( &dwFileSize, p, 4 );
					break;
				}
				p += *p + 1;
				p += 12;
			}
			delete [] pData;
			pData = 0;
			if( nOffset == -1 ) continue;
			dwSize = 0;
			for( int i = 1; i < 100; i ++ ) {
				sprintf( szPackFileName, "%s.p%02d", szOrgFile, i );
				fp = fopen( szPackFileName, "rb" );
				if( !fp ) continue;
				DWORD dw;
				fread( &dw, 1, 4, fp );
				if( (DWORD)nOffset < dwSize + dw ) {
					fseek( fp, nOffset - dwSize + 4, SEEK_SET );
					pData = new char[dwFileSize];
					fread( pData, 1, dwFileSize, fp );
					fclose( fp );
					break;
				}
				fclose( fp );
				dwSize += dw;
			}
			if( !pData ) continue;
			ZRESULT zs = ZipAdd(hz, ( thisNode.strPatchName + "/" + thisNode.strFileName ).c_str(), (void*)pData, dwFileSize, ZIP_MEMORY);
			delete [] pData; pData = 0;
			if( zs != ZR_OK )
				return( false );
		}
	}
	for(int i=0; i<(int)m_vNewFile.size(); i++)
	{
		// check terminate flag
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hTerimateHandle, 0))
		{
			CloseZip(hz);
			DeleteFile(szTempCmdFile);
			DeleteFile(szPatchPathFileName);
			return false;
		}

		const FileNode& thisNode = m_vNewFile[i];
		if(thisNode.bFold) continue;

		::SendMessage(m_hProgressWnd, WM_PROGRESS_ZIPFILE, (WPARAM)thisNode.strFileName.c_str(), 0);

		if( thisNode.strPackName.empty() ) {
			strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
			PathAppend(szOrgFile, thisNode.strFileName.c_str());
			ZipAdd(hz, thisNode.strPatchName.c_str(), (void*)szOrgFile, 0, ZIP_FILENAME);
		}
		else {
			DWORD dwSize = 0;
			strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
			PathAppend(szOrgFile, thisNode.strPatchName.c_str());
			PathAppend(szOrgFile, thisNode.strPackName.c_str());
			char szPackFileName[MAX_PATH] = {0};
			strncpy( szPackFileName, szOrgFile, MAX_PATH );
			strcat( szPackFileName, ".nam" );
			FILE* fp = fopen( szPackFileName, "rb" );
			if( !fp ) continue;
			fread( &dwSize, 1, 4, fp );
			char* pData = new char[dwSize];
			char* p = pData;
			fread( pData, 1, dwSize, fp );
			fclose( fp );
			int nOffset = -1;
			DWORD dwFileSize = 0;
			while( (DWORD)( p - pData ) < dwSize ) {
				if( ( *p == thisNode.strFileName.length() ) && ( strnicmp( p + 1, thisNode.strFileName.c_str(), *p ) == 0 ) ) {
					p += *p + 1;
					CopyMemory( &nOffset, p, 4 ); p += 4;
					CopyMemory( &dwFileSize, p, 4 );
					break;
				}
				p += *p + 1;
				p += 12;
			}
			delete [] pData;
			pData = 0;
			if( nOffset == -1 ) continue;
			dwSize = 0;
			for( int i = 1; i < 100; i ++ ) {
				sprintf( szPackFileName, "%s.p%02d", szOrgFile, i );
				fp = fopen( szPackFileName, "rb" );
				if( !fp ) continue;
				DWORD dw;
				fread( &dw, 1, 4, fp );
				if( (DWORD)nOffset < dwSize + dw ) {
					fseek( fp, nOffset - dwSize + 4, SEEK_SET );
					pData = new char[dwFileSize];
					fread( pData, 1, dwFileSize, fp );
					fclose( fp );
					break;
				}
				fclose( fp );
				dwSize += dw;
			}
			if( !pData ) continue;
			ZRESULT zs = ZipAdd(hz, ( thisNode.strPatchName + "/" + thisNode.strFileName ).c_str(), (void*)pData, dwFileSize, ZIP_MEMORY);
			delete [] pData; pData = 0;
			if( zs != ZR_OK )
				return( false );
		}
	}
	CloseZip(hz);

	DeleteFile(szTempCmdFile);

	///////////////////////////////////////////////////////////////
	// add crc for zip file, 2006.05.10 by tangpeng

	DWORD dwCRC, dwFileSize;
	if(NO_ERROR == FileCrc32Win32(szPatchPathFileName, dwCRC, dwFileSize))
	{
		fp = fopen( szPatchPathFileName, "a" );
		if( !fp ) return false;
		fwrite( &dwCRC, 1, 4, fp );
		fclose( fp );
	}
	else 
		return false;
	///////////////////////////////////////////////////////////////
	return true;
}

void CPatchMaker::_dispatchFileName( std::vector< FileNode >& fileData )
{
	m_vModel.clear(); 
	m_vMaterial.clear(); 
	m_vEffect.clear();
	m_vInterface.clear();
	m_vSound.clear(); 
	m_vScene.clear(); 
	m_vBrushes.clear(); 
	m_vNormal.clear(); 
	m_vConfig.clear();

	for(int i=0; i<(int)fileData.size(); i++)
	{
		FileNode& fi = fileData[i];
		if( stricmp( fi.strPackName.c_str(), "Model" ) == 0 ) {
			m_vModel.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Material" ) == 0 ) {
			m_vMaterial.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Effect" ) == 0 ) {
			m_vEffect.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Interface" ) == 0 ) {
			m_vInterface.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Sound" ) == 0 ) {
			m_vSound.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Scene" ) == 0 ) {
			m_vScene.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Brushes" ) == 0 ) {
			m_vBrushes.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else if( stricmp( fi.strPackName.c_str(), "Config" ) == 0 ) {
			m_vConfig.push_back( fi.strPatchName + "/" + fi.strFileName );
		}
		else {
			m_vNormal.push_back( fi.strFileName );
		}
		continue;
	}
}

void CPatchMaker::_writeCommandFile( FILE* fp, std::string strCommand )
{
/*
pdel fname from pname  从pname包中删除一个叫fname的文件
prpl fname from pname  从pname包中替换一个叫fname的文件
pnew fname from pname  在pname包中增加一个叫fname的文件
fdel fname 删除一个fname文件
frpl fname 替换一个fname文件
fnew fname 增加一个fname文件
padd 增加一个新包(目前没有支持)
pdel 删除一个包(目前没有支持)
*/
	if( !fp ) return;
	for(int i=0; i<(int)m_vModel.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Model\n", m_vModel[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Model\n", m_vModel[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Model\n", m_vModel[i].c_str());
	}
	for(int i=0; i<(int)m_vMaterial.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Material\n", m_vMaterial[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Material\n", m_vMaterial[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Material\n", m_vMaterial[i].c_str());
	}
	for(int i=0; i<(int)m_vEffect.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Effect\n", m_vEffect[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Effect\n", m_vEffect[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Effect\n", m_vEffect[i].c_str());
	}
	for(int i=0; i<(int)m_vInterface.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Interface\n", m_vInterface[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Interface\n", m_vInterface[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Interface\n", m_vInterface[i].c_str());
	}
	for(int i=0; i<(int)m_vSound.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Sound\n", m_vSound[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Sound\n", m_vSound[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Sound\n", m_vSound[i].c_str());
	}
	for(int i=0; i<(int)m_vScene.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Scene\n", m_vScene[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Scene\n", m_vScene[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Scene\n", m_vScene[i].c_str());
	}
	for(int i=0; i<(int)m_vBrushes.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Brushes\n", m_vBrushes[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Brushes\n", m_vBrushes[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Brushes\n", m_vBrushes[i].c_str());
	}
	for(int i=0; i<(int)m_vConfig.size(); i++)
	{
		if( strCommand == "del" )
			fprintf(fp, "pdel %s from Config\n", m_vConfig[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "prpl %s from Config\n", m_vConfig[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "pnew %s from Config\n", m_vConfig[i].c_str());
	}
	for(int i=0; i<(int)m_vNormal.size(); i++)
	{
		if( m_vNormal[i] == "(version)" ) continue;
		if( strCommand == "del" )
			fprintf(fp, "fdel %s\n", m_vNormal[i].c_str());
		else if( strCommand == "replace" )
			fprintf(fp, "frpl %s\n", m_vNormal[i].c_str());
		else if( strCommand == "new" )
			fprintf(fp, "fnew %s\n", m_vNormal[i].c_str());
	}
}

void CPatchMaker::toLowerCase( std::string& str )
{
	if( str.size() > 0 ) {
		byte* tmp = new byte[str.size()+1];
		memcpy( tmp, str.c_str(), str.size() );
		tmp[str.size()] = 0;
		_mbslwr((byte*)tmp);
		str = (char*)tmp;
		delete [] tmp;
	}
}

bool CPatchMaker::StringMatch( std::string str, std::string pattern, bool caseSensitive )
{
    std::string tmpStr = str;
	std::string tmpPattern = pattern;
    if (!caseSensitive)
    {
        toLowerCase(tmpStr);
        toLowerCase(tmpPattern);
    }

    std::string::const_iterator strIt = tmpStr.begin();
    std::string::const_iterator patIt = tmpPattern.begin();
	std::string::const_iterator lastWildCardIt = tmpPattern.end();
    while (strIt != tmpStr.end() && patIt != tmpPattern.end())
    {
        if (*patIt == '*')
        {
			lastWildCardIt = patIt;
            // Skip over looking for next character
            ++patIt;
            if (patIt == tmpPattern.end())
			{
				// Skip right to the end since * matches the entire rest of the string
				strIt = tmpStr.end();
			}
			else
            {
				// scan until we find next pattern character
                while(strIt != tmpStr.end() && *strIt != *patIt)
                    ++strIt;
            }
        }
        else
        {
            if (*patIt != *strIt)
            {
				if (lastWildCardIt != tmpPattern.end())
				{
					// The last wildcard can match this incorrect sequence
					// rewind pattern to wildcard and keep searching
					patIt = lastWildCardIt;
					lastWildCardIt = tmpPattern.end();
				}
				else
				{
					// no wildwards left
					return false;
				}
            }
            else
            {
                ++patIt;
                ++strIt;
            }
        }

    }
	// If we reached the end of both the pattern and the string, we succeeded
	if (patIt == tmpPattern.end() && strIt == tmpStr.end())
	{
        return true;
	}
	else
	{
		return false;
	}
}

void CPatchMaker::GenerateFilter( std::string filter )
{
	m_vFilter.clear();
	if( filter.size() == 0 ) return;

	while( true )
	{
		size_t ret = filter.find( "\r\n" );
		if( ret != -1 )
		{
			std::string strTemp = filter.substr( 0, ret );
			filter = filter.substr( ret + 2 );
			m_vFilter.push_back( strTemp );
		}
		else
		{
			break;
		}
	}
	if( filter.empty() == false ) m_vFilter.push_back( filter );
}

bool CPatchMaker::FileNameMatch( std::string fname )
{
	if( fname.size() == 0 ) return false;
	for( int i = 0; i < (int)m_vFilter.size(); i ++ ) {
		if( StringMatch( fname, m_vFilter[i], false ) ) {
			return true;
		}
	}
	return false;
}