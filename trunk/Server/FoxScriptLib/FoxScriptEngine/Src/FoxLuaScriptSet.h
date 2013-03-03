//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	
//---------------------------------------------------------------------------
#ifndef FOXLUASCRIPTSET_H
#define FOXLUASCRIPTSET_H
//---------------------------------------------------------------------------

/*
#include "FoxScriptSet.h"
#define MAXLUASCRIPTNUM  10
#define NUMPERCLEAR 3

//---------------------------------------------------------------------------
class ENGINE_API FoxLuaScriptSet : public FoxScriptSet
{
public:
	FoxLuaScriptSet();
	~FoxLuaScriptSet();

	FoxScript * CreateScript(char * szKey, int StackSize);
	FoxScript * CreateScript(DWORD nKey, int StackSize);
	BOOL ClearUpSet(void);//���нű�����⣬һ�����ֳ�����ִ�����
	DWORD ClearUpSet(DWORD);//���ָ�������Ľű�
	DWORD GetMaxScriptNum(){return m_nMaxScriptNum;	}
	DWORD GetNumPerClear(){return m_nNumPerClear;	}
	BOOL SetMaxScriptNum(DWORD nMaxCount = MAXLUASCRIPTNUM);
	BOOL SetNumPerClear(DWORD nNumPerClear = NUMPERCLEAR);


	virtual BOOL Run(DWORD nKey);
	virtual BOOL Run(char * szKey);
	virtual BOOL RunFunction(DWORD nKey, char * szFuncName, int nResults, char * szFormat, ...);
	virtual BOOL RunFunction(char * szKey, char * szFuncName, int nResults, char * szFormat, ...);
	

private:
	DWORD m_nMaxScriptNum;//���ű�������
	DWORD m_nNumPerClear;//ÿ������ű�����ʱ�������
};
//---------------------------------------------------------------------------
*/
#endif //FOXLUASCRIPTSET_H
