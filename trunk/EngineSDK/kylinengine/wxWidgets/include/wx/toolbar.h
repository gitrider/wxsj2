/////////////////////////////////////////////////////////////////////////////
// Name:        wx/toolbar.h
// Purpose:     wxToolBar interface declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.11.99
// RCS-ID:      $Id: toolbar.h,v 1.41 2005/05/04 18:52:05 JS Exp $
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TOOLBAR_H_BASE_
#define _WX_TOOLBAR_H_BASE_

#include "wx/defs.h"

// ----------------------------------------------------------------------------
// wxToolBar style flags
// ----------------------------------------------------------------------------

enum
{
    // lay out the toolbar horizontally
    wxTB_HORIZONTAL  = wxHORIZONTAL,    // == 0x0004

    // lay out the toolbar vertically
    wxTB_VERTICAL    = wxVERTICAL,      // == 0x0008

    // show 3D buttons (wxToolBarSimple only)
    wxTB_3DBUTTONS   = 0x0010,

    // "flat" buttons (Win32/GTK only)
    wxTB_FLAT        = 0x0020,

    // dockable toolbar (GTK only)
    wxTB_DOCKABLE    = 0x0040,

    // don't show the icons (they're shown by default)
    wxTB_NOICONS     = 0x0080,

    // show the text (not shown by default)
    wxTB_TEXT        = 0x0100,

    // don't show the divider between toolbar and the window (Win32 only)
    wxTB_NODIVIDER   = 0x0200,

    // no automatic alignment (Win32 only, useless)
    wxTB_NOALIGN     = 0x0400,

    // show the text and the icons alongside, not vertically stacked (Win32/GTK)
    wxTB_HORZ_LAYOUT = 0x0800,
    wxTB_HORZ_TEXT   = wxTB_HORZ_LAYOUT | wxTB_TEXT
};

#if wxUSE_TOOLBAR
    #include "wx/tbarbase.h"     // the base class for all toolbars

    #if defined(__WXUNIVERSAL__)
       #include "wx/univ/toolbar.h"
    #elif defined(__WXPALMOS__)
       #include "wx/palmos/toolbar.h"
    #elif defined(__WXMSW__) && (!defined(_WIN32_WCE) || (_WIN32_WCE >= 400 && !defined(__POCKETPC__) && !defined(__SMARTPHONE__)))
       #include "wx/msw/tbar95.h"
    #elif defined(__WXWINCE__)
       #include "wx/msw/wince/tbarwce.h"
    #elif defined(__WXMSW__)
       #include "wx/msw/tbarmsw.h"
    #elif defined(__WXMOTIF__)
       #include "wx/motif/toolbar.h"
    #elif defined(__WXGTK__)
        #include "wx/gtk/tbargtk.h"
    #elif defined(__WXMAC__)
       #include "wx/mac/toolbar.h"
    #elif defined(__WXCOCOA__)
       #include "wx/cocoa/toolbar.h"
    #elif defined(__WXPM__)
       #include "wx/os2/toolbar.h"
    #endif
#endif // wxUSE_TOOLBAR

#endif
    // _WX_TOOLBAR_H_BASE_
