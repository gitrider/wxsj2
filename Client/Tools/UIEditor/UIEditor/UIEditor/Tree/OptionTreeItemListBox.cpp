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

#include "stdafx.h"
#include "OptionTreeItemListBox.h"

// Added Headers
#include "OptionTree.h"
#include ".\optiontreeitemlistbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemListBox

COptionTreeItemListBox::COptionTreeItemListBox()
{
	// Initialize variables
	m_bFocus = FALSE;

  _bRootExpanded = false;

	// Set item type
	SetItemType(OT_ITEM_LISTBOX);
}

COptionTreeItemListBox::~COptionTreeItemListBox()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemListBox, CListBox)
	//{{AFX_MSG_MAP(COptionTreeItemListBox)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
  ON_CONTROL_REFLECT(LBN_SELCHANGE, OnLbnSelchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemListBox message handlers


void COptionTreeItemListBox::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// If we don't have focus, text is drawn.
	if (m_bFocus == TRUE)
	{
		return;
	}

	// Make sure options aren't NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Make sure there is a window
	if (!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}
  return;
	// Declare variables
	HGDIOBJ hOld;
	COLORREF crOld;
	int nOldBack;
	CRect rcText;
	CString strWindowText;
	COLORREF crOldBack;

	// Get window text
	GetWindowText(strWindowText);

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());
	
	// Set text color
	if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else
	{
		crOld = pDC->SetTextColor(GetTextColor());
	}

	// Set background mode
	nOldBack = pDC->SetBkMode(TRANSPARENT);

	// Set background color
	crOldBack = pDC->SetBkColor(GetBackgroundColor());	

	// Get rectangle
	rcText = rcRect;

	// Draw text
	pDC->DrawText(strWindowText, rcText, DT_SINGLELINE | DT_VCENTER);

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
	pDC->SetBkColor(crOldBack);

}

void COptionTreeItemListBox::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
	}
}

void COptionTreeItemListBox::OnRefresh()
{
	// Set the window text
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}
}

void COptionTreeItemListBox::OnMove()
{
	// Set window position
	if (IsWindow(GetSafeHwnd()) == TRUE)
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

	  // Hide window
	  //if (m_bFocus == FALSE && (m_otiParent == NULL || m_otiParent->IsExpanded() == FALSE) )
    if (m_bFocus == FALSE && 
        ( m_otiParent == NULL || 
          m_bExpanded == FALSE ||
          _bRootExpanded == FALSE ||  
          m_otiParent->IsExpanded() == FALSE) 
       )
		  ShowWindow(SW_HIDE);
		// -- Show window
    else
		  ShowWindow(SW_SHOW);
  }
}
void COptionTreeItemListBox::OnExpand(BOOL bExpand)
{
	// Show window
	if (bExpand == TRUE && 
      m_bExpanded == TRUE && 
      m_otiParent->IsExpanded() == TRUE)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
  _bRootExpanded = (bExpand == TRUE ? true : false);
}
void COptionTreeItemListBox::OnActivate()
{
	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set window position
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

		// -- Set focus
		SetFocus();
	}
}

void COptionTreeItemListBox::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;

	CListBox::OnSetFocus(pOldWnd);	
}

void COptionTreeItemListBox::OnKillFocus(CWnd* pNewWnd) 
{
	// Validate
	if (m_otOption == NULL)
	{
		CListBox::OnKillFocus(pNewWnd);
		return;
	}

	// See if new window is tree of list
	if (m_otOption->IsChild(pNewWnd) == TRUE)
	{
		// -- Mark focus
		m_bFocus = FALSE;

		// -- Commit changes
		CommitChanges();
	}

	CListBox::OnKillFocus(pNewWnd);	
}

BOOL COptionTreeItemListBox::CreateListItem(DWORD dwAddStyle,const int Height)
{
	// Declare variables
	//DWORD dwStyle = WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL|WS_HSCROLL;
  // no sort but multiselect
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_EXTENDEDSEL | WS_VSCROLL | WS_HSCROLL;

     
	BOOL bRet = FALSE;

	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create edit control
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Add style
		if (dwAddStyle != 0)
		{
			dwStyle |= dwAddStyle;
		}
		
		// -- Create the list box
		bRet = Create(dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup list
		if (bRet == TRUE)
		{
			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);

      COptionTreeItem::SetItemHeight(Height > OT_DEFHEIGHT ? Height : OT_LISTBOX_HEIGHT);

			// -- -- Set window position
			MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());		

      ShowWindow(SW_SHOW);
		}
	}


	return bRet;
}


void COptionTreeItemListBox::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}
void COptionTreeItemListBox::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		//ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemListBox::OnSelect()
{
	// Do nothing here
}

void COptionTreeItemListBox::OnShow(BOOL bShow)
{
  if(bShow == TRUE)
    this->ShowWindow(SW_SHOW);
  else
    this->ShowWindow(SW_HIDE);
}
void COptionTreeItemListBox::OnLbnSelchange()
{
  CommitChanges();
}
