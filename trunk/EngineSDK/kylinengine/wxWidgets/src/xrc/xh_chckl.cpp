/////////////////////////////////////////////////////////////////////////////
// Name:        xh_chckl.cpp
// Purpose:     XRC resource for wxCheckList
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id: xh_chckl.cpp,v 1.19 2005/05/22 00:29:27 MW Exp $
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xh_chckl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#if wxUSE_CHECKLISTBOX

#include "wx/xrc/xh_chckl.h"
#include "wx/checklst.h"
#include "wx/intl.h"
#include "wx/log.h"

IMPLEMENT_DYNAMIC_CLASS(wxCheckListBoxXmlHandler, wxXmlResourceHandler)

wxCheckListBoxXmlHandler::wxCheckListBoxXmlHandler()
: wxXmlResourceHandler(), m_insideBox(false)
{
    // wxListBox styles:
    XRC_ADD_STYLE(wxLB_SINGLE);
    XRC_ADD_STYLE(wxLB_MULTIPLE);
    XRC_ADD_STYLE(wxLB_EXTENDED);
    XRC_ADD_STYLE(wxLB_HSCROLL);
    XRC_ADD_STYLE(wxLB_ALWAYS_SB);
    XRC_ADD_STYLE(wxLB_NEEDED_SB);
    XRC_ADD_STYLE(wxLB_SORT);
    
    AddWindowStyles();
}

wxObject *wxCheckListBoxXmlHandler::DoCreateResource()
{
    if (m_class == wxT("wxCheckListBox")
#if WXWIN_COMPATIBILITY_2_4
        || m_class == wxT("wxCheckList")
#endif
       )
    {
#if WXWIN_COMPATIBILITY_2_4
        if (m_class == wxT("wxCheckList"))
            wxLogDebug(wxT("'wxCheckList' name is deprecated, use 'wxCheckListBox' instead."));
#endif
        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));
        wxString *strings = (wxString *) NULL;
        if (strList.GetCount() > 0)
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for(int i = 0; i < count; i++)
                strings[i] = strList[i];
        }

        XRC_MAKE_INSTANCE(control, wxCheckListBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        strList.GetCount(),
                        strings,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        // step through children myself (again.)
        wxXmlNode *n = GetParamNode(wxT("content"));
        if (n) n = n->GetChildren();
        int i = 0;
        while (n)
        {
            if (n->GetType() != wxXML_ELEMENT_NODE ||
                n->GetName() != wxT("item"))
               { n = n->GetNext(); continue; }

            // checking boolean is a bit ugly here (see GetBool() )
            wxString v = n->GetPropVal(wxT("checked"), wxEmptyString);
            v.MakeLower();
            if (v && v == wxT("1"))
                control->Check( i, true );

            i++;
            n = n->GetNext();
        }

        SetupWindow(control);

        if (strings != NULL)
            delete[] strings;
        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item checked="boolean">Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str);
        strList.Add(str);
        return NULL;
    }
}

bool wxCheckListBoxXmlHandler::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxCheckListBox")) ||
#if WXWIN_COMPATIBILITY_2_4
            IsOfClass(node, wxT("wxCheckList")) ||
#endif
           (m_insideBox && node->GetName() == wxT("item")));
}

#endif // wxUSE_CHECKLISTBOX

#endif // wxUSE_XRC
