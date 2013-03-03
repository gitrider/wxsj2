/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.h
// Purpose:     wxCursor class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: cursor.h,v 1.15 2004/06/28 19:22:47 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CURSOR_H_
#define _WX_CURSOR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "cursor.h"
#endif

#include "wx/object.h"
#include "wx/gdicmn.h"

class WXDLLEXPORT wxImage;

// Cursor
class WXDLLEXPORT wxCursor: public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxCursor)
        
public:
    wxCursor();
    
    // Copy constructors
    wxCursor(const wxCursor& cursor) { Ref(cursor); }
    
    wxCursor(const char bits[], int width, int height,
             int hotSpotX = -1, int hotSpotY = -1,
             const char maskBits[] = NULL);
    
    wxCursor(const wxString& name, long flags = wxBITMAP_TYPE_XBM,
        int hotSpotX = 0, int hotSpotY = 0);

#if wxUSE_IMAGE   
    wxCursor(const wxImage& image);
#endif
   
    wxCursor(wxStockCursor id);
    ~wxCursor();
    
    virtual bool Ok() const;
    
    wxCursor& operator = (const wxCursor& cursor)
        { if (*this == cursor) return (*this); Ref(cursor); return *this; }
    bool operator == (const wxCursor& cursor) const
        { return m_refData == cursor.m_refData; }
    bool operator != (const wxCursor& cursor) const
        { return m_refData != cursor.m_refData; }

    // Motif-specific.
    // Create/get a cursor for the current display
    WXCursor GetXCursor(WXDisplay* display) const;
private:
    void Create(const char bits[], int width, int height,
                int hotSpotX = -1, int hotSpotY = -1,
                const char maskBits[] = NULL);
    void Create(WXPixmap cursor, WXPixmap mask, int hotSpotX, int hotSpotY);

    // Make a cursor from standard id
    WXCursor MakeCursor(WXDisplay* display, wxStockCursor id) const;
};

extern WXDLLEXPORT void wxSetCursor(const wxCursor& cursor);

#endif
// _WX_CURSOR_H_
