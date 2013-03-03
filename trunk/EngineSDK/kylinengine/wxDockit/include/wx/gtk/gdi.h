///////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/Gdi.h
// Purpose:     wxGdi class
// Author:      Mark McCormack
// Modified by:
// Created:     23/10/04
// RCS-ID:      $Id: gdi.h,v 1.3 2005/05/12 14:03:42 frm Exp $
// Copyright:   (c) 2004 Mark McCormack
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WXGDI_H_
#define _WX_WXGDI_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/dc.h>
#include <wx/gdi.h>

// ----------------------------------------------------------------------------
// wxGdi
// ----------------------------------------------------------------------------

class wxGdi : public wxGdiBase {
public:
    // drawing
    virtual void DrawGripper( wxDC &dc, wxRect r, wxOrientation orientation, eGripperStyle style, const wxString &label = wxT("") );
    virtual void DrawFrame( wxDC &dc, wxRect r, bool fDocked );
    virtual void DrawPattern( wxDC &dc, wxRect r );
    virtual void DrawSplitter( wxDC &dc, wxRect r, wxOrientation orientation );
    virtual void DrawEmptyWorkspace( wxDC &dc, wxRect r, bool edge );
    virtual void DrawHeader( wxDC &dc, wxRect r, wxOrientation orientation, const wxString &label, const wxFont &font );

    // cursors
    virtual wxCursor GetMoveCursor();
    virtual wxCursor GetHSplitCursor();
    virtual wxCursor GetVSplitCursor();
};

extern wxGdi g_gdi;

#endif
    // _WX_WXGDI_H_
