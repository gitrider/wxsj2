#include "stdafx.h"
#include "MainFrm.h"
#include "Global.h"
#include "UIEditor.h"

static CHAR* pMBCSBuf = NULL;
static WCHAR* pUnicodeBuf = NULL;
static CHAR* pUtf8Buf = NULL;
static UINT nSizeMBCS = 0;
static UINT nSizeUnicode = 0;
static UINT nSizeUtf8 = 0;
//INT g_PreActiveEditor = 0 ;		//0  none 1 Combobox 2 Edit
INT g_ActiveEditor = 0;

CLeftTreeWindow*	g_leftTreeWindow = NULL;
CLeftTreeImage*     g_leftTreeImage = NULL;
CLuaDlg*			g_luaDlg = NULL;
CToolBarListDlg*	g_toolBarDlg = NULL;
CLayoutXMLPreviewDlg* g_xmlViewDlg = NULL;

CRightPropList* GetRightPropList()
{
	return ((CMainFrame*)AfxGetMainWnd())->GetRightView()->GetRightTab()->GetPropList();
}
CRightEventList* GetRightEventList()
{
	return ((CMainFrame*)AfxGetMainWnd())->GetRightView()->GetRightTab()->GetEventList();
}

CLeftTreeWindow* GetLeftWindowTree()
{
	return g_leftTreeWindow;
}
CLeftTreeImage* GetLeftImageTree()
{
	return g_leftTreeImage;
}

WCHAR* GetUnicode(const CHAR* pText)
{
	UINT nCount = MultiByteToWideChar(936,0,pText, -1, NULL, 0 );
	if (nCount > nSizeUnicode)
	{
		delete pUnicodeBuf;
		pUnicodeBuf = new WCHAR[nCount *2];
		nSizeUnicode = nCount*2;
	}
	memset(pUnicodeBuf, 0, sizeof(WCHAR)*nSizeUnicode);
	MultiByteToWideChar(936,0,pText, -1, pUnicodeBuf, nSizeUnicode );
	return pUnicodeBuf;
}
CHAR* GetMBCS(const WCHAR* pUnicode)
{
	UINT nCount = WideCharToMultiByte(936,0,pUnicode, -1, NULL, 0 ,0, FALSE);
	if (nCount > nSizeMBCS)
	{
		delete pMBCSBuf;
		pMBCSBuf = new CHAR[nCount*2];
		nSizeMBCS = nCount *2;
	}
	memset(pMBCSBuf, 0 , sizeof(CHAR)*nSizeMBCS);
	WideCharToMultiByte(936,0,pUnicode, -1, pMBCSBuf, nSizeMBCS ,0, FALSE);
	return pMBCSBuf;
}

WCHAR* GetString_PE(const CHAR* pText, INT pos)
{
	INT i = 0, j= 0;
	for (i=0; j<pos; i++)
	{
		if (pText[i] == '\0')
		{
			j++;
		}
	}
	return GetUnicode(&pText[i]);
}
CUIEditorView * g_pEditorView = NULL;
CUIEditorView* GetUIEditorView()
{
	return g_pEditorView;
}

//GBK编码转换到UTF8编码
int GBK_To_Utf8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpGBKStr)  //如果GBK字符串为NULL则出错退出
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,NULL,NULL);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
	if(!nRetLen)  //转换失败则出错退出
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,NULL,0,NULL,NULL);  //获取转换到UTF8编码后所需要的字符空间长度

	if(!lpUTF8Str)  //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)       
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nUTF8StrLen < nRetLen)  //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL);  //转换到UTF8编码

	if(lpUnicodeStr)
		delete []lpUnicodeStr;

	return nRetLen;
}

char* mbcs_to_utf8(const char * lpGBKStr)
{
	int nRetLen = 0;
	nRetLen = GBK_To_Utf8((unsigned char *) lpGBKStr,NULL,NULL);
	if (nSizeUtf8 < nRetLen)
	{
		delete[] pUtf8Buf;
		pUtf8Buf = new char[2*nRetLen + 1];
		nSizeUtf8 = 2*nRetLen;
	}
	nRetLen = GBK_To_Utf8((unsigned char *)lpGBKStr,(unsigned char *)pUtf8Buf,nRetLen);
	return pUtf8Buf;
}

char* utf8_to_mbcs(const char* lpUtf8)
{
	INT nRetLen = 0;
	wchar_t * lpUnicodeStr = NULL;
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUtf8,-1,NULL,NULL);  //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUtf8,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
	if(!nRetLen)  //转换失败则出错退出
	{
		delete[] lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,0,NULL,NULL);  //转换到UTF8编码
	if(nSizeMBCS < nRetLen)
	{
		delete[] pMBCSBuf;
		pMBCSBuf = new char[nRetLen*2];
	}
	::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,pMBCSBuf,nRetLen*2,NULL,NULL);  //转换到UTF8编码
	delete[] lpUnicodeStr;
	return pMBCSBuf;
}