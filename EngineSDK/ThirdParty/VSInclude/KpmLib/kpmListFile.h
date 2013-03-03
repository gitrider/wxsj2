#ifndef _KPM_LISTFILE_H_
#define _KPM_LISTFILE_H_
#include "kpmPlatform.h"
#include "kpmLib.h"
namespace KPM
{
	


	class ListFile
	{
	public:

		static char * GetListFileName();
	};
	
	DWORD	KPM_CreateListFile(TKPMArchive* pArch);
	DWORD	KPM_AddListFileClient(TKPMArchive* pArch, const char * szListFile);
	DWORD	KPM_AddListFilePatch(TKPMArchive* pArch, const char * szListFile);

	DWORD	KPM_AddNode2ListFile(TKPMArchive* pArch,const char* szfile);



	DWORD	KPM_ListFileRemoveNode(TKPMArchive * pArch, const char * szFileName);
	DWORD	KPM_ListFileRenameNode(TKPMArchive * pArch, const char * szOldFileName, const char * szNewFileName);

	DWORD	KPM_CreateAttrFile(TKPMArchive* pArch);
	DWORD	KPM_LoadAttrFileClient(TKPMArchive * ha);
	DWORD	KPM_LoadAttrFilePatch(TKPMArchive * ha);



}

#endif