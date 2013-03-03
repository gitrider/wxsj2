/////////////////////////////////////////////////////////////////////////////
// Name:        tooltip.cpp
// Purpose:     wxToolTip implementation
// Author:      Robert Roebling
// Id:          $Id: tooltip.cpp,v 1.22 2004/11/03 21:13:20 RR Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "tooltip.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/setup.h"

#if wxUSE_TOOLTIPS

#include "wx/tooltip.h"
#include "wx/window.h"

#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

static GtkTooltips *ss_tooltips = (GtkTooltips*) NULL;

//-----------------------------------------------------------------------------
// wxToolTip
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxToolTip, wxObject)

wxToolTip::wxToolTip( const wxString &tip )
{
    m_text = tip;
    m_window = (wxWindow*) NULL;
}

void wxToolTip::SetTip( const wxString &tip )
{
    m_text = tip;
    Apply( m_window );
}

void wxToolTip::Apply( wxWindow *win )
{
    if (!win) return;

    if (!ss_tooltips)
    {
        ss_tooltips = gtk_tooltips_new();
    }

    m_window = win;

    if (m_text.IsEmpty())
        m_window->ApplyToolTip( ss_tooltips, (wxChar*) NULL );
    else
        m_window->ApplyToolTip( ss_tooltips, m_text );
}

void wxToolTip::Enable( bool flag )
{
    if (!ss_tooltips) return;

    if (flag)
        gtk_tooltips_enable( ss_tooltips );
    else
        gtk_tooltips_disable( ss_tooltips );
}

void wxToolTip::SetDelay( long msecs )
{
    if (!ss_tooltips)
        return;

    gtk_tooltips_set_delay( ss_tooltips, (int)msecs );
}

#endif

