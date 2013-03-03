/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/dcscreen.h
// Purpose:     wxScreenDC class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: dcscreen.h,v 1.2 2005/01/18 21:14:24 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSCREEN_H_
#define _WX_DCSCREEN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dcscreen.h"
#endif

#include "wx/dcclient.h"

class WXDLLEXPORT wxScreenDC : public wxWindowDC
{
public:
    // Create a DC representing the whole screen
    wxScreenDC();

    // Compatibility with X's requirements for drawing on top of all windows
    static bool StartDrawingOnTop(wxWindow* WXUNUSED(window)) { return TRUE; }
    static bool StartDrawingOnTop(wxRect* WXUNUSED(rect) = NULL) { return TRUE; }
    static bool EndDrawingOnTop() { return TRUE; }

protected:
    virtual void DoGetSize(int *width, int *height) const;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxScreenDC)
};

#endif
    // _WX_DCSCREEN_H_

