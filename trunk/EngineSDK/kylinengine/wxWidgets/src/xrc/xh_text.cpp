/////////////////////////////////////////////////////////////////////////////
// Name:        xh_text.cpp
// Purpose:     XRC resource for wxTextCtrl
// Author:      Aleksandras Gluchovas
// Created:     2000/03/21
// RCS-ID:      $Id: xh_text.cpp,v 1.13 2005/05/22 15:42:37 JS Exp $
// Copyright:   (c) 2000 Aleksandras Gluchovas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_text.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_text.h"
#include "wx/textctrl.h"

IMPLEMENT_DYNAMIC_CLASS(wxTextCtrlXmlHandler, wxXmlResourceHandler)

wxTextCtrlXmlHandler::wxTextCtrlXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxTE_NO_VSCROLL);
    XRC_ADD_STYLE(wxTE_AUTO_SCROLL);
    XRC_ADD_STYLE(wxTE_PROCESS_ENTER);
    XRC_ADD_STYLE(wxTE_PROCESS_TAB);
    XRC_ADD_STYLE(wxTE_MULTILINE);
    XRC_ADD_STYLE(wxTE_PASSWORD);
    XRC_ADD_STYLE(wxTE_READONLY);
    XRC_ADD_STYLE(wxHSCROLL);
    XRC_ADD_STYLE(wxTE_RICH);
    XRC_ADD_STYLE(wxTE_RICH2);
    XRC_ADD_STYLE(wxTE_AUTO_URL);
    XRC_ADD_STYLE(wxTE_NOHIDESEL);
    XRC_ADD_STYLE(wxTE_LEFT);
    XRC_ADD_STYLE(wxTE_CENTRE);
    XRC_ADD_STYLE(wxTE_RIGHT);
    XRC_ADD_STYLE(wxTE_DONTWRAP);
    XRC_ADD_STYLE(wxTE_LINEWRAP);
    XRC_ADD_STYLE(wxTE_WORDWRAP);
    AddWindowStyles();
}

wxObject *wxTextCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(text, wxTextCtrl)

    text->Create(m_parentAsWindow,
                 GetID(),
                 GetText(wxT("value")),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 wxDefaultValidator,
                 GetName());

    SetupWindow(text);

    if (HasParam(wxT("maxlength")))
        text->SetMaxLength(GetLong(wxT("maxlength")));

    return text;
}

bool wxTextCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxTextCtrl"));
}

#endif // wxUSE_XRC
