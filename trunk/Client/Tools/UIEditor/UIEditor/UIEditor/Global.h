
#pragma  once
#include "UIStruct.h"
#include "UIEditorView.h"
#include "LeftTreeImage.h"
#include "LeftTreeWindow.h"

//#include "RightEventList.h"
//#include "RightPropList.h"


class CRightPropList;
class CRightEventList;
class CLuaDlg;
class CToolBarListDlg;
class CLayoutXMLPreviewDlg;

CWnd* GetShowWindow();
CRightPropList* GetRightPropList();
CRightEventList* GetRightEventList();
CLeftTreeWindow* GetLeftWindowTree();
CLeftTreeImage* GetLeftImageTree();

WCHAR* GetUnicode(const CHAR* pText);
CHAR* GetMBCS(const WCHAR* pUnicode);

WCHAR* GetString_PE(const CHAR* pText, INT pos);

char* mbcs_to_utf8(const char * lpGBKStr);
char* utf8_to_mbcs(const char* lpUtf8);

//void ApplyData(PE_List* pList, ENUM_WORK_FOR_WHO who);


BOOL GetIntergerFromList(PE_List* pList, INT* pOut, INT OutSize);

extern CUIEditorView * g_pEditorView;
extern INT g_PreActiveEditor;
extern INT g_ActiveEditor;
extern CLeftTreeWindow* g_leftTreeWindow ;
extern CLeftTreeImage*  g_leftTreeImage ;
extern CLuaDlg*			g_luaDlg;
extern CToolBarListDlg*	g_toolBarDlg;
extern CLayoutXMLPreviewDlg* g_xmlViewDlg;


struct AUTO_LOCALE_CONVERT
{
	AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};