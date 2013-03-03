// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net
// addded 2006 by Cornelius Eichhorst

#ifndef OT_ITEMLISTBOX
#define OT_ITEMLISTBOX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTreeItemComboBox.h : header file
//

// Added Headers
#include "CommonRes.h"
#include "OptionTreeDef.h"
#include "OptionTreeItem.h"

// Classes
class COptionTree;

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemListBox window

class COptionTreeItemListBox : public CListBox, public COptionTreeItem
{
// Construction
public:
	COptionTreeItemListBox();
	virtual void OnMove();
	virtual void OnRefresh();
	virtual void OnCommit();
	virtual void OnActivate();
	virtual void CleanDestroyWindow();
	virtual void OnDeSelect();
	virtual void OnSelect();
	virtual void DrawAttribute(CDC *pDC, const RECT &rcRect);
	virtual void OnExpand(BOOL bExpand);
  virtual void OnShow(BOOL bShow);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionTreeItemListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateListItem(DWORD dwAddStyle = 0,const int Height=OT_LISTBOX_HEIGHT);
	virtual ~COptionTreeItemListBox();

	// Generated message map functions
protected:
	BOOL m_bFocus;
	//{{AFX_MSG(COptionTreeItemListBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
  bool  _bRootExpanded;
public:
  afx_msg void OnLbnSelchange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !OT_ITEMLISTBOX
