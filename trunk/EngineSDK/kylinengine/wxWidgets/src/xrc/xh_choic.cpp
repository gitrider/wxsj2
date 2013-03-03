/////////////////////////////////////////////////////////////////////////////
// Name:        xh_choic.cpp
// Purpose:     XRC resource for wxChoice
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_choic.cpp,v 1.12 2005/01/07 21:33:13 VS Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_choic.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_choic.h"
#include "wx/choice.h"
#include "wx/intl.h"

IMPLEMENT_DYNAMIC_CLASS(wxChoiceXmlHandler, wxXmlResourceHandler)

wxChoiceXmlHandler::wxChoiceXmlHandler()
: wxXmlResourceHandler() , m_insideBox(false)
{
    XRC_ADD_STYLE(wxCB_SORT);
    AddWindowStyles();
}

wxObject *wxChoiceXmlHandler::DoCreateResource()
{
    if( m_class == wxT("wxChoice"))
    {
        // find the selection
        long selection = GetLong(wxT("selection"), -1);

        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));
        wxString *strings = (wxString *) NULL;
        if (strList.GetCount() > 0)
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for (int i = 0; i < count; i++)
                strings[i]=strList[i];
        }

        XRC_MAKE_INSTANCE(control, wxChoice)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        strList.GetCount(),
                        strings,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        if (selection != -1)
            control->SetSelection(selection);

        SetupWindow(control);

        if (strings != NULL)
            delete[] strings;
        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item>Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str);
        strList.Add(str);

        return NULL;
    }
}

bool wxChoiceXmlHandler::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxChoice")) ||
           (m_insideBox && node->GetName() == wxT("item")));
}

#endif // wxUSE_XRC
