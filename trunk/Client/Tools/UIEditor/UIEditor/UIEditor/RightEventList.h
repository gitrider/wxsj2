/*****************************************************************************
�ļ�:	RightEventList
����:	��Զ
����:	fangyuanning@126.com
˵��:	�ұߵ��¼�����
******************************************************************************/
#pragma once

#include "PropertyListBase.h"
// CRightEventList

class CRightEventList : public CPropertyListBase
{
	DECLARE_DYNAMIC(CRightEventList)

public:
	CRightEventList();
	virtual ~CRightEventList();
	void InsertItemS(const CHAR* szEventName, const CHAR* szParentName);
	void GetFunctionString(CString& outString, const CString& windowName);
	void GenSystemFunction(CString& outFunction, const CString& windowName);
	void UpdateWindowEventDefine(void);
	void OnTabLoseFocus();
	void SetCurrentFunctionPrefix(const CString& name);
	CString GetCurrentFunctionPrefix(void) { return m_nCurrentFunctionPrifexName; }

private:
	CString m_nCurrentFunctionPrifexName;

public:

protected:
	
	DECLARE_MESSAGE_MAP()

protected:
	void GenBaseFunction(CString& outFunction);
	void GenExtentionFunction(CString& outFunction);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


