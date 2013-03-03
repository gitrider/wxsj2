//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Win32 File Operation Class
//---------------------------------------------------------------------------
#include "FoxWin32.h"
#include "FoxFilePath.h"
#include "FoxFile.h"
#ifndef WIN32
//#include "zport.h"
#include "string.h"
#endif
//---------------------------------------------------------------------------
// ����:	FoxFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
FoxFile::FoxFile()
{
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}
//---------------------------------------------------------------------------
// ����:	~FoxFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
FoxFile::~FoxFile()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��һ���ļ���׼����ȡ
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
BOOL FoxFile::Open(LPSTR FileName)
{
	char PathName[MAXPATH];
	
	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL)
		Close();

	// get full path name
	g_GetFullPath(PathName, FileName);
/*#ifndef WIN32
	char *name_ptr = PathName;
	while(*name_ptr) {
		if(*name_ptr == '\\') *name_ptr = '/';
		name_ptr++;
	}
#endif
	// Open the file for read
	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_READ,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		OPEN_EXISTING,	// how to create
		FILE_ATTRIBUTE_NORMAL,// file attributes
		NULL);			// template file*/
/*
#ifndef WIN32
        char *ptr = PathName;
        while(*ptr) {
          if(*ptr == '\\') *ptr = '/';
          ptr++;
        }

		//open lower case file for linux
		char lcasePathName[MAXPATH];
		char szRootPath[MAXPATH];
		g_GetRootPath(szRootPath);
		strcpy(lcasePathName, PathName);
		if (memcmp(lcasePathName, szRootPath, strlen(szRootPath)) == 0)
			strlwr(lcasePathName + strlen(szRootPath));
		else
			strlwr(lcasePathName);
		if (NULL == (m_hFile = fopen(lcasePathName, "rb")))
#endif
*/
	m_hFile = fopen(PathName, "rb");

	// check file handle
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return FALSE;

	if (m_hFile == NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Create
// ����:	����һ���ļ���׼��д�롣
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
BOOL FoxFile::Create(LPSTR FileName)
{
	char PathName[MAXPATH];
	
	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL)
		Close();
	
	// get full path name
	g_GetFullPath(PathName, FileName);

	// change file attribute for write
//	SetFileAttributes(PathName, FILE_ATTRIBUTE_NORMAL);

	// create file for write
/*	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_WRITE,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		CREATE_ALWAYS,	// create or over write
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL);			// template file
*/	
	m_hFile = fopen(PathName, "wb+");
	// check file handle
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return FALSE;

	if (m_hFile == NULL)
		return FALSE;
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	Append
// ����:	��һ���ļ���׼���ں�������ݣ�������ļ������ڣ����������ļ���
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
BOOL FoxFile::Append(LPSTR FileName)
{
	char PathName[MAXPATH];
	
	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL)
		Close();
	
	// get full path name
	g_GetFullPath(PathName, FileName);

	// change file attribute for write
//	SetFileAttributes(PathName, FILE_ATTRIBUTE_NORMAL);

	// create file for write
/*	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_WRITE,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		OPEN_ALWAYS,	// Opens the file, if it exists. If the file does not exist, the function creates the file as if dwCreationDisposition were CREATE_NEW
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL);			// template file*/
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return FALSE;
	
	// check file handle
	m_hFile = fopen(PathName, "ab");
	if (m_hFile == NULL)
		return FALSE;

	Seek(0, FILE_END);
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	Close
// ����:	�رմ򿪵��ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void FoxFile::Close()
{
	//if (m_hFile != INVALID_HANDLE_VALUE)
		//CloseHandle(m_hFile);

	if (m_hFile)
		fclose((FILE*)m_hFile);
	
	//m_hFile	= (FILE *)INVALID_HANDLE_VALUE;
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}
//---------------------------------------------------------------------------
// ����:	Read
// ����:	��ȡ�ļ�����
// ����:	lpBuffer	��ȡ���ݴ�ŵ��ڴ�����
//			dwReadBytes	��ȡ���ݵ��ֽ���
// ����:	�ɹ����ض�ȡ���ֽ�����ʧ�ܷ���0��
//---------------------------------------------------------------------------
DWORD FoxFile::Read(LPVOID lpBuffer, DWORD dwReadBytes)
{
	DWORD dwBytesRead;
	
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;
	if (m_hFile == NULL)
		return 0;
	
//	ReadFile(m_hFile, lpBuffer, dwReadBytes, &dwBytesRead, NULL);
	dwBytesRead = fread(lpBuffer, 1, dwReadBytes, m_hFile);	
	m_dwPos += dwBytesRead;
	
	return dwBytesRead;
}
//---------------------------------------------------------------------------
// ����:	write
// ����:	д���ļ�����
// ����:	lpBuffer		д�����ݴ�ŵ��ڴ�����
//			dwWriteBytes	д�����ݵ��ֽ���
// ����:	�ɹ�����д����ֽ�����ʧ�ܷ���0��
//---------------------------------------------------------------------------
DWORD FoxFile::Write(LPVOID lpBuffer, DWORD dwWriteBytes)
{
	DWORD dwBytesWrite;
	
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;

	if (m_hFile == NULL)
		return 0;

	dwBytesWrite = fwrite(lpBuffer, 1, dwWriteBytes, m_hFile);	
//	WriteFile(m_hFile, lpBuffer, dwWriteBytes, &dwBytesWrite, NULL);
	
	m_dwPos += dwBytesWrite;
	
	return dwBytesWrite;
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ��ļ�ָ��λ��
// ����:	lDistance		�ƶ�����
//			dwMoveMethod	�ƶ�������FILE_BEGIN��FILE_CURRENT��FILE_END
// ����:	�ɹ�����ָ��λ�ã�ʧ�ܷ���SEEK_ERROR��
//---------------------------------------------------------------------------
DWORD FoxFile::Seek(LONG lDistance, DWORD dwMoveMethod)
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return SEEK_ERROR;

	if (m_hFile == NULL)
		return SEEK_ERROR;

//	m_dwPos = SetFilePointer(m_hFile, lDistance, NULL, dwMoveMethod);
	fseek(m_hFile, lDistance, dwMoveMethod);
	m_dwPos = ftell(m_hFile);
	return m_dwPos;
}
//---------------------------------------------------------------------------
// ����:	Tell
// ����:	ȡ���ļ�ָ��λ��
// ����:	void
// ����:	�ɹ�����ָ��λ�ã�ʧ�ܷ���SEEK_ERROR��
//---------------------------------------------------------------------------
DWORD FoxFile::Tell()
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return SEEK_ERROR;

	if (m_hFile == NULL)
		return SEEK_ERROR;

	return m_dwPos;
}
//---------------------------------------------------------------------------
// ����:	Size
// ����:	ȡ���ļ�����
// ����:	void
// ����:	�ɹ������ļ����ȣ�ʧ�ܷ���0��
//---------------------------------------------------------------------------
DWORD FoxFile::Size()
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;

	if (m_hFile == NULL)
		return 0;

	if (m_dwLen == 0) {
		DWORD temp = m_dwPos;
		m_dwLen = Seek(0, FILE_END);
		Seek(temp, FILE_BEGIN);
		//m_dwLen = GetFileSize(m_hFile, NULL);
	}

	return m_dwLen;
}
//---------------------------------------------------------------------------
