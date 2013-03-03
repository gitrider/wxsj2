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
			API�ӿ�
*************************************/

//����һ��Pak�ļ��ӿ�
IPakFile* createPakFile(void)
{
	return new PackFile;
}

//ɾ��һ��pak�ļ��ӿ�
void destroyPakFile(IPakFile* pFile)
{
	delete ((PackFile*)pFile);
}
	
//����һ��Maker�ӿ�
IPakMaker* createPakMaker(void)
{
	return (IPakMaker*)(new PakMaker);
}
//ɾ��һ��Maker�ӿ�
void destroyPakMaker(IPakMaker* pMaker)
{
	if(!pMaker) return;

	delete ((PakMaker*)pMaker);
}

//����һ��PatchMaker�ӿ�
IPatchMaker* createPatchMaker(void)
{
	return new PatchMaker;
}

//ɾ��һ��PatchMaker�ӿ�
void destroyPatchMaker(IPatchMaker* pPatchMaker)
{
	if(!pPatchMaker) return;

	delete ((PatchMaker*)pPatchMaker);
}

//����һ��Updater�ӿ�
IUpdater* createUpdater(void)
{
	return new Updater;
}

//ɾ��һ��PatchMaker�ӿ�
void destroyUpdater(IUpdater* pUpdater)
{
	if(!pUpdater) return;

	delete ((Updater*)pUpdater);
}

#endif

}


