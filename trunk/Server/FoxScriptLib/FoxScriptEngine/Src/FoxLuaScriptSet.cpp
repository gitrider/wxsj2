//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	
//---------------------------------------------------------------------------
#include "FoxWin32.h"

/*
#include "FoxDebug.h"
#include "FoxLuaScriptSet.h"

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::FoxLuaScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
FoxLuaScriptSet::FoxLuaScriptSet()
{
	FoxLuaScriptSet::FoxScriptSet();
	m_nMaxScriptNum = MAXLUASCRIPTNUM;
	m_nNumPerClear =  NUMPERCLEAR;
}

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::~FoxLuaScriptSet
// ����:	
// ����:	
//---------------------------------------------------------------------------
FoxLuaScriptSet::~FoxLuaScriptSet()
{
	FoxScriptSet::~FoxScriptSet();
}

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::CreateScript
// ����:	
// ����:	char * szKey
// ����:	int StackSize
// ����:	FoxScript * 
//---------------------------------------------------------------------------
FoxScript * FoxLuaScriptSet::CreateScript(char * szKey, int StackSize)
{
	//����
	ClearUpSet();	
	if (szKey == NULL && StackSize < 0) return NULL;
	char * szFileName =  GetScriptFileNameFromKey(szKey);
	if (szFileName == NULL ) return NULL;
	
	FoxLuaScript * pScript = new FoxLuaScript(StackSize);
	if (! pScript) return NULL;
	
	//�ű�����Ľű���Ϊ�ؼ�����
	if (! pScript->Init()) return NULL;
	if (! pScript->Load(szFileName)) return NULL;
	return pScript;
}

FoxScript * FoxLuaScriptSet::CreateScript(DWORD nKey, int StackSize)
{
	//����
	ClearUpSet();	
	if (StackSize < 0) return NULL;
	char * szFileName =  GetScriptFileNameFromKey(nKey);
	if (szFileName == NULL ) return NULL;
	
	FoxLuaScript * pScript = new FoxLuaScript(StackSize);
	if (! pScript) return NULL;
	
	//�ű�����Ľű���Ϊ�ؼ�����
	if (! pScript->Init()) return NULL;
	if (! pScript->Load(szFileName)) return NULL;
	
	return pScript;
}


//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::ClearUpSet
// ����:	
// ����:	void
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL FoxLuaScriptSet::ClearUpSet(void)
{
	
	TScript * pTScript = NULL;
	TScript * pDelTScript = NULL;

	if (GetCount() >= m_nMaxScriptNum) 
	{
		pTScript = m_pLestRecent;
//	printf("ClearUpSet\n");
		for (DWORD i = 0; i < m_nNumPerClear;i++)
		{
			pDelTScript = pTScript;
			pTScript = pTScript->pMoreRecent;
			DeleteScript(pDelTScript->szKey);
		}
	
		return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::ClearUpSet
// ����:	�������ʹ�ýű���˳��ɾ��nCount���ű�ʵ��
// ����:	DWORD nCount
// ����:	int ����ʵ��ɾ���Ľű�����
//---------------------------------------------------------------------------
DWORD FoxLuaScriptSet::ClearUpSet(DWORD nCount)
{
	
	TScript * pTScript = NULL;
	TScript * pDelTScript = NULL;
	
	DWORD	nUsedCount = GetCount();
	
	if (nUsedCount < nCount)
		nCount = nUsedCount;

		pTScript = m_pLestRecent;
		
		for (DWORD i = 0; i < nCount; i++)
		{
			pDelTScript = pTScript;
			pTScript = pTScript->pMoreRecent;
			DeleteScript(pDelTScript->szKey);
		}
		
	return nCount;

}

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::SetMaxScriptNum
// ����:	
// ����:	DWORD nMaxCount
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL FoxLuaScriptSet::SetMaxScriptNum(DWORD nMaxCount )
{
	if (nMaxCount < 0)
		return FALSE;
	
	m_nMaxScriptNum = nMaxCount;
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	FoxLuaScriptSet::SetNumPerClear
// ����:	
// ����:	DWORD nNumPerClear
// ����:	BOOL 
//---------------------------------------------------------------------------
BOOL FoxLuaScriptSet::SetNumPerClear(DWORD nNumPerClear)
{
	if (nNumPerClear < 0 )
		return FALSE;
	
	if (m_nMaxScriptNum < nNumPerClear)
		return FALSE;
	
	m_nNumPerClear = nNumPerClear;
	return TRUE;
}


BOOL FoxLuaScriptSet::Run(DWORD nKey)
{
	FoxLuaScript *pScript ;
	pScript = (FoxLuaScript* ) this->GetScript(nKey,1,1);
	if (pScript)
	{
		return pScript->Execute();
	}
	return FALSE;
	
}

BOOL FoxLuaScriptSet::Run(char * szKey)
{
	FoxLuaScript *pScript ;
	pScript = (FoxLuaScript* ) this->GetScript(szKey,1,1);
	if (pScript)
	{
		return pScript->Execute();
	}
	return FALSE;
	
}


BOOL FoxLuaScriptSet::RunFunction(DWORD nKey, char * szFuncName, int nResults, char * szFormat, ...)
{
	FoxLuaScript * pScript;
	pScript = (FoxLuaScript *) this->GetScript(nKey, 1, 1);
	if (pScript)
	{
		va_list va;
		BOOL bResult;
		va_start(va, szFormat);
		bResult = pScript->CallFunction(szFuncName, nResults, szFormat, va);
		va_end(va);
		return bResult;

	}
	return FALSE;

}

BOOL FoxLuaScriptSet::RunFunction(char * szKey, char * szFuncName, int nResults, char * szFormat, ...)
{
	FoxLuaScript * pScript;
	pScript = (FoxLuaScript *) this->GetScript(szKey, 1, 1);
	
	if (pScript)
	{
		va_list va;
		BOOL bResult;
		va_start(va, szFormat);
		bResult = pScript->CallFunction(szFuncName, nResults, szFormat, va);
		va_end(va);
		return bResult;
		
	}
	return FALSE;

}

*/
