#if !defined(AFX_TRANSPARENTBUTTON_H__45F4670F_E198_11D3_A999_9E9231505A2F__INCLUDED_)
#define AFX_TRANSPARENTBUTTON_H__45F4670F_E198_11D3_A999_9E9231505A2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//*********************************************************
// TransParentButton.h : header file
//*********************************************************
// CTransParentButton verson 1.2  update date: 3-24-2000
// Author:  andygood, mailto:andygood@263.net
// Modify it to your own favor and enjoy it!
//*********************************************************
// the align of the icon
#define AL_LEFT     0
#define AL_TOP      1
#define AL_ME		2
// the state to draw
#define ST_MOVEIN    0
#define ST_MOVEOUT   1
#define ST_MOUSEUP   2
#define ST_MOUSEDOWN 3
/////////////////////////////////////////////////////////////////////////////
// CTransParentButton window

class CTransParentButton : public CButton
{
// Construction
public:
	CTransParentButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransParentButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetIcon(UINT  nNormal,UINT nMouseIn);
	BOOL SetCursor(UINT nCursorID);
	void SetStyle(bool bIsflat=false);
	void SetBkMode(bool bTransparent);
	void SetMouseDownTextColor(COLORREF color);
	void SetNormalTextColor(COLORREF color);
	void SetActiveTextColor(COLORREF color);
	void SetDisableTextColor(COLORREF color);
	void SetIconYPos(int y);
	void SetAlign(int nAlign);
	void SetAlign(int x, int y);
	void SetIconXPos(int x);
	void Draw();
	void DrawIcon(HICON hIcon,BOOL bDisabled=FALSE);
	void DrawIcon();
	void SetIcon(HICON  hNormal,HICON hMouseIn);
	void DrawText();
	void Refresh();
	void SetFont(CFont* pFont);
	CDC memDC;
	bool m_bDCStored;
	bool m_bMove;
	void DrawBorders();
	virtual ~CTransParentButton();

	// Generated message map functions
protected:
	int m_nAlignX, m_nAlignY;
	CFont* m_pFont;
	bool m_bIsEnabled;
	HCURSOR m_hCursor;
	bool m_bHasFocus;
	bool m_bIsFlat;
	bool m_bBkTransparent;
	COLORREF m_cl3D;
	COLORREF m_clText;
	COLORREF m_clMouseDownText;
	COLORREF m_clActiveText;
	COLORREF m_clNormalText;
	COLORREF m_clDisableText;
	bool m_bUseIcon;
	int m_IconYPos;
	int m_IconXPos;
	int m_DrawState;
	int m_IconAlign;
	HICON m_hMouseInIcon;
	HICON m_hNormalIcon;
	//{{AFX_MSG(CTransParentButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPARENTBUTTON_H__45F4670F_E198_11D3_A999_9E9231505A2F__INCLUDED_)
