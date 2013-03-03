/*****************************************************************************/
/* kpmLib.h                        Copyright (c) kylin 2007-2008			 */
/*---------------------------------------------------------------------------*/
/* kpmLib library v 1.00                                                     */
/*                                                                           */
/* Author : pengxiang zeng													 */
/* E-mail : zpxiang@263.net                                                  */
/* WWW    :																	 */
/*---------------------------------------------------------------------------*/
/*   Date     Verion   auth     Comment                                      */
/* --------   ----     ----     -------                                      */
/* 2008-2-22  1.00     zpx      Created                                      */
/*****************************************************************************/


#ifndef _KPMLIB_H_
#define _KPMLIB_H_

#include "kpmPlatform.h"
#include "kpmTypes.h"

namespace KPM
{
	/** Archive create/open/close														
		@param 
		dwHashTableSize 描述HashTable的大小的数据,更新包尽量小,而为客户端所使用的包尽量大
		wUseType	USE_TYPE_PATCH 表示创建补丁,USE_TYPE_CLIENT表示创建客户端包
	*/
	TKPMArchive*		KPM_CreateArchive(const char* szFilename,DWORD& oresult,DWORD dwHashTableSize,DWORD dwUseType);
	TKPMArchive*		KPM_OpenArchive(const char* szFilename,DWORD& oresult);
	bool				KPM_CloseArchive(TKPMArchive* hArchive);

	// File add/remove/rename
	BOOL				KPM_AddFile(TKPMArchive* hArchive, const char * szRealFileName,const char* szAliasFileName, 
									DWORD dwFlags,DWORD dwQuality, DWORD nFileType); 
	BOOL				KPM_RemoveFile(TKPMArchive* hArchive,const char * szAliasName);
	BOOL				KPM_RenameFile(TKPMArchive* hArchive, const char * szOldFileName, const char * szNewFileName);
	
	// File opening/closing
	TKPMFile*			KPM_FileOpen(TKPMArchive* hArchive, const char * szFileName, DWORD& oresult);
	DWORD				KPM_FileClose(TKPMFile* pFile);
	
	
	bool				KPM_ReadFile(TKPMFile* pFile, VOID * lpBuffer, DWORD dwToRead, DWORD * pdwReaded = NULL);
	// support update attr and list file;
	DWORD				KPM_GetFileSize(TKPMFile* pFile);
	DWORD				KPM_SetFilePointer(TKPMFile* pFile, LONG FileOffSet, DWORD dwMethod);
	DWORD				KPM_GetFilePos(TKPMFile* pFile);
	DWORD				KPM_FileSkip(TKPMFile* pFile,LONG Count);
	DWORD				KPM_FileSeek(TKPMFile* pFile,LONG Count);
	DWORD				KPM_FileTell(TKPMFile* pFile);
	DWORD				KPM_FileEof(TKPMFile* pFile);
	
	//File Search 
	TKPMSearch*			KPM_FindFirstFile(TKPMArchive* hArchive, const char * szMask,FILE_FIND_DATA * lpFindFileData);
	bool				KPM_FindNextFile(TKPMSearch* hFind, FILE_FIND_DATA * lpFindFileData);
	bool				KPM_SearchClose(TKPMSearch* hFind);
	
	//Set compress type
	DWORD				KPM_SetDataCompression(int nDataCompression);
	DWORD				KPM_GetDataCompression();

};



#endif


