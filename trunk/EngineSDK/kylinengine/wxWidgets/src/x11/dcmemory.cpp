/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.cpp
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: dcmemory.cpp,v 1.8 2004/05/23 20:53:30 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dcmemory.h"
#endif

#include "wx/dcmemory.h"
#include "wx/settings.h"
#include "wx/utils.h"

#include "wx/x11/private.h"

IMPLEMENT_DYNAMIC_CLASS(wxMemoryDC,wxWindowDC)

wxMemoryDC::wxMemoryDC() : wxWindowDC()
{
    m_ok = FALSE;
    
    m_display = (WXDisplay *) wxGlobalDisplay();

    int screen = DefaultScreen( wxGlobalDisplay() );
    m_cmap = (WXColormap) DefaultColormap( wxGlobalDisplay(), screen );
}

wxMemoryDC::wxMemoryDC( wxDC *WXUNUSED(dc) )
  : wxWindowDC()
{
    m_ok = FALSE;

    m_display = (WXDisplay *) wxGlobalDisplay();
    
    int screen = DefaultScreen( wxGlobalDisplay() );
    m_cmap = (WXColormap) DefaultColormap( wxGlobalDisplay(), screen );
}

wxMemoryDC::~wxMemoryDC()
{
}

void wxMemoryDC::SelectObject( const wxBitmap& bitmap )
{
    Destroy();
    
    m_selected = bitmap;
    if (m_selected.Ok())
    {
        if (m_selected.GetPixmap())
        {
            m_window = (WXWindow) m_selected.GetPixmap();
        }
        else
        {
            m_window = m_selected.GetBitmap();
        }

        m_isMemDC = TRUE;

        SetUpDC();
    }
    else
    {
        m_ok = FALSE;
        m_window = NULL;
    }
}

void wxMemoryDC::DoGetSize( int *width, int *height ) const
{
    if (m_selected.Ok())
    {
        if (width) (*width) = m_selected.GetWidth();
        if (height) (*height) = m_selected.GetHeight();
    }
    else
    {
        if (width) (*width) = 0;
        if (height) (*height) = 0;
    }
}
