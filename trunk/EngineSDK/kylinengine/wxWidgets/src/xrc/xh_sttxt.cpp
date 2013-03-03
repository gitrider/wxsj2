/////////////////////////////////////////////////////////////////////////////
// Name:        xh_sttxt.cpp
// Purpose:     XRC resource for wxStaticText
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_sttxt.cpp,v 1.10 2005/01/07 21:33:14 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_sttxt.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_sttxt.h"
#include "wx/stattext.h"

IMPLEMENT_DYNAMIC_CLASS(wxStaticTextXmlHandler, wxXmlResourceHandler)

wxStaticTextXmlHandler::wxStaticTextXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxST_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxALIGN_LEFT);
    XRC_ADD_STYLE(wxALIGN_RIGHT);
    XRC_ADD_STYLE(wxALIGN_CENTRE);
    AddWindowStyles();
}

wxObject *wxStaticTextXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxStaticText)

    text->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    GetName());

    SetupWindow(text);

    return text;
}

bool wxStaticTextXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticText"));
}

#endif // wxUSE_XRC
