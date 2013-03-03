/////////////////////////////////////////////////////////////////////////////
// Name:        popupwin.h
// Purpose:
// Author:      Robert Roebling
// Created:
// Id:          $Id: popupwin.h,v 1.9 2004/05/23 20:50:54 JS Exp $
// Copyright:   (c) 2001 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKPOPUPWINH__
#define __GTKPOPUPWINH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "popupwin.h"
#endif

#include "wx/defs.h"
#include "wx/panel.h"
#include "wx/icon.h"

//-----------------------------------------------------------------------------
// wxPopUpWindow
//-----------------------------------------------------------------------------

class wxPopupWindow: public wxPopupWindowBase
{
public:
    wxPopupWindow() { }
    virtual ~wxPopupWindow();

    wxPopupWindow(wxWindow *parent, int flags = wxBORDER_NONE)
        { (void)Create(parent, flags); }
    bool Create(wxWindow *parent, int flags = wxBORDER_NONE);

    virtual bool Show( bool show = TRUE );

    // implementation
    // --------------

    virtual void DoMoveWindow(int x, int y, int width, int height);

    virtual void OnInternalIdle();


protected:
    void GtkOnSize( int x, int y, int width, int height );

    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxPopupWindow)
};

#endif // __GTKPOPUPWINDOWH__
