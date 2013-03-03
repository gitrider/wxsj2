#include "StdAfx.h"
#include ".\filecollector.h"
#include <algorithm>
#include <mbstring.h>
#include "kpmLib.h"

//HashTable������
#define MIN_HASH_TABLE_SIZE	100
#define MAX_HASH_TABLE_SIZE	30000


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

bool CFileCollector::CollectPath(const char* szPath, const char* szVersionFile, CString& strVersion)
{
	m_vFileBuf.clear();
	strVersion = "";

	//-----------------------------------------
	//������ʱ�ļ���
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);

	CHAR szTempFile[MAX_PATH] = {0};
	memcpy(szTempFile,szVersionFile,strlen(szVersionFile));

	//-----------------------------------------
	//���汾��
	FILE* fp = fopen(szTempFile, "r");
	if(fp == 0)
		return false;

	CHAR szLine[1024] = {0};
	fgets(szLine, 1024, fp);

	//�汾��
	char* pDot = strchr(szLine, '|');
	if(!pDot) 
		return false;
	*pDot = 0;
	strVersion = szLine;

	//�ڵ����
	int nNodeNumber = atoi(pDot+1);

	//-----------------------------------------
	//�����ļ�
	m_vFileBuf.reserve(nNodeNumber);
	for(int i=0; i<nNodeNumber; i++)
	{
		fgets(szLine, 1024, fp);

		FileNode newNode;
		
		char* pDot1 = strchr(szLine, '|');
		if(!pDot1) 
			continue;

		*pDot1++ = 0;
		newNode.strFileName = szLine;
		if(*pDot1 == 'x')
		{
			newNode.bFold = true;
			newNode.nCRC = newNode.nSize = 0;
			m_vFileBuf.push_back(newNode);
			continue;
		}

		char* pDot2 = strchr(pDot1, '|');
		if(!pDot2) 
			continue; 
		*pDot2++ = 0;

		char* pDot3 = strchr(pDot2, '|');
		if(!pDot3) 
			continue; 
		*pDot3++ = 0;
		if( strchr( pDot3, 10 ) ) 
			*strchr( pDot3, 10 ) = 0;

		sscanf(pDot1, "%08x", &newNode.nSize);
		sscanf(pDot2, "%08x", &newNode.nCRC);
		newNode.strPackName = pDot3;
		newNode.bFold = false;

		if( newNode.strPackName.empty() )
		{
			newNode.strPatchName = newNode.strFileName = szLine;
		}
		else
		{
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

	m_strPath = szPath;
	m_strVersion = (LPCTSTR)strVersion;
	return true;
}

bool CFileCollector::CollectPath(const char* szPath)
{
	assert(szPath);
	m_vFileBuf.clear();

	::SendMessage(m_hProgressWnd, WM_PROGRESS_TITLE, (WPARAM)"����Ŀ¼...", 0);

	m_strPath = szPath;
	if(!_CollectPath(szPath, "", m_vFileBuf))
		return false;

	return true;
}

//����MPQ��������Version�ļ�
void  CFileCollector::BrowseMPQPack(const char* szPath,const char* FileName, std::vector< FileNode >& vFileBuf)
{
	string		strDir = szPath;
	int nPathLen = (int)m_strPath.size();

	KPM::FILE_FIND_DATA	sf;
	BOOL bFound		= TRUE;

	DWORD R;
	string FilePath = strDir + "/" + FileName;
	KPM::TKPMArchive* pArchive = KPM::KPM_OpenArchive(FilePath.c_str(),R);
	KPM::TKPMSearch*		hFind = KPM::KPM_FindFirstFile(pArchive,"*",&sf);

	while(hFind != NULL && bFound != FALSE)
	{
		if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && 
			strcmp(sf.cFileName, SIGNATURE_NAME) != 0 && 
			strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
		{
			//�ҵ��ļ�
			KPM::TKPMFile*	pFile = KPM_FileOpen(pArchive,sf.cFileName,R);
			FileNode newNode;

			//��ȻMPQ���л���CRC��MD5���ֱ��룬��ֻ��һ������һ�νϼ����ǰ�ȫ��
			newNode.nCRC = pFile->pCrc32->dwValue;	
			newNode.bFold = false;
			newNode.nSize = sf.dwCompSize;
			_mbslwr((byte*)sf.cFileName);
			newNode.strFileName = sf.cFileName;
			_mbslwr((byte*)FileName);
			newNode.strPackName = FileName;

			vFileBuf.push_back(newNode);
		}

		bFound = KPM::KPM_FindNextFile(hFind,&sf);
		int hh = 0;
	}

	KPM::KPM_SearchClose(hFind);
	KPM::KPM_CloseArchive(pArchive);

}//BrowseMPQPack end
		


//ǰ������������Ч������ֻ��Ϊ�˽ӿ�һ��
//szPath����ǰĿ¼
bool CFileCollector::_CollectPath(const char* szPath, const char* szRelPath, std::vector< FileNode >& vFileBuf)
{
	intptr_t	ptrFile;  
	struct		_finddata_t file;
	string		strDir = szPath;
	string		strFindTemp = strDir + "/*.mpq";
	if( (ptrFile = _findfirst( strFindTemp.c_str(), &file )) != -1L )
	{	
		BrowseMPQPack(szPath,file.name,vFileBuf);

		//�õ��ļ���չ��,��Ϊ�������ļ��У����Բ���Ҫ�ݹ�
		while( _findnext( ptrFile, &file ) == 0 )
		{	
			//�õ��ļ���չ��,��Ϊ�������ļ��У����Բ���Ҫ�ݹ�
			string FilePath = strDir + "/" + file.name;
			BrowseMPQPack(szPath,file.name,vFileBuf);
		}

		_findclose( ptrFile );				//�رղ��Ҿ��

	}//if end

	return true;

}

void CFileCollector::OutputVersionFile(const char* szFileName, const char* szVersion)
{
	//---------------------------------------------
	//������ʱ�ļ���
	CHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);

	CHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempPath, "pw", MAX_PATH, szTempFile);

	//---------------------------------------------
	//���ȱ��浽��ʱ�ļ�
	FILE* fp = fopen(szTempFile, "w");
	if(fp == 0) 
	{
		char szMsg[MAX_PATH];
		_snprintf(szMsg, MAX_PATH, "�����ļ�%sʧ�ܣ�", szTempFile);
		MessageBox(NULL, szMsg, "ERROR", MB_OK|MB_ICONSTOP);
		return;
	}

	fprintf(fp, "%s|%d\n", szVersion, (int)m_vFileBuf.size());

	//���·���ĳ���
	for(int i=0; i<(int)m_vFileBuf.size(); i++)
	{
		const FileNode& theNode = m_vFileBuf[i];
		if(theNode.bFold)
		{
			fprintf(fp, "%s|%08x|%08x|%s\n", theNode.strFileName.c_str(), theNode.nSize, theNode.nCRC,theNode.strPackName.c_str());
		}
		else
		{
			fprintf(fp, "%s|%08x|%08x|%s\n", theNode.strFileName.c_str(), theNode.nSize, 
				theNode.nCRC, theNode.strPackName.c_str());
		}
	}
	fclose(fp);

	//��ȡ�ļ�
	fp = fopen(szTempFile, "rb");
	if(fp == NULL) 
		return ;
	
	fseek(fp, 0, SEEK_END);
	INT nFileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	BYTE* pBuf = new BYTE[nFileLen];
	if(pBuf == NULL) 
		return ;
	fread(pBuf, 1, nFileLen, fp);
	fclose(fp);
	fp = NULL;

	//���浽version�ļ�
	fp = fopen(szFileName, "wb");
	if(fp == NULL) 
		return ;
	fwrite(pBuf, 1, nFileLen, fp);
	fclose(fp);
	delete[] pBuf;

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

	m_bTestVersion = FALSE;
}

CPatchMaker::~CPatchMaker()
{
	::CloseHandle(m_hTerimateHandle);
	m_hTerimateHandle = 0;
}

//�ж��Ƿ���һ��MPQ��
bool CPatchMaker::IsNewMPQPackage(CompareMap& VerMap,std::vector< FileNode >& verFile,FileNode& node)
{
	CompareMap::iterator it = VerMap.begin();
	for (;it!=VerMap.end();it++ )
	{	
		if ( it->second.pFileNode->strPackName == node.strPackName )
			return false;
	}

	std::vector< FileNode >::iterator it2 = verFile.begin();
	for(;it2!=verFile.end();it2++)
	{
		if (it2->strPackName == node.strPackName)
		{
			node.bFold = true;
			return false;
		}
	}

	return true;
}

void CPatchMaker::GeneratePatchData(std::string strFilter)
{
	GenerateFilter( strFilter );

	//���ɾͰ汾�Ƚ�����map
	CompareMap oldVerMap;
	_generateCompareMap(m_oldVerData.GetFileNode(), oldVerMap);

	CompareMap newVerMap;
	_generateCompareMap(m_newVerData.GetFileNode(), newVerMap);

	m_vNewFile.clear();
	m_vDeleteFile.clear();

	//������Ҫ���µ��ļ�
	for(int i=0; i<(int)m_newVerData.GetFileNode().size(); i++)
	{
		FileNode thisNode = m_newVerData.GetFileNode()[i];

		//ת���ļ���ΪСд
		char szTemp[1024] = {0};
		strncpy(szTemp, thisNode.strFileName.c_str(), 1024);
		_mbslwr((byte*)&szTemp[0]);

		if( FileNameMatch( szTemp ) ) 
		{
			CompareMap::iterator itFind = oldVerMap.find(szTemp);
			if(itFind != oldVerMap.end()) 
			{
				itFind->second.bMasked = true;
			}
			continue;//���˵�
		}

		//launch��һ������Ҫ��ӵ�
		//if ( thisNode.strPackName == "newlaunch.mpq" )
		//{
		//	thisNode.bFold = true;
		//	//��m_vNewFile��Ѱ��newlaunch���������ظ�
		//	std::vector< FileNode >::iterator it = m_vNewFile.begin();
		//	for(;it!=m_vNewFile.end();it++)
		//	{
		//		if (it->strPackName == "newlaunch.mpq")
		//		{
		//			thisNode.bFold = false;
		//			break ;
		//		}
		//	}
		//	if (thisNode.bFold)
		//		m_vNewFile.push_back(thisNode);	
		//	continue;
		//}

		//�ھɰ�������ͬ����û����˵�������һ���°�
		if ( IsNewMPQPackage(oldVerMap,m_vNewFile,thisNode) )
		{
			thisNode.bFold = true;
			m_vNewFile.push_back(thisNode);
		}
		else if (!thisNode.bFold)
		{
			//�ھɰ汾��Ѱ����ͬ�ļ�
			CompareMap::iterator itFind = oldVerMap.find(szTemp);
			if(itFind == oldVerMap.end())
			{
				//����ӵ��ļ�
				thisNode.bFold = false;
				m_vNewFile.push_back(thisNode);
				continue;
			}	
	
			//�Ƚ������ļ�
			const FileNode* ppp = itFind->second.pFileNode;
			itFind->second.bMasked = true;
			const FileNode& theOldNode = *(itFind->second.pFileNode);

			if(theOldNode.nCRC != thisNode.nCRC || theOldNode.nSize != thisNode.nSize)
			{
				//��Ҫ���µ��ļ�
				thisNode.bFold = false;
				m_vReplaceFile.push_back(thisNode);
			}

		}//else end
	}//for end

	//����汾�����ļ�
	FileNode verNode;
	verNode.bFold = false;
	verNode.strPatchName = verNode.strFileName = "(version)";
	m_vReplaceFile.push_back(verNode);

	//������Ҫɾ�����ļ�
	CompareMap::iterator itOld;
	for(itOld = oldVerMap.begin(); itOld != oldVerMap.end(); itOld++)
	{
		FileNode thisNode = (FileNode&)*itOld->second.pFileNode;

		//Ϊ����launch���ڴ˲��ܱ�ɾ����ֻ����ӣ����ʱ���Զ�ɾ��ԭ�еİ���
		if ( thisNode.strPackName == "NewLaunch.mpq" )
		{
			continue;
		}

		if ( IsNewMPQPackage(newVerMap,m_vDeleteFile,thisNode) )
		{
			thisNode.bFold = true;
			m_vDeleteFile.push_back(thisNode);
		}
		else if (!thisNode.bFold)
		{
			if(itOld->second.bMasked) 
				continue;

			thisNode.bFold = false;

			//��δ��ǵ��ļ�����Ҫɾ��
			m_vDeleteFile.push_back(*(itOld->second.pFileNode));
		}
	}

//GeneratePatchData end
}

void CPatchMaker::_generateCompareMap(const std::vector< FileNode >& fileData, CompareMap& compareMap)
{
	for(int i=0; i<(int)fileData.size(); i++)
	{
		const FileNode& thisNode = fileData[i];
		CompareNode newNode;

		//ת���ļ���ΪСд
		char szTemp[1024] = {0};
		strncpy(szTemp, thisNode.strFileName.c_str(), 1024);
		_mbslwr((byte*)&szTemp[0]);

		newNode.pFileNode = &thisNode;
		newNode.bMasked = false;
		compareMap.insert(std::make_pair(szTemp, newNode));
	}
}

bool CPatchMaker::WritePatchFile()
{
	::SendMessage(m_hProgressWnd, WM_PROGRESS_TITLE, (WPARAM)"ѹ���ļ�...", 0);

	//--------------------------------------------------------------
	//���������ļ�

	FILE* fp = fopen("(command)", "w");
	if(fp == 0)
	{
		char szMsg[MAX_PATH] = {0};
		_snprintf(szMsg, MAX_PATH, "д(command)�ļ�����!");
		AfxGetApp()->GetMainWnd()->MessageBox(szMsg, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	// add by tangpeng
	_writeCommandFile( fp, "del",m_vDeleteFile );
	_writeCommandFile( fp, "replace",m_vReplaceFile );
	_writeCommandFile( fp, "new",m_vNewFile );
	fprintf(fp, "frpl (version)\n");
	fclose(fp);
	fp=0;

	//--------------------------------------------------------------
	//����Patch�ļ���
	char szPatchFileName[MAX_PATH];
	_snprintf(szPatchFileName, MAX_PATH, "cjsh-%s-%s.mpq", 
		m_oldVerData.GetVersion().c_str(), m_newVerData.GetVersion().c_str());

	char szPatchPathFileName[MAX_PATH] = {0};
	strncpy(szPatchPathFileName, m_strPatchPath.c_str(), MAX_PATH);
	PathAppend(szPatchPathFileName, szPatchFileName);
	
	m_strPatchFile = szPatchPathFileName;

	//����PatchMqp
	DWORD R = 0;
	int HashTableSize = m_vReplaceFile.size() + m_vNewFile.size() + 2;
	HashTableSize = HashTableSize * 1.5;	//Hash���С���ļ�ʵ�ʸ�����1.5��

	if ( HashTableSize<MIN_HASH_TABLE_SIZE )
		HashTableSize = MIN_HASH_TABLE_SIZE;

	if (HashTableSize>MAX_HASH_TABLE_SIZE)
	{
		MessageBox(NULL, "Hash��̫�������ļ�����", "ERROR", MB_OK|MB_ICONSTOP);
		return false;
	}

	KPM::TKPMArchive* pArchive = KPM::KPM_CreateArchive(szPatchPathFileName,R, HashTableSize, USE_TYPE_PATCH);	
	if ( !pArchive )
	{	
		char szMsg[MAX_PATH] = {0};
		_snprintf(szMsg, MAX_PATH, "������%s����!", szPatchPathFileName);
		AfxGetApp()->GetMainWnd()->MessageBox(szMsg, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	//���������ļ�
	KPM::KPM_AddFile(pArchive,"(command)","(command)",KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE,0,KPM::FILE_TYPE_DATA);

	if (!m_bTestVersion)
		DeleteFile("(command)");

	//================
	// ��Ϊ���ʱ������·���� ���Դ�����ʱ�ļ�����ӵ���������
	static int s_Temp = 50;

	CHAR szTempStr[1024] = {0};
	CHAR szTempPathStr[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPathStr);

	//std::string newPackPath = m_newVerData.GetPath();
	//FileNode tempThisNode = m_newVerData.GetFileNode()[0];
	//std::string newPackFileName = newPackPath + "\\" + tempThisNode.strPackName;

//	DWORD TempR;
//	KPM::TKPMArchive* pTempArchive = KPM::KPM_OpenArchive( newPackFileName.c_str(), TempR );
//	if( !pTempArchive )
//	{
//		char szBuf[1024];
//		sprintf( szBuf, "�򿪰��ļ�%sʧ��!\n", newPackFileName.c_str() );
//		AfxGetApp()->GetMainWnd()->MessageBox(szBuf, 0, MB_OK|MB_ICONSTOP);
////		Log( szBuf, (DWORD)strlen( szBuf ) );
//		return( FALSE );			//�޷��򿪰��ļ�
//	}

	CHAR szOrgFile[MAX_PATH] = {0};

	//������Ҫ�滻���ļ�
	for(int i=0; i<(int)m_vReplaceFile.size(); i++)
	{
		// check terminate flag
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hTerimateHandle, 0))
		{
			KPM_CloseArchive(pArchive);
			DeleteFile(szPatchPathFileName);
			return false;
		}

		const FileNode& thisNode = m_vReplaceFile[i];
		if(thisNode.bFold) 
			continue;
		::SendMessage(m_hProgressWnd, WM_PROGRESS_ZIPFILE, (WPARAM)thisNode.strFileName.c_str(), 0);

		if( !thisNode.bFold )				//���ǰ�
		{
			if (thisNode.strFileName.find("(version)") != string::npos )
			{
				//VERSION�ļ�
				strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
				PathAppend(szOrgFile, thisNode.strFileName.c_str());
				KPM::KPM_AddFile(pArchive,szOrgFile,thisNode.strFileName.c_str(),KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE,0,KPM::FILE_TYPE_DATA);
			}
			else
			{
				//��ͨ�ļ�
				CHAR szTempFile[MAX_PATH] = {0};
				::GetTempFileName( szTempPathStr, itoa( s_Temp++, szTempStr, 10 ), MAX_PATH, szTempFile );

				if( !CreateTempFile(thisNode,szTempFile ) )
				{
					return false;
				}

				//KPM::KPM_AddFile(pArchive,thisNode.strFileName.c_str(),thisNode.strFileName.c_str(),KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE|KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
				KPM::KPM_AddFile(pArchive,szTempFile,thisNode.strFileName.c_str(),KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE|KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
			}
		}
		else
		{
			KPM::KPM_AddFile(pArchive,thisNode.strPackName.c_str(),thisNode.strPackName.c_str(),KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
		}

	}//for end

	//������Ҫ��ӵ��ļ�
	for(int i=0; i<(int)m_vNewFile.size(); i++)
	{
		// check terminate flag
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hTerimateHandle, 0))
		{
			KPM_CloseArchive(pArchive);
			DeleteFile(szPatchPathFileName);
			return false;
		}

		const FileNode& thisNode = m_vNewFile[i];
		::SendMessage(m_hProgressWnd, WM_PROGRESS_ZIPFILE, (WPARAM)thisNode.strFileName.c_str(), 0);

		if( !thisNode.bFold )		//���ǰ�
		{

			CHAR szTempFile[MAX_PATH] = {0};
			::GetTempFileName( szTempPathStr, itoa( s_Temp++, szTempStr, 10 ), MAX_PATH, szTempFile );

			if( !CreateTempFile(thisNode,szTempFile ) )
			{
				return false;
			}

			KPM::KPM_AddFile(pArchive,szTempFile,thisNode.strFileName.c_str(),KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE|KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
			//KPM::KPM_AddFile(pArchive,thisNode.strFileName.c_str(),thisNode.strFileName.c_str(),KPM_FILE_ENCRYPTED|KPM_FILE_IMPLODE|KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
		
		}
		else
		{
			strncpy(szOrgFile, m_newVerData.GetPath().c_str(), MAX_PATH);
			PathAppend(szOrgFile, thisNode.strPackName.c_str());
			bool ret = KPM::KPM_AddFile(pArchive,szOrgFile,thisNode.strPackName.c_str(),KPM_FILE_REPLACEEXISTING,0,KPM::FILE_TYPE_DATA);
			if ( !ret )
			{
				assert(false);
			}
		}
	}//for end

	KPM_CloseArchive(pArchive);
	return true;

}//WritePatchFile end

//pdel pname ɾ��һ��pname��
//prpl pname �滻һ��pname��
//pnew pname ����һ��pname���

//frpl fname �滻һ���ļ���������Version�ļ��������ⲻӦ����ɢ����ļ�

//fpdel fname from pname  ��pname����ɾ��һ����fname���ļ�
//fprpl fname from pname  ��pname�����滻һ����fname���ļ�
//fpnew fname from pname  ��pname��������һ����fname���ļ�
void CPatchMaker::_writeCommandFile( FILE* fp, std::string strCommand,std::vector< FileNode >& fileData )
{
	if( !fp )
		return;

	for(int i=0; i<(int)fileData.size(); i++)
	{
		FileNode& fi = fileData[i];

		if( fi.strFileName == "(version)" ) 
			continue;

		if( strCommand == "del" )
		{
			if ( fi.bFold )		//ɾ��һ����
			{
				if ( m_bTestVersion )	//�ڲ����԰�
				{
					//��¼�Ĳ�����ɾ���İ��������Ǹð��������ļ���
					string PackName = m_oldVerData.GetPath() + "/"+fi.strPackName;
					DWORD R;
					KPM::FILE_FIND_DATA	sf;
					BOOL bFound		= TRUE;
					KPM::TKPMArchive* pArchive = KPM::KPM_OpenArchive( PackName.c_str(), R );
					KPM::TKPMSearch*		hFind = KPM::KPM_FindFirstFile(pArchive,"*",&sf);
					while(hFind != NULL && bFound != FALSE)
					{
						if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && 
							 strcmp(sf.cFileName, SIGNATURE_NAME) != 0 &&  
							 strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
						{
							fprintf(fp, "pdel %s\n",sf.cFileName );
						}

						bFound = KPM_FindNextFile(hFind,&sf);
					}

					KPM_SearchClose(hFind);
					KPM_CloseArchive(pArchive);
				}
				else
					fprintf(fp, "pdel %s\n", fi.strPackName.c_str());
			}
			else				//�Ӱ���ɾ���ļ�
			{
				fprintf(fp, "fpdel %s from %s\n", fi.strFileName.c_str(),fi.strPackName.c_str());
			}
		}
		else if( strCommand == "replace" )
		{
			if ( fi.bFold )		//�滻һ����
			{
				fprintf(fp, "prpl %s\n", fi.strPackName.c_str());
			}
			else 
			{
				fprintf(fp, "fprpl %s from %s\n", fi.strFileName.c_str(),fi.strPackName.c_str());
			}
		}
		else if( strCommand == "new" )
		{	
			if ( fi.bFold )		//���һ����
			{
				fprintf(fp, "pnew %s\n", fi.strPackName.c_str());
			}
			else
			{
				fprintf(fp, "fpnew %s from %s\n", fi.strFileName.c_str(),fi.strPackName.c_str());
			}
		}
	}

	return ;

}//_writeCommandFile end

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


bool CPatchMaker::CreateTempFile( KPM::TKPMArchive* pTempArchive, const std::string& strFileName, const std::string& tempFileName )
{
	//KPM::FILE_FIND_DATA	sf;
	DWORD TempR;

	//KPM::TKPMSearch* hTempFind = KPM::KPM_FindFirstFile( pTempArchive, strFileName.c_str(), &sf );
	//if( !hTempFind )
	//{
	//	char szBuf[1024];
	//	sprintf( szBuf, "���ļ�%sʧ��!\n", strFileName.c_str() );
	//	return( FALSE );			//�޷����ļ�
	//}

	KPM::TKPMFile* pTempFile = KPM::KPM_FileOpen( pTempArchive, strFileName.c_str(), TempR );
	if( !pTempFile )
	{
		char szBuf[1024];
		sprintf( szBuf, "���ļ�%sʧ��!\n", strFileName.c_str() );
		return( FALSE );			//�޷���Patch�ļ�
	}

	DWORD FileSize = KPM_GetFileSize(pTempFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM::KPM_FileEof(pTempFile);	
		KPM::KPM_FileClose(pTempFile);
		KPM::KPM_CloseArchive(pTempArchive);
		return( FALSE );
	}

	DWORD Read;
	KPM::KPM_ReadFile( pTempFile, lpFile, FileSize, &Read );
	if( Read != FileSize )
	{
		KPM::KPM_FileEof(pTempFile);	
		KPM::KPM_FileClose(pTempFile);
		KPM::KPM_CloseArchive(pTempArchive);
//		Log( "���ļ�����", 10 );
		return( FALSE );		
	}

	KPM::KPM_FileEof(pTempFile);	
	KPM::KPM_FileClose(pTempFile);
//	KPM::KPM_CloseArchive(pTempArchive);


	FILE* fp = fopen( tempFileName.c_str(), "wb" );
	if( fp == NULL ) 
		return( FALSE );

	int Write = fwrite( lpFile, 1, FileSize, fp );
	if(Write != FileSize)
	{
//		Log( "д�ļ�ʧ��",10 );
		return( FALSE );
	}

	if( fclose( fp ) != 0 )
	{
//		Log( "�ر��ļ�����",12);
	}

	delete []lpFile;


	return true;
}


bool CPatchMaker::CreateTempFile(const FileNode &node,const std::string& tempFileName)
{
	// ��Ϊ���ʱ������·���� ���Դ�����ʱ�ļ�����ӵ���������

	std::string newPackPath = m_newVerData.GetPath();
	std::string newPackFileName = newPackPath + "\\" + node.strPackName;

	DWORD TempR;
	KPM::TKPMArchive* pTempArchive = KPM::KPM_OpenArchive( newPackFileName.c_str(), TempR );
	if( !pTempArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "�򿪰��ļ�%sʧ��!\n", newPackFileName.c_str() );
		AfxGetApp()->GetMainWnd()->MessageBox(szBuf, 0, MB_OK|MB_ICONSTOP);
		return( FALSE );			//�޷��򿪰��ļ�
	}

	CreateTempFile(pTempArchive,node.strFileName,tempFileName);
	KPM_CloseArchive(pTempArchive);

	return true;
}

