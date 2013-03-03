
/** 资源提供系统
 */

#pragma once


#include "GIResourceProvider.h"



namespace Ogre
{
	class PakFileSystemArchive;
	class ResourceGroupManager;
	class OgreStringVector;
};

class CResourceProvider : public tResourceProvider
{
public:

	CResourceProvider(VOID);
	virtual ~CResourceProvider(VOID);
	virtual DWORD loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup = NULL);
	virtual BOOL unloadResource(LPSTR szMemAddr, DWORD dwSize);

protected:

	KL_DECLARE_DYNAMIC(CResourceProvider);

};	// class
	
