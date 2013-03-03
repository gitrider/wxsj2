/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
#if !defined(AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_)
#define AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TWScriptEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit window


class CTWScriptEdit : public CRichEditCtrl
{
// Construction
public:
	CTWScriptEdit();

	void Initialize();
// Attributes
public:
	void SetCaseSensitive(BOOL bSensitive);
	void SetChangeCase(BOOL bChange);

	void SetSLComment(WCHAR chComment, WCHAR chComment2 = 0);
	void SetSLComment(LPCWSTR lpszComment);
	void SetStringQuotes(LPCWSTR lpszStrQ);

	void AddKeywords(LPCWSTR lpszKwd);
	void AddConstants(LPCWSTR lpszKwd);
	void ClearKeywords();
	void ClearConstants();

	void SetKeywordColor(COLORREF clr, BOOL bBold);
	void SetConstantColor(COLORREF clr, BOOL bBold);
	void SetCommentColor(COLORREF clr, BOOL bBold);
	void SetNumberColor(COLORREF clr, BOOL bBold);
	void SetStringColor(COLORREF clr, BOOL bBold);

// Operations
public:
	void FormatAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTWScriptEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTWScriptEdit();

	// Generated message map functions
protected:
	BOOL IsStringQuote(WCHAR ch);
	int IsKeyword(LPCWSTR lpszSymbol);
	int IsConstant(LPCWSTR lpszSymbol);

	void SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr);
	void FormatTextRange(int nStart, int nEnd);
	void FormatTextLines(int nStart, int nEnd);
	void ChangeCase(int nStart, int nEnd, LPCWSTR lpszStr);


	struct SymbolColor {
		COLORREF clrColor;
		BOOL bBold;
	};

	enum ChangeType {ctUndo, ctUnknown, ctReplSel, ctDelete, ctBack, ctCut, ctPaste, ctMove};

	BOOL m_bCaseSensitive;
	BOOL m_bChangeCase;
	WCHAR m_chComment;
	WCHAR m_chComment2;
	CStringW m_strComment;
	CStringW m_strStringQuotes;
	CStringW m_strKeywords;
	CStringW m_strKeywordsLower;
	CStringW m_strConstants;
	CStringW m_strConstantsLower;

	SymbolColor m_icComment;
	SymbolColor m_icNumber;
	SymbolColor m_icString;
	SymbolColor m_icKeyword;
	SymbolColor m_icConstant;

	BOOL m_bInForcedChange;
	ChangeType m_changeType;
	CHARRANGE m_crOldSel;

	//{{AFX_MSG(CTWScriptEdit)
	afx_msg void OnChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProtected(NMHDR*, LRESULT* pResult);
	afx_msg void OnSelChange(NMHDR*, LRESULT* pResult);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCopy() { Copy(); }
	afx_msg void OnCut() { Cut(); }
	afx_msg void OnPaste() { Paste(); }
	afx_msg void OnSelectall() { SetSel(0, -1); }
	afx_msg void OnUndo() { Undo(); }
	afx_msg void OnClear() { Clear(); }
	afx_msg void OnSelectfont();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWSCRIPTEDIT_H__82F5E419_1C74_11D1_87FA_00403395B157__INCLUDED_)
