#pragma once

//脏话过滤功能
#include "GIKernel.h"
#include <set>
#include <vector>

// 新字符串过滤 [8/19/2006]
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
	//添加函数
	VOID				AddInclude(LPCSTR pNew){if(pNew) m_vIncluce.push_back(pNew);}
	VOID				AddFullCmp(LPCSTR pNew){if(pNew) m_sFullCmp.insert(pNew);}
	//清除函数
	VOID				Clean(){m_vIncluce.clear(); m_sFullCmp.clear();}

	//检查函数
	BOOL				IsInclude(const STRING& strIn);
	BOOL				IsFullCmp(const STRING& strIn);

	//替换函数，将非法字符替换成'?'
	VOID				ReplaceToSign(const STRING& strIn, STRING& strOut);
	//替换函数，将非法字符替换成'?'
	VOID				ReplaceToSign_Normal(const STRING& strIn, STRING& strOut);
	//替换函数，将非法字符替换成乱序的符号排列
	VOID				ReplaceToSign_New(const STRING& strIn, STRING& strOut);

	BOOL				IsIncludeOrFullCmp(const STRING& strIn){return (IsFullCmp(strIn)||IsInclude(strIn));}

protected:
	CStringFilter(){}
	virtual ~CStringFilter(){}
};
