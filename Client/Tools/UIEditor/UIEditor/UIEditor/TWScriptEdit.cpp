/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
// TWScriptEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TWScriptEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_RICH_UNDO                    101
#define ID_RICH_CUT                     102
#define ID_RICH_COPY                    103
#define ID_RICH_PASTE                   104
#define ID_RICH_CLEAR                   105
#define ID_RICH_SELECTALL               106
#define ID_RICH_SETFONT                 107

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit

CTWScriptEdit::CTWScriptEdit()
{
	m_chComment = 1;
	m_bCaseSensitive = FALSE;
	m_bChangeCase = TRUE;

	SetStringQuotes(L"\"");

	SetKeywordColor(RGB(0,0,255), FALSE);
	SetConstantColor(RGB(0,0,0), TRUE);
	SetCommentColor(RGB(0,128,0), FALSE);
	SetNumberColor(RGB(255,0,255), FALSE);
	SetStringColor(RGB(255,0,255), FALSE);

	m_bInForcedChange = FALSE;
	m_changeType = ctUndo;
	m_crOldSel.cpMin = m_crOldSel.cpMax = 0;
}

CTWScriptEdit::~CTWScriptEdit()
{
}

void CTWScriptEdit::Initialize() 
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = MAX_TAB_STOPS;
	for( int itab = 0 ; itab < pf.cTabCount  ; itab++ )
		pf.rgxTabs[itab] = (itab + 1) * 1440/5 ;

	SetParaFormat( pf );

	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED; 
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0xEE; 
	strcpy(cfDefault.szFaceName, _T("Courier New")); 

	SetDefaultCharFormat(cfDefault);
	
	SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_PROTECTED);
}

void CTWScriptEdit::SetSLComment(WCHAR chComment, WCHAR chComment2)
{
	m_chComment = chComment;
	m_chComment2 = chComment2;
}

void CTWScriptEdit::SetSLComment(LPCWSTR lpszComment)
{
	m_strComment = lpszComment;
}

void CTWScriptEdit::AddKeywords(LPCWSTR lpszKwd)
{
	m_strKeywords = m_strKeywords + lpszKwd;
	m_strKeywordsLower = m_strKeywords;
	if (!m_bCaseSensitive)
		m_strKeywordsLower.MakeLower();
}

void CTWScriptEdit::ClearKeywords()
{
	m_strKeywords.Empty();
	m_strKeywordsLower.Empty();
}							  

void CTWScriptEdit::AddConstants(LPCWSTR lpszConst)
{
	m_strConstants = m_strConstants + lpszConst;
	m_strConstantsLower = m_strConstants;
	if (!m_bCaseSensitive)
		m_strConstantsLower.MakeLower();
}

void CTWScriptEdit::ClearConstants()
{
	m_strConstants.Empty();
	m_strConstantsLower.Empty();
}							  

void CTWScriptEdit::SetCaseSensitive(BOOL bSensitive)
{
	m_bCaseSensitive = bSensitive;
}

void CTWScriptEdit::SetChangeCase(BOOL bChange)
{
	m_bChangeCase = bChange;
}

void CTWScriptEdit::SetStringQuotes(LPCWSTR lpszStrQ)
{
	m_strStringQuotes = lpszStrQ;
}

void CTWScriptEdit::SetKeywordColor(COLORREF clr, BOOL bBold)
{
	m_icKeyword.clrColor = clr;
	m_icKeyword.bBold = bBold;
}

void CTWScriptEdit::SetConstantColor(COLORREF clr, BOOL bBold)
{
	m_icConstant.clrColor = clr;
	m_icConstant.bBold = bBold;
}

void CTWScriptEdit::SetCommentColor(COLORREF clr, BOOL bBold)
{
	m_icComment.clrColor = clr;
	m_icComment.bBold = bBold;
}

void CTWScriptEdit::SetNumberColor(COLORREF clr, BOOL bBold)
{
	m_icNumber.clrColor = clr;
	m_icNumber.bBold = bBold;
}

void CTWScriptEdit::SetStringColor(COLORREF clr, BOOL bBold)
{
	m_icString.clrColor = clr;
	m_icString.bBold = bBold;
}

BEGIN_MESSAGE_MAP(CTWScriptEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTWScriptEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_PROTECTED, OnProtected)
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_RICH_COPY, OnCopy)
	ON_COMMAND(ID_RICH_CUT, OnCut)
	ON_COMMAND(ID_RICH_PASTE, OnPaste)
	ON_COMMAND(ID_RICH_SELECTALL, OnSelectall)
	ON_COMMAND(ID_RICH_UNDO, OnUndo)
	ON_COMMAND(ID_RICH_CLEAR, OnClear)
	ON_COMMAND(ID_RICH_SETFONT, OnSelectfont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit message handlers

UINT CTWScriptEdit::OnGetDlgCode() 
{
	UINT uCode = CRichEditCtrl::OnGetDlgCode();
	
	uCode = DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTMESSAGE | DLGC_WANTTAB;

	return uCode;
}

void CTWScriptEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == '\t' && GetKeyState(VK_CONTROL) >= 0) {
		ReplaceSel("\t");
		return;
	}
	
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}


int CTWScriptEdit::IsKeyword(LPCWSTR lpszSymbol)
{
	CStringW strSymbol; strSymbol.Format(L" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strKeywordsLower.Find(strSymbol);
}

int CTWScriptEdit::IsConstant(LPCWSTR lpszSymbol)
{
	CStringW strSymbol; strSymbol.Format(L" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strConstantsLower.Find(strSymbol);
}

BOOL CTWScriptEdit::IsStringQuote(WCHAR ch)
{
	return (m_strStringQuotes.Find(ch) >= 0);
}


void CTWScriptEdit::SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr)
{
	if (nStart >= nEnd)
		return;

	SetSel(nStart, nEnd);

	DWORD dwEffects = bBold?CFE_BOLD:0;

	CHARFORMAT cfm;
	cfm.cbSize = sizeof(cfm);
    GetSelectionCharFormat(cfm);
	
	if ((cfm.dwMask & CFM_COLOR)  && cfm.crTextColor == clr && 
		(cfm.dwMask & CFM_BOLD) && (cfm.dwEffects & CFE_BOLD) == dwEffects)
		return;

	cfm.dwEffects = dwEffects;
	cfm.crTextColor = clr;
	cfm.dwMask = CFM_BOLD | CFM_COLOR;

	SetSelectionCharFormat(cfm);
}

void CTWScriptEdit::ChangeCase(int nStart, int nEnd, LPCWSTR lpsz)
{
	ASSERT((nEnd - nStart) == (int)_tcslen(lpsz));

	if (!m_bCaseSensitive && m_bChangeCase) {
		SetSel(nStart, nEnd);
		ReplaceSel(GetMBCS(lpsz));
	}
}

void CTWScriptEdit::FormatTextRange(int nStart, int nEnd)
{
	if (nStart >= nEnd)
		return;

	m_bInForcedChange = TRUE;

	CHARRANGE crOldSel;

	GetSel(crOldSel);
	LockWindowUpdate();
	HideSelection(TRUE, FALSE);

	WCHAR *pBuffer = NULL;
	try {
		SetSel(nStart, nEnd);
		//pBuffer = new WCHAR[nEnd - nStart + 1];
		CHAR* pBuffer2 = new CHAR[nEnd - nStart + 1];
		long nLen = GetSelText(pBuffer2);
		pBuffer = GetUnicode(pBuffer2);
		ASSERT(nLen <= nEnd - nStart);

		pBuffer[nLen] = 0;

		WCHAR *pStart, *pPtr;
		pStart = pPtr = pBuffer;

		WCHAR* pSymbolStart = NULL;
		SymbolColor ic;

		while (*pPtr != 0) {
			WCHAR ch = *pPtr;

			if (ch == m_chComment && (m_chComment2 == 0 || pPtr[1] == m_chComment2)) {
				pSymbolStart = pPtr;
				do {
					ch = *(++pPtr);
				} while (ch != 0 && ch != '\r');
				ic = m_icComment;
			} else if (IsStringQuote(ch)) { // Process strings
				pSymbolStart = pPtr;
				WCHAR ch1 = ch;
				do {
					ch = *(++pPtr);
				} while (ch != 0 && ch != ch1 && ch != '\r');
				if (ch == ch1) pPtr++;
				ic = m_icString;
			} else if (_istdigit(ch)) { // Process numbers
				pSymbolStart = pPtr;
				wcstod(pSymbolStart, &pPtr);
				ic = m_icNumber;
			} else if (_istalpha(ch) || ch == '_') { // Process keywords
				pSymbolStart = pPtr;
				do {
					ch = *(++pPtr);
				} while (_istalnum(ch) || ch == '_');
				*pPtr = 0;
				int nPos = IsKeyword(pSymbolStart);
				if (nPos >= 0) {
					ChangeCase(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, 
								m_strKeywords.Mid(nPos+1, pPtr - pSymbolStart));
					if (wcsicmp(m_strComment, pSymbolStart) == 0) {
						*pPtr = ch;
						*pSymbolStart = m_chComment;
						if (pSymbolStart[1] != 0 && m_chComment2 != 0)
							pSymbolStart[1] = m_chComment2;
						pPtr = pSymbolStart;
						pSymbolStart = NULL;
						continue;
					}
					ic = m_icKeyword;
				} else {
					nPos = IsConstant(pSymbolStart);
					if (nPos >= 0) {
							ChangeCase(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, 
										m_strConstants.Mid(nPos+1, pPtr - pSymbolStart));
						ic = m_icConstant;
					} else {
						pSymbolStart = NULL;
					}
				}
				*pPtr = ch;
			} else {
				pPtr++;
			}

			if (pSymbolStart != NULL) {
				ASSERT(pSymbolStart < pPtr);
				SetFormatRange(nStart + pStart - pBuffer, nStart + pSymbolStart - pBuffer, FALSE, RGB(0,0,0));
				SetFormatRange(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, ic.bBold, ic.clrColor);
				pStart = pPtr;
				pSymbolStart = 0;
			} else if (*pPtr == 0)
				SetFormatRange(nStart + pStart - pBuffer, nStart + pPtr - pBuffer, FALSE, RGB(0,0,0));
		}

	} catch(...){}

	//delete [] pBuffer;

	SetSel(crOldSel);
	HideSelection(FALSE, FALSE);
	UnlockWindowUpdate();

	m_bInForcedChange = FALSE;
}

void CTWScriptEdit::FormatTextLines(int nLineStart, int nLineEnd)
{
	long nStart = LineIndex(LineFromChar(nLineStart));
	long nEnd = LineIndex(LineFromChar(nLineEnd));
	nEnd += LineLength(nLineEnd);

	FormatTextRange(nStart, nEnd);
}


void CTWScriptEdit::FormatAll()
{
	FormatTextRange(0, GetTextLength());
}

LRESULT CTWScriptEdit::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT res = Default();
	FormatAll();
	return res;	
}

void CTWScriptEdit::OnChange() 
{
	if (m_bInForcedChange)
		return;

	CHARRANGE crCurSel; 
	GetSel(crCurSel);

	if (m_changeType == ctMove && crCurSel.cpMin == crCurSel.cpMax) {
		// cut was canceled, so this is paste operation
		m_changeType = ctPaste;
	}

	switch(m_changeType) {
	case ctReplSel:// old=(x,y) -> cur=(x+len,x+len)
	case ctPaste:  // old=(x,y) -> cur=(x+len,x+len)
		FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);
		break;
	case ctDelete: // old=(x,y) -> cur=(x,x)
	case ctBack:   // old=(x,y) -> cur=(x,x), newline del => old=(x,x+1) -> cur=(x-1,x-1)
	case ctCut:    // old=(x,y) -> cur=(x,x)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctUndo:   // old=(?,?) -> cur=(x,y)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctMove:   // old=(x,x+len) -> cur=(y-len,y) | cur=(y,y+len)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		if (crCurSel.cpMin > m_crOldSel.cpMin) // move after
			FormatTextLines(m_crOldSel.cpMin, m_crOldSel.cpMin);
		else // move before
			FormatTextLines(m_crOldSel.cpMax, m_crOldSel.cpMax);
		break;
	default:
		FormatAll();
		break;
	}

	//undo action does not call OnProtected, so make it default
	m_changeType = ctUndo;
}

void CTWScriptEdit::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENPROTECTED* pEP = (ENPROTECTED*)pNMHDR;

	// determine type of change will occur

	switch (pEP->msg) {
	case WM_KEYDOWN:
		switch (pEP->wParam) {
		case VK_DELETE:
			m_changeType = ctDelete;
			break;
		case VK_BACK:
			m_changeType = ctBack;
			break;
		default:
			m_changeType = ctUnknown;
			break;
		}
		break;
	case EM_REPLACESEL:
	case WM_CHAR:
		m_changeType = ctReplSel;
		break;
	case WM_PASTE:
		m_changeType = (m_changeType == ctCut)?ctMove:ctPaste;
		break;
	case WM_CUT:
		m_changeType = ctCut;
		break;
	default:
		m_changeType = ctUnknown;
		break;
	};

	if (pEP->msg != EM_SETCHARFORMAT && m_changeType != ctMove)
		m_crOldSel = pEP->chrg;

	*pResult = FALSE;
}

void CTWScriptEdit::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SELCHANGE* pSC = (SELCHANGE*)pNMHDR;

	*pResult = 0;
}

void CTWScriptEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//设置为焦点
	SetFocus();
	//创建一个弹出式菜单
	CMenu popmenu;
	popmenu.CreatePopupMenu();
	//添加菜单项目
	popmenu.AppendMenu(0, ID_RICH_UNDO, "&Undo");
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_CUT, "&Cut");
	popmenu.AppendMenu(0, ID_RICH_COPY, "C&opy");
	popmenu.AppendMenu(0, ID_RICH_PASTE, "&Paste");
	popmenu.AppendMenu(0, ID_RICH_CLEAR, "C&lear");
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SELECTALL, "Select &All");
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SETFONT, "Select &Font");

	//初始化菜单项
	UINT nUndo=(CanUndo() ? 0 : MF_GRAYED );
	popmenu.EnableMenuItem(ID_RICH_UNDO, MF_BYCOMMAND|nUndo);

	UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED) ;
	popmenu.EnableMenuItem(ID_RICH_CUT, MF_BYCOMMAND|nSel);
	popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND|nSel);
	popmenu.EnableMenuItem(ID_RICH_CLEAR, MF_BYCOMMAND|nSel);

	UINT nPaste=(CanPaste() ? 0 : MF_GRAYED) ;
	popmenu.EnableMenuItem(ID_RICH_PASTE, MF_BYCOMMAND|nPaste);

	//显示菜单
	CPoint pt;
	GetCursorPos(&pt);
	popmenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	popmenu.DestroyMenu();
	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CTWScriptEdit::OnSelectfont() 
{
	// TODO: Add your command handler code here
	CHARFORMAT cf;
	LOGFONT lf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	memset(&lf, 0, sizeof(LOGFONT));
	//判断是否选择了内容
	BOOL m_bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
	if (m_bSelect)
	{
		GetSelectionCharFormat(cf);
	}
	else
	{
		GetDefaultCharFormat(cf);
	}
	//得到相关字体属性
	BOOL bIsBold = cf.dwEffects & CFE_BOLD;
	BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
	BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
	BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;
	//设置属性
	lf.lfCharSet = cf.bCharSet;
	lf.lfHeight = cf.yHeight/15;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	lf.lfItalic = bIsItalic;
	lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);
	lf.lfUnderline = bIsUnderline;
	lf.lfStrikeOut = bIsStrickout;
	sprintf(lf.lfFaceName, cf.szFaceName);

	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = cf.crTextColor;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCharFormat(cf);//获得所选字体的属性
		if (m_bSelect) 
			SetSelectionCharFormat(cf);	//为选定的内容设定所选字体
		else
			SetWordCharFormat(cf);	//为将要输入的内容设定字体
	}



}
