
#include "StdAfx.h"
#include "TestUpdateSystem.h"
#include "UpdateSystem.h"

TestUpdateSystem::TestUpdateSystem(void)
{
}

TestUpdateSystem::~TestUpdateSystem(void)
{

}


//删除一个完整的包,内部测试版：删除不针对包，需删除所有零散文件
void TestUpdateSystem::DelMpqPackage(string& PackName)
{
	// 遍历MPQ包得到所有文件名
	DWORD R;
	FILE_FIND_DATA	sf;
	BOOL bFound		= TRUE;
	TKPMArchive* pArchive = KPM_OpenArchive( PackName.c_str(), R );
	TKPMSearch*		hFind = KPM_FindFirstFile(pArchive,"*",&sf);

	while(hFind != NULL && bFound != FALSE)
	{
		if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && strcmp(sf.cFileName, SIGNATURE_NAME) != 0
			  && strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
		{
			BOOL bFlag = TRUE;
			if( PathFileExists( sf.cFileName ) ) 
			{
				if( SetFileAttributes( sf.cFileName, FILE_ATTRIBUTE_NORMAL ) == FALSE 
						||	DeleteFile( sf.cFileName ) == FALSE )
					bFlag = FALSE;

				if( bFlag == FALSE )
				{
					char szBuf[1024];
					sprintf( szBuf, "没有删除的文件%s\n", sf.cFileName );
					UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
					assert(false);
				}
			}
			else 
			{
				char szBuf[1024];
				sprintf( szBuf, "要删除的文件%s不存在\n",sf.cFileName );
				( szBuf, (DWORD)strlen( szBuf ) );
				assert(false);
			}
		}
		bFound = KPM_FindNextFile(hFind,&sf);
	}

	char szBuf[1024];
	sprintf( szBuf, "删除包%s成功!\n",PackName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	KPM_SearchClose(hFind);
	KPM_CloseArchive(pArchive);

}//DelMpqPackage end


//从包中删除一个文件，内部测试版：删除不针对包，而是删除目录下的零散文件
bool TestUpdateSystem::DelFileFromPackage(string& FileName)
{
	BOOL bFlag = TRUE;
	if( PathFileExists( FileName.c_str() ) ) 
	{
		if( SetFileAttributes( FileName.c_str(), FILE_ATTRIBUTE_NORMAL ) == FALSE 
				||	DeleteFile( FileName.c_str() ) == FALSE )
			bFlag = FALSE;

		if( bFlag == FALSE )
		{
			char szBuf[1024];
			sprintf( szBuf, "没有删除的文件%s\n", FileName.c_str() );
			UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			return (false);
		}
	}
	else 
	{
		char szBuf[1024];
		sprintf( szBuf, "要删除的文件%s不存在\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		return (false);
	}

	char szBuf[1024];
	sprintf( szBuf, "成功删除文件%s\n",FileName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
	return true;
}


//添加一个文件到包中,内部测试版：添加不针对包，而是添加到相应目录中
//PackName:  不是指添加后的包名，是指原文件所处的包，
BOOL TestUpdateSystem::AddFileToPackage(string& PackName,string& FileName)
{
	DWORD R;
	string  MPQPack = PackName;
	TKPMArchive* pArchive = KPM_OpenArchive(PackName.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "打开包%s失败!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		
		assert(false);
		return FALSE;			
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,FileName.c_str(),R);
	if ( !pFile )
	{
		char szBuf[1024];
		sprintf( szBuf, "从包中%s打开文件%s失败!\n",PackName.c_str(), FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;			
	}

	DWORD FileSize = KPM_GetFileSize(pFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "分配空间失败!\n" );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "从包中%s读文件%s失败!\n",PackName.c_str(), FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;		
	}

	//根据CRC32和MD5码判断文件完整性
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//文件损坏
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "包中%s文件%s损坏!\n",PackName.c_str(), FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	//判断上层目录是否为空，为空则创建
	string FilePath = FileName;
	string::size_type pos = FilePath.find("/");
	while ( ( pos = FilePath.find("/",++pos) ) != string::npos )
	{
		string tmp = FilePath.substr(0,pos);

		if( PathFileExists( tmp.c_str() ) == false ) 
		{
			CreateDirectory(tmp.c_str(),0);
			tmp = FilePath;
		}		
	}

	FILE* fp = fopen( FileName.c_str(), "wb" );
	if( fp == NULL ) 
	{
		char szBuf[1024];
		sprintf( szBuf, "打开文件%s失败!\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );		
		return FALSE;
	}

	int Write = fwrite(lpFile, 1, FileSize, fp );
	if (Write!=FileSize)
	{
		char szBuf[1024];
		sprintf( szBuf, "写文件%s失败!\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	fclose( fp );
	fp = 0;
	
	char szBuf[1024];
	sprintf( szBuf, "向包中%s添加文件%s成功!\n",PackName.c_str(), FileName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	return TRUE;

}//AddFileToPackage end


//添加一个完整的包,内部测试版：添加不针对包，需添加所有零散文件
//
BOOL TestUpdateSystem::AddPackage(string& ZipName,string PackName)
{
	//将差异包中的MPQ包在硬盘中生成临时MPQ包
	DWORD R;
	FILE_FIND_DATA	sf;
	TKPMArchive* pArchive = KPM_OpenArchive( ZipName.c_str(), R );
	if (!pArchive)
	{
		char szBuf[1024];
		sprintf( szBuf, "打开包%s失败!\n", ZipName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,PackName.c_str(),R);
	if (!pFile)
	{
		char szBuf[1024];
		sprintf( szBuf, "从包中打开%s文件%s失败!\n",ZipName.c_str(), PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		return FALSE;
	}

	DWORD FileSize = KPM_GetFileSize(pFile);

	LPSTR lpFile = new CHAR[FileSize];
	if( !lpFile ) 
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "分派空间失败!\n" );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;
	}

	DWORD Read;
	KPM_ReadFile(pFile,lpFile,FileSize,&Read);
	if( Read != FileSize )
	{
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "从包中%s读%s失败!\n",ZipName.c_str(), PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;		
	}

	//根据CRC32和MD5码判断文件完整性
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//文件损坏
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "包中%s文件%s损坏!\n",ZipName.c_str(), PackName.c_str());
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	//保存到临时文件
	FILE* fp = fopen( PackName.c_str(), "wb" );
	if( fp == NULL ) 
	{	
		char szBuf[1024];
		sprintf( szBuf, "打开临时文件%s失败!\n",PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		return FALSE;
	}
	
	int Write = fwrite(lpFile, 1, FileSize, fp );
	if (Write!=FileSize)
	{
		char szBuf[1024];
		sprintf( szBuf, "写临时%s失败!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	fclose( fp );
	fp = 0;

	//将差异包中的MPQ包在硬盘中生成临时MPQ包：******动作完成


	// 遍历临时MPQ包得到所有文件名
	BOOL bFound		= TRUE;
	TKPMArchive* pArchive2 = KPM_OpenArchive( PackName.c_str(), R );
	if (!pArchive2) 
	{
		char szBuf[1024];
		sprintf( szBuf, "打开%s包失败!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	TKPMSearch* hFind2 = KPM_FindFirstFile(pArchive2,"*",&sf);
	if (!hFind2) 
	{
		char szBuf[1024];
		sprintf( szBuf, "在包中%s查找失败!\n",PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	while(hFind2 != NULL && bFound != FALSE)
	{
		if ( strcmp(sf.cFileName, LISTFILE_NAME) != 0 && 
			 strcmp(sf.cFileName, SIGNATURE_NAME) != 0 && 
			 strcmp(sf.cFileName, ATTRIBUTES_NAME) != 0 )
		{
			TKPMFile*	pFile = KPM_FileOpen(pArchive2,sf.cFileName,R);
			if ( !pFile )
			{
				char szBuf[1024];
				sprintf( szBuf, "从包中%s打开%s失败!\n",PackName.c_str(), sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;			
			}
			
			LPSTR lpFile = new CHAR[sf.dwFileSize];
			if( !lpFile ) 
			{
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive2);

				char szBuf[1024];
				sprintf( szBuf, "分配空间失败!\n");
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;
			}

			DWORD Read;
			KPM_ReadFile(pFile,lpFile,sf.dwFileSize,&Read);
			if( Read != sf.dwFileSize )
			{
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive2);

				char szBuf[1024];
				sprintf( szBuf, "从包中%s读%s失败!\n",PackName.c_str(), sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;		
			}

			//根据CRC32和MD5码判断文件完整性
			if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
			{
				//文件损坏
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive2);

				char szBuf[1024];
				sprintf( szBuf, "包中%s%s文件损坏!\n",PackName.c_str() ,sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;
			}

			KPM_FileEof(pFile);	
			KPM_FileClose(pFile);

			//判断上层目录是否为空，为空则创建
			string FilePath = sf.cFileName;
			string::size_type pos = FilePath.find("/");
			while ( ( pos = FilePath.find("/",++pos) ) != string::npos )
			{
				string tmp = FilePath.substr(0,pos);

				if( PathFileExists( tmp.c_str() ) == false ) 
				{
					CreateDirectory(tmp.c_str(),0);
					tmp = FilePath;
				}		
			}

			FILE* fp = fopen( sf.cFileName, "wb" );
			if( fp == NULL ) 
			{
				char szBuf[1024];
				sprintf( szBuf, "打开临时文件%s失败!\n", sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			
				return FALSE;
			}

			int Write = fwrite(lpFile, 1, sf.dwFileSize, fp );
			if (Write!=sf.dwFileSize)
			{
				char szBuf[1024];
				sprintf( szBuf, "写临时%s文件失败!\n", sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;
			}

			fclose( fp );
			fp = 0;

		}
		bFound = KPM_FindNextFile(hFind2,&sf);
	}

	char szBuf[1024];
	sprintf( szBuf, "创建包%s成功!\n", PackName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	KPM_SearchClose(hFind2);
	KPM_CloseArchive(pArchive2);
	::DeleteFile(PackName.c_str());

	return TRUE;

}//AddPackage end


//根据CRC32和MD5码判断文件完整性
BOOL TestUpdateSystem::IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen)
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
			printf("MD5码与CRC32码都相同，说明文件未损坏");
			return TRUE;	
		}
		else
		{
			//取其中一段进行比较
			TKPMMD5 *temp = KPM_ALLOC(TKPMMD5, 0x4000); ;
			memset(temp, 0, sizeof(TKPMMD5) * 0x4000);
			memcpy(temp->Value,pFile->pMD5->Value,16);
			if ( _mbscmp(temp->Value,pMD5->Value) == 0 )
			{
				//MD5码与CRC32码都相同，说明文件未损坏
				return TRUE;					
			}
		}
		return FALSE;		
	}

	//文件损坏
	return FALSE;

}//IsAllRightCRC32AndMD5 end


