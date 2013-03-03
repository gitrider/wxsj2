/////////////////////////////////////////////////////////////////////////////
// Name:        xh_statbar.cpp
// Purpose:     XRC resource for wxStatusBar
// Author:      Brian Ravnsgaard Riis
// Created:     2004/01/21
// RCS-ID:      $Id: xh_statbar.cpp,v 1.11 2005/01/07 21:33:14 VS Exp $
// Copyright:   (c) 2004 Brian Ravnsgaard Riis
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_statbar.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_STATUSBAR

#include "wx/frame.h"
#include "wx/string.h"
#include "wx/log.h"

#include "wx/xrc/xh_statbar.h"
#include "wx/statusbr.h"

IMPLEMENT_DYNAMIC_CLASS(wxStatusBarXmlHandler, wxXmlResourceHandler)

wxStatusBarXmlHandler::wxStatusBarXmlHandler() :
        wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxST_SIZEGRIP);
    AddWindowStyles();
}

wxObject *wxStatusBarXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(statbar, wxStatusBar)

    statbar->Create(m_parentAsWindow,
                    GetID(),
                    GetStyle(),
                    GetName());

    int fields = GetLong(wxT("fields"), 1);
    wxString widths = GetParamValue(wxT("widths"));
    wxString styles = GetParamValue(wxT("styles"));

    if (fields > 1 && !widths.IsEmpty())
    {
        int *width = new int[fields];

        for (int i = 0; i < fields; ++i)
        {
            width[i] = wxAtoi(widths.BeforeFirst(wxT(',')));
            if(widths.Find(wxT(',')))
                widths.Remove(0, widths.Find(wxT(',')) + 1);
        }
        statbar->SetFieldsCount(fields, width);
        delete[] width;
    }
    else
        statbar->SetFieldsCount(fields);

    if (!styles.IsEmpty())
    {
        int *style = new int[fields];
        for (int i = 0; i < fields; ++i)
        {
            style[i] = wxSB_NORMAL;

            wxString first = styles.BeforeFirst(wxT(','));
            if (first == wxT("wxSB_NORMAL"))
                style[i] = wxSB_NORMAL;
            else if (first == wxT("wxSB_FLAT"))
                style[i] = wxSB_FLAT;
            else if (first == wxT("wxSB_RAISED"))
                style[i] = wxSB_RAISED;

            if (!first.IsEmpty())
                wxLogError(wxT("Error in resource, unknown statusbar field style: ") + first);
            if(styles.Find(wxT(',')))
                styles.Remove(0, styles.Find(wxT(',')) + 1);
        }
        statbar->SetStatusStyles(fields, style);
        delete [] style;
    }

    if (m_parentAsWindow)
    {
        wxFrame *parentFrame = wxDynamicCast(m_parent, wxFrame);
        if (parentFrame)
            parentFrame->SetStatusBar(statbar);
    }

    return statbar;
}

bool wxStatusBarXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStatusBar"));
}

#endif // wxUSE_XRC && wxUSE_STATUSBAR

