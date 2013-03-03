///////////////////////////////////////////////////////////////////////////////
// Name:        gtk/renderer.cpp
// Purpose:     implementation of wxRendererNative for wxGTK
// Author:      Vadim Zeitlin
// Modified by:
// Created:     20.07.2003
// RCS-ID:      $Id: renderer.cpp,v 1.31 2005/04/16 09:52:25 JS Exp $
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/renderer.h"
#include <gtk/gtk.h>
#include "wx/gtk/win_gtk.h"

#include "wx/window.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

#ifdef __WXGTK20__
    #include "wx/settings.h"
#endif // GTK 2.0

#ifdef __WXGTK20__
    #define WXUNUSED_IN_GTK1(arg) arg
#else
    #define WXUNUSED_IN_GTK1(arg)
#endif

// ----------------------------------------------------------------------------
// wxRendererGTK: our wxRendererNative implementation
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxRendererGTK : public wxDelegateRendererNative
{
public:
    // draw the header control button (used by wxListCtrl)
    virtual void DrawHeaderButton(wxWindow *win,
                                  wxDC& dc,
                                  const wxRect& rect,
                                  int flags = 0);

#ifdef __WXGTK20__
    // draw the expanded/collapsed icon for a tree control item
    virtual void DrawTreeItemButton(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0);
#endif // GTK+ 2.0

    virtual void DrawSplitterBorder(wxWindow *win,
                                    wxDC& dc,
                                    const wxRect& rect,
                                    int flags = 0);
    virtual void DrawSplitterSash(wxWindow *win,
                                  wxDC& dc,
                                  const wxSize& size,
                                  wxCoord position,
                                  wxOrientation orient,
                                  int flags = 0);

    virtual void DrawComboBoxDropButton(wxWindow *win,
                                        wxDC& dc,
                                        const wxRect& rect,
                                        int flags = 0);

    virtual void DrawDropArrow(wxWindow *win,
                               wxDC& dc,
                               const wxRect& rect,
                               int flags = 0);

    virtual wxSplitterRenderParams GetSplitterParams(const wxWindow *win);

private:
    // FIXME: shouldn't we destroy these windows somewhere?

    // used by DrawHeaderButton and DrawComboBoxDropButton
    static GtkWidget *GetButtonWidget();

#ifdef __WXGTK20__
    // used by DrawTreeItemButton()
    static GtkWidget *GetTreeWidget();
#endif // GTK+ 2.0
};

// ============================================================================
// implementation
// ============================================================================

/* static */
wxRendererNative& wxRendererNative::GetDefault()
{
    static wxRendererGTK s_rendererGTK;

    return s_rendererGTK;
}

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

GtkWidget *
wxRendererGTK::GetButtonWidget()
{
    static GtkWidget *s_button = NULL;
    static GtkWidget *s_window = NULL;

    if ( !s_button )
    {
        s_window = gtk_window_new( GTK_WINDOW_POPUP );
        gtk_widget_realize( s_window );
        s_button = gtk_button_new();
        gtk_container_add( GTK_CONTAINER(s_window), s_button );
        gtk_widget_realize( s_button );
    }

    return s_button;
}

#ifdef __WXGTK20__

GtkWidget *
wxRendererGTK::GetTreeWidget()
{
    static GtkWidget *s_tree = NULL;
    static GtkWidget *s_window = NULL;

    if ( !s_tree )
    {
        s_tree = gtk_tree_view_new();
        s_window = gtk_window_new( GTK_WINDOW_POPUP );
        gtk_widget_realize( s_window );
        gtk_container_add( GTK_CONTAINER(s_window), s_tree );
        gtk_widget_realize( s_tree );
    }

    return s_tree;
}

#endif // GTK+ 2.0

// ----------------------------------------------------------------------------
// list/tree controls drawing
// ----------------------------------------------------------------------------

void
wxRendererGTK::DrawHeaderButton(wxWindow *win,
                                wxDC& dc,
                                const wxRect& rect,
                                int flags)
{

    GtkWidget *button = GetButtonWidget();

    gtk_paint_box
    (
        button->style,
        // FIXME: I suppose GTK_PIZZA(win->m_wxwindow)->bin_window doesn't work with wxMemoryDC.
        //   Maybe use code similar as in DrawComboBoxDropButton below?
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        flags & wxCONTROL_DISABLED ? GTK_STATE_INSENSITIVE : GTK_STATE_NORMAL,
        GTK_SHADOW_OUT,
        NULL,
        button,
        "button",
        dc.XLOG2DEV(rect.x) -1, rect.y -1, rect.width +2, rect.height +2
    );
}

#ifdef __WXGTK20__

// draw a ">" or "v" button
void
wxRendererGTK::DrawTreeItemButton(wxWindow* win,
                                  wxDC& dc, const wxRect& rect, int flags)
{
    GtkWidget *tree = GetTreeWidget();

    GtkStateType state;
    if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;
        
    // VZ: I don't know how to get the size of the expander so as to centre it
    //     in the given rectangle, +2/3 below is just what looks good here...
    gtk_paint_expander
    (
        tree->style,
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        state,
        NULL,
        tree,
        "treeview",
        dc.LogicalToDeviceX(rect.x) + 2,
        dc.LogicalToDeviceY(rect.y) + 3,
        flags & wxCONTROL_EXPANDED ? GTK_EXPANDER_EXPANDED
                                   : GTK_EXPANDER_COLLAPSED
    );
}

#endif // GTK+ 2.0

// ----------------------------------------------------------------------------
// splitter sash drawing
// ----------------------------------------------------------------------------

#ifndef __WXGTK20__
    // the full sash width (should be even)
    static const wxCoord SASH_SIZE = 8;

    // margin around the sash
    static const wxCoord SASH_MARGIN = 2;
#endif // GTK+ 2.x/1.x

static int GetGtkSplitterFullSize()
{
#ifdef __WXGTK20__
    static GtkWidget *s_paned = NULL;
    if (s_paned == NULL)
        s_paned = gtk_vpaned_new();

    gint handle_size;
    gtk_widget_style_get (s_paned, "handle_size", &handle_size, NULL);
      
    return handle_size;
#else
    return SASH_SIZE + SASH_MARGIN;
#endif
}

wxSplitterRenderParams
wxRendererGTK::GetSplitterParams(const wxWindow *WXUNUSED(win))
{
    // we don't draw any border, hence 0 for the second field
    return wxSplitterRenderParams
           (
               GetGtkSplitterFullSize(),
               0,
#ifdef __WXGTK20__
               true     // hot sensitive
#else // GTK+ 1.x
               false    // not
#endif // GTK+ 2.x/1.x
           );
}

void
wxRendererGTK::DrawSplitterBorder(wxWindow * WXUNUSED(win),
                                  wxDC& WXUNUSED(dc),
                                  const wxRect& WXUNUSED(rect),
                                  int WXUNUSED(flags))
{
    // nothing to do
}

void
wxRendererGTK::DrawSplitterSash(wxWindow *win,
                                wxDC& dc,
                                const wxSize& size,
                                wxCoord position,
                                wxOrientation orient,
                                int WXUNUSED_IN_GTK1(flags))
{
    if ( !win->m_wxwindow->window )
    {
        // window not realized yet
        return;
    }
    
    wxCoord full_size = GetGtkSplitterFullSize();

    // are we drawing vertical or horizontal splitter?
    const bool isVert = orient == wxVERTICAL;

    GdkRectangle rect;
    GdkRectangle erase_rect;
    if ( isVert )
    {
        int h = win->GetClientSize().GetHeight();

        rect.x = position;
        rect.y = 0;
        rect.width = full_size;
        rect.height = h;

        erase_rect.x = position;
        erase_rect.y = 0;
        erase_rect.width = full_size;
        erase_rect.height = h;
    }
    else // horz
    {
        int w = win->GetClientSize().GetWidth();

        rect.x = 0;
        rect.y = position;
        rect.height = full_size;
        rect.width = w;

        erase_rect.y = position;
        erase_rect.x = 0;
        erase_rect.height = full_size;
        erase_rect.width = w;
    }

#if 0
    // RR: After a correction to the orientation of the sash
    //     this doesn't seem to be required anymore and it
    //     seems to confuse some themes

    // we must erase everything first, otherwise the garbage
    // from the old sash is left when dragging it
    gtk_paint_flat_box
    (
        win->m_wxwindow->style,
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        GTK_STATE_NORMAL,
        GTK_SHADOW_NONE,
        NULL,
        win->m_wxwindow,
        (char *)"viewportbin", // const_cast
        erase_rect.x,
        erase_rect.y,
        erase_rect.width,
        erase_rect.height
    );
#endif

#ifdef __WXGTK20__
    gtk_paint_handle
    (
        win->m_wxwindow->style,
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        flags & wxCONTROL_CURRENT ? GTK_STATE_PRELIGHT : GTK_STATE_NORMAL,
        GTK_SHADOW_NONE,
        NULL /* no clipping */,
        win->m_wxwindow,
        "paned",
        rect.x,
        rect.y,
        rect.width,
        rect.height,
        isVert ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL
    );
#else // GTK+ 1.x

    // leave some margin before sash itself
    position += SASH_MARGIN / 2;

    // and finally draw it using GTK paint functions
    typedef void (*GtkPaintLineFunc)(GtkStyle *, GdkWindow *,
                                                GtkStateType,
                                                GdkRectangle *, GtkWidget *,
                                                gchar *,
                                                gint, gint, gint);

    GtkPaintLineFunc func = isVert ? gtk_paint_vline : gtk_paint_hline;

    (*func)
    (
        win->m_wxwindow->style,
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        GTK_STATE_NORMAL,
        NULL,
        win->m_wxwindow,
        (char *)"paned", // const_cast
        0, isVert ? size.y : size.x, position + SASH_SIZE / 2 - 1
    );

    gtk_paint_box
    (
        win->m_wxwindow->style,
        GTK_PIZZA(win->m_wxwindow)->bin_window,
        GTK_STATE_NORMAL,
        GTK_SHADOW_OUT,
        (GdkRectangle*) NULL,
        win->m_wxwindow,
        (char *)"paned", // const_cast
        isVert ? position : size.x - 2*SASH_SIZE,
        isVert ? size.y - 2*SASH_SIZE : position,
        SASH_SIZE, SASH_SIZE
    );
#endif // GTK+ 2.x/1.x
}

void
wxRendererGTK::DrawDropArrow(wxWindow *win,
                             wxDC& dc,
                             const wxRect& rect,
                             int flags)
{
    GtkWidget *button = GetButtonWidget();

    // If we give GTK_PIZZA(win->m_wxwindow)->bin_window as
    // a window for gtk_paint_xxx function, then it won't
    // work for wxMemoryDC. So that is why we assume wxDC
    // is wxWindowDC (wxClientDC, wxMemoryDC and wxPaintDC
    // are derived from it) and use its m_window.
    wxWindowDC& wdc = (wxWindowDC&)dc;

    // only doing debug-time checking here (it should
    // probably be enough)
    wxASSERT ( wdc.IsKindOf(CLASSINFO(wxWindowDC)) );

    // draw arrow so that there is even space horizontally
    // on both sides
    int arrowX = rect.width/4 + 1;
    int arrowWidth = rect.width - (arrowX*2);

    // scale arrow's height accoording to the width
    int arrowHeight = rect.width/3;
    int arrowY = (rect.height-arrowHeight)/2 +
                 ((rect.height-arrowHeight) & 1);

    GtkStateType state;

    if ( flags & wxCONTROL_PRESSED )
        state = GTK_STATE_ACTIVE;
    else if ( flags & wxCONTROL_DISABLED )
        state = GTK_STATE_INSENSITIVE;
    else if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    // draw arrow on button
    gtk_paint_arrow
    (
        button->style,
        wdc.m_window,
        state,
        flags & wxCONTROL_PRESSED ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
        NULL,
        button,
        "arrow",
        GTK_ARROW_DOWN,
        FALSE,
        rect.x + arrowX,
        rect.y + arrowY,
        arrowWidth,
        arrowHeight
    );
}

void
wxRendererGTK::DrawComboBoxDropButton(wxWindow *win,
                                      wxDC& dc,
                                      const wxRect& rect,
                                      int flags)
{
    GtkWidget *button = GetButtonWidget();

    // for reason why we do this, see DrawDropArrow
    wxWindowDC& wdc = (wxWindowDC&)dc;
    wxASSERT ( wdc.IsKindOf(CLASSINFO(wxWindowDC)) );

    // draw button
    GtkStateType state;

    if ( flags & wxCONTROL_PRESSED )
        state = GTK_STATE_ACTIVE;
    else if ( flags & wxCONTROL_DISABLED )
        state = GTK_STATE_INSENSITIVE;
    else if ( flags & wxCONTROL_CURRENT )
        state = GTK_STATE_PRELIGHT;
    else
        state = GTK_STATE_NORMAL;

    gtk_paint_box
    (
        button->style,
        wdc.m_window,
        state,
        flags & wxCONTROL_PRESSED ? GTK_SHADOW_IN : GTK_SHADOW_OUT,
        NULL,
        button,
        "button",
        rect.x, rect.y, rect.width, rect.height
    );

    // draw arrow on button
    DrawDropArrow(win,dc,rect,flags);

}

