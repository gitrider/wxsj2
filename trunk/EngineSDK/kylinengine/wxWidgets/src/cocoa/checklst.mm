/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/checklst.mm
// Purpose:     wxCheckListBox
// Author:      David Elliott
// Modified by:
// Created:     2003/03/18
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_CHECKLISTBOX

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/app.h"
    #include "wx/checklst.h"
#endif //WX_PRECOMP

IMPLEMENT_DYNAMIC_CLASS(wxCheckListBox, wxListBox)
BEGIN_EVENT_TABLE(wxCheckListBox, wxCheckListBoxBase)
END_EVENT_TABLE()
// WX_IMPLEMENT_COCOA_OWNER(wxCheckListBox,NSButton,NSControl,NSView)

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    wxCArrayString chs(choices);

    return Create(parent, winid, pos, size, chs.GetCount(), chs.GetStrings(),
                  style, validator, name);
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            int n, const wxString choices[],
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    if(!CreateControl(parent,winid,pos,size,style,validator,name))
        return false;

    if(m_parent)
        m_parent->CocoaAddChild(this);
    return true;
}

wxCheckListBox::~wxCheckListBox()
{
}

bool wxCheckListBox::IsChecked(size_t item) const
{
    return false;
}


void wxCheckListBox::Check(size_t item, bool check)
{
}

#endif
