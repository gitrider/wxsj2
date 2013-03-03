#ifndef _UTIL_H_
#define _UTIL_H_

#include "kpmLib.h"
namespace KPM
{




	void FreeArchive(TKPMArchive *& ha);
	BOOL IsValidArchiveHandle(TKPMArchive * ha);

	void FreeFile(TKPMFile *& hf);
	BOOL IsValidFileHandle(TKPMFile * hf);

	void FreeAttributes(TKPMAttr * pAttr);

	int		SaveListFile(TKPMArchive * ha,BOOL bInternal);
	int		SaveListFile(TKPMArchive * ha);

	int		SaveKPMTables(TKPMArchive * ha);
	DWORD	SaveAttrFile(TKPMArchive * ha,BOOL bInternal);
	DWORD	SaveAttrFile(TKPMArchive * ha);


	int		AddFileToArchive(TKPMArchive * ha, HANDLE hFile, const char * szArchivedName, 
								DWORD dwFlags, DWORD dwQuality, int nFileType, BOOL * pbReplaced);


	int		InternalUpdateFile(TKPMArchive * ha, HANDLE hFile,TKPMFile* pFile, const char * szArchivedName, 
								DWORD dwFlags, DWORD dwQuality, int nFileType, BOOL * pbReplaced);

	//internal functions
	DWORD	KPM_AddInternalFile(TKPMArchive* pArch,const char* filename);


	

}

#endif