/////////////////////////////////////////////////////////////////////////////
// Name:        dcmemory.cpp
// Purpose:
// Author:      Robert Roebling, Vaclav Slavik
// RCS-ID:      $Id: dcmemory.cpp,v 1.7 2004/05/23 20:52:45 JS Exp $
// Copyright:   (c) 1998 Robert Roebling, 
//                  2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dcmemory.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/dcmemory.h"


//-----------------------------------------------------------------------------
// wxMemoryDC
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMemoryDC,wxWindowDC)

wxMemoryDC::wxMemoryDC() : wxDC()
{
    m_isMemDC = TRUE;
}

wxMemoryDC::wxMemoryDC(wxDC *WXUNUSED(dc)) : wxDC()
{
    m_isMemDC = TRUE;
}

wxMemoryDC::~wxMemoryDC()
{
}

void wxMemoryDC::SelectObject(const wxBitmap& bitmap)
{
    if ( bitmap.Ok() )
    {
        m_selected = bitmap;
        SetMGLDC(m_selected.CreateTmpDC(), TRUE);
    }
}

void wxMemoryDC::SetPen(const wxPen &pen)
{
    wxCHECK_RET( Ok(), wxT("invalid dc") );

    if ( GetDepth() == 1 && *wxTRANSPARENT_PEN != pen )
    {
        if ( *wxWHITE_PEN == pen )
            wxDC::SetPen(*wxBLACK_PEN);
        else
            wxDC::SetPen(*wxWHITE_PEN);
    }
    else
    {
        wxDC::SetPen(pen);
    }
}

void wxMemoryDC::SetBrush(const wxBrush &brush)
{
    wxCHECK_RET( Ok(), wxT("invalid dc") );

    if ( GetDepth() == 1 && *wxTRANSPARENT_BRUSH != brush )
    {
        if ( *wxWHITE_BRUSH == brush )
            wxDC::SetBrush(*wxBLACK_BRUSH);
        else
            wxDC::SetBrush(*wxWHITE_BRUSH);
    }
    else
    {
        wxDC::SetBrush(brush);
    }
}

void wxMemoryDC::SetTextForeground(const wxColour &col)
{
    wxCHECK_RET( Ok(), wxT("invalid dc") );

    if ( GetDepth() == 1 )
    {
        if ( col == *wxWHITE )
            wxDC::SetTextForeground(*wxBLACK);
        else
            wxDC::SetTextForeground(*wxWHITE);
    }
    else
    {
        wxDC::SetTextForeground(col);
    }
}

void wxMemoryDC::SetTextBackground(const wxColour &col)
{
    wxCHECK_RET( Ok(), wxT("invalid dc") );

    if ( GetDepth() == 1 )
    {
        if ( col == *wxWHITE )
            wxDC::SetTextBackground(*wxBLACK);
        else
            wxDC::SetTextBackground(*wxWHITE);
    }
    else
    {
        wxDC::SetTextBackground(col);
    }
}
