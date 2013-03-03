/////////////////////////////////////////////////////////////////////////////
// Name:        statbmp.cpp
// Purpose:     wxStaticBitmap
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: statbmp.cpp,v 1.27 2005/05/10 06:26:59 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
  #pragma implementation "statbmp.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_STATBMP

#include "wx/statbmp.h"
#include "wx/dcclient.h"

IMPLEMENT_DYNAMIC_CLASS(wxStaticBitmap, wxControl)

/*
 * wxStaticBitmap
 */

BEGIN_EVENT_TABLE(wxStaticBitmap, wxStaticBitmapBase)
    EVT_PAINT(wxStaticBitmap::OnPaint)
END_EVENT_TABLE()

bool wxStaticBitmap::Create(wxWindow *parent, wxWindowID id,
           const wxBitmap& bitmap,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    SetName(name);

    m_backgroundColour = parent->GetBackgroundColour() ;
    m_foregroundColour = parent->GetForegroundColour() ;

    m_bitmap = bitmap;
    if ( id == -1 )
          m_windowId = (int)NewControlId();
    else
        m_windowId = id;

    m_windowStyle = style;

    bool ret = wxControl::Create( parent, id, pos, size, style , wxDefaultValidator , name );
    SetBestSize( size ) ;
    
    return ret;
}

void wxStaticBitmap::SetBitmap(const wxBitmap& bitmap)
{
    m_bitmap = bitmap;
    InvalidateBestSize();
    SetSize(GetBestSize());
    Refresh() ;
}

void wxStaticBitmap::OnPaint( wxPaintEvent& WXUNUSED(event) ) 
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    dc.DrawBitmap( m_bitmap , 0 , 0 , TRUE ) ;
}

wxSize wxStaticBitmap::DoGetBestSize() const
{
    if ( m_bitmap.Ok() )
        return DoGetSizeFromClientSize( wxSize(m_bitmap.GetWidth(), m_bitmap.GetHeight()) );

    // this is completely arbitrary
    return DoGetSizeFromClientSize( wxSize(16, 16) ); 
}

#endif

