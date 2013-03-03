//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef FoxFilePath_H
#define FoxFilePath_H
//---------------------------------------------------------------------------
#ifndef MAXPATH
#define MAXPATH   260
#define MAXDIR    256
#define MAXFILE   256
#define MAXEXT    256
#define MAXDRIVE    3
#endif
//---------------------------------------------------------------------------
ENGINE_API	void	g_SetRootPath(LPSTR lpPathName = NULL);
ENGINE_API	void	g_GetRootPath(LPSTR lpPathName);
ENGINE_API	void	g_SetFilePath(LPSTR lpPathName);
ENGINE_API	void	g_GetFilePath(LPSTR lpPathName);
ENGINE_API	void	g_GetFullPath(LPSTR lpPathName, LPSTR lpFileName);
ENGINE_API	void	g_GetHalfPath(LPSTR lpPathName, LPSTR lpFileName);
ENGINE_API	void	g_GetPackPath(LPSTR lpPathName, LPSTR lpFileName);
ENGINE_API	void	g_GetDiskPath(LPSTR lpPathName, LPSTR lpFileName);
ENGINE_API	void	g_CreatePath(LPSTR lpPathName);
// һ��·����һ���ļ������ϲ���lpGet���γ�һ��������·���ļ���
ENGINE_API	void	g_UnitePathAndName(char *lpPath, char *lpFile, char *lpGet);

//---------------------------------------------------------------------------
ENGINE_API	BOOL	g_FileExists(LPSTR lpPathName);
ENGINE_API	DWORD	g_FileName2Id(LPSTR lpFileName);
ENGINE_API	void	g_ChangeFileExt(LPSTR lpFileName, LPSTR lpNewExt);
ENGINE_API	void	g_ExtractFileName(LPSTR lpFileName, LPSTR lpFilePath);
ENGINE_API	void	g_ExtractFilePath(LPSTR lpPathName, LPSTR lpFilePath);
//---------------------------------------------------------------------------
#endif
