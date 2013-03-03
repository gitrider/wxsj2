
/** GIResourceProvider.h
*/


#pragma once


#include "GINode.h"



/** ��Դ�ṩϵͳ
*/
class KYLIN_API tResourceProvider : public tNode
{
public:

	virtual DWORD loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup = NULL) = 0;
	virtual BOOL unloadResource(LPSTR szMemAddr, DWORD dwSize) = 0;


	KL_DECLARE_DYNAMIC(tResourceProvider);

};
