///////////////////////////////////////////////////////////////////////////////
// Name:        wx/renderer.h
// Purpose:     wxRendererNative class declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.07.2003
// RCS-ID:      $Id: renderer.h,v 1.19 2005/03/24 00:22:01 VZ Exp $
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/*
   Renderers are used in wxWidgets for two similar but different things:
    (a) wxUniversal uses them to draw everything, i.e. all the control
    (b) all the native ports use them to draw generic controls only

   wxUniversal needs more functionality than what is included in the base class
   as it needs to draw stuff like scrollbars which are never going to be
   generic. So we put the bare minimum needed by the native ports here and the
   full wxRenderer class is declared in wx/univ/renderer.h and is only used by
   wxUniveral (although note that native ports can load wxRenderer objects from
   theme DLLs and use them as wxRendererNative ones, of course).
 */

#ifndef _WX_RENDERER_H_
#define _WX_RENDERER_H_

class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxWindow;

#include "wx/gdicmn.h" // for wxPoint

// some platforms have their own renderers, others use the generic one
#if defined(__WXMSW__) || defined(__WXMAC__) || defined(__WXGTK__)
    #define wxHAS_NATIVE_RENDERER
#else
    #undef wxHAS_NATIVE_RENDERER
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// control state flags used in wxRenderer and wxColourScheme
enum
{
    wxCONTROL_DISABLED   = 0x00000001,  // control is disabled
    wxCONTROL_FOCUSED    = 0x00000002,  // currently has keyboard focus
    wxCONTROL_PRESSED    = 0x00000004,  // (button) is pressed
    wxCONTROL_ISDEFAULT  = 0x00000008,  // only applies to the buttons
    wxCONTROL_ISSUBMENU  = wxCONTROL_ISDEFAULT, // only for menu items
    wxCONTROL_EXPANDED   = wxCONTROL_ISDEFAULT, // only for the tree items
    wxCONTROL_CURRENT    = 0x00000010,  // mouse is currently over the control
    wxCONTROL_SELECTED   = 0x00000020,  // selected item in e.g. listbox
    wxCONTROL_CHECKED    = 0x00000040,  // (check/radio button) is checked
    wxCONTROL_CHECKABLE  = 0x00000080,  // (menu) item can be checked
    wxCONTROL_UNDETERMINED = wxCONTROL_CHECKABLE, // (check) undetermined state

    wxCONTROL_FLAGS_MASK = 0x000000ff,

    // this is a pseudo flag not used directly by wxRenderer but rather by some
    // controls internally
    wxCONTROL_DIRTY      = 0x80000000
};

// ----------------------------------------------------------------------------
// helper structs
// ----------------------------------------------------------------------------

// wxSplitterWindow parameters
struct WXDLLEXPORT wxSplitterRenderParams
{
    // the only way to initialize this struct is by using this ctor
    wxSplitterRenderParams(wxCoord widthSash_, wxCoord border_, bool isSens_)
        : widthSash(widthSash_), border(border_), isHotSensitive(isSens_)
        {
        }

    // the width of the splitter sash
    const wxCoord widthSash;

    // the width of the border of the splitter window
    const wxCoord border;

    // true if the splitter changes its appearance when the mouse is over it
    const bool isHotSensitive;
};

// wxRendererNative interface version
struct WXDLLEXPORT wxRendererVersion
{
    wxRendererVersion(int version_, int age_) : version(version_), age(age_) { }

    // default copy ctor, assignment operator and dtor are ok

    // the current version and age of wxRendererNative interface: different
    // versions are incompatible (in both ways) while the ages inside the same
    // version are upwards compatible, i.e. the version of the renderer must
    // match the version of the main program exactly while the age may be
    // highergreater or equal to it
    //
    // NB: don't forget to increment age after adding any new virtual function!
    enum
    {
        Current_Version = 1,
        Current_Age = 5
    };


    // check if the given version is compatible with the current one
    static bool IsCompatible(const wxRendererVersion& ver)
    {
        return ver.version == Current_Version && ver.age >= Current_Age;
    }

    const int version;
    const int age;
};

// ----------------------------------------------------------------------------
// wxRendererNative: abstracts drawing methods needed by the native controls
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRendererNative
{
public:
    // drawing functions
    // -----------------

    // draw the header control button (used by wxListCtrl)
    virtual void DrawHeaderButton(wxWindow *win,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0) = 0;

    // draw the expanded/collapsed icon for a tree control item
    virtual void DrawTreeItemButton(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0) = 0;

    // draw the border for sash window: this border must be such that the sash
    // drawn by DrawSash() blends into it well
    virtual void DrawSplitterBorder(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0) = 0;

    // draw a (vertical) sash
    virtual void DrawSplitterSash(wxWindow *win,
                                  wxDC& dc,
                                  const wxSize& size,
                                  wxCoord position,
                                  wxOrientation orient,
                                  int flags = 0) = 0;

    // draw a combobox dropdown button
    //
    // flags may use wxCONTROL_PRESSED and wxCONTROL_CURRENT
    virtual void DrawComboBoxDropButton(wxWindow *win,
                                        wxDC& dc,
                                        const wxRect& rect,
                                        int flags = 0) = 0;

    // draw a dropdown arrow
    //
    // flags may use wxCONTROL_PRESSED and wxCONTROL_CURRENT
    virtual void DrawDropArrow(wxWindow *win,
                               wxDC& dc,
                               const wxRect& rect,
                               int flags = 0) = 0;

    // geometry functions
    // ------------------

    // get the splitter parameters: the x field of the returned point is the
    // sash width and the y field is the border width
    virtual wxSplitterRenderParams GetSplitterParams(const wxWindow *win) = 0;


    // pseudo constructors
    // -------------------

    // return the currently used renderer
    static wxRendererNative& Get();

    // return the generic implementation of the renderer
    static wxRendererNative& GetGeneric();

    // return the default (native) implementation for this platform
    static wxRendererNative& GetDefault();


    // changing the global renderer
    // ----------------------------

#if wxUSE_DYNLIB_CLASS
    // load the renderer from the specified DLL, the returned pointer must be
    // deleted by caller if not NULL when it is not used any more
    static wxRendererNative *Load(const wxString& name);
#endif // wxUSE_DYNLIB_CLASS

    // set the renderer to use, passing NULL reverts to using the default
    // renderer
    //
    // return the previous renderer used with Set() or NULL if none
    static wxRendererNative *Set(wxRendererNative *renderer);


    // miscellaneous stuff
    // -------------------

    // this function is used for version checking: Load() refuses to load any
    // DLLs implementing an older or incompatible version; it should be
    // implemented simply by returning wxRendererVersion::Current_XXX values
    virtual wxRendererVersion GetVersion() const = 0;

    // virtual dtor for any base class
    virtual ~wxRendererNative();
};

// ----------------------------------------------------------------------------
// wxDelegateRendererNative: allows reuse of renderers code
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxDelegateRendererNative : public wxRendererNative
{
public:
    wxDelegateRendererNative()
        : m_rendererNative(GetGeneric()) { }

    wxDelegateRendererNative(wxRendererNative& rendererNative)
        : m_rendererNative(rendererNative) { }


    virtual void DrawHeaderButton(wxWindow *win,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0)
        { m_rendererNative.DrawHeaderButton(win, dc, rect, flags); }

    virtual void DrawTreeItemButton(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0)
        { m_rendererNative.DrawTreeItemButton(win, dc, rect, flags); }

    virtual void DrawSplitterBorder(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0)
        { m_rendererNative.DrawSplitterBorder(win, dc, rect, flags); }

    virtual void DrawSplitterSash(wxWindow *win,
                                  wxDC& dc,
                                  const wxSize& size,
                                  wxCoord position,
                                  wxOrientation orient,
                                  int flags = 0)
        { m_rendererNative.DrawSplitterSash(win, dc, size,
                                            position, orient, flags); }

    virtual void DrawComboBoxDropButton(wxWindow *win,
                                        wxDC& dc,
                                        const wxRect& rect,
                                        int flags = 0)
        { m_rendererNative.DrawComboBoxDropButton(win, dc, rect, flags); }

    virtual void DrawDropArrow(wxWindow *win,
                               wxDC& dc,
                               const wxRect& rect,
                               int flags = 0)
        { m_rendererNative.DrawDropArrow(win, dc, rect, flags); }

    virtual wxSplitterRenderParams GetSplitterParams(const wxWindow *win)
        { return m_rendererNative.GetSplitterParams(win); }

    virtual wxRendererVersion GetVersion() const
        { return m_rendererNative.GetVersion(); }

protected:
    wxRendererNative& m_rendererNative;

    DECLARE_NO_COPY_CLASS(wxDelegateRendererNative)
};

// ----------------------------------------------------------------------------
// inline functions implementation
// ----------------------------------------------------------------------------

#ifndef wxHAS_NATIVE_RENDERER

// default native renderer is the generic one then
/* static */ inline
wxRendererNative& wxRendererNative::GetDefault()
{
    return GetGeneric();
}

#endif // !wxHAS_NATIVE_RENDERER

#endif // _WX_RENDERER_H_

