
#include "StdAfx.h"
#include "TestUpdateSystem.h"
#include "UpdateSystem.h"

TestUpdateSystem::TestUpdateSystem(void)
{
}

TestUpdateSystem::~TestUpdateSystem(void)
{

}


//ɾ��һ�������İ�,�ڲ����԰棺ɾ������԰�����ɾ��������ɢ�ļ�
void TestUpdateSystem::DelMpqPackage(string& PackName)
{
	// ����MPQ���õ������ļ���
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
					sprintf( szBuf, "û��ɾ�����ļ�%s\n", sf.cFileName );
					UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
					assert(false);
				}
			}
			else 
			{
				char szBuf[1024];
				sprintf( szBuf, "Ҫɾ�����ļ�%s������\n",sf.cFileName );
				( szBuf, (DWORD)strlen( szBuf ) );
				assert(false);
			}
		}
		bFound = KPM_FindNextFile(hFind,&sf);
	}

	char szBuf[1024];
	sprintf( szBuf, "ɾ����%s�ɹ�!\n",PackName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	KPM_SearchClose(hFind);
	KPM_CloseArchive(pArchive);

}//DelMpqPackage end


//�Ӱ���ɾ��һ���ļ����ڲ����԰棺ɾ������԰�������ɾ��Ŀ¼�µ���ɢ�ļ�
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
			sprintf( szBuf, "û��ɾ�����ļ�%s\n", FileName.c_str() );
			UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			return (false);
		}
	}
	else 
	{
		char szBuf[1024];
		sprintf( szBuf, "Ҫɾ�����ļ�%s������\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		return (false);
	}

	char szBuf[1024];
	sprintf( szBuf, "�ɹ�ɾ���ļ�%s\n",FileName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
	return true;
}


//���һ���ļ�������,�ڲ����԰棺��Ӳ���԰���������ӵ���ӦĿ¼��
//PackName:  ����ָ��Ӻ�İ�������ָԭ�ļ������İ���
BOOL TestUpdateSystem::AddFileToPackage(string& PackName,string& FileName)
{
	DWORD R;
	string  MPQPack = PackName;
	TKPMArchive* pArchive = KPM_OpenArchive(PackName.c_str(),R);
	if ( !pArchive )
	{
		char szBuf[1024];
		sprintf( szBuf, "�򿪰�%sʧ��!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		
		assert(false);
		return FALSE;			
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,FileName.c_str(),R);
	if ( !pFile )
	{
		char szBuf[1024];
		sprintf( szBuf, "�Ӱ���%s���ļ�%sʧ��!\n",PackName.c_str(), FileName.c_str() );
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
		sprintf( szBuf, "����ռ�ʧ��!\n" );
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
		sprintf( szBuf, "�Ӱ���%s���ļ�%sʧ��!\n",PackName.c_str(), FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;		
	}

	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "����%s�ļ�%s��!\n",PackName.c_str(), FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	//�ж��ϲ�Ŀ¼�Ƿ�Ϊ�գ�Ϊ���򴴽�
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
		sprintf( szBuf, "���ļ�%sʧ��!\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );		
		return FALSE;
	}

	int Write = fwrite(lpFile, 1, FileSize, fp );
	if (Write!=FileSize)
	{
		char szBuf[1024];
		sprintf( szBuf, "д�ļ�%sʧ��!\n",FileName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	fclose( fp );
	fp = 0;
	
	char szBuf[1024];
	sprintf( szBuf, "�����%s����ļ�%s�ɹ�!\n",PackName.c_str(), FileName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	return TRUE;

}//AddFileToPackage end


//���һ�������İ�,�ڲ����԰棺��Ӳ���԰��������������ɢ�ļ�
//
BOOL TestUpdateSystem::AddPackage(string& ZipName,string PackName)
{
	//��������е�MPQ����Ӳ����������ʱMPQ��
	DWORD R;
	FILE_FIND_DATA	sf;
	TKPMArchive* pArchive = KPM_OpenArchive( ZipName.c_str(), R );
	if (!pArchive)
	{
		char szBuf[1024];
		sprintf( szBuf, "�򿪰�%sʧ��!\n", ZipName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;
	}

	TKPMFile*	pFile = KPM_FileOpen(pArchive,PackName.c_str(),R);
	if (!pFile)
	{
		char szBuf[1024];
		sprintf( szBuf, "�Ӱ��д�%s�ļ�%sʧ��!\n",ZipName.c_str(), PackName.c_str() );
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
		sprintf( szBuf, "���ɿռ�ʧ��!\n" );
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
		sprintf( szBuf, "�Ӱ���%s��%sʧ��!\n",ZipName.c_str(), PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;		
	}

	//����CRC32��MD5���ж��ļ�������
	if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
	{
		//�ļ���
		KPM_FileEof(pFile);	
		KPM_FileClose(pFile);
		KPM_CloseArchive(pArchive);

		char szBuf[1024];
		sprintf( szBuf, "����%s�ļ�%s��!\n",ZipName.c_str(), PackName.c_str());
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		return FALSE;
	}

	KPM_FileEof(pFile);	
	KPM_FileClose(pFile);
	KPM_CloseArchive(pArchive);

	//���浽��ʱ�ļ�
	FILE* fp = fopen( PackName.c_str(), "wb" );
	if( fp == NULL ) 
	{	
		char szBuf[1024];
		sprintf( szBuf, "����ʱ�ļ�%sʧ��!\n",PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
		return FALSE;
	}
	
	int Write = fwrite(lpFile, 1, FileSize, fp );
	if (Write!=FileSize)
	{
		char szBuf[1024];
		sprintf( szBuf, "д��ʱ%sʧ��!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	fclose( fp );
	fp = 0;

	//��������е�MPQ����Ӳ����������ʱMPQ����******�������


	// ������ʱMPQ���õ������ļ���
	BOOL bFound		= TRUE;
	TKPMArchive* pArchive2 = KPM_OpenArchive( PackName.c_str(), R );
	if (!pArchive2) 
	{
		char szBuf[1024];
		sprintf( szBuf, "��%s��ʧ��!\n", PackName.c_str() );
		UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

		assert(false);
		return FALSE;
	}

	TKPMSearch* hFind2 = KPM_FindFirstFile(pArchive2,"*",&sf);
	if (!hFind2) 
	{
		char szBuf[1024];
		sprintf( szBuf, "�ڰ���%s����ʧ��!\n",PackName.c_str() );
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
				sprintf( szBuf, "�Ӱ���%s��%sʧ��!\n",PackName.c_str(), sf.cFileName );
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
				sprintf( szBuf, "����ռ�ʧ��!\n");
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
				sprintf( szBuf, "�Ӱ���%s��%sʧ��!\n",PackName.c_str(), sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;		
			}

			//����CRC32��MD5���ж��ļ�������
			if ( !IsAllRightCRC32AndMD5(pFile,(unsigned char *)lpFile,Read) )		
			{
				//�ļ���
				KPM_FileEof(pFile);	
				KPM_FileClose(pFile);
				KPM_CloseArchive(pArchive2);

				char szBuf[1024];
				sprintf( szBuf, "����%s%s�ļ���!\n",PackName.c_str() ,sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

				assert(false);
				return FALSE;
			}

			KPM_FileEof(pFile);	
			KPM_FileClose(pFile);

			//�ж��ϲ�Ŀ¼�Ƿ�Ϊ�գ�Ϊ���򴴽�
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
				sprintf( szBuf, "����ʱ�ļ�%sʧ��!\n", sf.cFileName );
				UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );
			
				return FALSE;
			}

			int Write = fwrite(lpFile, 1, sf.dwFileSize, fp );
			if (Write!=sf.dwFileSize)
			{
				char szBuf[1024];
				sprintf( szBuf, "д��ʱ%s�ļ�ʧ��!\n", sf.cFileName );
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
	sprintf( szBuf, "������%s�ɹ�!\n", PackName.c_str() );
	UpdateSystem::GetMe()->Log( szBuf, (DWORD)strlen( szBuf ) );

	KPM_SearchClose(hFind2);
	KPM_CloseArchive(pArchive2);
	::DeleteFile(PackName.c_str());

	return TRUE;

}//AddPackage end


//����CRC32��MD5���ж��ļ�������
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


