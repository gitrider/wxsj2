/////////////////////////////////////////////////////////////////////////////
// Name:        xh_gauge.cpp
// Purpose:     XRC resource for wxGauge
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_gauge.cpp,v 1.10 2005/01/07 21:33:13 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_gauge.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_GAUGE

#include "wx/xrc/xh_gauge.h"
#include "wx/gauge.h"

IMPLEMENT_DYNAMIC_CLASS(wxGaugeXmlHandler, wxXmlResourceHandler)

wxGaugeXmlHandler::wxGaugeXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxGA_HORIZONTAL);
    XRC_ADD_STYLE(wxGA_VERTICAL);
    XRC_ADD_STYLE(wxGA_PROGRESSBAR);
    XRC_ADD_STYLE(wxGA_SMOOTH);   // windows only
    AddWindowStyles();
}

wxObject *wxGaugeXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxGauge)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetLong(wxT("range"), wxGAUGE_DEFAULT_RANGE),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    if( HasParam(wxT("value")))
    {
        control->SetValue(GetLong(wxT("value")));
    }
    if( HasParam(wxT("shadow")))
    {
        control->SetShadowWidth(GetDimension(wxT("shadow")));
    }
    if( HasParam(wxT("bezel")))
    {
        control->SetBezelFace(GetDimension(wxT("bezel")));
    }

    SetupWindow(control);

    return control;
}

bool wxGaugeXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGauge"));
}

#endif // wxUSE_XRC && wxUSE_GAUGE
