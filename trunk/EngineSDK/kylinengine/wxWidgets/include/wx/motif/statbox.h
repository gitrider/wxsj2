/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.h
// Purpose:     wxStaticBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: statbox.h,v 1.18 2005/02/06 17:38:21 MBN Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBOX_H_
#define _WX_STATBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "statbox.h"
#endif

// Group box
class WXDLLEXPORT wxStaticBox: public wxStaticBoxBase
{
    DECLARE_DYNAMIC_CLASS(wxStaticBox)
        
public:
    wxStaticBox();
    wxStaticBox(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, name);
    }

    ~wxStaticBox();
    
    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticBoxNameStr);
    
    virtual bool ProcessCommand(wxCommandEvent& WXUNUSED(event))
    {
        return false;
    }

    virtual WXWidget GetLabelWidget() const { return m_labelWidget; }

    virtual void SetLabel(const wxString& label);

private:
    WXWidget  m_labelWidget;
   
private:
    DECLARE_EVENT_TABLE()
};

#endif
// _WX_STATBOX_H_
