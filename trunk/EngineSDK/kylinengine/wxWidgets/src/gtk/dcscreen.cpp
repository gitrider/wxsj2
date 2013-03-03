/////////////////////////////////////////////////////////////////////////////
// Name:        dcscreen.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id: dcscreen.cpp,v 1.23 2005/04/05 23:09:25 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dcscreen.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcscreen.h"
#include "wx/window.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

//-----------------------------------------------------------------------------
// global data initialization
//-----------------------------------------------------------------------------

GdkWindow *wxScreenDC::sm_overlayWindow  = (GdkWindow*) NULL;
int wxScreenDC::sm_overlayWindowX = 0;
int wxScreenDC::sm_overlayWindowY = 0;

//-----------------------------------------------------------------------------
// wxScreenDC
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxScreenDC,wxPaintDC)

wxScreenDC::wxScreenDC()
{
    m_ok = FALSE;
    m_cmap = gdk_colormap_get_system();
    m_window = GDK_ROOT_PARENT();

#ifdef __WXGTK20__
    m_context = gdk_pango_context_get();
    m_layout = pango_layout_new( m_context );
//    m_fontdesc = pango_font_description_copy( widget->style->font_desc );
#endif

    m_isScreenDC = TRUE;

    SetUpDC();

    gdk_gc_set_subwindow( m_penGC, GDK_INCLUDE_INFERIORS );
    gdk_gc_set_subwindow( m_brushGC, GDK_INCLUDE_INFERIORS );
    gdk_gc_set_subwindow( m_textGC, GDK_INCLUDE_INFERIORS );
    gdk_gc_set_subwindow( m_bgGC, GDK_INCLUDE_INFERIORS );
}

wxScreenDC::~wxScreenDC()
{
    gdk_gc_set_subwindow( m_penGC, GDK_CLIP_BY_CHILDREN );
    gdk_gc_set_subwindow( m_brushGC, GDK_CLIP_BY_CHILDREN );
    gdk_gc_set_subwindow( m_textGC, GDK_CLIP_BY_CHILDREN );
    gdk_gc_set_subwindow( m_bgGC, GDK_CLIP_BY_CHILDREN );

    EndDrawingOnTop();
}

bool wxScreenDC::StartDrawingOnTop( wxWindow * )
{
    return true;
}

bool wxScreenDC::StartDrawingOnTop( wxRect * )
{
    return true;
}

bool wxScreenDC::EndDrawingOnTop()
{
    return true;
}

void wxScreenDC::DoGetSize(int *width, int *height) const
{
    wxDisplaySize(width, height);
}
