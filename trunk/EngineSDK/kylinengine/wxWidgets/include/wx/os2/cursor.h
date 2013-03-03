/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.h
// Purpose:     wxCursor class
// Author:      David Webster
// Modified by:
// Created:     10/13/99
// RCS-ID:      $Id: cursor.h,v 1.9 2004/05/23 20:51:38 JS Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CURSOR_H_
#define _WX_CURSOR_H_

#include "wx/bitmap.h"

class WXDLLEXPORT wxCursorRefData: public wxGDIImageRefData
{
public:
    wxCursorRefData();
    ~wxCursorRefData() { Free(); }
    virtual void Free(void);
    bool                            m_bDestroyCursor;
}; // end of CLASS wxCursorRefData

#define M_CURSORDATA ((wxCursorRefData *)m_refData)
#define M_CURSORHANDLERDATA ((wxCursorRefData *)bitmap->m_refData)

// Cursor
class WXDLLEXPORT wxCursor: public wxBitmap
{
public:
    wxCursor();

    // Copy constructors
    wxCursor(const wxCursor& rCursor) { Ref(rCursor); }
    wxCursor(const wxImage& rImage);

    wxCursor( const char acBits[]
             ,int        nWidth
             ,int        nHeight
             ,int        nHotSpotX = -1
             ,int        nHotSpotY = -1
             ,const char zMaskBits[] = NULL
            );
    wxCursor( const wxString& rsName
             ,long            lFlags = wxBITMAP_TYPE_CUR_RESOURCE
             ,int             nHotSpotX = 0
             ,int             nHotSpotY = 0
            );
    wxCursor(int nCursorType);
    inline ~wxCursor() { }

    inline wxCursor& operator = (const wxCursor& rCursor)
    {
        if (*this == rCursor)
            return (*this);
        Ref(rCursor);
        return *this;
    }
    inline bool operator == (const wxCursor& rCursor) const { return m_refData == rCursor.m_refData; }
    inline bool operator != (const wxCursor& rCursor) const { return m_refData != rCursor.m_refData; }

    inline WXHCURSOR GetHCURSOR(void) const { return (M_CURSORDATA ? M_CURSORDATA->m_hCursor : 0); }
    inline void      SetHCURSOR(WXHCURSOR hCursor) { SetHandle((WXHANDLE)hCursor); }

protected:
    inline virtual wxGDIImageRefData* CreateData(void) const { return (new wxCursorRefData); }

private:
    DECLARE_DYNAMIC_CLASS(wxCursor)
}; // end of CLASS wxCursor

#endif
    // _WX_CURSOR_H_
