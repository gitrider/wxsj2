///////////////////////////////////////////////////////////////////////////////
// Name:        wx/popupwin.h
// Purpose:     wxPopupWindow interface declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.01.01
// RCS-ID:      $Id: popupwin.h,v 1.35 2005/04/15 03:04:30 RD Exp $
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_POPUPWIN_H_BASE_
#define _WX_POPUPWIN_H_BASE_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "popupwinbase.h"
#endif

#include "wx/defs.h"

#if wxUSE_POPUPWIN

#include "wx/window.h"

// ----------------------------------------------------------------------------
// wxPopupWindow: a special kind of top level window used for popup menus,
// combobox popups and such.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxPopupWindowBase : public wxWindow
{
public:
    wxPopupWindowBase() { }
    virtual ~wxPopupWindowBase();

    // create the popup window
    //
    // style may only contain border flags
    bool Create(wxWindow *parent, int style = wxBORDER_NONE);

    // move the popup window to the right position, i.e. such that it is
    // entirely visible
    //
    // the popup is positioned at ptOrigin + size if it opens below and to the
    // right (default), at ptOrigin - sizePopup if it opens above and to the
    // left &c
    //
    // the point must be given in screen coordinates!
    virtual void Position(const wxPoint& ptOrigin,
                          const wxSize& size);

    virtual bool IsTopLevel() const { return true; }

    DECLARE_NO_COPY_CLASS(wxPopupWindowBase)
};


// include the real class declaration
#ifdef __WXMSW__
    #include "wx/msw/popupwin.h"
#elif __WXPM__
    #include "wx/os2/popupwin.h"
#elif __WXGTK__
    #include "wx/gtk/popupwin.h"
#elif __WXX11__
    #include "wx/x11/popupwin.h"
#elif __WXMOTIF__
    #include "wx/motif/popupwin.h"
#elif __WXMGL__
    #include "wx/mgl/popupwin.h"
#else
    #error "wxPopupWindow is not supported under this platform."
#endif

// ----------------------------------------------------------------------------
// wxPopupTransientWindow: a wxPopupWindow which disappears automatically
// when the user clicks mouse outside it or if it loses focus in any other way
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxPopupWindowHandler;
class WXDLLEXPORT wxPopupFocusHandler;

class WXDLLEXPORT wxPopupTransientWindow : public wxPopupWindow
{
public:
    // ctors
    wxPopupTransientWindow() { Init(); }
    wxPopupTransientWindow(wxWindow *parent, int style = wxBORDER_NONE);

    virtual ~wxPopupTransientWindow();

    // popup the window (this will show it too) and keep focus at winFocus
    // (or itself if it's NULL), dismiss the popup if we lose focus
    virtual void Popup(wxWindow *focus = NULL);

    // hide the window
    virtual void Dismiss();

    // can the window be dismissed now?
    //
    // VZ: where is this used??
    virtual bool CanDismiss()
        { return true; }

    // called when a mouse is pressed while the popup is shown: return true
    // from here to prevent its normal processing by the popup (which consists
    // in dismissing it if the mouse is cilcked outside it)
    virtual bool ProcessLeftDown(wxMouseEvent& event);

    // Overridden to grab the input on some plaforms
    virtual bool Show( bool show = true );

protected:
    // common part of all ctors
    void Init();

    // this is called when the popup is disappeared because of anything
    // else but direct call to Dismiss()
    virtual void OnDismiss();

    // dismiss and notify the derived class
    void DismissAndNotify();

    // remove our event handlers
    void PopHandlers();

    // get alerted when child gets deleted from under us
    void OnDestroy(wxWindowDestroyEvent& event);

#ifdef __WXMSW__
    // check if the mouse needs captured or released
    void OnIdle(wxIdleEvent& event);
#endif
    
    // the child of this popup if any
    wxWindow *m_child;

    // the window which has the focus while we're shown
    wxWindow *m_focus;

    // these classes may call our DismissAndNotify()
    friend class wxPopupWindowHandler;
    friend class wxPopupFocusHandler;

    // the handlers we created, may be NULL (if not, must be deleted)
    wxPopupWindowHandler *m_handlerPopup;
    wxPopupFocusHandler  *m_handlerFocus;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxPopupTransientWindow)
    DECLARE_NO_COPY_CLASS(wxPopupTransientWindow)
};

#if wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

// ----------------------------------------------------------------------------
// wxPopupComboWindow: wxPopupTransientWindow used by wxComboBox
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxComboBox;
class WXDLLEXPORT wxComboControl;

class WXDLLEXPORT wxPopupComboWindow : public wxPopupTransientWindow
{
public:
    wxPopupComboWindow() { m_combo = NULL; }
    wxPopupComboWindow(wxComboControl *parent);

    bool Create(wxComboControl *parent);

    // position the window correctly relatively to the combo
    void PositionNearCombo();

protected:
    // notify the combo here
    virtual void OnDismiss();

    // forward the key presses to the combobox
    void OnKeyDown(wxKeyEvent& event);

    // the parent combobox
    wxComboControl *m_combo;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxPopupComboWindow)
};

#endif // wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

#endif // wxUSE_POPUPWIN

#endif // _WX_POPUPWIN_H_BASE_

