#include "StdAfx.h"
#include "AXP.h"

#include "AXPPackFile.h"
#include "AXPPackMaker.h"
#include "AXPPatchMaker.h"
#include "AXPUpdater.h"

namespace AXP
{

// Moved to their own implementation file.
#if 0

/************************************
			API接口
*************************************/

//创建一个Pak文件接口
IPakFile* createPakFile(void)
{
	return new PackFile;
}

//删除一个pak文件接口
void destroyPakFile(IPakFile* pFile)
{
	delete ((PackFile*)pFile);
}
	
//创建一个Maker接口
IPakMaker* createPakMaker(void)
{
	return (IPakMaker*)(new PakMaker);
}
//删除一个Maker接口
void destroyPakMaker(IPakMaker* pMaker)
{
	if(!pMaker) return;

	delete ((PakMaker*)pMaker);
}

//创建一个PatchMaker接口
IPatchMaker* createPatchMaker(void)
{
	return new PatchMaker;
}

//删除一个PatchMaker接口
void destroyPatchMaker(IPatchMaker* pPatchMaker)
{
	if(!pPatchMaker) return;

	delete ((PatchMaker*)pPatchMaker);
}

//创建一个Updater接口
IUpdater* createUpdater(void)
{
	return new Updater;
}

//删除一个PatchMaker接口
void destroyUpdater(IUpdater* pUpdater)
{
	if(!pUpdater) return;

	delete ((Updater*)pUpdater);
}

#endif

}


