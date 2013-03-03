///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/choice.h
// Purpose:     the universal choice
// Author:      Vadim Zeitlin
// Modified by:
// Created:     30.08.00
// RCS-ID:      $Id: choice.h,v 1.8 2004/05/23 20:51:45 JS Exp $
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_CHOICE_H_
#define _WX_UNIV_CHOICE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "univchoice.h"
#endif

#include "wx/combobox.h"

// VS: This is only a *temporary* implementation, real wxChoice should not
//     derive from wxComboBox and may have different l&f 
class WXDLLEXPORT wxChoice : public wxComboBox
{
public:
    wxChoice() {}
    wxChoice(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = (const wxString *) NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxChoiceNameStr)
    {
        Create(parent, id, pos, size, n, choices, style, validator, name);
    }
    wxChoice(wxWindow *parent, wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxChoiceNameStr);

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = (wxString *) NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxChoiceNameStr);
    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxChoiceNameStr);
    
private:
    void OnComboBox(wxCommandEvent &event);
    
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxChoice)
};


#endif // _WX_UNIV_CHOICE_H_
