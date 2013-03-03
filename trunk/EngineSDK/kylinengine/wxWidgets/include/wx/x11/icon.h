/////////////////////////////////////////////////////////////////////////////
// Name:        icon.h
// Purpose:     wxIcon class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: icon.h,v 1.10 2004/05/23 20:51:52 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ICON_H_
#define _WX_ICON_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "icon.h"
#endif

#include "wx/bitmap.h"

//-----------------------------------------------------------------------------
// wxIcon
//-----------------------------------------------------------------------------

class wxIcon: public wxBitmap
{
public:
    wxIcon();
    wxIcon( const wxIcon& icon);
    wxIcon( const char **bits, int width=-1, int height=-1 );

    // For compatibility with wxMSW where desired size is sometimes required to
    // distinguish between multiple icons in a resource.
    wxIcon( const wxString& filename, wxBitmapType type = wxBITMAP_TYPE_XPM, 
            int WXUNUSED(desiredWidth)=-1, int WXUNUSED(desiredHeight)=-1 ) :
        wxBitmap(filename, type)
    {
    }
    wxIcon( char **bits, int width=-1, int height=-1 );

    wxIcon(const wxIconLocation& loc)
        : wxBitmap(loc.GetFileName(), wxBITMAP_TYPE_ANY)
    {
    }

    wxIcon& operator=(const wxIcon& icon);
    bool operator==(const wxIcon& icon) const { return m_refData == icon.m_refData; }
    bool operator!=(const wxIcon& icon) const { return !(*this == icon); }

    // create from bitmap (which should have a mask unless it's monochrome):
    // there shouldn't be any implicit bitmap -> icon conversion (i.e. no
    // ctors, assignment operators...), but it's ok to have such function
    void CopyFromBitmap(const wxBitmap& bmp);
  
private:
    DECLARE_DYNAMIC_CLASS(wxIcon)
};


#endif
// _WX_ICON_H_
