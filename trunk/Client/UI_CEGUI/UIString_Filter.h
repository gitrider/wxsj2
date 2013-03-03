#pragma once

//�໰���˹���
#include "GIKernel.h"
#include <set>
#include <vector>

// ���ַ������� [8/19/2006]
class CStringFilter
{
	friend class CUIStringSystem;

protected:
	typedef	std::vector<STRING>		INCLUDE_FILTER;
	typedef std::set<STRING>		FULLCMP_FILTER;

	struct PICE
	{
		STRING		pice;
		BOOL		bReplace;
	};

	INCLUDE_FILTER		m_vIncluce;
	FULLCMP_FILTER		m_sFullCmp;

public:
	//��Ӻ���
	VOID				AddInclude(LPCSTR pNew){if(pNew) m_vIncluce.push_back(pNew);}
	VOID				AddFullCmp(LPCSTR pNew){if(pNew) m_sFullCmp.insert(pNew);}
	//�������
	VOID				Clean(){m_vIncluce.clear(); m_sFullCmp.clear();}

	//��麯��
	BOOL				IsInclude(const STRING& strIn);
	BOOL				IsFullCmp(const STRING& strIn);

	//�滻���������Ƿ��ַ��滻��'?'
	VOID				ReplaceToSign(const STRING& strIn, STRING& strOut);
	//�滻���������Ƿ��ַ��滻��'?'
	VOID				ReplaceToSign_Normal(const STRING& strIn, STRING& strOut);
	//�滻���������Ƿ��ַ��滻������ķ�������
	VOID				ReplaceToSign_New(const STRING& strIn, STRING& strOut);

	BOOL				IsIncludeOrFullCmp(const STRING& strIn){return (IsFullCmp(strIn)||IsInclude(strIn));}

protected:
	CStringFilter(){}
	virtual ~CStringFilter(){}
};
