
/*
|
|	�ı���Դ������
|
*/

#pragma once


#include "GIBasicType.h"
#include "GIUISystem.h"
#include <hash_map>
#include <set>
#include <CEGUIWindowManager.h>
#include "ceguiwindow.h"
#include <CEGUISystem.h>
#include "UIString_Filter.h"


struct UI_FACECODE
{
	const CHAR*		szCode;
	CEGUI::utf32	utfCode;
};

class CUIStringSystem
{
	// ���ֹ��˽ṹ
	struct stStrFilter 
	{
		STRING	szString;			// ���˴ʻ�
		// ���˹���
		// 0��ʾ���ԣ�1��ʾ���о͹��ˣ�2��ʾ��ȫƥ�����
		INT		chatFilter;			// ����ʱ����
		INT		roleFilter;			// ��������ʱ����
		INT		nameFilter;			// ����ʱ����
		INT		stallFilter;		// ��̯ʱ����
	};

public:
	//ת���ַ���
	VOID				ParserString_Prebuild(const STRING& strSource, STRING& strOut);
	VOID				ParserString_Runtime(const STRING& strSource, CEGUI::String32& strOut, const FILTER_TYPE filterType = FT_NONE);
	STRING				ParserString_VarParam(const STRING& strID, va_list ptr);
	VOID				ParserString_NoColor(const STRING& strSrc, STRING&strOut, BOOL bControl = FALSE);
	STRING				ParserString_NoVarParam(const STRING& strID);

	//��Ʒ����ϵͳר��
	VOID				ParserString_ReverseRuntime(const CEGUI::String32& strIn, STRING& strOut);
	VOID				ParserString_ElementOnly_Runtime(const STRING& strSource, CEGUI::String32& strOut);

	//����ģ���ִ���ȡ��
	STRING				getTalkTemplate(const STRING& strKey, UINT index);
	STRING				ModifyChatTemplate(const STRING& strSrc, const STRING& strTalker, const STRING& strTarget);

	STRING				getTalkRandHelpMsg();

	//utf<->mbcs
	static VOID		mbcs_to_utf8(const std::string& strIn, std::string& strOut);
	static VOID		utf8_to_mbcs(const std::string& strIn, std::string& strOut);
	std::string		string_check_valid(const char* szString);

public:
	//��ʼ��
	VOID				Init(LPCTSTR szStringDBC = "../Config/Dictionary.txt");

	//�����ַ����еķǷ��ַ�
	BOOL	_CheckStringFilter(const STRING& strIn, const FILTER_TYPE filterType = FT_NONE );
	//��ȫƥ�����
	BOOL	_CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType = "all", BOOL bUseAllTable = TRUE);
	//����ַ����ĺϷ���
	BOOL	_CheckStringCode(const STRING& strIn, STRING& strOut);
	VOID	_ReplaceStringUseFilter(const STRING& strIn, STRING& strOut, INT nFilterType /* = STR_FILTER_TALK */);

public:
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, STRING >			DIRECTIONARY;
#else
	typedef stdext::hash_map< STRING, STRING >		DIRECTIONARY;
#endif

	typedef std::map< INT, CEGUI::utf32 >		CODE_CONVERT;

	typedef	std::vector< stStrFilter >				CODE_FILTER;

	typedef std::vector< STRING >					UI_TALK_TEMPLATE;
	typedef std::map< STRING, UI_TALK_TEMPLATE >	TALK_TEMPLATE_MAP;
	typedef std::set<STRING>						UI_FULLCMP_TEMPLATE;
	typedef	std::map< STRING, UI_FULLCMP_TEMPLATE >	FULLCMP_MAP;

protected:
	//------------------------------------
	//�ַ����ֵ�
	DIRECTIONARY		m_theDirectionary;
	CODE_CONVERT		m_mapCodeConvertTable;
	CODE_FILTER			m_vecFilterTable;
	TALK_TEMPLATE_MAP	m_talkTemplateTable;
	FULLCMP_MAP			m_fullCompareTable;
	CStringFilter		m_allFilter[FT_MAX];

protected:
	std::pair< BOOL, DIRECTIONARY::iterator >	_IsValidDictionKey(const STRING& strKey);
	//����ַ����ĺϷ���
	//�ᵽpublic��
//	BOOL	_CheckStringCode(const STRING& strIn, STRING& strOut);
	
public:
	CUIStringSystem();
	virtual ~CUIStringSystem();

	static CUIStringSystem*	GetMe(VOID) { return s_pMe; }
private:
	VOID				ParserString_RuntimeNew(const STRING& strSource, CEGUI::String32& strOut, const FILTER_TYPE filterType = FT_NONE);
	VOID				GenerateTalkTemplateTable();
	VOID				GenerateFullCompareTable();
protected:
	static CUIStringSystem*	s_pMe;
};
