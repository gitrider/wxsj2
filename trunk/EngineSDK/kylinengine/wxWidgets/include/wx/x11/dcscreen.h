/////////////////////////////////////////////////////////////////////////////
// Name:        dcscreen.h
// Purpose:     wxScreenDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: dcscreen.h,v 1.6 2004/05/23 20:51:52 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSCREEN_H_
#define _WX_DCSCREEN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcscreen.h"
#endif

#include "wx/dcclient.h"

//-----------------------------------------------------------------------------
// wxScreenDC
//-----------------------------------------------------------------------------

class wxScreenDC : public wxPaintDC
{
public:
    wxScreenDC();
    virtual ~wxScreenDC();

    static bool StartDrawingOnTop( wxWindow *window );
    static bool StartDrawingOnTop( wxRect *rect = (wxRect *) NULL );
    static bool EndDrawingOnTop();

    // implementation

    static WXWindow   *sm_overlayWindow;
    static int         sm_overlayWindowX;
    static int         sm_overlayWindowY;

protected:
    virtual void DoGetSize(int *width, int *height) const;

private:
    DECLARE_DYNAMIC_CLASS(wxScreenDC)
};


#endif
// _WX_DCSCREEN_H_

