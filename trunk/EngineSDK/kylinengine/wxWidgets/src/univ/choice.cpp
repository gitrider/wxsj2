/////////////////////////////////////////////////////////////////////////////
// Name:        univ/choice.cpp
// Purpose:     wxChoice implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     15.12.00
// RCS-ID:      $Id: choice.cpp,v 1.9 2004/08/10 13:08:39 ABX Exp $
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "univchoice.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CHOICE

#ifndef WX_PRECOMP
    #include "wx/choice.h"
    #include "wx/arrstr.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(wxChoice, wxControl)

BEGIN_EVENT_TABLE(wxChoice, wxComboBox)
    EVT_COMBOBOX(wxID_ANY, wxChoice::OnComboBox)
END_EVENT_TABLE()

wxChoice::wxChoice(wxWindow *parent, wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   const wxArrayString& choices,
                   long style,
                   const wxValidator& validator,
                   const wxString& name)
{
    Create(parent, id, pos, size, choices, style, validator, name);
}

bool wxChoice::Create(wxWindow *parent, wxWindowID id,
                      const wxPoint& pos,
                      const wxSize& size,
                      const wxArrayString& choices,
                      long style,
                      const wxValidator& validator,
                      const wxString& name)
{
    wxCArrayString chs(choices);

    return Create(parent, id, pos, size, chs.GetCount(), chs.GetStrings(),
                  style, validator, name);
}

bool wxChoice::Create(wxWindow *parent, wxWindowID id,
                      const wxPoint& pos,
                      const wxSize& size,
                      int n, const wxString choices[],
                      long WXUNUSED(style),
                      const wxValidator& validator,
                      const wxString& name)
{
    wxString value;
    if ( n )
        value = choices[0];
    return wxComboBox::Create(parent, id, value,
                                 pos, size, n, choices,
                                 wxCB_READONLY, validator, name);
}


void wxChoice::OnComboBox(wxCommandEvent& event)
{
    if ( event.GetId() == GetId() )
    {
        event.SetEventType(wxEVT_COMMAND_CHOICE_SELECTED);
        event.Skip();
        GetEventHandler()->ProcessEvent(event);
    }
    else
        event.Skip();
}

#endif // wxUSE_CHOICE
