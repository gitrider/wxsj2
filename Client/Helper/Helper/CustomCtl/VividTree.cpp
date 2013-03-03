///////////////////////////////////////////////////////////////////////////
// VividTree.cpp : Implementation of Class VividTree
///////////////////////////////////////////////////////////////////////////
//
// Copyright(C) 2005 Jim Alsup  All rights reserved
//              email: ja.alsup@gmail.com
//
// License: This code is provided "as is" with no expressed or implied 
//          warranty. You may use, or derive works from this file without
//          any restrictions except those listed below.
//
//        - This original header must be kept in the derived work.
//
//        - If your derived work is distributed in any form, you must
//          notify the author via the email address above and provide a 
//          short description of the product and intended audience.  
//
//        - You may not sell this code or a derived version of it as part of 
//          a comercial code library. 
//
//        - Offering the author a free licensed copy of any end product 
//          using this code is not required, but does endear you with a 
//          bounty of good karma.
//
///////////////////////////////////////////////////////////////////////////

//
// This owner draw tree control allows for the creation of a tree control
// that loosely resembles the user interface used by Skype's main window.
//
// Beside demonstrating the basics of creating an ownerdrawn tree control,
// but more importantly, demonstrates gradient backgrounds fills, bitmap 
// backgrounds, flicker free drawing, and how to deal with these in a 
// scrollable and resizable dialog.
//
// The underlying gradient or bitmap image is sized according the natural
// size of the visible (on screen or not) tree data.  This can be more
// readily understood and seen by opening and collasping the tree leaf
// with a long text name.  Notice the bmp or gradient resizes to the
// actual screen content.  
//
// Version History
//
// Sept 1, 2005  1.0.0  Initial Version

#include "stdafx.h"
#include "..\resource.h"
#include ".\vividtree.h"

#define _OWNER_DRAWN 1  // Set to 0 to use Windows draw code.  


// Gradient Filling Helper Routine
void GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad )
{
	TRIVERTEX        vert[2];
	GRADIENT_RECT    mesh;

	vert[0].x      = rect.left;
	vert[0].y      = rect.top;
	vert[0].Alpha  = 0x0000;
	vert[0].Blue   = GetBValue(col_from) << 8;
	vert[0].Green  = GetGValue(col_from) << 8;
	vert[0].Red    = GetRValue(col_from) << 8;

	vert[1].x      = rect.right;
	vert[1].y      = rect.bottom; 
	vert[1].Alpha  = 0x0000;
	vert[1].Blue   = GetBValue(col_to) << 8;
	vert[1].Green  = GetGValue(col_to) << 8;
	vert[1].Red    = GetRValue(col_to) << 8;

	mesh.UpperLeft  = 0;
	mesh.LowerRight = 1;
#if _MSC_VER >= 1300  // only VS7 and above has GradientFill as a pDC member
	pDC->GradientFill( vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#else
	GradientFill( pDC->m_hDC, vert, 2, &mesh, 1, vert_grad ? GRADIENT_FILL_RECT_V : 
		GRADIENT_FILL_RECT_H );
#endif
}



// CTreeCtrlEx

IMPLEMENT_DYNAMIC(VividTree, CTreeCtrl)

VividTree::VividTree()
{
	//m_gradient_bkgd_sel = RGB( 0x80, 0xa0, 0xff );	//Blueish
	m_gradient_bkgd_sel = RGB(170,155,119);
	m_gradient_bkgd_from = RGB( 0xff, 0xff, 0xff );	//White
	m_gradient_bkgd_to = RGB( 0xd5, 0xd5, 0xe0 );	//Light Greyish Blue
	m_bkgd_mode = BK_MODE_GRADIENT;
	m_bmp_tiled_mode = false;
	m_gradient_horz = true;

    VERIFY( m_bmp_tree_closed.LoadBitmap( IDB_TREE_CLOSED ) ) ; 
    m_bmp_tree_closed.GetSafeHandle();
    VERIFY( m_bmp_tree_open.LoadBitmap( IDB_TREE_OPENED ) ) ; 
    m_bmp_tree_open.GetSafeHandle();
	m_icon = NULL;
}


VividTree::~VividTree()
{
    if (m_bmp_back_ground.GetSafeHandle())
        m_bmp_back_ground.DeleteObject();
    if (m_bmp_tree_closed.GetSafeHandle())
        m_bmp_tree_closed.DeleteObject();
    if (m_bmp_tree_open.GetSafeHandle())
        m_bmp_tree_open.DeleteObject();
}


BEGIN_MESSAGE_MAP(VividTree, CTreeCtrl)
	ON_WM_ERASEBKGND()
#if _OWNER_DRAWN
	ON_WM_PAINT()
#endif
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
//	ON_WM_CREATE()
END_MESSAGE_MAP()



// CVividTree message handlers


BOOL VividTree::OnEraseBkgnd(CDC* pDC)
{
    // nothing to do here -- see OnPaint
	return TRUE;
}


#if _OWNER_DRAWN
void VividTree::OnPaint()
{
	CPaintDC dc(this);	// Device context for painting
	CDC dc_ff;			// Memory base device context for flicker free painting
	CBitmap bm_ff;		// The bitmap we paint into
	CBitmap *bm_old;
	CFont *font, *old_font;
	CFont fontDC;
	int old_mode;

	GetClientRect(&m_rect);
	SCROLLINFO scroll_info;
	// Determine window portal to draw into taking into account
	// scrolling position
	if ( GetScrollInfo( SB_HORZ, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		m_h_offset = -scroll_info.nPos;
		m_h_size = max( scroll_info.nMax+1, m_rect.Width());
	}
	else
	{
		m_h_offset = m_rect.left;
		m_h_size = m_rect.Width();
	}
	if ( GetScrollInfo( SB_VERT, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		if ( scroll_info.nMin == 0 && scroll_info.nMax == 100) 
			scroll_info.nMax = 0;
		m_v_offset = -scroll_info.nPos * GetItemHeight();
		m_v_size = max( (scroll_info.nMax+2)*((int)GetItemHeight()+1), m_rect.Height() );
	}
	else
	{
		m_v_offset = m_rect.top;
		m_v_size = m_rect.Height();
	}

	// Create an offscreen dc to paint with (prevents flicker issues)
	dc_ff.CreateCompatibleDC( &dc );
    bm_ff.CreateCompatibleBitmap( &dc, m_rect.Width(), m_rect.Height() );
    // Select the bitmap into the off-screen DC.
    bm_old = (CBitmap *)dc_ff.SelectObject( &bm_ff );
	// Default font in the DC is not the font used by 
	// the tree control, so grab it and select it in.
	font = GetFont();
	old_font = dc_ff.SelectObject( font );
	// We're going to draw text transparently
	old_mode = dc_ff.SetBkMode( TRANSPARENT );

	DrawBackGround( &dc_ff );
	DrawItems( &dc_ff );
	DrawItemLines(&dc_ff);

    // Now Blt the changes to the real device context - this prevents flicker.
    dc.BitBlt( m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &dc_ff, 0, 0, SRCCOPY);

	dc_ff.SelectObject( old_font );
	dc_ff.SetBkMode( old_mode );
    dc_ff.SelectObject( bm_old );
}
#endif


// Draw TreeCtrl Background - 
void VividTree::DrawBackGround( CDC* pDC )
{
	BkMode mode = m_bkgd_mode;

	if ( mode == BK_MODE_BMP )
	{
		if ( !m_bmp_back_ground.GetSafeHandle() )
			mode = BK_MODE_GRADIENT;
	}
	if ( mode == BK_MODE_GRADIENT )
	{
		GradientFillRect( pDC, 
			CRect( m_h_offset, m_v_offset, m_h_size + m_h_offset, m_v_size + m_v_offset ), 
			m_gradient_bkgd_from, m_gradient_bkgd_to, !m_gradient_horz );
	}
	else if ( mode == BK_MODE_FILL )
		pDC->FillSolidRect( m_rect, pDC->GetBkColor() ); 
	else if ( mode == BK_MODE_BMP )
	{
		BITMAP bm;
		CDC dcMem;
	      
		VERIFY(m_bmp_back_ground.GetObject(sizeof(bm), (LPVOID)&bm));
		dcMem.CreateCompatibleDC(NULL);
		CBitmap* bmp_old = (CBitmap*)dcMem.SelectObject( &m_bmp_back_ground ); 
		
		if ( m_bmp_tiled_mode ) 	// BitMap Tile Mode
		{
			for ( int y = 0; y <= (m_v_size / bm.bmHeight); y++ )
			{
				for ( int x = 0; x <= (m_h_size / bm.bmWidth); x++ )
				{
					pDC->BitBlt((x*bm.bmWidth) + m_h_offset, (y*bm.bmHeight) + m_v_offset,
						bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
				}
			}
		}
		else  // BITMAP Stretch Mode
		{
			pDC->StretchBlt( m_h_offset, m_v_offset, m_h_size, m_v_size, 
				&dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY );
		}
		// CleanUp
		dcMem.SelectObject( bmp_old );
	}
	else
		ASSERT( 0 );  // Unknown BackGround Mode
}

void VividTree::DrawItemLines(CDC* pDC)
{
	// draw items
	HTREEITEM show_item, parent;
	CRect rc_item, rc_parent;
	bool has_children;

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;

	do
	{
		parent = GetParentItem( show_item );
		has_children = ItemHasChildren( show_item ) || parent == NULL;

		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{
			if( parent )
			{
				CPen pen(PS_SOLID, 1, RGB(0xff,0xff,0));
				CPen* pOldPen = pDC->SelectObject(&pen);

				int nOldRop = pDC->SetROP2(R2_COPYPEN);

				GetItemRect(parent, rc_parent, TRUE);
				BITMAP bm;
				m_bmp_tree_open.GetBitmap(&bm);

				pDC->MoveTo(rc_parent.left-bm.bmWidth/2-2, rc_parent.bottom);
				pDC->LineTo(rc_parent.left-bm.bmWidth/2-2, rc_item.top+bm.bmHeight/2);
				pDC->MoveTo(rc_parent.left-bm.bmWidth/2-2, rc_item.top+bm.bmHeight/2);
				if(has_children)
				{
					pDC->LineTo(rc_item.left-bm.bmWidth-2, rc_item.top+bm.bmHeight/2);
				}
				else
				{
					pDC->LineTo(rc_item.left-2, rc_item.top+bm.bmHeight/2);
				}

				pDC->SetROP2(nOldRop);
				pDC->SelectObject(pOldPen);
			}
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
}

// Draw TreeCtrl Items
void VividTree::DrawItems( CDC *pDC )
{
	// draw items
	HTREEITEM show_item, parent;
	CRect rc_item;
	CString name;
	COLORREF color;
	DWORD tree_style;
	BITMAP bm;
	CDC dc_mem;
	CBitmap *button;
	int count = 0;
	int state;
	bool selected;
	bool has_children;

	show_item = GetFirstVisibleItem();
	if ( show_item == NULL )
		return;

	dc_mem.CreateCompatibleDC(NULL);
	color = pDC->GetTextColor();
	tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); 

	do
	{
		state = GetItemState( show_item, TVIF_STATE );
		parent = GetParentItem( show_item );
		has_children = ItemHasChildren( show_item ) || parent == NULL;
		selected = (state & TVIS_SELECTED) && ((this == GetFocus()) || 
				(tree_style & TVS_SHOWSELALWAYS));

		if ( GetItemRect( show_item, rc_item, TRUE ) )
		{
			if( selected)
			{
				CRect rc_all;
				GetItemRect(show_item,&rc_all, FALSE);
				pDC->FillSolidRect(&rc_all, RGB(170,155,119));
			}
			if ( has_children  || selected )
			{
				COLORREF from;
				CRect rect;
				// Show 
				if ( selected )
					from = m_gradient_bkgd_sel;
				else
					from = m_gradient_bkgd_to - (m_gradient_bkgd_from - m_gradient_bkgd_to);
				rect.top = rc_item.top;
				rect.bottom = rc_item.bottom;
				rect.right = m_h_size + m_h_offset;
				if ( !has_children )
					rect.left = rc_item.left + m_h_offset;
				else
					rect.left = m_h_offset;
				//GradientFillRect( pDC, rect, from, m_gradient_bkgd_to, FALSE );
				//pDC->SetTextColor( RGB( 0, 0, 255 ) );
				pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

				if ( has_children )
				{
					// Draw an Open/Close button
					if ( state & TVIS_EXPANDED )
						button = &m_bmp_tree_open;
					else
						button = &m_bmp_tree_closed;
					VERIFY(button->GetObject(sizeof(bm), (LPVOID)&bm));
					CBitmap *bmp_old = (CBitmap*)dc_mem.SelectObject(button); 
					pDC->TransparentBlt(rc_item.left - bm.bmWidth - 2, rc_item.top, bm.bmWidth, bm.bmHeight, 
						&dc_mem, 0, 0,bm.bmWidth,bm.bmWidth, GetSysColor(COLOR_HIGHLIGHTTEXT));
					//pDC->BitBlt( rc_item.left - bm.bmWidth - 2, rc_item.top, bm.bmWidth, bm.bmHeight, 
					//	&dc_mem, 0, 0, SRCCOPY );
					// CleanUp
					dc_mem.SelectObject( bmp_old );
				}
			}
			if ( !has_children )
			{
				// lookup the ICON instance (if any) and draw it
				HICON icon;
				icon = GetItemIcon( show_item );
				if ( icon != NULL )
					DrawIconEx( pDC->m_hDC, rc_item.left - 18, rc_item.top, icon, 16, 16,0,0, DI_NORMAL );
			}
			name = GetItemText( show_item );
			rc_item.DeflateRect( 0,1,0,1 );
			if ( selected )
			{
				if ( !has_children  )
					pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT) );
				COLORREF col = pDC->GetBkColor();
				//pDC->SetBkColor( /*GetSysColor(COLOR_HIGHLIGHT)*/RGB(170,155,119) );
				pDC->DrawText( name, rc_item, DT_LEFT );
				pDC->SetTextColor( color );
				pDC->SetBkColor( col );
			}
			else
			{
				pDC->SetTextColor( GetSysColor(COLOR_HIGHLIGHTTEXT) );
				pDC->DrawText( name, rc_item, DT_LEFT );
				pDC->SetTextColor( color );
			}
			//if ( state & TVIS_BOLD )
			//	pDC->SelectObject( font );
		}
	} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
}



void VividTree::SetBitmapID( UINT id )
{
	// delete any existing bitmap
    if (m_bmp_back_ground.GetSafeHandle())
        m_bmp_back_ground.DeleteObject();
	// add in the new bitmap
    VERIFY( m_bmp_back_ground.LoadBitmap( id ) ) ; 
    m_bmp_back_ground.GetSafeHandle();
}



// Determine if a referenced item is visible within the control window
bool VividTree::ItemIsVisible( HTREEITEM item )
{
	HTREEITEM scan_item;
	scan_item = GetFirstVisibleItem();
	while ( scan_item != NULL )
	{
		if ( item == scan_item )
			return true;
		scan_item = GetNextVisibleItem( scan_item );
	}
	return false;
}


// For a given tree node return an ICON for display on the left side.
// This default implementation only returns one icon.
// This function is virtual and meant to be overriden by deriving a class
// from VividTree and supplying your own icon images. 
HICON VividTree::GetItemIcon( HTREEITEM item )		
{
	return m_icon;  // default implementation - overridable
}


// If the background is a bitmap, and a tree is expanded/collapsed we
// need to redraw the entire background because windows moves the bitmap
// up (on collapse) destroying the position of the background.
void VividTree::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if ( GetBkMode() == VividTree::BK_MODE_BMP && ItemIsVisible( pNMTreeView->itemNew.hItem ) )
		Invalidate();  // redraw everything
	*pResult = 0;
}




