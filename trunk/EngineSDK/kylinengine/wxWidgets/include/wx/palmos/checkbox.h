/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palmos/checkbox.h
// Purpose:     wxCheckBox class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by: Wlodzimierz ABX Skiba - native implementation
// Created:     10/13/04
// RCS-ID:      $Id: checkbox.h,v 1.4 2005/01/31 08:04:38 ABX Exp $
// Copyright:   (c) William Osborne, Wlodzimierz Skiba
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKBOX_H_
#define _WX_CHECKBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "checkbox.h"
#endif

// Checkbox item (single checkbox)
class WXDLLEXPORT wxCheckBox : public wxCheckBoxBase
{
public:
    wxCheckBox() { }
    wxCheckBox(wxWindow *parent,
               wxWindowID id,
               const wxString& label,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxCheckBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxCheckBoxNameStr);

    virtual void SetValue(bool value);
    virtual bool GetValue() const;

    virtual void Command(wxCommandEvent& event);

    // send a notification event, return true if processed
    bool SendClickEvent();

protected:
    virtual wxSize DoGetBestSize() const;

    virtual void DoSet3StateValue(wxCheckBoxState value);

    virtual wxCheckBoxState DoGet3StateValue() const;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxCheckBox)
};

#endif
    // _WX_CHECKBOX_H_
