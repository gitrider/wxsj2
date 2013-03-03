#include "StdAfx.h"

#include "Util.h"
#include "Global.h"

#include <zzip/zzip.h>
#include <zzip/lib.h>

bool ExtractResToFile(unsigned int nResID, const char* szResType,  const char* szFileName)
{
	HRSRC hRes = ::FindResource((HMODULE)g_hInstance,MAKEINTRESOURCE(nResID), szResType);
	if(!hRes) return false;

	DWORD nLen = ::SizeofResource((HMODULE)g_hInstance, hRes);

	HGLOBAL hTable = ::LoadResource((HMODULE)g_hInstance, hRes);
	if(!hTable) return false;

	LPWORD pTable = (LPWORD)::LockResource(hTable);
	if(!pTable) 
	{
		::FreeResource(hTable); 
		return false;
	}

	FILE* fp;
	if((fp = fopen(szFileName, "wb")) == 0)
	{
		UnlockResource(pTable); pTable = 0;
		FreeResource(hTable);  hTable = 0;
		return false;
	}
	fwrite(pTable, 1, nLen, fp);

	fclose(fp); fp = 0;
	UnlockResource(pTable); pTable = 0;
	FreeResource(hTable);  hTable = 0;
	return true;
}

bool ExtractFileFromZip(const char* szZipFile, const char* szFileInZip, const char* szFileInDisk)
{
	//��zip�ļ�
	zzip_error_t zzipError;
	ZZIP_DIR* mZzipDir = zzip_dir_open(szZipFile, &zzipError);
	if (zzipError != ZZIP_NO_ERROR) 
	{
		return false;
	}

	//�õ��ļ���Ϣ
	ZZIP_STAT zstat;
	memset(&zstat, 0, sizeof(ZZIP_STAT));

	//���ļ�,����򲻿����ǿ��ļ�
	ZZIP_FILE* zzipFile = zzip_file_open(mZzipDir, szFileInZip, ZZIP_ONLYZIP | ZZIP_CASELESS);
	if(!zzipFile)
	{
		zzip_dir_close(mZzipDir);
		return false;
	}

	//����ļ���Ϣ
	int zip_err = zzip_dir_stat(mZzipDir, szFileInZip, &zstat, ZZIP_CASEINSENSITIVE);
	if(zip_err!=0)
	{
		zzip_file_close(zzipFile);
		zzip_dir_close(mZzipDir);
		return false;
	}

	//�������ļ�
	HANDLE hDiskFile = ::CreateFile(szFileInDisk, 
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE,
		0);

	if(hDiskFile == INVALID_HANDLE_VALUE)
	{
		zzip_file_close(zzipFile);
		zzip_dir_close(mZzipDir);
		return false;
	}

	if(zstat.st_size > 0)
	{
		const int MAX_BUFFER_SIZE = 4096;
		char buffer[MAX_BUFFER_SIZE] = {0};

		zzip_seek(zzipFile, 0, SEEK_SET);
		zzip_ssize_t zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));

		//ʵ���Ѿ�д��ĳߴ�
		unsigned int nActWriteSize = 0;

		//�ֿ��д�ļ�����
		do
		{
			//�ļ�����
			if(zReadSize==0) break;

			//д������ļ�
			DWORD dwBytesWrite;
			if(!WriteFile(hDiskFile, buffer, (DWORD)zReadSize, &dwBytesWrite, 0) || 
				dwBytesWrite != (DWORD)zReadSize)
			{
				zzip_file_close(zzipFile);
				zzip_dir_close(mZzipDir);
				CloseHandle(hDiskFile);
				return false;
			}

			//�ļ�����
			if(zzip_tell(zzipFile) >=zstat.st_size) break;

			zReadSize = zzip_file_read(zzipFile, buffer, sizeof(buffer));
		}while(true);
	}
	//�رվ��
	CloseHandle(hDiskFile); hDiskFile=0;
	zzip_file_close(zzipFile);	zzipFile=0;
	zzip_dir_close(mZzipDir); mZzipDir=0;

	return true;
}
